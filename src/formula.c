#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "formula.h"

extern const int MAX_FORMULA_TOKENS;
extern const int MAX_GROUP_SIZE;

float apply_maths(struct Formula *formula, float x)
{
  switch (formula->type)
  {
  case linear_formula:
    return x * formula->a;
  case additive_formula:
    return x + formula->a;
  case cos_formula:
    return cos(x);
  case sin_formula:
    return sin(x);
  case tg_formula:
    return tan(x);
  case quadratic_formula:
    return (formula->a * pow(x, 2)) + (formula->b * x) + formula->c;
  case exit_input:
    printf("Incorrect formula type exit supplied\n");
    exit(-1);
  case bad_formula:
    printf("Bad formula supplied\n");
    exit(-1);
  default:
    printf("Unknown formula\n");
    exit(-1);
  }
}

enum input_type parse_input(struct Formula *formula)
{
  size_t input_size = strlen(formula->input);

  char *groups[MAX_FORMULA_TOKENS];
  bool was_space = true;
  int group_index = -1;
  int letter_index = -1;
  for (size_t i = 0; i < input_size; ++i)
  {
    if (formula->input[i] == '\n')
    {
      break;
    }

    if (isspace(formula->input[i]))
    {
      was_space = true;
      letter_index = -1;
      continue;
    }
    else
    {
      if (was_space)
      {
        was_space = false;
        groups[++group_index] = calloc(MAX_GROUP_SIZE, sizeof(char));
      }

      groups[group_index][++letter_index] = formula->input[i];
      groups[group_index][letter_index + 1] = '\0';
    }
  }

  if (group_index == -1)
  {
    // Input is empty
    return bad_formula;
  }

  if (strncmp(groups[0], "exit", 4) == 0)
  {
    return exit_input;
  }

  if (strncmp(groups[0], "y", 1) != 0)
  {
    // No y supplied, or in the wrong place
    return bad_formula;
  }

  if (group_index < 2 || strncmp(groups[1], "=", 1) != 0)
  {
    // No equals sign supplied, or in the wrong place
    return bad_formula;
  }

  // y = cos x
  if (group_index == 3 && strncmp(groups[2], "cos", 3) == 0)
  {
    if (strlen(groups[3]) == 1 && groups[3][0] == 'x')
    {
      return cos_formula;
    }

    return bad_formula;
  }

  // y = sin x
  if (group_index == 3 && strncmp(groups[2], "sin", 3) == 0)
  {
    if (strlen(groups[3]) == 1 && groups[3][0] == 'x')
    {
      return sin_formula;
    }

    return bad_formula;
  }

  // y = tg x
  // y = tan x
  if (group_index == 3 && (strncmp(groups[2], "tg", 3) == 0 || strncmp(groups[2], "tan", 3) == 0))
  {
    if (strlen(groups[3]) == 1 && groups[3][0] == 'x')
    {
      return tg_formula;
    }

    return bad_formula;
  }

  // y = x + a
  // y = x - a
  if (group_index == 4 && strlen(groups[3]) == 1 && (groups[3][0] == '-' || groups[3][0] == '+'))
  {
    char *end_ptr_a;
    formula->a = strtof(groups[4], &end_ptr_a);
    // error if number parsing went bad
    if (*end_ptr_a != '\0' || groups[4] == end_ptr_a)
    {
      return bad_formula;
    }

    if (groups[3][0] == '-')
    {
      formula->a = -formula->a;
    }

    return additive_formula;
  }

  // y = a x
  if (group_index == 3 && (isdigit(groups[2][0]) || (strlen(groups[2]) > 1 && groups[2][0] == '-')))
  {
    char *end_ptr_a;
    formula->a = strtof(groups[2], &end_ptr_a);
    // error if number parsing went bad
    if (*end_ptr_a != '\0' || groups[2] == end_ptr_a)
    {
      return bad_formula;
    }

    if (strlen(groups[3]) != 1 || groups[3][0] != 'x')
    {
      return bad_formula;
    }

    return linear_formula;
  }

  // y = a x2 + b x + c
  if (group_index == 8)
  {
    char *end_ptr_a;
    formula->a = strtof(groups[2], &end_ptr_a);
    // error if number parsing went bad
    if (*end_ptr_a != '\0' || groups[2] == end_ptr_a)
    {
      return bad_formula;
    }

    if (strncmp(groups[3], "x2", 4) != 0)
    {
      return bad_formula;
    }

    if (strlen(groups[6]) != 1 && groups[6][0] != 'x')
    {
      return bad_formula;
    }

    char *end_ptr_b;
    formula->b = strtof(groups[5], &end_ptr_b);
    // error if number parsing went bad
    if (*end_ptr_b != '\0' || groups[5] == end_ptr_b)
    {
      return bad_formula;
    }

    char *end_ptr_c;
    formula->c = strtof(groups[8], &end_ptr_c);
    // error if number parsing went bad
    if (*end_ptr_c != '\0' || groups[8] == end_ptr_c)
    {
      return bad_formula;
    }

    if (strlen(groups[4]) != 1 || (groups[4][0] != '-' && groups[4][0] != '+'))
    {
      return bad_formula;
    }

    if (groups[4][0] == '-')
    {
      formula->b = -formula->b;
    }

    if (strlen(groups[7]) != 1 || (groups[7][0] != '-' && groups[7][0] != '+'))
    {
      return bad_formula;
    }

    if (groups[7][0] == '-')
    {
      formula->c = -formula->c;
    }

    return quadratic_formula;
  }

  return bad_formula;
}
