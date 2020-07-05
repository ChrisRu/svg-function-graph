#include "table.h"

extern const int TABLE_CELL_WIDTH;
extern const float TABLE_X[];
extern const char TABLE_LABEL[];
extern const char TABLE_LABEL_X[];
extern const char TABLE_LABEL_Y[];

void print_table(const float *x, const float *y, size_t cell_count)
{
  const int label_width = strlen(TABLE_LABEL);

  int table_width = label_width + (cell_count * (TABLE_CELL_WIDTH + 1));
  char *header = calloc((table_width + 1), sizeof(char));
  char *row_x = calloc((table_width + 1), sizeof(char));
  char *row_y = calloc((table_width + 1), sizeof(char));
  strncpy(header, TABLE_LABEL, label_width + 1);
  strncpy(row_x, TABLE_LABEL_X, label_width + 1);
  strncpy(row_y, TABLE_LABEL_Y, label_width + 1);

  // populate header line
  for (int i = 0; i <= table_width - label_width; i++)
  {
    if (i % (TABLE_CELL_WIDTH + 1) == 0)
    {
      header[i + label_width - 1] = '+';
    }
    else
    {
      header[i + label_width - 1] = '-';
    }
  }

  // populate x and y rows
  for (size_t i = 0; i < cell_count; i++)
  {
    int offset = label_width + (i * (TABLE_CELL_WIDTH + 1));
    char content_x[TABLE_CELL_WIDTH + 1];
    char content_y[TABLE_CELL_WIDTH + 1];
    sprintf(content_x, "% *.2f", TABLE_CELL_WIDTH, x[i]);
    sprintf(content_y, "% *.2f", TABLE_CELL_WIDTH, y[i]);
    strncpy(row_x + offset, content_x, TABLE_CELL_WIDTH + 1);
    strncpy(row_y + offset, content_y, TABLE_CELL_WIDTH + 1);
    row_x[offset + TABLE_CELL_WIDTH] = '|';
    row_y[offset + TABLE_CELL_WIDTH] = '|';
  }

  header[table_width] = '\0';
  row_x[table_width] = '\0';
  row_y[table_width] = '\0';

  // print rows to screen
  printf("%s\n", header);
  printf("%s\n", row_x);
  printf("%s\n", row_y);
  printf("%s\n", header);
}
