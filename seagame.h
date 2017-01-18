#ifndef SEAGAME_H
#define SEAGAME_H
#include <QVector>
#include <QObject>
#include <QDebug>
#include <QMultiMap>

#include "points.h"
#include "ships.h"

#define SHIP_COUNT              10
#define FOUR_DECK_SHIP_COUNT    1
#define THREE_DECK_SHIP_COUNT   2
#define TWO_DECK_SHIP_COUNT     3
#define ONE_DECK_SHIP_COUNT     4

static bool GameStarted = false;

class SeaField
{

public:

    using Field = QVector<QVector<Point>>; //псевдоним поля для игры

    SeaField(int _i, int _j):i(_i),j(_j)
    {
        QVector<Point> tmp;
		_Field = new Field;
		
		for (int Row = 0; Row < i; ++Row)
        {
            for (int Col = 0; Col < j; ++Col)
            {
                tmp.push_back(Point(Row,Col));
            }
            _Field->push_back(tmp);
            tmp.clear();
        }    

		_ships = new QMultiMap<int, Ship>;

    }

    Point operator[](const int _i) const
    {
        int Row = _i / i;
        int Col = _i % j;
        Point tmp = _Field->at(Row).at(Col);
        return tmp;
    }
    Point operator[](const Point& p) const
    {
        Point tmp = _Field->at(p.x).at(p.y);
        return tmp;
    }

    //управление точками

    void setPoint(const Point& p)
    {
        //_Field[p.x][p.y] = p;
		_Field->operator[](p.x).operator[](p.y) = p;
    }
    //index = row * GetRowCount() + column
    void setPoint(const int index, bool fill = true)
    {
        setPoint(Point(index, fill));
    }

    bool checkPoint(const Point& p, bool onlyHorizontal = 0, bool onlyVertical = 0) const
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

    const Point& getPoint(int row, int col) const
    {
		return _Field->at(row).at(col);
    }

    int getPointCount(bool isFill = true) const
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

    bool CheckShip(const Ship& someShip) const
    {
        //1.проверка выхода корабля за пределы поля игры
        int maxRowPosition = (i-someShip.getLenght());
        int maxColPosition = (j-someShip.getLenght());

        if ((someShip.horizont()) && someShip.getFirstPoint().x > maxRowPosition) return false;
        else
        if (someShip.getFirstPoint().y > maxColPosition) return false;

        //2.проверка на пересчение с другими кораблями. Между кораблями должна быть одна точка
        for (const Point& p : someShip.getPoints())
        {
            if (this->operator[](p).fill) return false;
            if (!checkPoint(p)) return false;
        }

        return true;
    }

    void SetShip(const Ship& someShip)
    {
        for(Point p : someShip.getPoints())
        {
            setPoint(p);
        }

     scanShips();

    }

    const QList<Ship>& getShip(const int lenghtOfShip = 0) const
	{
		if (lenghtOfShip == 0)
		{
            return _ships->values();
		}
        return _ships->values(lenghtOfShip);
	}

	QMultiMap<int,Ship>* getShips(const int lenghtOfShip = 0)
	{
		if (lenghtOfShip == 0)
		{
			return _ships;
		}		 
			return _ships;
	}

    bool FindShipByPoint(const Point& p, Ship* &Result) const
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

    uint getShipCount() const
    {
        return _ships->count();
    }

    void scanShips()
    {
       _ships->clear();

       Point FirstPoint;
       int filled = 0;

       //обход поля по горизонтали (слева-направо)
       for (int x = 0; x < i; ++x)
       {

           if (filled)
           {
               _ships->insert(filled,Ship(FirstPoint,true,filled));
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
                         _ships->insert(filled,Ship(FirstPoint,true,filled));
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
           _ships->insert(filled,Ship(FirstPoint,true,filled));
           filled = 0;
       }

       //обход поля по вертикали (сверху - вниз)
       for (int x = 0; x < i; ++x)
       {

           if (filled)
           {
               _ships->insert(filled,Ship(FirstPoint,false,filled));
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
           _ships->insert(filled,Ship(FirstPoint,false,filled));
           filled = 0;
       }

    }

    int i;          //кол-во строк
    int j;          //кол-во столбцов
    Field* _Field;  //игровое поле

protected:
     int activeNumberOfShip; // номер текущего корабля. Используется при начальной нумерации кораблей

private:
     QMultiMap<int,Ship>* _ships; //массив кораблей на поле


};

class SeaGame: public QObject
{
    Q_OBJECT

    public:
        SeaGame(int i = 10, int j = 10): _i(i), _j(j)
    {
        initializeFields();
    }
		~SeaGame();

        const SeaField* GetComputerField() const { return ComputerField; }
        const SeaField* GetPlayerField()   const { return PlayerField; }

        int GetRowCount() const { return _i;}
        int GetColumnCount() const { return _j;}

		void SetPlayerShip();
        bool PlayerIsReady() const;

	
        void GameLoop();
        bool EndOfGame() const;

        void PlayerShoot(const Point&);
        void ComputerShoot(const Point&);
       
        void initializeFields();
		void clearFields();
		void SetShip(const int lenght, SeaField* To);

    private:
        const int _i,_j;           //размерность горизонтали и вертикали

        SeaField* PlayerField;     //поле игрока
        SeaField* ComputerField;   //поле компьютера

   signals:
        void RepaintForm();

  public slots:
        void PlayerClick(int row, int column);

};


#endif // SEAGAME_H
