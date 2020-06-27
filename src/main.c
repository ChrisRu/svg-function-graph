#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "formula.c"
#include "render.c"

const int MAX_FORMULA_INPUT = 100;

int main(const int argc, char *argv[])
{
  // Initialize random
  srand(time(NULL));

  char *FILE;
  float W;
  float H;
  float X;
  float Y;

  if (argc == 6)
  {
    FILE = argv[1];
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

  struct Formula formulae[60];
  size_t formula_count = 0;
  while (true)
  {
    char formula[MAX_FORMULA_INPUT];
    float table_y[11];
    float a;
    float b;
    float c;
    enum input_type input_type;

    printf("\n > ");
    if (fgets(formula, MAX_FORMULA_INPUT, stdin) == NULL)
    {
      break;
    }

    input_type = parse_input(formula, &a, &b, &c);
    switch (input_type)
    {
    case bad_formula:
      printf("Bad command or function\n");
      break;
    case exit_input:
      printf("Exiting...\n");
      if (FILE == NULL)
      {
        // No file supplied, no SVG writing needed
        return 0;
      }

      create_svg_file(FILE, X, Y, W, H, formulae, formula_count);

      return 0;
    default:
    {
      struct Formula new_formula;
      char *cloned_formula = malloc((strlen(formula) + 1) * sizeof(char));
      strcpy(cloned_formula, formula);
      new_formula.input = cloned_formula;
      new_formula.type = input_type;
      new_formula.a = roundf(a * 100) / 100;
      new_formula.b = roundf(b * 100) / 100;
      new_formula.c = roundf(c * 100) / 100;
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