output = svggraph
args = -std=c99 -Wall -Werror -Wextra -pedantic -g -lm

default:
	gcc $(args) src/main.c -o $(output)

clean:
	rm $(output)
