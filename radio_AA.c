#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <SDL2/SDL.h>

#define AUDIO_BUFFER_SIZE 192000

// --- Estado global do player ---
volatile bool running = true;
volatile bool paused = false;
volatile float volume = 1.0f;

// --- Terminal raw mode ---
struct termios orig_termios;
void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}
void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

// --- Thread de teclado ---
void *keyboard_thread(void *arg) {
    while (running) {
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            switch(c) {
                case 'q':
                case 'Q':
                    running = false;
                    break;
                case 'p':
                case 'P':
                    paused = !paused;
                    printf("\nPlayer: %s\n", paused ? "PAUSADO" : "TOCANDO");
                    fflush(stdout);
                    break;
                case '+':
                case '=':
                    volume += 0.1f;
                    if (volume > 2.0f) volume = 2.0f;
                    printf("\nVolume: %.1f\n", volume);
                    fflush(stdout);
                    break;
                case '-':
                case '_':
                    volume -= 0.1f;
                    if (volume < 0.0f) volume = 0.0f;
                    printf("\nVolume: %.1f\n", volume);
                    fflush(stdout);
                    break;
            }
        }
        usleep(10000);
    }
    return NULL;
}

// --- Banner ASCII ---
void print_banner() {
printf("██████╗░░█████╗░██████╗░██╗░█████╗░  ░█████╗░██╗░░░░░░█████╗░██╗░░░██╗██████╗░ \n");
printf("██╔══██╗██╔══██╗██╔══██╗██║██╔══██╗  ██╔══██╗██║░░░░░██╔══██╗██║░░░██║██╔══██╗ \n");
printf("██████╔╝███████║██║░░██║██║██║░░██║  ██║░░╚═╝██║░░░░░██║░░██║██║░░░██║██║░░██║ \n");
printf("██╔══██╗██╔══██║██║░░██║██║██║░░██║  ██║░░██╗██║░░░░░██║░░██║██║░░░██║██║░░██║ \n");
printf("██║░░██║██║░░██║██████╔╝██║╚█████╔╝  ╚█████╔╝███████╗╚█████╔╝╚██████╔╝██████╔╝ \n");
printf("╚═╝░░╚═╝╚═╝░░╚═╝╚═════╝░╚═╝░╚════╝░  ░╚════╝░╚══════╝░╚════╝░░╚═════╝░╚═════╝░ \n");

    printf("  ____    _    ____ ___ ___  _   _ \n");;
    printf("\n        RADIO CLOUD\n\n");
    printf("Controles:\n[P] Play/Pause  [+] Aumentar volume  [-] Diminuir volume  [Q] Sair\n\n");
    fflush(stdout);
}

// --- Função para ler configuração ---
char *read_config(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Erro ao abrir arquivo de configuração %s\n", filename);
        return NULL;
    }
    char line[512];
    char *url = NULL;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "servidor_url:", 13) == 0) {
            char *p = strchr(line, ':');
            if (p) {
                p++;
                while (*p == ' ' || *p == '\t') p++;
                url = strdup(p);
                char *nl = strchr(url, '\n');
                if (nl) *nl = '\0';
            }
        }
    }
    fclose(f);
    return url;
}

// --- Estrutura para cURL ---
struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) return 0;

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// --- Função para buscar JSON do servidor ---
cJSON *fetch_json(const char *url) {
    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

    res = curl_easy_perform(curl_handle);
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    if(res != CURLE_OK) {
        fprintf(stderr, "Erro ao buscar JSON: %s\n", curl_easy_strerror(res));
        free(chunk.memory);
        return NULL;
    }

    cJSON *json = cJSON_Parse(chunk.memory);
    free(chunk.memory);
    return json;
}

// --- Lista rádios numeradas com validação ---
void list_radios(cJSON *radios) {
    int i = 0;
    cJSON *radio;
    cJSON_ArrayForEach(radio, radios) {
        cJSON *nome = cJSON_GetObjectItemCaseSensitive(radio, "nome_radio");
        cJSON *desc = cJSON_GetObjectItemCaseSensitive(radio, "descricao");
        cJSON *reg  = cJSON_GetObjectItemCaseSensitive(radio, "regiao");
        const char *n = (cJSON_IsString(nome) && nome->valuestring) ? nome->valuestring : "(sem nome)";
        const char *d = (cJSON_IsString(desc) && desc->valuestring) ? desc->valuestring : "(sem descrição)";
        const char *r = (cJSON_IsString(reg)  && reg->valuestring)  ? reg->valuestring  : "(sem região)";
        printf("[%d] %s - %s (%s)\n", i, n, d, r);
        i++;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <URL da rádio> ou -a\n", argv[0]);
        return 1;
    }

    const char *url = NULL;
    bool url_allocated = false;

    if (strcmp(argv[1], "-a") == 0) {
        char *server_url = read_config("config.txt");
        if (!server_url) {
            fprintf(stderr, "Sem servidor_url no config.txt\n");
            return 1;
        }

        cJSON *radios = fetch_json(server_url);
        if (!radios || !cJSON_IsArray(radios)) {
            fprintf(stderr, "JSON inválido: esperado um array de rádios\n");
            free(server_url);
            if (radios) cJSON_Delete(radios);
            return 1;
        }

        int radios_count = cJSON_GetArraySize(radios);
        if (radios_count <= 0) {
            fprintf(stderr, "Lista de rádios vazia\n");
            cJSON_Delete(radios);
            free(server_url);
            return 1;
        }

        printf("=== Lista de rádios (%d) ===\n", radios_count);
        list_radios(radios);

        printf("\nEscolha o número da rádio [0-%d]: ", radios_count - 1);
        int escolha = -1;
        if (scanf("%d", &escolha) != 1) {
            fprintf(stderr, "Entrada inválida\n");
            cJSON_Delete(radios);
            free(server_url);
            return 1;
        }
        if (escolha < 0 || escolha >= radios_count) {
            fprintf(stderr, "Número fora do intervalo\n");
            cJSON_Delete(radios);
            free(server_url);
            return 1;
        }

        cJSON *radio = cJSON_GetArrayItem(radios, escolha);
        if (!radio) {
            fprintf(stderr, "Item da lista não encontrado\n");
            cJSON_Delete(radios);
            free(server_url);
            return 1;
        }

        cJSON *url_item  = cJSON_GetObjectItemCaseSensitive(radio, "url");
        cJSON *nome_item = cJSON_GetObjectItemCaseSensitive(radio, "nome_radio");
        if (!cJSON_IsString(url_item) || !url_item->valuestring ||
            !cJSON_IsString(nome_item) || !nome_item->valuestring) {
            fprintf(stderr, "JSON inválido: faltam campos 'url' ou 'nome_radio'\n");
            cJSON_Delete(radios);
            free(server_url);
            return 1;
        }

        // DUPLICA a URL para evitar use-after-free após cJSON_Delete
        char *url_copy = strdup(url_item->valuestring);
        if (!url_copy) {
            fprintf(stderr, "Falha ao alocar memória para URL\n");
            cJSON_Delete(radios);
            free(server_url);
            return 1;
        }
        url = url_copy;
        url_allocated = true;

        printf("Tocando: %s\n", nome_item->valuestring);

        cJSON_Delete(radios);
        free(server_url);
    } else {
        url = argv[1];
    }

    // --- Player original ---
    print_banner();
    enable_raw_mode();

    pthread_t kb_thread;
    pthread_create(&kb_thread, NULL, keyboard_thread, NULL);

    avformat_network_init();

    AVFormatContext *fmt_ctx = NULL;
    if (avformat_open_input(&fmt_ctx, url, NULL, NULL) != 0) {
        fprintf(stderr, "Erro ao abrir stream: %s\n", url);
        running = false;
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        fprintf(stderr, "Erro ao obter info do stream\n");
        avformat_close_input(&fmt_ctx);
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }

    int audio_stream_index = -1;
    for (unsigned i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            break;
        }
    }

    if (audio_stream_index == -1) {
        fprintf(stderr, "Stream de áudio não encontrado\n");
        avformat_close_input(&fmt_ctx);
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }

    AVCodecParameters *codecpar = fmt_ctx->streams[audio_stream_index]->codecpar;
    AVCodec *codec = avcodec_find_decoder(codecpar->codec_id);
    if (!codec) {
        fprintf(stderr, "Decoder não encontrado\n");
        avformat_close_input(&fmt_ctx);
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }

    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
        fprintf(stderr, "Falha ao alocar contexto do codec\n");
        avformat_close_input(&fmt_ctx);
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }

    if (avcodec_parameters_to_context(codec_ctx, codecpar) < 0) {
        fprintf(stderr, "Erro ao copiar parâmetros para contexto do codec\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }
    if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
        fprintf(stderr, "Erro ao abrir codec\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }

    if (codec_ctx->channels <= 0) {
        codec_ctx->channels = codecpar->channels;
    }
    if (codec_ctx->channels <= 0) {
        fprintf(stderr, "Número de canais inválido\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }

    if (codec_ctx->channel_layout == 0)
        codec_ctx->channel_layout = av_get_default_channel_layout(codec_ctx->channels);

    SwrContext *swr = swr_alloc_set_opts(NULL,
        AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100,
        codec_ctx->channel_layout, codec_ctx->sample_fmt, codec_ctx->sample_rate,
        0, NULL);
    if (!swr || swr_init(swr) < 0) {
        fprintf(stderr, "Erro ao inicializar resampler\n");
        if (swr) swr_free(&swr);
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }

    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL_Init falhou: %s\n", SDL_GetError());
        swr_free(&swr);
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }

    SDL_AudioSpec spec = {0};
    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.samples = 1024;

    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    if (dev == 0) {
        fprintf(stderr, "SDL_OpenAudioDevice falhou: %s\n", SDL_GetError());
        SDL_Quit();
        swr_free(&swr);
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }
    SDL_PauseAudioDevice(dev, 0);

    AVPacket pkt;
    AVFrame *frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Falha ao alocar frame\n");
        SDL_CloseAudioDevice(dev);
        SDL_Quit();
        swr_free(&swr);
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }

    uint8_t *buffer = av_malloc(AUDIO_BUFFER_SIZE);
    if (!buffer) {
        fprintf(stderr, "Falha ao alocar buffer de áudio\n");
        av_frame_free(&frame);
        SDL_CloseAudioDevice(dev);
        SDL_Quit();
        swr_free(&swr);
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        disable_raw_mode();
        if (url_allocated) free((void*)url);
        return 1;
    }

    while (running) {
        if (!paused && av_read_frame(fmt_ctx, &pkt) >= 0) {
            if (pkt.stream_index == audio_stream_index) {
                if (avcodec_send_packet(codec_ctx, &pkt) == 0) {
                    while (avcodec_receive_frame(codec_ctx, frame) == 0) {
                        int samples = swr_convert(
                            swr,
                            &buffer, AUDIO_BUFFER_SIZE / 2,
                            (const uint8_t **)frame->data,
                            frame->nb_samples
                        );

                        if (samples > 0) {
                            int total_samples = samples * 2; // stereo
                            int16_t *s = (int16_t *)buffer;
                            for (int i = 0; i < total_samples; i++) {
                                float sample = s[i] * volume;
                                if (sample > 32767.0f) sample = 32767.0f;
                                if (sample < -32768.0f) sample = -32768.0f;
                                s[i] = (int16_t)sample;
                            }
                            SDL_QueueAudio(dev, buffer, total_samples * sizeof(int16_t));
                        }
                    }
                }
            }
            av_packet_unref(&pkt);
        } else {
            SDL_Delay(10); // evita loop quente quando pausado ou sem pacote
        }
    }

    pthread_join(kb_thread, NULL);
    disable_raw_mode();

    SDL_CloseAudioDevice(dev);
    SDL_Quit();
    swr_free(&swr);
    av_frame_free(&frame);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);
    avformat_network_deinit();
    av_free(buffer); 
    if (url_allocated) free((void*)url);

    printf("\nPlayer encerrado.\n");
    return 0;
}

