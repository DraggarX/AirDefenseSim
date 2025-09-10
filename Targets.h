#ifndef TARGETS_H
#define TARGETS_H
#pragma once
#include "Vector3D.hpp"
#include <string>

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

    // Виртуальный метод движения (на будущее)
    virtual void update(double dt) {
        position = position + velocity * dt;
    }

protected:
    std::string name;
    Vector3D position;
    Vector3D velocity;
    // Можно добавить ЭПР, тип самолета, вооружение и т.д.
};

// Пример наследника – истребитель
class Fighter : public Target {
public:
    Fighter(const std::string& _name, const Vector3D& _pos, const Vector3D& _vel)
        : Target(_name, _pos, _vel) {}
    // Могут быть дополнительные параметры/методы
};

// Пример наследника – бомбардировщик
class Bomber : public Target {
public:
    Bomber(const std::string& _name, const Vector3D& _pos, const Vector3D& _vel)
        : Target(_name, _pos, _vel) {}
    // Могут быть дополнительные параметры/методы
};

#endif // TARGETS_H
