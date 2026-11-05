#ifndef PACKER_H
#define PACKER_H

#include "Robot.h"

class Packer : public Robot {
public:
    Packer(std::string name, int x, int y, int battery);
    void work() override;
};

#endif
