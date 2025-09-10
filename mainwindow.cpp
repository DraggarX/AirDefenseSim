// mainwindow.cpp
#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), arrowAngle(0)
{
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // Левый радар + включение
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftRadar = new RadarWidget(this);

    QLabel *leftLabel = new QLabel("Радар", this);
    QCheckBox *leftToggle = new QCheckBox("Включить", this); leftToggle->setChecked(true);
    leftLayout->addWidget(leftRadar);
    leftLayout->addWidget(leftLabel, 0, Qt::AlignCenter);
    leftLayout->addWidget(leftToggle, 0, Qt::AlignCenter);

    // Правый радар + включение
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightRadar = new RadarInstallationWidget(this);
    rightRadar->setCentralAngle(30); // угол между радиусами фиксируется здесь

    QLabel *rightLabel = new QLabel("Радар-установка", this);
    QCheckBox *rightToggle = new QCheckBox("Включить", this); rightToggle->setChecked(true);
    rightLayout->addWidget(rightRadar);
    rightLayout->addWidget(rightLabel, 0, Qt::AlignCenter);
    rightLayout->addWidget(rightToggle, 0, Qt::AlignCenter);

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 1);

    setCentralWidget(central);

    connect(leftToggle, &QCheckBox::toggled, this, &MainWindow::onLeftRadarToggle);
    connect(rightToggle, &QCheckBox::toggled, this, &MainWindow::onRightRadarToggle);

    // Таймер для анимации стрелки
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateArrow);
    timer->start(30);
}

void MainWindow::updateArrow()
{
    arrowAngle += 2.5f;
    if (arrowAngle > 360) arrowAngle -= 360;
    leftRadar->setArrowAngle(arrowAngle);
}

void MainWindow::onLeftRadarToggle(bool checked)
{
    leftRadar->setEnabledScan(checked);
}
void MainWindow::onRightRadarToggle(bool checked)
{
    rightRadar->setEnabledScan(checked);
}
