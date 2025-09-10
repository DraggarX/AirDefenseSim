#include "Missile.hpp"
#include <iostream>
#include <cmath>

// Исправленный класс F16 наследуется от Target, а не от Missile
#include "Targets.h"

class F16Simulation : public F16Fighter {
public:
    F16Simulation() : F16Fighter("F-16 Sim", Vector3D(0, 0, 1000), Vector3D(250, 0, 0)) {
        // Инерционные параметры
        mass = 12200; // масса кг
        inertia = Vector3D(23000, 21000, 3000);
    }

    // Переопределим update для простого движения
    void update(double dt) override {
        // Простое движение с постоянной скоростью
        position = position + velocity * dt;
    }

private:
    double mass;
    Vector3D inertia;
};

int main() {
    F16Simulation f16;

    double dt = 0.01; // шаг времени
    double totalTime = 30.0; // время симуляции

    for(double t = 0; t < totalTime; t += dt) {
        f16.update(dt);
        Vector3D pos = f16.getPosition();
        std::cout << "t=" << t << " x=" << pos.x << " y=" << pos.y << " z=" << pos.z << "\n";
    }

    return 0;
}
