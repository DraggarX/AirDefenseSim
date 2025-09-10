#include <iostream>
#include "Missile.hpp"
#include "Simulation.hpp"

// Класс F16 на основе Missile с параметрами F16
class F16 : public Missile {
public:
    F16() {
        mass = 12200;            // масса кг (примерная)
        position = Vector3D(0,0,1000); // старт (0,0,1000 м)
        velocity = Vector3D(250,0,0);  // скорость 250 м/с (около 900 км/ч)
        // Инерционные параметры (приближённые)
        inertia = Vector3D(23000, 21000, 3000);

        // можно добавить аэродинамические коэффициенты, силы и т.д.
    }

    // Переопределим applyForces, чтобы лететь ровно, без изменения двигателя и курсов
    void applyForces(double dt) override {
        // Движение с постоянной скоростью без внешних сил (для простоты)
    }
};

int main() {
    F16 f16;
    Simulation sim;

    double dt = 0.01;  // шаг времени
    double totalTime = 30.0; // время симуляции

    sim.addMissile(&f16);

    for(double t=0; t < totalTime; t+= dt) {
        sim.update(dt);
        Vector3D pos = f16.position;
        std::cout << "t=" << t << " x=" << pos.x << " y=" << pos.y << " z=" << pos.z << "\n";
    }

    return 0;
}
