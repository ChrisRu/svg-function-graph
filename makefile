output = svggraph
args = -std=c99 -Wall -Werror -Wextra -Wno-unused-variable -pedantic -g -lm

default: src/svggraph.c src/render.c src/formula.c
	gcc $(args) -o $(output) src/svggraph.c
