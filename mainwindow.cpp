#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    game(new SeaGame())
{
    ui->setupUi(this);

    //right field
    QObject::connect(ui->tableWidgetRight,SIGNAL(cellClicked(int,int)),game,SLOT(PlayerClick(int,int)));
    QObject::connect(ui->tableWidgetRight,SIGNAL(cellDoubleClicked(int,int)),game,SLOT(PlayerDoubleClick(int,int)));
    //left field
    QObject::connect(ui->tableWidgetLeft, SIGNAL(cellClicked(int,int)), game, SLOT(PlayerClick(int,int)));
    //refresh form
    QObject::connect(game, SIGNAL(RepaintForm()), this, SLOT(repaint()));

    PrepareForm();
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

    game->initializeFields();
    GameStarted = true;
    repaint();
}

void MainWindow::on_action_Start_game_triggered()
{
    //game->GameLoop();
}

void MainWindow::PrepareForm()
{
    for (int row = 0; row < ui->tableWidgetLeft->rowCount(); row++)
       for (int col = 0; col < ui->tableWidgetLeft->columnCount(); col++)
        {
            ui->tableWidgetLeft->setItem(row,col,new QTableWidgetItem(0));
            ui->tableWidgetRight->setItem(row,col, new QTableWidgetItem(0));
        }

    ui->tableWidgetLeft->verticalHeader()->setVisible(true);
    ui->tableWidgetLeft->horizontalHeader()->setVisible(true);
    ui->tableWidgetRight->verticalHeader()->setVisible(true);
    ui->tableWidgetRight->horizontalHeader()->setVisible(true);
}

void MainWindow::repaint()
{
   ui->statusbar->showMessage(!GameStarted ? "Click 'New Game'" : "");
   ui->menuNewGame->actions().at(1)->setEnabled(GameStarted && game->PlayerIsReady());
   ui->tableWidgetRight->setEnabled(GameStarted);

   QColor black = QColor(Qt::black);
   QColor white = QColor(Qt::white);
   QColor red   = QColor(Qt::red);
   QColor blue = QColor(Qt::blue);

    for (int row = 0; row < game->GetRowCount(); row++)
        for (int col = 0; col < game->GetColumnCount(); col++)
        {
            const Point& p = game->GetPlayerField()->getPoint(row, col);

            //show computer ships
            QColor itemColor = white;

            for (Point& Shots : game->GetComputerField()->getShots())
            {
              if (p == Shots) { itemColor = blue;}
            }

            Ship* FindComputerShip;
            if (game->GetComputerField()->FindShipByPoint(p,FindComputerShip))
            {
                if (FindComputerShip->getDeckByPoint(p).fill == 1)
                    itemColor = red;
                else
                    itemColor = black;
            }

            ui->tableWidgetLeft->item(row, col)->setBackgroundColor(itemColor);

            //show players ships
            itemColor = white;
			
            for (Point& Shots : game->GetPlayerField()->getShots())
            {
              if (p == Shots) { itemColor = blue;}
            }

            Ship* FindPlayerShip;
            if (game->GetPlayerField()->FindShipByPoint(p, FindPlayerShip))
			{
                if (FindPlayerShip->getDeckByPoint(p).fill == 1)
					itemColor = red;
				else
					itemColor = black;
			}

			ui->tableWidgetRight->item(row, col)->setBackgroundColor(itemColor);

		}

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


