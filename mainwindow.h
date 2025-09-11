#pragma once

#include <QtWidgets>
#include "dualradarwidget.h"
#include "Targets.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void updateArrow();
    void updateNarrowScan();
    void updateSimulation();
    void onScaleChanged(int value);
    void onTargetSelected(QListWidgetItem *item);

private:
    void setupUI();
    void initializeTargets();

    RadarWidget *circularRadar_;
    RadarInstallationWidget *sectorRadar_;
    RadarWithControls *circularControls_;
    RadarInstallationWithControls *sectorControls_;
    QListWidget *targetList_;
    QTimer *arrowTimer_;
    QTimer *narrowTimer_;
    QTimer *simTimer_;

    float arrowAngle_{0};
    float narrowAngle_{90 - 15};

    std::shared_ptr<Target> focusedTarget_;
};
