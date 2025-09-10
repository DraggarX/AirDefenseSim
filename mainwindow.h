// mainwindow.h
#pragma once

#include <QtWidgets>
#include "dualradarwidget.h"
#include "Targets.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void updateArrow();                  // Анимация стрелки кругового радара
    void updateNarrowScan();             // Анимация сканирования секторного радара
    void updateSimulation();             // Обновление положения целей
    void onScaleChanged(int value);      // Изменение дальности радаров
    void onTargetSelected(QListWidgetItem *item); // Пользователь выбрал цель

private:
    void setupUI();
    void setupMenu();
    void initializeTargets();

    RadarWidget *circularRadar_;
    RadarInstallationWidget *sectorRadar_;
    RadarWithControls *circularControls_;
    RadarInstallationWithControls *sectorControls_;

    QListWidget *targetList_;            // Список обнаруженных целей

    QTimer *arrowTimer_;
    QTimer *narrowTimer_;
    QTimer *simTimer_;

    float arrowAngle_{0};
    float narrowAngle_{90 - 15};        // Начать посередине сектора

    std::shared_ptr<Target> focusedTarget_; // Цель, выбранная для сопровождения
};
