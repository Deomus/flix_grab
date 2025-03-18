#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../waiting_spinner.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    WaitingSpinner* spinner = new WaitingSpinner(this);

    spinner->setFixedSize(50, 50);
   
    spinner->setColor(QColor(81, 4, 71));


    spinner->start(); // gets the show on the road!

}

MainWindow::~MainWindow()
{
    delete ui;
}
