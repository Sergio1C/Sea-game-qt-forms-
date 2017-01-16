#include "ships.h"

Ship::Ship()
{

}

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

bool Ship::operator==(const Ship& SomeShip)
{
	if (this->getLenght() == 0)
		return false;
	
	for (Point p1 : getPoints())
     {
        for (Point p : SomeShip.getPoints())
        {
           if (p1 != p ) return false;
        }
    }
    return true;
}

bool Ship::operator!=(const Ship& SomeShip)
{
    return !(*this == SomeShip);
}

Point& Ship::getDeckByPoint(const Point& p)
{
	QVector<Point>::iterator FindPoint = std::find(_decks.begin(), _decks.end(), p);
	return *FindPoint;
}

void Ship::setDeckByPoint(const Point& p, bool fill = true)
{
	QVector<Point>::iterator FindPoint = std::find(_decks.begin(), _decks.end(), p);
	
	if (FindPoint != _decks.end())
		FindPoint->fill = fill;
}

Ship Ship::operator=(Ship& SomeShip)
{

    if (*this != const_cast<const Ship&>(SomeShip))
    {
        _points.clear();
		for (Point p : SomeShip.getPoints())
			_points.push_back(std::move(p));

        _decks.clear();
        for (Point p : SomeShip.getDecks())
            _decks.push_back(std::move(p));	
	}

    return * this;
}

bool Ship::IsBroken() const
{
    for (Point p : _decks)
    {
        if (!p.fill) return false;
    }

    return true;
}

