## Arquivos necessarios para compilação
 ´´´  
sudo apt update
sudo apt install -y \
    build-essential \
    pkg-config \
    libsdl2-dev \
    libavformat-dev \
    libavcodec-dev \
    libavutil-dev \
    libswresample-dev \
    libcurl4-openssl-dev \
    libcjson-dev
```
## O que é instalado, aqui uma explicação

 build-essential → fornece gcc, make e ferramentas básicas de compilação

    pkg-config → ajuda a localizar flags de compilação das libs

    libsdl2-dev → SDL2 para áudio

    libavformat-dev, libavcodec-dev, libavutil-dev, libswresample-dev → FFmpeg

    libcurl4-openssl-dev → cURL para buscar JSON

    libcjson-dev → cJSON para parsear JSON



### Compilação 
Aqui voce tem um makefile que automatiza o processo de compilacção passando configurações para ajustar. A diferenca desse
codigo e que vc pode mudar o servidor de audio de pulseaudio para alsa.


## Depois de instalar os pacotes, compile com:
bash
# Compilação

# Depois de instalar os pacot
#Arquivos do projeto

Código: radio_AA.c

Automação: Makefile (compilar, rodar, suprimir leaks do ASAN, forçar ALSA)
Configuração: config.txt (URL do servidor JSON com lista de rádios)

Crie o Makefile com .RECIPEPREFIX para não depender de TAB:
```
.RECIPEPREFIX = >

CC = gcc
CFLAGS = -g -fsanitize=address
LIBS = `pkg-config --cflags --libs sdl2` -lavformat -lavcodec -lavutil -lswresample -lcurl -lcjson -lpthread

# alvo principal: compila o programa a partir de radio_AA.c
radio_AA: radio_AA.c
> $(CC) $(CFLAGS) radio_AA.c -o radio_AA $(LIBS)

# alvo para rodar com -a (usando ALSA e suprimindo leaks do ASAN)
run: radio_AA
> ASAN_OPTIONS=detect_leaks=0 SDL_AUDIODRIVER=alsa ./radio_AA -a

# alvo para rodar com uma URL direta (edite a URL abaixo)
run-url: radio_AA
> ASAN_OPTIONS=detect_leaks=0 SDL_AUDIODRIVER=alsa ./radio_AA http://stream-url.com/live.mp3

# alvo para limpar binários
clean:
> rm -f radio_AA
```

# Arquivo config.txt
Esse arquivo vc tem que colocar na pasta o arquivo que roda, ou seja depois de compilado,
ele indica qual servidor contem a lista de radios que o programa vai buscar. O servidor

##Exemplo de config.txt:

servidor_url: http://seu-servidor.com/radios.json

##Como compilar e executar

#Compilar o binário:
make

#Rodar com lista de rádios (usa config.txt):R
make run
#Rodar com URL direta (edite o Makefile ou rode manualmente):
make run-url
#Ou mnaualmente, depois de compilado:
ASAN_OPTIONS=detect_leaks=0 SDL_AUDIODRIVER=alsa ./radio_AA http://stream-url.com/live.mp3
#Rodar manualmente com supressão de leaks e ALSA:
ASAN_OPTIONS=detect_leaks=0 SDL_AUDIODRIVER=alsa ./radio_AA -a
#Limpar binário:
make clean



#Controles dentro do player
São controles do programa que voce vai usar quando estiver usando
    Play/Pause: P

    Aumentar volume: + ou =

    Diminuir volume: - ou _

    Sair: Q
