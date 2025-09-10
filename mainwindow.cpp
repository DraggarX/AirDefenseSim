#include "mainwindow.h"
#include <QtWidgets>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), arrowAngle(0)
{
    setupUI();
    initializeTargets();

    // Таймер для анимации стрелки радара
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::updateArrow);
    animationTimer->start(30); // 30ms обновление

    // Таймер для обновления симуляции движения целей
    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &MainWindow::updateSimulation);
    simulationTimer->start(100); // 100ms обновление симуляции
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // Левый радар с контролами
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftRadar = new RadarWidget(this);
    leftRadarControls = new RadarWithControls(leftRadar, this);

    QLabel *leftLabel = new QLabel("Круговой радар", this);
    leftLabel->setAlignment(Qt::AlignCenter);
    leftLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    QCheckBox *leftToggle = new QCheckBox("Включить сканирование", this);
    leftToggle->setChecked(true);

    leftLayout->addWidget(leftLabel);
    leftLayout->addWidget(leftRadarControls);
    leftLayout->addWidget(leftToggle, 0, Qt::AlignCenter);

    // Правый радар с контролами
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightRadar = new RadarInstallationWidget(this);
    rightRadar->setCentralAngle(30); // угол сектора 30 градусов
    rightRadarControls = new RadarInstallationWithControls(rightRadar, this);

    QLabel *rightLabel = new QLabel("Секторный радар", this);
    rightLabel->setAlignment(Qt::AlignCenter);
    rightLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    QCheckBox *rightToggle = new QCheckBox("Включить сканирование", this);
    rightToggle->setChecked(true);

    rightLayout->addWidget(rightLabel);
    rightLayout->addWidget(rightRadarControls);
    rightLayout->addWidget(rightToggle, 0, Qt::AlignCenter);

    // Добавляем разделитель
    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Sunken);

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addWidget(separator);
    mainLayout->addLayout(rightLayout, 1);

    setCentralWidget(central);

    // Подключаем сигналы
    connect(leftToggle, &QCheckBox::toggled, this, &MainWindow::onLeftRadarToggle);
    connect(rightToggle, &QCheckBox::toggled, this, &MainWindow::onRightRadarToggle);

    // Устанавливаем заголовок и размер окна
    setWindowTitle("Система ПВО - Симулятор радаров");
    resize(1000, 600);

    // Стиль окна
    setStyleSheet("QMainWindow { background-color: #2b2b2b; }"
                  "QLabel { color: white; }"
                  "QCheckBox { color: white; }");
}

void MainWindow::initializeTargets()
{
    // Получаем менеджер целей и создаем два F-16
    TargetManager& manager = TargetManager::getInstance();
    manager.createDefaultF16Targets();

    // Обновляем отображение целей на радарах
    leftRadarControls->updateTargets();
    rightRadarControls->updateTargets();
}

void MainWindow::updateArrow()
{
    arrowAngle += 2.5f;
    if (arrowAngle > 360) arrowAngle -= 360;
    leftRadar->setArrowAngle(arrowAngle);
}

void MainWindow::updateSimulation()
{
    // Обновляем позиции целей
    TargetManager& manager = TargetManager::getInstance();
    manager.updateTargets(0.1); // dt = 0.1 секунды

    // Обновляем отображение на радарах
    leftRadarControls->updateTargets();
    rightRadarControls->updateTargets();
}

void MainWindow::onLeftRadarToggle(bool checked)
{
    leftRadar->setEnabledScan(checked);
}

void MainWindow::onRightRadarToggle(bool checked)
{
    rightRadar->setEnabledScan(checked);
}
