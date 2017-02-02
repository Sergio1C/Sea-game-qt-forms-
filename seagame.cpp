#include "seagame.h"
#include <QTime>
#include <QTableWidget>

bool SeaGame::GameInit = false;
bool SeaGame::GameStarted = false;

SeaGame::~SeaGame()
{
	delete[] PlayerField;
	delete[] ComputerField;
}

void SeaGame::initializeFields()
{
    //расстановим корабли компьютера
    int seed = QTime::currentTime().msecsSinceStartOfDay();
    qsrand(seed);

    clearFields();

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

    ComputerField->scanShips();

}

void SeaGame::clearFields()
{
    ComputerField->clear();
    PlayerField->clear();
}

void SeaGame::SetShip(const int lenght, SeaField* To)
{
    while (true)
    {
        int rand0_or_1 = qrand() % 2;
        bool horizont = (rand0_or_1) == 1 ? true : false;
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
    //предполагаем, что компьютер уже расставил свои корабли, в таком случае, количество кораблей
    //должно быть одинаково, просто проверим это.
    int PlayerShipCount   = PlayerField->getShipCount();
    int ComputerShipCount = ComputerField->getShipCount();

    return (PlayerShipCount == ComputerShipCount);
}

//слот клика по полям
void SeaGame::PlayerClick(int row, int column)
{    
    QObject *obj = QObject::sender();
    QString senderName = qobject_cast<QTableWidget*>(obj)->objectName();
    
    if (!this->GameInit) return;
    
    //клик по полю игрока
    if (senderName == "tableWidgetRight")
    {
        if (GameStarted)
        {
            return;
        }

        Point p(PlayerField->operator []( row * GetRowCount() + column));
        p.fill = !p.fill;
        PlayerField->setPoint(p);
        PlayerField->scanShips();
    }

    //клик по полю компьютера
    if (senderName == "tableWidgetLeft")
    {               

        Point p(PlayerField->operator []( row * GetRowCount() + column));

        if (PlayerShoot(p))
         {
             PlayerField->scanShips();
         }
        else
         {
            ComputerShoot();
            ComputerField->scanShips();
         }
    }

    RepaintForm();
}

void SeaGame::GameLoop()
{
    while (!EndOfGame())
    {



    }

}

bool SeaGame::PlayerShoot(const Point& In)
{
     if (ComputerField->isNewShots(In))
     {
        Ship* FindShip;
        if  (!ComputerField->FindShipByPoint(In,FindShip))
           {
             return false;
           }

        //пробиваем палубу
        FindShip->setDeckByPoint(In, true);

        return true;

     }
     return true;
}

void SeaGame::ComputerShoot()
{
   while (true)
   {
    int pos = qrand() % (_i*_j);
    Point In(pos);

    if (!PlayerField->isNewShots(In)) continue;

    Ship* Findship;
    if (!PlayerField->FindShipByPoint(In, Findship))
    {
        return;
    }

   Point& Deck = Findship->getDeckByPoint(In);

   if (Deck.fill == true) continue;

   Deck.fill = true;
   }

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

