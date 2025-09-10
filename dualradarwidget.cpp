#include "dualradarwidget.h"
#include <QPainter>
#include <QPen>
#include <QtMath>

/* ---------- RadarWidget (вращающаяся стрелка) ---------- */
RadarWidget::RadarWidget(QWidget *parent)
    : QWidget(parent), m_arrowAngle(0), m_enabled(true)
{
    setMinimumSize(250, 250);
}

void RadarWidget::setTargets(const QVector<QPointF>& targets)
{
    m_targets = targets;
    update();
}

void RadarWidget::setArrowAngle(float angleDegrees)
{
    m_arrowAngle = angleDegrees;
    update();
}

void RadarWidget::setEnabledScan(bool enabled)
{
    m_enabled = enabled;
    update();
}

void RadarWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int radius = qMin(width(), height()) / 2 - 10;
    QPoint center(width() / 2, height() / 2);

    painter.setPen(QPen(Qt::green, 2));
    painter.drawEllipse(center, radius, radius);
    for (int r = radius/4; r < radius; r += radius/4)
        painter.drawEllipse(center, r, r);

    painter.drawLine(center.x(), center.y() - radius, center.x(), center.y() + radius);
    painter.drawLine(center.x() - radius, center.y(), center.x() + radius, center.y());

    if (m_enabled) {
        // Вращающаяся стрелка (желтая)
        painter.setPen(QPen(Qt::yellow, 4));
        float rad = qDegreesToRadians(m_arrowAngle);
        QPoint arrow(center.x() + int(radius * cos(rad)), center.y() - int(radius * sin(rad)));
        painter.drawLine(center, arrow);
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);
    for (const QPointF& target : m_targets) {
        int x = center.x() + static_cast<int>(target.x() * radius);
        int y = center.y() - static_cast<int>(target.y() * radius);
        painter.drawEllipse(QPoint(x, y), 6, 6);
    }
}


/* ---------- RadarInstallationWidget (два радиуса с фиксированным углом) ---------- */
RadarInstallationWidget::RadarInstallationWidget(QWidget *parent)
    : QWidget(parent), m_centralAngle(30), m_enabled(true)
{
    setMinimumSize(250, 250);
}

void RadarInstallationWidget::setCentralAngle(float angleDegrees)
{
    m_centralAngle = angleDegrees;
    update();
}

void RadarInstallationWidget::setEnabledScan(bool enabled)
{
    m_enabled = enabled;
    update();
}

void RadarInstallationWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int radius = qMin(width(), height()) / 2 - 10;
    QPoint center(width() / 2, height() / 2);

    painter.setPen(QPen(Qt::cyan, 2));
    painter.drawEllipse(center, radius, radius);
    for (int r = radius/4; r < radius; r += radius/4)
        painter.drawEllipse(center, r, r);

    // Два радиуса с фиксированным углом (например, вокруг вертикали)
    if (m_enabled) {
        float baseAngle = 90; // вертикально вверх
        float half = m_centralAngle / 2;

        float rad1 = qDegreesToRadians(baseAngle - half);
        float rad2 = qDegreesToRadians(baseAngle + half);

        QPoint p1(center.x() + int(radius * cos(rad1)), center.y() - int(radius * sin(rad1)));
        QPoint p2(center.x() + int(radius * cos(rad2)), center.y() - int(radius * sin(rad2)));

        painter.setPen(QPen(Qt::cyan, 4));
        painter.drawLine(center, p1);
        painter.drawLine(center, p2);
    }
}
