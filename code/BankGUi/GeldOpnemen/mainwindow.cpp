#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("#centralWidget{ "
        "border-image:url(:/Project-3-4-Bank/code/BankGUi/GeldOpnemen/imgs/SYMPLE_logo4.png\"); 0 0 0 0 stretch stretch;"
                        "}");
}

MainWindow::~MainWindow()
{
    delete ui;

}
