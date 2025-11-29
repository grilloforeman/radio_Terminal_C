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

# alvo para rodar com uma URL direta
run-url: radio_AA
> ASAN_OPTIONS=detect_leaks=0 SDL_AUDIODRIVER=alsa ./radio_AA http://stream-url.com/live.mp3

# alvo para limpar binários
clean:
> rm -f radio_AA

