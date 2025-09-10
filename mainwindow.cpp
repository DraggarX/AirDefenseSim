// mainwindow.cpp
#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "MainWindow ctor";

    // Проверочное содержимое
    auto *central = new QWidget(this);
    auto *layout  = new QHBoxLayout(central);

    // Заглушки вместо радаров
    auto *left  = new QFrame();
    auto *right = new QFrame();
    left->setFrameShape(QFrame::Box);
    right->setFrameShape(QFrame::Box);
    left->setMinimumSize(200,200);
    right->setMinimumSize(200,200);

    layout->addWidget(left);
    layout->addWidget(right);
    setCentralWidget(central);

    setWindowTitle("Test Window");
    resize(500,300);
    show();
}
