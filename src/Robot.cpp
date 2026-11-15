#include "Robot.h"
#include <iostream>

// Constructor
Robot::Robot(std::string name, int x, int y, int battery)
    : name(name), x(x), y(y), battery(battery) {
    std::cout << "Robot " << name << " created.\n";
}

// Destructor
Robot::~Robot() {
    std::cout << "Robot " << name << " destroyed.\n";
}

// Getter
std::string Robot::getName() const {
    return name;
}

int Robot::getBattery() const {
    return battery;
}
int Robot::getX() const {
    return x;
}

int Robot::getY() const {
    return y;
}

// Move
void Robot::moveOneStep(int targetX, int targetY) {

    if (x < targetX) {
        x++;
    }
    else if (x > targetX) {
        x--;
    }
    else if (y < targetY) {
        y++;
    }
    else if (y > targetY) {
        y--;
    }
}
