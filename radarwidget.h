// radarwidget.h
#ifndef RADARWIDGET_H
#define RADARWIDGET_H

#include <QWidget>
#include <QPointF>

class RadarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RadarWidget(QWidget *parent = nullptr);

    void setTargets(const QVector<QPointF>& targets);
    void setScanAngle(float angleDegrees);      // Угол обзора сектора (напр. 30 градусов)
    void setScanRotation(float rotationDegrees);// Текущий угол поворота сектора (вращение)

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPointF> m_targets;
    float m_scanAngle;
    float m_scanRotation;
};

#endif // RADARWIDGET_H
