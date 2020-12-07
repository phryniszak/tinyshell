CC             = gcc
CFLAGS         = -Wall -g -std=c99 -I./shell

all: app

app: ./shell/shell.c shell_commands.c main.c
	$(CC) $(CFLAGS) -o app.out ./shell/shell.c shell_commands.c main.c

clean:
	rm -f app.out