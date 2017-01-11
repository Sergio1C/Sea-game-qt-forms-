#ifndef POINTS_H
#define POINTS_H

class Point
{
public:
   Point():x(0),y(0),fill(false) {}
   Point(int _x, int _y, bool _fill = false): x(_x),y(_y), fill(_fill) {}
   Point(int pos, bool _fill = false): fill(_fill)
   {
       int dimension = 10;

       x = pos / dimension;
       y = pos % dimension;

   }

   Point(const Point& P)
   {
    x = P.x;
    y = P.y;
    fill = P.fill;
   }

   friend bool operator==(const Point& Left, const Point& Right)
   {
       if ((Left.x == Right.x) && (Left.y == Right.y))
           return true;

       return false;
   }
   friend bool operator!=(const Point& Left, const Point& Right)
   {
       return !(Left == Right);
   }

   int x;
   int y;
   bool fill;
};

#endif // POINTS_H
