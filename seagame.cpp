#include "seagame.h"
#include <QTime>
#include <QTableWidget>

void SeaGame::initializeFields()
{
    //создадим пустые поля
    PlayerField = new SeaField(_i,_j);
    ComputerField = new SeaField(_i,_j);

    //расстановим корабли компьютера
    int seed = QTime::currentTime().msecsSinceStartOfDay();
    qsrand(seed);

    //4-палубные
    SetShip(4);

    //3-палубные
    SetShip(3);
    SetShip(3);

    //2-палубные
    SetShip(2);
    SetShip(2);
    SetShip(2);

    //1-палубные
    SetShip(1);
    SetShip(1);
    SetShip(1);
    SetShip(1);

}

void SeaGame::SetShip(const int lenght)
{
    while (true)
    {
        int RandomNumber = qrand() % 2;
        bool horizont = (RandomNumber) == 1 ? true : false;
        int pos = qrand() % (_i*_j);

        Point FirstPoint(pos);
        Ship SomeShip(FirstPoint, horizont, lenght);

        if (!ComputerField->CheckShip(SomeShip)) continue;
        ComputerField->SetShip(SomeShip);

        return;
    };

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
    Ship FindShip;

    if (!ComputerField->FindShipByPoint(In,FindShip))
        return;

    //Point& deck = FindShip.getDeckByPoint(In);

    //пробиваем палубу
    //deck.fill = true;

}

bool SeaGame::EndOfGame() const
{
    for (Ship s : PlayerField->getShip())
    {
        if (!(s.IsBroken())) return false;
    }

    for (Ship s : ComputerField->getShip())
    {
        if (!(s.IsBroken())) return false;
    }

    return true;
}

