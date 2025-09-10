#ifndef TARGETS_H
#define TARGETS_H

#pragma once
#include "Vector3D.hpp"
#include <string>
#include <vector>
#include <memory>

// Базовый класс цели
class Target {
public:
    Target(const std::string& _name, const Vector3D& _pos, const Vector3D& _vel)
        : name(_name), position(_pos), velocity(_vel) {}

    virtual ~Target() = default;

    // Геттеры для основных параметров
    std::string getName() const { return name; }
    Vector3D getPosition() const { return position; }
    Vector3D getVelocity() const { return velocity; }

    // Виртуальный метод движения
    virtual void update(double dt) {
        position = position + velocity * dt;
    }

protected:
    std::string name;
    Vector3D position;
    Vector3D velocity;
};

// F-16 Fighter
class F16Fighter : public Target {
public:
    F16Fighter(const std::string& _name, const Vector3D& _pos, const Vector3D& _vel)
        : Target(_name, _pos, _vel), rcs(5.0) {}

    double getRCS() const { return rcs; }

private:
    double rcs; // Радиолокационная отражающая поверхность
};

// Менеджер целей
class TargetManager {
public:
    static TargetManager& getInstance() {
        static TargetManager instance;
        return instance;
    }

    void addTarget(std::shared_ptr<Target> target) {
        targets.push_back(target);
    }

    const std::vector<std::shared_ptr<Target>>& getTargets() const {
        return targets;
    }

    void updateTargets(double dt) {
        for (auto& target : targets) {
            target->update(dt);
        }
    }

    void createDefaultF16Targets() {
        targets.clear(); // Очищаем предыдущие цели

        auto f16_1 = std::make_shared<F16Fighter>(
            "F-16A-001",
            Vector3D(15000, 8000, 3000),    // 15км по X, 8км по Y, 3км высота
            Vector3D(-200, -50, 0)          // Движется со скоростью ~200 м/с
            );

        auto f16_2 = std::make_shared<F16Fighter>(
            "F-16A-002",
            Vector3D(12000, -5000, 2500),   // 12км по X, -5км по Y, 2.5км высота
            Vector3D(-180, 30, 10)          // Движется со скоростью ~180 м/с
            );

        addTarget(f16_1);
        addTarget(f16_2);
    }

private:
    std::vector<std::shared_ptr<Target>> targets;
    TargetManager() = default;
};

#endif // TARGETS_H
