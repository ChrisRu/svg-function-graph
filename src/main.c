#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "formula.c"
#include "svg.c"
#include "table.c"

#define MAX_FORMULAE 11
#define MAX_FORMULA_INPUT 100

char *SVG_FILE;
float W;
float H;
float X;
float Y;

struct Formula *formulae[MAX_FORMULAE];
size_t formula_count = 0;

float table_y[sizeof(TABLE_X)];

int main(const int argc, char *argv[])
{
  // Initialize random
  srand(time(NULL));

  if (argc == 6)
  {
    SVG_FILE = argv[1];
    W = strtof(argv[2], NULL);
    H = strtof(argv[3], NULL);
    X = strtof(argv[4], NULL);
    Y = strtof(argv[5], NULL);
  }
  else if (argc != 1)
  {
    printf("Incorrect amount of arguments supplied I'm afraid\n");
    return -1;
  }

  while (true)
  {
    char input[MAX_FORMULA_INPUT];

    printf("\n> ");
    if (fgets(input, MAX_FORMULA_INPUT, stdin) == NULL)
    {
      break;
    }

    struct Formula *new_formula = malloc(sizeof(struct Formula));
    strcpy(new_formula->input, input);
    new_formula->type = parse_input(new_formula);
    new_formula->a = roundf(new_formula->a * 100) / 100;
    new_formula->b = roundf(new_formula->b * 100) / 100;
    new_formula->c = roundf(new_formula->c * 100) / 100;

    switch (new_formula->type)
    {
    case bad_formula:
      printf("Bad command or function\n");
      break;
    case exit_input:
      printf("Exiting...\n");
      if (SVG_FILE == NULL)
      {
        // No file supplied, no SVG writing needed
        return 0;
      }

      create_svg_file(SVG_FILE, X, Y, W, H, formulae, formula_count);

      return 0;
    default:
    {
      formulae[formula_count++] = new_formula;

      for (size_t i = 0; i < sizeof(TABLE_X) / sizeof(TABLE_X[0]); i++)
      {
        table_y[i] = apply_maths(new_formula, TABLE_X[i]);
      }

      print_table(TABLE_X, table_y, sizeof(TABLE_X) / sizeof(TABLE_X[0]));

      break;
    }
    }
  }

  return 0;
}
