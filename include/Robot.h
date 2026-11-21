#ifndef ROBOT_H
#define ROBOT_H

#include <string>

enum class RobotStatus {
    Idle,
    Moving,
    Working,
    Charging
};

class Robot {
private:
    std::string name;
    int x;
    int y;
    int battery;
    RobotStatus status;

public:
    Robot(std::string name, int x, int y, int battery);
    virtual ~Robot();

    virtual void work() = 0;
    virtual std::string getType() const = 0;

    std::string getName() const;
    int getBattery() const;
    int getX() const;
    int getY() const;

    // Robot Status
    void setStatus(RobotStatus newStatus);
    RobotStatus getStatus() const;
    std::string getStatusName() const;

    // Movement
    void moveOneStep(int targetX, int targetY);

    // Battery
    void decreaseBattery(int amount);
    void chargeBattery(int amount);
    bool isLowBattery() const;
    int distanceTo(int targetX, int targetY) const;
};

#endif
