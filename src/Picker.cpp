#include "Picker.h"
#include <iostream>

Picker::Picker(std::string name, int x, int y, int battery)
    : Robot(name, x, y, battery) {
}

void Picker::work() {
    std::cout << getName() << " is picking items.\n";
}
