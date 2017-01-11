#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    game(nullptr)
{
    ui->setupUi(this);

    /*signalMapper = new QSignalMapper(this);

    connect(ui->tableWidgetLeft, SIGNAL(cellClicked(int,int)), signalMapper, SLOT(map(int,int)));
    signalMapper->setMapping(ui->tableWidgetLeft,"tableWidgetLeft");

    connect(ui->tableWidgetRight, SIGNAL(cellClicked(int,int)), signalMapper, SLOT(map(int,int)));
    signalMapper->setMapping(ui->tableWidgetRight,"tableWidgetRight");
    */
    repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

    game->~SeaGame();
    repaint();
    return;

    }

    //player is ready.game started
    if (game != 0 && game->PlayerIsReady() && !GameStarted)
    {
        GameStarted = true;
        //GameLoop();
        return;
    }

    PrepareForm();

    game = new SeaGame();

    //right field
    //connect(signalMapper, SIGNAL(mapped(const QString&)), game, SLOT(PlayerClick(const QString&));
    QObject::connect(ui->tableWidgetRight,SIGNAL(cellClicked(int,int)),game,SLOT(PlayerClick(int,int)));
    QObject::connect(ui->tableWidgetRight,SIGNAL(cellDoubleClicked(int,int)),game,SLOT(PlayerDoubleClick(int,int)));
    //left field
    QObject::connect(ui->tableWidgetLeft, SIGNAL(cellClicked(int,int)), game, SLOT(PlayerClick(int,int)));

    QObject::connect(game, SIGNAL(RepaintForm()), this, SLOT(repaint()));
    game->RepaintForm();


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
   if (game == nullptr)
    {   ui->statusbar->showMessage("click new game");
        return;
    }

   ui->tableWidgetRight->setEnabled(!GameStarted);

   QColor black = QColor(Qt::black);
   QColor white = QColor(Qt::white);
   QColor red   = QColor(Qt::red);

    for (int row = 0; row < game->GetRowCount(); row++)
        for (int col = 0; col < game->GetColumnCount(); col++)
        {
            //fill players ships
            Point PlayerPoint = game->GetPlayerField()->getPoint(row, col);
            QColor itemColor = PlayerPoint.fill ? black:white;
            ui->tableWidgetRight->item(row,col)->setBackgroundColor(itemColor);

            Ship FindPlayerShip;
            if (game->GetPlayerField()->FindShipByPoint(PlayerPoint,FindPlayerShip))
            {
                if (FindPlayerShip.getDeckByPoint(PlayerPoint).fill)
                    ui->tableWidgetRight->item(row,col)->setBackgroundColor(red);

            }

            //fill computer ships
            Point ComputerPoint = game->GetComputerField()->getPoint(row, col);
            QColor itemColor2 = ComputerPoint.fill ? black:white;
            ui->tableWidgetLeft->item(row,col)->setBackgroundColor(itemColor2);

            Ship FindComputerShip;
            if (game->GetComputerField()->FindShipByPoint(ComputerPoint,FindComputerShip))
            {
                if (FindComputerShip.getDeckByPoint(ComputerPoint).fill)
                    ui->tableWidgetRight->item(row,col)->setBackgroundColor(red);

            }
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

void MainWindow::on_tableWidgetLeft_cellClicked(int row, int column)
{

    repaint();
}

void MainWindow::on_tableWidgetRight_cellClicked(int row, int column)
{
   repaint();
}

void MainWindow::on_pushButton_clicked()
{
    SeaField* test =  const_cast<SeaField*>(game->GetComputerField());
    test->scanShips();
}
