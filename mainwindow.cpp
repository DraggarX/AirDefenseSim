#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    initializeTargets();

    arrowTimer_ = new QTimer(this);
    connect(arrowTimer_, &QTimer::timeout, this, &MainWindow::updateArrow);
    arrowTimer_->start(30);

    narrowTimer_ = new QTimer(this);
    connect(narrowTimer_, &QTimer::timeout, this, &MainWindow::updateNarrowScan);
    narrowTimer_->start(50);

    simTimer_ = new QTimer(this);
    connect(simTimer_, &QTimer::timeout, this, &MainWindow::updateSimulation);
    simTimer_->start(100);
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(central);

    auto *leftLayout = new QVBoxLayout();
    circularRadar_ = new RadarWidget(this);
    circularControls_ = new RadarWithControls(circularRadar_, this);

    targetList_ = new QListWidget(this);
    targetList_->setFixedWidth(200);
    connect(targetList_, &QListWidget::itemClicked,
            this, &MainWindow::onTargetSelected);

    leftLayout->addWidget(new QLabel("Круговой радар", this), 0, Qt::AlignCenter);
    leftLayout->addWidget(circularControls_);
    leftLayout->addWidget(new QLabel("Обнаруженные цели", this), 0, Qt::AlignCenter);
    leftLayout->addWidget(targetList_);

    auto *rightLayout = new QVBoxLayout();
    sectorRadar_ = new RadarInstallationWidget(this);
    sectorRadar_->setCentralAngle(30);
    sectorControls_ = new RadarInstallationWithControls(sectorRadar_, this);

    rightLayout->addWidget(new QLabel("Секторный радар", this), 0, Qt::AlignCenter);
    rightLayout->addWidget(sectorControls_);

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 1);
    setCentralWidget(central);

    auto *scaleSlider = new QSlider(Qt::Horizontal, this);
    scaleSlider->setRange(5000, 50000);
    scaleSlider->setValue(20000);
    connect(scaleSlider, &QSlider::valueChanged,
            this, &MainWindow::onScaleChanged);
    statusBar()->addWidget(new QLabel("Дальность:"), 0);
    statusBar()->addWidget(scaleSlider);
}

void MainWindow::initializeTargets()
{
    auto &mgr = TargetManager::getInstance();
    mgr.createDefaultF16Targets();
}

void MainWindow::updateArrow()
{
    arrowAngle_ += 2.5f;
    if (arrowAngle_ > 360) arrowAngle_ -= 360;
    circularRadar_->setArrowAngle(arrowAngle_);
}

void MainWindow::updateNarrowScan()
{
    narrowAngle_ += 2.0f;
    if (narrowAngle_ > 360) narrowAngle_ -= 360;
    sectorRadar_->setCentralAngle(30);
    sectorRadar_->update();
}

void MainWindow::updateSimulation()
{
    auto &mgr = TargetManager::getInstance();
    mgr.updateTargets(0.1);

    circularRadar_->setTargets(mgr.getTargets());

    targetList_->clear();
    for (const auto &t : mgr.getTargets()) {
        targetList_->addItem(QString::fromStdString(t->getName()));
    }

    if (focusedTarget_) {
        sectorRadar_->setTargets({focusedTarget_});
    }
}

void MainWindow::onScaleChanged(int value)
{
    circularRadar_->setRadarRange(value);
    sectorRadar_->setRadarRange(value);
}

void MainWindow::onTargetSelected(QListWidgetItem *item)
{
    auto &mgr = TargetManager::getInstance();
    for (const auto &t : mgr.getTargets()) {
        if (QString::fromStdString(t->getName()) == item->text()) {
            focusedTarget_ = t;
            break;
        }
    }
}
