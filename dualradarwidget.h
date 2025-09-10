#ifndef DUALRADARWIDGET_H
#define DUALRADARWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPointF>

// Первый виджет: радар с вращающейся стрелкой
class RadarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RadarWidget(QWidget *parent = nullptr);
    void setTargets(const QVector<QPointF>& targets);
    void setArrowAngle(float angleDegrees);
    void setEnabledScan(bool enabled);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPointF> m_targets;
    float m_arrowAngle = 0;
    bool m_enabled = true;
};

// Второй виджет: радар-установка с двумя радиусами
class RadarInstallationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RadarInstallationWidget(QWidget *parent = nullptr);
    void setCentralAngle(float angleDegrees);
    void setEnabledScan(bool enabled);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    float m_centralAngle = 30; // угол между радиусами (фиксируется через setCentralAngle)
    bool m_enabled = true;
};

#endif // DUALRADARWIDGET_H
