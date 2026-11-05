#include "Packer.h"
#include <iostream>

Packer::Packer(std::string name, int x, int y, int battery)
    : Robot(name, x, y, battery) {
}

void Packer::work() {
    std::cout << getName() << " is packing items.\n";
}
