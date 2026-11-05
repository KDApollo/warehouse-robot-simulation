#ifndef PICKER_H
#define PICKER_H

#include "Robot.h"

class Picker : public Robot {
public:
    Picker(std::string name, int x, int y, int battery);
    void work() override;
};

#endif
