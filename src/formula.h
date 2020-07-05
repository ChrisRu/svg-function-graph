#ifndef FORMULA_INCLUDED
#define FORMULA_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_FORMULA_INPUT 100
#define MAX_FORMULA_TOKENS 8
#define MAX_GROUP_SIZE 5

typedef enum input_type
{
  exit_input,
  bad_formula,
  linear_formula,
  additive_formula,
  cos_formula,
  sin_formula,
  tg_formula,
  quadratic_formula
} input_type;

typedef struct formula
{
  char input[MAX_FORMULA_INPUT];
  input_type type;
  float a;
  float b;
  float c;
} formula;

float apply_maths(formula *formula, float x);

input_type parse_input(formula *formula);

#endif /* FORMULA_INCLUDED */
