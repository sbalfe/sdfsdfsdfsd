/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

/* This is the code you will need to replace for Lab 1.
 *
 * It contains two simple implementations that loop over the longest axis adding the gradient to the position on the other axis at each step.
 * The objective is for you to remove the use of floating point variables and work entirely with integers.
 * You should use Bresenhams Line Algorithm to achieve this.
 */

#include <iostream>
#include "linedrawer.h"


auto calculateDirection(int ex, int ey, int sy , int sx){

    int dirx;
    int diry;
    dirx= 1;
    if (sx > ex){
        dirx = -1;
    }

    diry = 1;
    if (sy > ey){
        diry= -1;
    }
    return std::make_pair(dirx, diry);
}

int draw_x_line(FrameBuffer *fb, int sx, int sy, int ex, int ey)
{

  auto [dirx, diry] = calculateDirection(ex, ey, sy, sx);

  int x  = sx;
  int wy = sy;

  int dy = ey - sy;
  int dx = ex - sx;
  int fy = dy/2;

  while (x != ex)
  {
    fb->plotPixel(x, (int) wy, 0.0f, 1.0f, 0.0f);

    fy += abs(dy);

    x += dirx;

    if (fy > abs(dx)){
        wy += diry;
        fy -= abs(dx);
    }
  }
  return 0;
}

int draw_y_line(FrameBuffer *fb, int sx, int sy, int ex, int ey)
{
  auto [dirx, diry] = calculateDirection(ex, ey, sy, sx);

  int y = sy;int wx = sx;

  int dy = ey - sy;
  int dx = ex - sx;
  int fx = dx/2;

  while (y != ey)
  {
    fb->plotPixel((int)wx, y, 0.0f, 1.0f, 0.0f);

    fx += abs(dx);
    y += diry;

    if (fx > abs(dy)){
        wx += dirx;
        fx -= abs(dy);
    }
  }

  return 0;
}


int draw_line(FrameBuffer *fb, int sx, int sy, int ex, int ey)
{
  if ((sx == ex) && (sy==ey))
  {
    return fb->plotPixel(sx, sy, 1.0f, 1.0f, 1.0f);
    
  } else if (((ex-sx)* (ex-sx)) >= ((ey-sy)* (ey-sy)))
  {
    return draw_x_line(fb, sx, sy, ex, ey);
    
  } else
  {
    return draw_y_line(fb, sx, sy, ex, ey);
  }
}
