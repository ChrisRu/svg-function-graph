output = svggraph
args = -std=c99 -Wall -Werror -Wextra -pedantic -g -lm

default: src/*.c
	gcc $(args) src/main.c -o $(output)
