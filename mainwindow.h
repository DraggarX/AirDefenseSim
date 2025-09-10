#pragma once
#include <QtWidgets>
#include "dualradarwidget.h"
#include "Targets.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void updateArrow();
    void onLeftRadarToggle(bool checked);
    void onRightRadarToggle(bool checked);
    void updateSimulation();
    void onToggleSimulation();   // слот для меню

private:
    void initializeTargets();
    void setupUI();
    void setupMenu();            // создаём меню

    RadarWithControls       *leftRadarControls;
    RadarInstallationWithControls *rightRadarControls;
    RadarWidget             *leftRadar;
    RadarInstallationWidget *rightRadar;

    QTimer *animationTimer;
    QTimer *simulationTimer;
    QAction *toggleSimAction;   // пункт меню

    float arrowAngle;
    bool simRunning{false};
};
