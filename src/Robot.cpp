#include "Robot.h"
#include <iostream>
#include <cstdlib>

// Constructor
Robot::Robot(std::string name, int x, int y, int battery)
    : name(name), x(x), y(y), battery(battery),
      status(RobotStatus::Idle) {
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


void Robot::decreaseBattery(int amount) {
    battery -= amount;

    if (battery < 0) {
        battery = 0;
    }
}

void Robot::chargeBattery(int amount) {
    battery += amount;

    if (battery > 100) {
        battery = 100;
    }
}

bool Robot::isLowBattery() const {
    return battery < 20;
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
    else{
        return;
    }

    decreaseBattery(1);
}

int Robot::distanceTo(int targetX, int targetY) const {
    int distanceX = abs(x - targetX);
    int distanceY = abs(y - targetY);

    return distanceX + distanceY;
}

void Robot::setStatus(RobotStatus newStatus) {
    status = newStatus;
}

RobotStatus Robot::getStatus() const {
    return status;
}

std::string Robot::getStatusName() const {
    switch (status) {
        case RobotStatus::Idle:
            return "Idle";

        case RobotStatus::Moving:
            return "Moving";

        case RobotStatus::Working:
            return "Working";

        case RobotStatus::Charging:
            return "Charging";
    }

    return "Unknown";
}
