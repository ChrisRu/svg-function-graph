#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

const int MAX_FORMULA_TOKENS = 5;

enum input_type
{
  exit_input,
  bad_formula,
  linear_formula,
  positive_formula,
  negative_formula,
  cos_formula,
  sin_formula,
  tg_formula
};

typedef struct Formula
{
  char *input;
  enum input_type type;
  float a;
} Formula;

float apply_maths(enum input_type type, float x, const float a);

enum input_type parse_input(const char *input, float *a);