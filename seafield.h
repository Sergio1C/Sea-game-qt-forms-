#ifndef SEAFIELD_H
#define SEAFIELD_H

#include "points.h"
#include "ships.h"
#include <QVector>
#include <QMultiMap>

class SeaField
{

public:

    using Field = QVector<QVector<Point>>; //псевдоним поля для игры

    SeaField(int _i, int _j);

    ~SeaField();

    Point operator[](const int _i) const;

    Point operator[](const Point&) const;

    //управление точками
    void setPoint(const Point&);

    //index = row * GetRowCount() + column
    void setPoint(const int index, bool fill = true);

    bool checkPoint(const Point&, bool onlyHorizontal = 0, bool onlyVertical = 0) const;

    const Point& getPoint(int row, int col) const;

    int getPointCount(bool isFill = true) const;

    //управление кораблями
    bool CheckShip(const Ship&) const;

    void SetShip(const Ship& someShip);

    QList<Ship> getShip(const int lenghtOfShip = 0) const;

    QMultiMap<int,Ship>* getShips(const int lenghtOfShip = 0);

    bool FindShipByPoint(const Point& p, Ship* &Result) const;

    uint getShipCount(bool OnlyLives = false) const;

    //возвращает близлижайщие точки около корабля по границе с ним
    QVector<Point> getArroundPoint(const Ship& someShip) const;

    void scanShips();

    void clear();

    bool isNewShots(const Point &);

    QVector<Point>& getShots() const { return *_shots; }

    const int i;          //кол-во строк
    const int j;          //кол-во столбцов
    Field* _Field;        //игровое поле

private:
     QMultiMap<int,Ship>* _ships;           //массив кораблей на поле
     QVector<Point>* _shots;                //массив обстреленных точек

};




#endif // SEAFIELD_H
