#include "seagame.h"
#include <QTime>
#include <QTableWidget>

   SeaGame::~SeaGame()
{
	ComputerField->getShips()->clear();
	PlayerField->getShips()->clear();
	GameStarted = false;
}

void SeaGame::initializeFields()
{
    //создадим пустые поля
    PlayerField = new SeaField(_i,_j);
    ComputerField = new SeaField(_i,_j);

    //расстановим корабли компьютера
    int seed = QTime::currentTime().msecsSinceStartOfDay();
    qsrand(seed);

    //4-палубные
    SetShip(4, ComputerField);

    //3-палубные
    SetShip(3, ComputerField);
	SetShip(3, ComputerField);

    //2-палубные
    SetShip(2, ComputerField);
	SetShip(2, ComputerField);
	SetShip(2, ComputerField);

    //1-палубные
	SetShip(1, ComputerField);
	SetShip(1, ComputerField);
	SetShip(1, ComputerField);
	SetShip(1, ComputerField);
}

void SeaGame::SetShip(const int lenght, SeaField* To)
{
    while (true)
    {
        int RandomNumber = qrand() % 2;
        bool horizont = (RandomNumber) == 1 ? true : false;
        int pos = qrand() % (_i*_j);

        Point FirstPoint(pos);
        Ship SomeShip(FirstPoint, horizont, lenght);

        if (!To->CheckShip(SomeShip)) continue;
        To->SetShip(SomeShip);

        return;
    };

}

void SeaGame::SetPlayerShip()
{
	SetShip(4, PlayerField);
	
	SetShip(3, PlayerField);
	SetShip(3, PlayerField);
	
	SetShip(2, PlayerField);
	SetShip(2, PlayerField);
	SetShip(2, PlayerField);

	SetShip(1, PlayerField);
	SetShip(1, PlayerField);
	SetShip(1, PlayerField);
	SetShip(1, PlayerField);

	PlayerField->scanShips();
}

bool SeaGame::PlayerIsReady() const
{
    //предполагаем, что компьютер уже расставил свои корабли, в таком случае, количество заполненных
    //точек на полях должно быть одинаково, просто проверим это.
    int ComputerPointCount = ComputerField->getPointCount();
    int PlayerPointCount   = PlayerField->getPointCount();

    if (ComputerPointCount != PlayerPointCount) return false;

    int PlayerShipCount   = PlayerField->getShipCount();
    int ComputerShipCount = ComputerField->getShipCount();

    return (PlayerShipCount == ComputerShipCount);

}

//слоты поля игрока
void SeaGame::PlayerClick(int row, int column)
{    
    QObject *obj = QObject::sender();
    QString senderName = qobject_cast<QTableWidget*>(obj)->objectName();

    //клик по полю игрока
    if (senderName == "tableWidgetRight")
    {
        Point p(PlayerField->operator []( row * GetRowCount() + column));
        p.fill = !p.fill;
        PlayerField->setPoint(p);
        PlayerField->scanShips();
    }

    //клик по полю компьютера
    if (senderName == "tableWidgetLeft")
    {
        Point p(PlayerField->operator []( row * GetRowCount() + column));
        PlayerShoot(p);
    }

    RepaintForm();
}

void SeaGame::GameLoop()
{
    while (!EndOfGame())
    {



    }

}

void SeaGame::PlayerShoot(const Point& In)
{
    Ship* FindShip;

    if (!ComputerField->FindShipByPoint(In,FindShip))
        return;
	//пробиваем палубу
	FindShip->setDeckByPoint(In, true);				
}

bool SeaGame::EndOfGame() const
{
    for (const Ship& s : PlayerField->getShip())
    {
        if (!(s.IsBroken())) return false;
    }

    for (const Ship& s : ComputerField->getShip())
    {
        if (!(s.IsBroken())) return false;
    }

    return true;
}

