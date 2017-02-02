#include "seafield.h"

SeaField::SeaField(int _i, int _j):i(_i),j(_j)
{
    _Field = new Field;
    _ships = new QMultiMap<int, Ship>;
    _shots = new QVector<Point>;
    clear();
}

SeaField::~SeaField()
{
    delete[] _ships;
    delete _Field;
}

Point SeaField::operator[](const int _i) const
{
    int Row = _i / i;
    int Col = _i % j;
    Point tmp = _Field->at(Row).at(Col);
    return tmp;
}

Point SeaField::operator[](const Point& p) const
{
    Point tmp = _Field->at(p.x).at(p.y);
    return tmp;
}

//управление точками

void SeaField::setPoint(const Point& p)
{
    _Field->operator[](p.x).operator[](p.y) = p;
}
//index = row * GetRowCount() + column
void SeaField::setPoint(const int index, bool fill)
{
    setPoint(Point(index, fill));
}

bool SeaField::checkPoint(const Point& p, bool onlyHorizontal, bool onlyVertical) const
{
    if (p.x > 0 && !onlyVertical)
    {
        if (this->operator[](Point(p.x - 1, p.y    )).fill) return false;
    }
    if (p.y > 0 && !onlyHorizontal)
    {
        if (this->operator[](Point(p.x,     p.y - 1)).fill) return false;
    }
    if (p.x > 0 && p.y > 0 && !(onlyHorizontal || onlyVertical))
    {
     if (this->operator[](Point(p.x - 1, p.y - 1)).fill) return false;
    }
    if (p.x > 0 && p.y < j-1 && !(onlyHorizontal || onlyVertical))
    {
        if (this->operator[](Point(p.x - 1, p.y + 1)).fill) return false;
    }
    if (p.x < i-1 && p.y > 0 && !(onlyHorizontal || onlyVertical))
    {
        if (this->operator[](Point(p.x + 1, p.y - 1)).fill) return false;
    }
    if (p.x < i-1 && !onlyVertical)
    {
        if (this->operator[](Point(p.x + 1, p.y    )).fill) return false;
    }
    if (p.y < j-1 && !onlyHorizontal)
    {
        if (this->operator[](Point(p.x ,    p.y + 1)).fill) return false;
    }
    if (p.x < i-1 && p.y < j-1 && !(onlyHorizontal || onlyVertical))
    {
        if (this->operator[](Point(p.x + 1, p.y + 1)).fill) return false;
    }

    return true;

}

const Point& SeaField::getPoint(int row, int col) const
{
    return _Field->at(row).at(col);
}

int SeaField::getPointCount(bool isFill) const
{
    int _count = 0;

    for (QVector<Point> &row: *_Field)
    {
        for(Point &p: row)
        {
            if (p.fill && isFill)
                ++_count;
        }
    }

    return _count;
}

//управление кораблями

bool SeaField::CheckShip(const Ship& someShip) const
{
    //1.проверка выхода корабля за пределы поля игры
    int maxRowPosition = (i-someShip.getLenght());
    int maxColPosition = (j-someShip.getLenght());

    if ((someShip.horizont()) && someShip.getFirstPoint().x > maxRowPosition) return false;
    else
    if (someShip.getFirstPoint().y > maxColPosition) return false;

    //2.проверка на пересечение с другими кораблями. Между кораблями должна быть одна точка
    for (const Point& p : someShip.getPoints())
    {
        if (this->operator[](p).fill) return false;
        if (!checkPoint(p)) return false;
    }

    return true;
}

void SeaField::SetShip(const Ship& someShip)
{
    for(Point p : someShip.getPoints())
    {
        setPoint(p);
    }

 scanShips();

}

QList<Ship> SeaField::getShip(const int lenghtOfShip) const
{
    if (lenghtOfShip == 0)
    {
        return _ships->values();
    }
    return _ships->values(lenghtOfShip);
}

QMultiMap<int,Ship>* SeaField::getShips(const int lenghtOfShip)
{
    if (lenghtOfShip == 0)
    {
        return _ships;
    }
        return _ships;
}

bool SeaField::FindShipByPoint(const Point& p, Ship* &Result) const
{
    for (const Ship &ship: *_ships)
    {
        for (const Point &_p : ship.getPoints())

            if (p == _p)
            {
               Result = const_cast<Ship*>(&ship);
               return true;
            }
    }

    return false;

}

uint SeaField::getShipCount() const
{
    return _ships->count();
}

QVector<Point> SeaField::getArroundPoint(const Ship& ship) const
{

    QVector<Point> ArroundPoints;

    for (const Point& p : ship.getPoints())
    {
        for (const Point& p2 : p.getArroundPoints(i,j))
        {
            ArroundPoints.push_back(p2);
        }
    }

    return ArroundPoints;

}

void SeaField::scanShips()
{
   _ships->clear();

   Point FirstPoint;
   int filled = 0;

   //обход поля по горизонтали (слева-направо)
   for (int x = 0; x < i; ++x)
   {

       if (filled)
       {
           _ships->insert(filled,Ship(FirstPoint,false,filled));
           filled = 0;
       }

       for (int y = 0; y < j; ++y)
       {
           Point p = _Field->at(x).at(y);

           if (!p.fill && filled == 0 ) continue;

           if (!p.fill)
           {
               if (filled)
               {
                     _ships->insert(filled,Ship(FirstPoint,false,filled));
               }
               filled = 0;
               continue;
           }

           if (p.fill && checkPoint(p,true,false))
           {
               if (!filled)
               FirstPoint = p;
               filled++;
           }
       }

   }

   //не забудем последний по горизонтали
   if (filled)
   {
       _ships->insert(filled,Ship(FirstPoint,false,filled));
       filled = 0;
   }

   //обход поля по вертикали (сверху - вниз)
   for (int x = 0; x < i; ++x)
   {

       if (filled)
       {
           _ships->insert(filled,Ship(FirstPoint,true,filled));
           filled = 0;
       }

       for (int y = 0; y < j; ++y)
       {
           Point p = _Field->at(y).at(x);

           if (!p.fill && filled == 0 ) continue;
           //пропускаем однопалубные корабли, они были добавлены при первом проходе выше
           if (p.fill && checkPoint(p)) continue;

           if (!p.fill)
           {
               if (filled)
               {
                    _ships->insert(filled,Ship(FirstPoint,true,filled));
               }
               filled = 0;
               continue;
           }

           if (p.fill && checkPoint(p,false,true))
           {
               if (!filled)
               FirstPoint = p;
               filled++;
           }
       }

   }

   //не забудем последний по вертикали
   if (filled)
   {
       _ships->insert(filled,Ship(FirstPoint,true,filled));
       filled = 0;
   }

   //проход по найденным кораблям
   for (QMultiMap<int,Ship>::iterator it = _ships->begin(); it != _ships->end(); it++)
   {
       Ship ship = it.value();

       if (!ship.IsBroken()) continue;

       //если корабль потоплен, обстреляем соседние точки
        for (Point& p : getArroundPoint(ship))
           {
               QVector<Point>::iterator FindPoint = std::find(_shots->begin(), _shots->end(), p);

               if (FindPoint == _shots->end())
                   _shots->push_back(p);

           }
   }

}

void SeaField::clear()
{
   //clear and create new Point
   _Field->clear();
   QVector<Point> tmp;

    for (int Row = 0; Row < i; ++Row)
    {
        for (int Col = 0; Col < j; ++Col)
        {
            tmp.push_back(Point(Row,Col));
        }
        _Field->push_back(tmp);
        tmp.clear();
    }

    //ships
    scanShips();

   _shots->clear();

}

bool SeaField::isNewShots(const Point& p)
{
    QVector<Point>::iterator it = std::find(_shots->begin(),_shots->end(),p);
    if (it != _shots->end())
    {
        return false;
    }

        _shots->push_back(p);
        return true;
}
