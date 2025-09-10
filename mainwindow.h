// mainwindow.h
#pragma once
#include <QMainWindow>
#include <QTimer>
#include "dualradarwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void updateArrow();
    void onLeftRadarToggle(bool checked);
    void onRightRadarToggle(bool checked);

private:
    RadarWidget *leftRadar;
    RadarInstallationWidget *rightRadar;
    QTimer *timer;
    float arrowAngle;
};
