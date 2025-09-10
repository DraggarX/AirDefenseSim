#include "mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), arrowAngle(0)
{
    setupUI();
    setupMenu();             // создаём меню
    initializeTargets();

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::updateArrow);
    animationTimer->start(30);

    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &MainWindow::updateSimulation);
    // не запускаем сразу, ждём команды из меню
}

void MainWindow::setupMenu()
{
    QMenu *simMenu = menuBar()->addMenu(tr("Simulation"));
    toggleSimAction = simMenu->addAction(tr("Start Simulation"));
    toggleSimAction->setCheckable(true);
    connect(toggleSimAction, &QAction::triggered, this, &MainWindow::onToggleSimulation);
}

void MainWindow::onToggleSimulation()
{
    simRunning = !simRunning;
    if (simRunning) {
        simulationTimer->start(100);
        toggleSimAction->setText(tr("Stop Simulation"));
    } else {
        simulationTimer->stop();
        toggleSimAction->setText(tr("Start Simulation"));
    }
}

void MainWindow::setupUI()
{
    // Центральный виджет и радары — без изменений
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // Левый радар
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftRadar = new RadarWidget(this);
    leftRadarControls = new RadarWithControls(leftRadar, this);
    QCheckBox *leftToggle = new QCheckBox(tr("Enable Scan"), this);
    leftToggle->setChecked(true);
    leftLayout->addWidget(new QLabel(tr("Circular Radar"), this), 0, Qt::AlignCenter);
    leftLayout->addWidget(leftRadarControls);
    leftLayout->addWidget(leftToggle, 0, Qt::AlignCenter);

    // Правый радар
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightRadar = new RadarInstallationWidget(this);
    rightRadarControls = new RadarInstallationWithControls(rightRadar, this);
    QCheckBox *rightToggle = new QCheckBox(tr("Enable Scan"), this);
    rightToggle->setChecked(true);
    rightLayout->addWidget(new QLabel(tr("Sector Radar"), this), 0, Qt::AlignCenter);
    rightLayout->addWidget(rightRadarControls);
    rightLayout->addWidget(rightToggle, 0, Qt::AlignCenter);

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 1);
    setCentralWidget(central);

    connect(leftToggle, &QCheckBox::toggled, this, &MainWindow::onLeftRadarToggle);
    connect(rightToggle, &QCheckBox::toggled, this, &MainWindow::onRightRadarToggle);
}

void MainWindow::initializeTargets()
{
    auto &mgr = TargetManager::getInstance();
    mgr.createDefaultF16Targets();
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
    auto &mgr = TargetManager::getInstance();
    mgr.updateTargets(0.1);
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
