#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidget>
#include <QSignalMapper>
#include <seagame.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SeaGame* game;
    void PrepareForm();

private slots:
    void on_actionCancel_triggered();

    void on_actionNew_game_triggered();

    void on_pushButton_clicked();

    void on_pushButtonRight_clicked();

    void on_action_Start_game_triggered();

public slots:
    void repaint();


};

#endif // MAINWINDOW_H
