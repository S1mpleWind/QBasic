#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdLineEdit_editingFinished()
{
    QString cmd = ui->cmdLineEdit->text();
    ui->cmdLineEdit->setText("");

    ui->CodeDisplay->append(cmd);
}

void MainWindow::on_btnLoadCode_clicked()
{
    qDebug()<<"LOAD";
}

void MainWindow::on_btnRunCode_clicked()
{
     qDebug()<<"RUN";
}

void MainWindow::on_btnClearCode_clicked()
{
    qDebug()<<"CLEAR";

    //reset ui
    ui->CodeDisplay->setText("");
    ui->treeDisplay->setText(""); // TODO: should clear the datastructure and update automatically

    //TODO:clear the program structure / sytax tree

}
