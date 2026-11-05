#ifndef ROBOT_H
#define ROBOT_H

#include <string>

class Robot {
private:
    std::string name;
    int x;
    int y;
    int battery;

public:
    Robot(std::string name, int x, int y, int battery);
    virtual ~Robot();

    virtual void work() = 0;

    std::string getName() const;
    int getBattery() const;
};

#endif
