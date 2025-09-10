#include "dualradarwidget.h"
#include <QPainter>
#include <QtMath>
#include <QDebug>

/* ---------- RadarWidget (вращающаяся стрелка) ---------- */
RadarWidget::RadarWidget(QWidget *parent)
    : QWidget(parent), m_arrowAngle(0), m_enabled(true), m_radarRange(20000)
{
    setMinimumSize(300, 300);
}

void RadarWidget::setTargets(const std::vector<std::shared_ptr<Target>>& targets)
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

void RadarWidget::setRadarRange(double range)
{
    m_radarRange = range;
    update();
}

QPointF RadarWidget::worldToScreen(const Vector3D& worldPos, const QPoint& center, int radius) const
{
    // Преобразуем мировые координаты (в метрах) в экранные
    double scale = radius / m_radarRange;
    double screenX = center.x() + worldPos.x * scale;
    double screenY = center.y() - worldPos.y * scale; // Y инвертирован
    return QPointF(screenX, screenY);
}

void RadarWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int radius = qMin(width(), height()) / 2 - 20;
    QPoint center(width() / 2, height() / 2);

    // Рисуем сетку радара
    painter.setPen(QPen(Qt::green, 2));
    painter.drawEllipse(center, radius, radius);

    // Концентрические окружности
    for (int r = radius/4; r < radius; r += radius/4) {
        painter.drawEllipse(center, r, r);
    }

    // Координатные оси
    painter.drawLine(center.x(), center.y() - radius, center.x(), center.y() + radius);
    painter.drawLine(center.x() - radius, center.y(), center.x() + radius, center.y());

    // Подписи масштаба
    painter.setPen(Qt::green);
    painter.setFont(QFont("Arial", 8));
    for (int i = 1; i <= 4; i++) {
        int r = (radius * i) / 4;
        double range_km = (m_radarRange * i) / 4000; // в км
        QString label = QString::number(range_km, 'f', 1) + "км";
        painter.drawText(center.x() + r + 5, center.y() - 5, label);
    }

    // Вращающаяся стрелка (желтая)
    if (m_enabled) {
        painter.setPen(QPen(Qt::yellow, 4));
        float rad = qDegreesToRadians(m_arrowAngle);
        QPoint arrow(center.x() + int(radius * cos(rad)), center.y() - int(radius * sin(rad)));
        painter.drawLine(center, arrow);
    }

    // Отображение целей
    painter.setPen(QPen(Qt::red, 2));
    painter.setBrush(Qt::red);
    for (const auto& target : m_targets) {
        Vector3D pos = target->getPosition();
        double distance = sqrt(pos.x * pos.x + pos.y * pos.y);

        // Показываем только цели в пределах дальности радара
        if (distance <= m_radarRange) {
            QPointF screenPos = worldToScreen(pos, center, radius);

            // Проверяем, что цель в пределах экрана
            if (screenPos.x() >= 0 && screenPos.x() <= width() &&
                screenPos.y() >= 0 && screenPos.y() <= height()) {

                painter.drawEllipse(screenPos, 8, 8);

                // Отображаем координаты цели
                painter.setPen(Qt::white);
                painter.setFont(QFont("Arial", 9));
                QString coords = QString("(%1,%2,%3)")
                                     .arg(pos.x, 0, 'f', 0)
                                     .arg(pos.y, 0, 'f', 0)
                                     .arg(pos.z, 0, 'f', 0);
                painter.drawText(screenPos.x() + 10, screenPos.y() - 5, coords);

                // Имя цели
                painter.setPen(Qt::cyan);
                painter.drawText(screenPos.x() + 10, screenPos.y() + 10,
                                 QString::fromStdString(target->getName()));

                painter.setPen(QPen(Qt::red, 2));
                painter.setBrush(Qt::red);
            }
        }
    }
}

/* ---------- RadarInstallationWidget (два радиуса с фиксированным углом) ---------- */
RadarInstallationWidget::RadarInstallationWidget(QWidget *parent)
    : QWidget(parent), m_centralAngle(30), m_enabled(true), m_radarRange(20000)
{
    setMinimumSize(300, 300);
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

void RadarInstallationWidget::setTargets(const std::vector<std::shared_ptr<Target>>& targets)
{
    m_targets = targets;
    update();
}

void RadarInstallationWidget::setRadarRange(double range)
{
    m_radarRange = range;
    update();
}

QPointF RadarInstallationWidget::worldToScreen(const Vector3D& worldPos, const QPoint& center, int radius) const
{
    double scale = radius / m_radarRange;
    double screenX = center.x() + worldPos.x * scale;
    double screenY = center.y() - worldPos.y * scale;
    return QPointF(screenX, screenY);
}

void RadarInstallationWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int radius = qMin(width(), height()) / 2 - 20;
    QPoint center(width() / 2, height() / 2);

    // Рисуем сетку радара
    painter.setPen(QPen(Qt::cyan, 2));
    painter.drawEllipse(center, radius, radius);

    for (int r = radius/4; r < radius; r += radius/4) {
        painter.drawEllipse(center, r, r);
    }

    // Подписи масштаба
    painter.setPen(Qt::cyan);
    painter.setFont(QFont("Arial", 8));
    for (int i = 1; i <= 4; i++) {
        int r = (radius * i) / 4;
        double range_km = (m_radarRange * i) / 4000;
        QString label = QString::number(range_km, 'f', 1) + "км";
        painter.drawText(center.x() + r + 5, center.y() - 5, label);
    }

    // Два радиуса с фиксированным углом
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

    // Отображение целей
    painter.setPen(QPen(Qt::red, 2));
    painter.setBrush(Qt::red);
    for (const auto& target : m_targets) {
        Vector3D pos = target->getPosition();
        double distance = sqrt(pos.x * pos.x + pos.y * pos.y);

        if (distance <= m_radarRange) {
            QPointF screenPos = worldToScreen(pos, center, radius);

            if (screenPos.x() >= 0 && screenPos.x() <= width() &&
                screenPos.y() >= 0 && screenPos.y() <= height()) {

                painter.drawEllipse(screenPos, 8, 8);

                painter.setPen(Qt::white);
                painter.setFont(QFont("Arial", 9));
                QString coords = QString("(%1,%2,%3)")
                                     .arg(pos.x, 0, 'f', 0)
                                     .arg(pos.y, 0, 'f', 0)
                                     .arg(pos.z, 0, 'f', 0);
                painter.drawText(screenPos.x() + 10, screenPos.y() - 5, coords);

                painter.setPen(Qt::magenta);
                painter.drawText(screenPos.x() + 10, screenPos.y() + 10,
                                 QString::fromStdString(target->getName()));

                painter.setPen(QPen(Qt::red, 2));
                painter.setBrush(Qt::red);
            }
        }
    }
}

/* ---------- RadarWithControls ---------- */
RadarWithControls::RadarWithControls(RadarWidget* radar, QWidget *parent)
    : QWidget(parent), m_radar(radar)
{
    setupUI();
}

void RadarWithControls::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Добавляем сам радар
    layout->addWidget(m_radar);

    // Контролы масштаба
    QHBoxLayout* controlsLayout = new QHBoxLayout();

    QLabel* rangeTitle = new QLabel("Дальность радара:");
    m_rangeSlider = new QSlider(Qt::Horizontal);
    m_rangeSlider->setMinimum(5000);   // 5 км
    m_rangeSlider->setMaximum(50000);  // 50 км
    m_rangeSlider->setValue(20000);    // 20 км по умолчанию

    m_rangeLabel = new QLabel("20.0 км");

    controlsLayout->addWidget(rangeTitle);
    controlsLayout->addWidget(m_rangeSlider);
    controlsLayout->addWidget(m_rangeLabel);

    layout->addLayout(controlsLayout);

    connect(m_rangeSlider, &QSlider::valueChanged, this, &RadarWithControls::onRangeChanged);
}

void RadarWithControls::onRangeChanged(int value)
{
    double range_km = value / 1000.0;
    m_rangeLabel->setText(QString::number(range_km, 'f', 1) + " км");
    m_radar->setRadarRange(value);
}

void RadarWithControls::updateTargets()
{
    TargetManager& manager = TargetManager::getInstance();
    m_radar->setTargets(manager.getTargets());
}

/* ---------- RadarInstallationWithControls ---------- */
RadarInstallationWithControls::RadarInstallationWithControls(RadarInstallationWidget* radar, QWidget *parent)
    : QWidget(parent), m_radar(radar)
{
    setupUI();
}

void RadarInstallationWithControls::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(m_radar);

    QHBoxLayout* controlsLayout = new QHBoxLayout();

    QLabel* rangeTitle = new QLabel("Дальность радара:");
    m_rangeSlider = new QSlider(Qt::Horizontal);
    m_rangeSlider->setMinimum(5000);
    m_rangeSlider->setMaximum(50000);
    m_rangeSlider->setValue(20000);

    m_rangeLabel = new QLabel("20.0 км");

    controlsLayout->addWidget(rangeTitle);
    controlsLayout->addWidget(m_rangeSlider);
    controlsLayout->addWidget(m_rangeLabel);

    layout->addLayout(controlsLayout);

    connect(m_rangeSlider, &QSlider::valueChanged, this, &RadarInstallationWithControls::onRangeChanged);
}

void RadarInstallationWithControls::onRangeChanged(int value)
{
    double range_km = value / 1000.0;
    m_rangeLabel->setText(QString::number(range_km, 'f', 1) + " км");
    m_radar->setRadarRange(value);
}

void RadarInstallationWithControls::updateTargets()
{
    TargetManager& manager = TargetManager::getInstance();
    m_radar->setTargets(manager.getTargets());
}
