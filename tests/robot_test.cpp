#include <iostream>
#include <cassert>

#include "Picker.h"

int main() {

    std::cout << "========== ROBOT UNIT TEST ==========\n";

    // Test 1: Robot Movement
    {
        Picker robot("TestRobot", 0, 0, 100);

        robot.moveOneStep(3, 0);

        assert(robot.getX() == 1);
        assert(robot.getY() == 0);
        assert(robot.getBattery() == 99);

        std::cout << "[PASS] Robot Movement\n";
    }

    // Test 2: Battery Charging
    {
        Picker robot("TestRobot", 0, 0, 50);

        robot.chargeBattery(20);

        assert(robot.getBattery() == 70);

        std::cout << "[PASS] Battery Charging\n";
    }

    // Test 3: Battery Maximum
    {
        Picker robot("TestRobot", 0, 0, 90);

        robot.chargeBattery(20);

        assert(robot.getBattery() == 100);

        std::cout << "[PASS] Battery Maximum\n";
    }

    // Test 4: Low Battery Detection
    {
        Picker robot("TestRobot", 0, 0, 19);

        assert(robot.isLowBattery() == true);

        std::cout << "[PASS] Low Battery Detection\n";
    }

    std::cout << "\nAll Robot Tests Passed!\n";

    return 0;
}
