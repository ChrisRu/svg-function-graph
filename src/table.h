#include <stdio.h>

const int TABLE_CELL_WIDTH = 6;
const float TABLE_X[] = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
const char TABLE_LABEL[] = "+---+";
const char TABLE_LABEL_X[] = "| x |";
const char TABLE_LABEL_Y[] = "| y |";

void print_table(const float *x, const float *y, size_t cell_count);
