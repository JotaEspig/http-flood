
CC=gcc
PROJECT_NAME=http-flood

$(PROJECT_NAME):
	$(CC) -o $(PROJECT_NAME) $(wildcard src/*.c)

clean:
	rm $(PROJECT_NAME)
