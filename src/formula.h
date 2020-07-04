#ifndef FORMULA_INCLUDED
#define FORMULA_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_FORMULA_INPUT 100

const int MAX_FORMULA_TOKENS = 8;
const int MAX_GROUP_SIZE = 5;

enum input_type
{
  exit_input,
  bad_formula,
  linear_formula,
  additive_formula,
  cos_formula,
  sin_formula,
  tg_formula,
  quadratic_formula
};

typedef struct Formula
{
  char input[MAX_FORMULA_INPUT];
  enum input_type type;
  float a;
  float b;
  float c;
} Formula;

float apply_maths(struct Formula *formula, float x);

enum input_type parse_input(const char *input, float *a, float *b, float *c);

#endif /* FORMULA_INCLUDED */
