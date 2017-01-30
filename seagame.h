#ifndef SEAGAME_H
#define SEAGAME_H
#include <QVector>
#include <QObject>
#include <QDebug>
#include <QMultiMap>

#include "seafield.h"

#define SHIP_COUNT              10
#define FOUR_DECK_SHIP_COUNT    1
#define THREE_DECK_SHIP_COUNT   2
#define TWO_DECK_SHIP_COUNT     3
#define ONE_DECK_SHIP_COUNT     4

class SeaGame: public QObject
{
    Q_OBJECT

    public:
        SeaGame(int i = 10, int j = 10): _i(i), _j(j)
    {
            //создадим пустые поля
            PlayerField = new SeaField(_i,_j);
            ComputerField = new SeaField(_i,_j);
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

        bool PlayerShoot(const Point&);
        void ComputerShoot();
       
        void initializeFields();
		void clearFields();
		void SetShip(const int lenght, SeaField* To);

        static bool GameInit;    //флаг - признак инициализации игры
        static bool GameStarted; //флаг - признак старта игры

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
