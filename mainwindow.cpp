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
    if (game->GameInit)
    {
        QMessageBox::StandardButton ret = QMessageBox::question(this,"",
                                         "The Sea Game is now play.Close it and play again?",
                                         QMessageBox::Yes | QMessageBox::No);

        if (ret == QMessageBox::No) return;

        game->clearFields();
        game->GameInit = false;
        game->GameStarted = false;
        repaint();
    }

    game->initializeFields();
    game->GameInit = true;
    repaint();
}

void MainWindow::on_action_Start_game_triggered()
{
    game->GameStarted = !game->GameStarted;
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

    ui->tableWidgetLeft->verticalHeader()->setVisible(true);
    ui->tableWidgetLeft->horizontalHeader()->setVisible(true);
    ui->tableWidgetRight->verticalHeader()->setVisible(true);
    ui->tableWidgetRight->horizontalHeader()->setVisible(true);
}

void MainWindow::repaint()
{
   ui->statusbar->showMessage(!game->GameInit ? "Click 'New Game'" : "");
   ui->menuNewGame->actions().at(1)->setEnabled(game->GameInit && game->PlayerIsReady());

   ui->tableWidgetLeft->setEnabled(game->GameStarted);
   ui->tableWidgetRight->setEnabled(game->GameInit);
   ui->pushButtonRight->setEnabled(game->GameInit && !game->GameStarted);

   QPixmap past  = QPixmap(":/images/past.jpg");
   QPixmap cross = QPixmap(":/images/cross.jpg");
   QPixmap empty = QPixmap(cross.width(),cross.height());
   empty.fill();
   QPixmap ship  = QPixmap(cross.width(),cross.height());
   ship.fill(QColor("black"));

   const SeaField* ComputerField = game->GetComputerField();
   const SeaField* PlayerField   = game->GetPlayerField();

    for (int row = 0; row < game->GetRowCount(); row++)
        for (int col = 0; col < game->GetColumnCount(); col++)
        {
            const Point& p = ComputerField->getPoint(row, col);

            //show computer ships
            QPixmap itemColor = empty;

            for (Point& p_shot : ComputerField->getShots())
            {
              if (p == p_shot) { itemColor = past;}
            }

            Ship* FindComputerShip;
            if (ComputerField->FindShipByPoint(p,FindComputerShip))
            {               
                if (FindComputerShip->getDeckByPoint(p).fill == 1)
                    itemColor = cross; //deck is broken
                else
                   {
                    if (game->EndOfGame())
                        itemColor = ship;
                   }
            }

           ui->tableWidgetLeft->item(row,col)->setIcon(QIcon(itemColor));

            //show players ships
            itemColor = empty;
			
            for (Point& Shots : PlayerField->getShots())
            {
              if (p == Shots) { itemColor = past;}
            }

            Ship* FindPlayerShip;
            if (PlayerField->FindShipByPoint(p, FindPlayerShip))
			{
                if (FindPlayerShip->getDeckByPoint(p).fill == 1)
                    itemColor = cross;
				else
                    itemColor = ship;
			}

            ui->tableWidgetRight->item(row, col)->setIcon(itemColor);
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


void MainWindow::on_pushButtonRight_clicked()
{
    const_cast<SeaField*>(game->GetPlayerField())->clear();
    game->SetPlayerShip();
    repaint();
}


