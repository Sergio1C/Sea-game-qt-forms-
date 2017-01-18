#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    game(new SeaGame())
{
    ui->setupUi(this);
    repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
	delete game;
}

//top menu
void MainWindow::on_actionCancel_triggered()
{
    close();
}

void MainWindow::on_actionNew_game_triggered()
{
    if (GameStarted)
    {
    QMessageBox::StandardButton ret = QMessageBox::question(this,"",
                                     "The Sea Game is now play.Close it and play again?",
                                     QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::No) return;

	game->clearFields();
	GameStarted = false;
	repaint();
    return;

    }

    //player is arranging ships...game started
    if (game->PlayerIsReady() && !GameStarted)
    {
        GameStarted = true;
        //GameLoop();
        return;
    }

    PrepareForm();

    //right field
    QObject::connect(ui->tableWidgetRight,SIGNAL(cellClicked(int,int)),game,SLOT(PlayerClick(int,int)));
    QObject::connect(ui->tableWidgetRight,SIGNAL(cellDoubleClicked(int,int)),game,SLOT(PlayerDoubleClick(int,int)));
   
	//left field
    QObject::connect(ui->tableWidgetLeft, SIGNAL(cellClicked(int,int)), game, SLOT(PlayerClick(int,int)));

    QObject::connect(game, SIGNAL(RepaintForm()), this, SLOT(repaint()));
	repaint();
}

void MainWindow::PrepareForm()
{
    for (int row = 0; row < ui->tableWidgetLeft->rowCount(); row++)
       for (int col = 0; col < ui->tableWidgetLeft->columnCount(); col++)
        {
            ui->tableWidgetLeft->setItem(row,col,new QTableWidgetItem(0));
            ui->tableWidgetRight->setItem(row,col, new QTableWidgetItem(0));
        }
}

void MainWindow::repaint()
{
   if (!GameStarted)
    {   ui->statusbar->showMessage("Arrange your ships");
        return;
    }

   ui->tableWidgetRight->setEnabled(!GameStarted);

   QColor black = QColor(Qt::black);
   QColor white = QColor(Qt::white);
   QColor red   = QColor(Qt::red);

    for (int row = 0; row < game->GetRowCount(); row++)
        for (int col = 0; col < game->GetColumnCount(); col++)
        {
            //show players ships
			QColor itemColor = white;
			const Point& PlayerPoint = game->GetPlayerField()->getPoint(row, col);
			
			Ship* FindPlayerShip;;
			if (game->GetPlayerField()->FindShipByPoint(PlayerPoint, FindPlayerShip))
			{
				if (FindPlayerShip->getDeckByPoint(PlayerPoint).fill == 1)
					itemColor = red;
				else
					itemColor = black;
			}
			
			ui->tableWidgetRight->item(row, col)->setBackgroundColor(itemColor);

            //fill computer ships
			itemColor = white;			
			const Point& ComputerPoint = game->GetComputerField()->getPoint(row, col);

            Ship* FindComputerShip;
            if (game->GetComputerField()->FindShipByPoint(ComputerPoint,FindComputerShip))
            {            				
				if (FindComputerShip->getDeckByPoint(ComputerPoint).fill == 1)
					itemColor = red;
				else
					itemColor = black;
            }
        
			ui->tableWidgetLeft->item(row, col)->setBackgroundColor(itemColor);
		}

        QString textButton = (game->PlayerIsReady() && !GameStarted)? "Start" : "New game";
        ui->menuNewGame->actions().at(0)->setText(textButton);

        //Status bar:
        QString textStatusBar = "Arrange your ships: ";
        if (!game->PlayerIsReady())
        {
            //4-deck
            if (game->GetPlayerField()->getShip(4).count() != FOUR_DECK_SHIP_COUNT)
                textStatusBar += "4-deck ";
            //3-deck
            if (game->GetPlayerField()->getShip(3).count() != THREE_DECK_SHIP_COUNT)
                textStatusBar += "3-deck ";
            //2-deck
            if (game->GetPlayerField()->getShip(2).count() != TWO_DECK_SHIP_COUNT)
                textStatusBar += "2-deck ";
            //1-deck
            if (game->GetPlayerField()->getShip(1).count() != ONE_DECK_SHIP_COUNT)
                textStatusBar += "1-deck ";

            ui->statusbar->showMessage(textStatusBar);
        }
        else
        ui->statusbar->showMessage("click start game");
}

//tablewidgets
void MainWindow::on_tableWidgetLeft_cellClicked(int row, int column)
{

    repaint();
}

void MainWindow::on_tableWidgetRight_cellClicked(int row, int column)
{
   repaint();
}

//bottom buttons
void MainWindow::on_pushButton_clicked()
{
    SeaField* test =  const_cast<SeaField*>(game->GetComputerField());
    test->scanShips();
}

void MainWindow::on_pushButtonRight_clicked()
{
	if (game == nullptr)
	{
		return;
	}
	else
	{
		if (game->GetPlayerField()->getShipCount() == 0)
		{
			game->SetPlayerShip();
			repaint();
		}		
	}
}
