# Make SVG Graph from formulas

## Building

Requires make and gcc to be installed. In the root folder, run:

`make`

## Running without generating SVG file

`./svggraph`

## Running with SVG file

Replace W, H, X, Y with numbers.

`./svgraph output.svg W H X Y`

## Supported functions

Replace a, b, c with numbers.

- `y = a x`
- `y = x + a`
- `y = x - a`
- `y = sin a`
- `y = cos a`
- `y = tg a` or `y = tan a`
- HARD WIP: `y = a x2 + b x + c`
