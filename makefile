output = svggraph
args = -std=c99 -Wall -Werror -Wextra -pedantic -g -lm

default: src/main.c src/render.c src/formula.c
	gcc $(args) src/main.c -o $(output)
