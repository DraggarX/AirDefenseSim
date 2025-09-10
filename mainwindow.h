#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QtWidgets>
#include <QMainWindow>
#include <QTimer>
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

private:
    RadarWithControls *leftRadarControls;
    RadarInstallationWithControls *rightRadarControls;
    RadarWidget *leftRadar;
    RadarInstallationWidget *rightRadar;
    QTimer *animationTimer;
    QTimer *simulationTimer;
    float arrowAngle;

    void initializeTargets();
    void setupUI();
};

#endif // MAINWINDOW_H
