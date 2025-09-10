#ifndef DUALRADARWIDGET_H
#define DUALRADARWIDGET_H

#include <QtWidgets>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtMath>
#include "Targets.h"

// Первый виджет: радар с вращающейся стрелкой
class RadarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RadarWidget(QWidget *parent = nullptr);
    void setTargets(const std::vector<std::shared_ptr<Target>>& targets);
    void setArrowAngle(float angleDegrees);
    void setEnabledScan(bool enabled);
    void setRadarRange(double range);
    double getRadarRange() const { return m_radarRange; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<std::shared_ptr<Target>> m_targets;
    float m_arrowAngle = 0;
    bool m_enabled = true;
    double m_radarRange = 20000; // Дальность в метрах

    QPointF worldToScreen(const Vector3D& worldPos, const QPoint& center, int radius) const;
};

// Второй виджет: радар-установка с двумя радиусами
class RadarInstallationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RadarInstallationWidget(QWidget *parent = nullptr);
    void setCentralAngle(float angleDegrees);
    void setEnabledScan(bool enabled);
    void setTargets(const std::vector<std::shared_ptr<Target>>& targets);
    void setRadarRange(double range);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    float m_centralAngle = 30;
    bool m_enabled = true;
    std::vector<std::shared_ptr<Target>> m_targets;
    double m_radarRange = 20000;

    QPointF worldToScreen(const Vector3D& worldPos, const QPoint& center, int radius) const;
};

// Комбинированный виджет с контролами масштаба
class RadarWithControls : public QWidget
{
    Q_OBJECT
public:
    explicit RadarWithControls(RadarWidget* radar, QWidget *parent = nullptr);
    void updateTargets();

private slots:
    void onRangeChanged(int value);

private:
    RadarWidget* m_radar;
    QSlider* m_rangeSlider;
    QLabel* m_rangeLabel;

    void setupUI();
};

class RadarInstallationWithControls : public QWidget
{
    Q_OBJECT
public:
    explicit RadarInstallationWithControls(RadarInstallationWidget* radar, QWidget *parent = nullptr);
    void updateTargets();

private slots:
    void onRangeChanged(int value);

private:
    RadarInstallationWidget* m_radar;
    QSlider* m_rangeSlider;
    QLabel* m_rangeLabel;

    void setupUI();
};

#endif // DUALRADARWIDGET_H
