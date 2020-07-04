#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "render.h"
#include "formula.h"

extern const int TABLE_CELL_WIDTH;
extern const float TABLE_X[];
extern const char TABLE_LABEL[];
extern const char TABLE_LABEL_X[];
extern const char TABLE_LABEL_Y[];

char *generate_random_color()
{
  int r = 255;
  int g = 255;
  int b = 255;

  while ((r + g + b) / 3 > 150)
  {
    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;
  }

  const char format[] = "rgb(%u,%u,%u)";
  size_t needed = snprintf(NULL, 0, format, r, g, b);
  char *buffer = calloc(needed + 1, sizeof(char));
  sprintf(buffer, format, r, g, b);

  return buffer;
}

bool should_draw_line(enum input_type type, float plot_y, float prev_plot_y)
{
  switch (type)
  {
  case tg_formula:
    return prev_plot_y - plot_y < 0;
  default:
    return true;
  }
}

void create_svg_file(char *file, float x, float y, float width, float height, struct Formula *formulae[], size_t formula_count)
{
  FILE *fp = fopen(file, "w+");

  fputs("<?xml version='1.0'?>", fp);

  float font_size = height / 50;
  float center_x = width / 2;
  float center_y = height / 2;

  fprintf(
      fp,
      "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 %g %g' "
      "width='%gpx' height='%gpx' preserveAspectRatio='none'>",
      width,
      height,
      width,
      height);

  fprintf(
      fp,
      "<style>"
      "  text {"
      "    font-size: %gpx;"
      "    font-family: Monaco, 'Fira Code', Consolas, monospace;"
      "  }"
      "</style>",
      font_size);

  // Grid
  float small_grid_size_width = width / x / 2;
  float mini_grid_size_width = small_grid_size_width / 10;
  float small_grid_size_height = height / y / 2;
  float mini_grid_size_height = small_grid_size_height / 10;
  fprintf(
      fp,
      " <defs>"
      "   <clipPath id='view-clip'>"
      "     <rect x='0' y='0' width='100%%' height='100%%' />"
      "   </clipPath>"
      "   <pattern id='grid-sm' width='%g' height='%g' patternUnits='userSpaceOnUse'>"
      "     <path d='M %g 0 L 0 0 0 %g' fill='none' stroke='#ddd' stroke-width='0.5' />"
      "   </pattern>"
      "   <pattern id='grid' width='%g' height='%g' patternUnits='userSpaceOnUse'>"
      "     <rect width='%g' height='%g' fill='url(#grid-sm)' />"
      "     <path d='M %g 0 L 0 0 0 %g' fill='none' stroke='#bbb' stroke-width='0.75' />"
      "   </pattern>"
      " </defs>",
      mini_grid_size_width,
      mini_grid_size_height,
      mini_grid_size_width,
      mini_grid_size_height,
      small_grid_size_width,
      small_grid_size_height,
      small_grid_size_width,
      small_grid_size_height,
      small_grid_size_width,
      small_grid_size_height);
  fprintf(
      fp,
      "<rect width='100%%' height='100%%' fill='url(#grid)' />"
      "<line x1='0' y1='50%%' x2='100%%' y2='50%%' stroke='#888' stroke-width='0.75' />"
      "<line x1='50%%' y1='0' x2='50%%' y2='100%%' stroke='#888' stroke-width='0.75' />");

  // Numbers x
  float number_offset_x = font_size * 0.4;
  float number_offset_y = (font_size * 1.2);
  for (int i = -x; i < x; i++)
  {
    float number_x = ((width / (x * 2)) * i) + center_x - number_offset_x;
    float number_y = height / 2 + number_offset_y;
    fprintf(
        fp,
        "<text x='%g' y='%g' fill='#888' text-anchor='end'>%d</text>",
        number_x,
        number_y,
        i);
  }

  // Numbers y
  for (int i = -y; i < y; i++)
  {
    // Ignore 0 as the x line has already plotted this number
    if (i == 0)
    {
      continue;
    }

    float number_x = center_x - number_offset_x;
    float number_y = ((height / (y * 2)) * i) + (height / 2) + number_offset_y;
    fprintf(
        fp,
        "<text x='%g' y='%g' fill='#888' text-anchor='end'>%d</text>",
        number_x,
        number_y,
        -i);
  }

  float x_step = width / x / 2;
  float y_step = height / y / 2;

  for (size_t i = 0; i < formula_count; i++)
  {
    char *color = generate_random_color();

    // Formula text
    float text_x = width - font_size;
    float text_y = (i + 1) * (font_size * 1.2);
    fprintf(
        fp,
        "<text x='%g' y='%g' fill='%s' text-anchor='end'>%s</text>",
        text_x,
        text_y,
        color,
        formulae[i]->input);

    // Formula line
    fprintf(fp, "<path d='");

    float prev_plot_y = 0;
    bool outside_boundaries = true;
    for (int j = 0; j <= width; j++)
    {
      float plot_x = (j - center_x) / x_step;
      float plot_y = apply_maths(formulae[i], plot_x);

      // M is for starting a path, L is for moving a line to a point
      char movement = outside_boundaries || !should_draw_line(formulae[i]->type, plot_y, prev_plot_y) ? 'M' : 'L';

      prev_plot_y = plot_y;

      float path_x = plot_x * x_step + center_x;
      float path_y = -plot_y * y_step + center_y;

      // This is (unnecessary) optimization, to not draw points that are out of bounds
      if ((path_x < 0 || path_x > width) || (path_y < 0 || path_y > height))
      {
        // Don't print if previous was already out of boundaries
        if (outside_boundaries)
        {
          continue;
        }

        outside_boundaries = true;
      }
      else
      {
        outside_boundaries = false;
      }

      fprintf(fp, "%c %g %g", movement, path_x, path_y);
    }

    fprintf(fp, "' fill='none' stroke='%s' stroke-width='1.5' />", color);
  }

  fputs("</svg>", fp);

  fclose(fp);
}

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
