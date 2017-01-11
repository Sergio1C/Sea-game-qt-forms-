#ifndef SHIPS_H
#define SHIPS_H
#include <QVector>
#include "points.h"

class Ship
{

public:
    Ship();
    Ship(const Point& First, bool horizont, const int lenght);
    Ship(const Ship&);
    ~Ship(){}

    Ship& operator=(const Ship&);

    bool horizont() const
    {
        if (_points.count() < 2) return false;
        return (_points.at(0).y == _points.at(1).y);
    }
    bool IsBroken() const;

    const Point& getFirstPoint() const
    {
        return _points.at(0);
    }

    const QVector<Point>& getPoints() const
    {
        return _points;
    }

    const QVector<Point>& getDecks() const
    {
        return _decks;
    }

    Point& getDeckByPoint(Point& p)
    {
        QVector<Point>::iterator FindPoint = std::find(_decks.begin(), _decks.end(), p);
        return *FindPoint;
    }

    int getLenght() const
    {
      return _points.count();
    }


private:
    QVector<Point> _points; //точки на поле,определяют местоположение
    QVector<Point> _decks;  //палубы, те же точки, но определяют повреждения корабля

};

#endif // SHIPS_H
