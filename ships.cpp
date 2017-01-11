#include "ships.h"

Ship::Ship(const Point& First, bool horizont, const int lenght)
{
    for (int i = 0; i < lenght; i++)
    {
      if (horizont)
      {
           _points.push_back(Point(First.x + i, First.y, true));
            _decks.push_back(Point(First.x + i, First.y, false));
      }
      else
      {
          _points.push_back(Point(First.x, First.y + i, true));
           _decks.push_back(Point(First.x + i, First.y, false));
      }
    }
}

Ship::Ship()
{

}

Ship::Ship(const Ship& SomeShip)
{
    for (Point p: SomeShip.getPoints())
    {
        _points.push_back(p);
    }
    for (Point p: SomeShip.getDecks())
    {
        _decks.push_back(p);
    }
}

bool Ship::IsBroken() const
{
    for (Point p : _decks)
    {
        if (!p.fill) return false;
    }

    return true;
}

