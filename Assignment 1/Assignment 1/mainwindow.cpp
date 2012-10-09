#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
  * These 4 slots are responsible for signaling GLPanel's changeMode(Mode) slot.
  */
void MainWindow::on_actionRectangle_triggered()
{
    emit(actionTriggered("Rectangle"));
}

void MainWindow::on_actionCircle_triggered()
{
    emit(actionTriggered("Circle"));
}

void MainWindow::on_actionTriangle_triggered()
{
    emit(actionTriggered("Triangle"));
}

void MainWindow::on_actionCustom_triggered()
{
    emit(actionTriggered("Custom"));
}

void MainWindow::disableWidgets(QString str)
{
    if (QString::compare(str, "Rectangle", Qt::CaseInsensitive) == 0)
    {
        ui->rectangleGroup->setEnabled(true);
        ui->triangleGroup->setEnabled(false);
        ui->circleGroup->setEnabled(false);
    } else if (QString::compare(str, "Circle", Qt::CaseInsensitive) == 0)
    {
        ui->rectangleGroup->setEnabled(false);
        ui->triangleGroup->setEnabled(false);
        ui->circleGroup->setEnabled(true);
    } else if (QString::compare(str, "Triangle", Qt::CaseInsensitive) == 0)
    {
        ui->rectangleGroup->setEnabled(false);
        ui->triangleGroup->setEnabled(true);
        ui->circleGroup->setEnabled(false);
    } else if (QString::compare(str, "Custom", Qt::CaseInsensitive) == 0)
    {
        ui->rectangleGroup->setEnabled(false);
        ui->triangleGroup->setEnabled(false);
        ui->circleGroup->setEnabled(false);
    }
}




