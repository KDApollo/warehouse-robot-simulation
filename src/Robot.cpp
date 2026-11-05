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
