#include <iostream>
#include <limits>
#include "Warehouse.h"
#include "Picker.h"
#include "Packer.h"

int main() {

    Warehouse warehouse(10, 5);

    int chargeX, chargeY;

    while (true) {

        std::cout << "Enter charging station X (0-9): ";

        if (!(std::cin >> chargeX)) {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );

            std::cout << "Invalid input! Numbers only.\n\n";
            continue;
        }

        std::cout << "Enter charging station Y (0-4): ";

        if (!(std::cin >> chargeY)) {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );

            std::cout << "Invalid input! Numbers only.\n\n";
            continue;
        }

        if (chargeX >= 0 && chargeX < 10 &&
            chargeY >= 0 && chargeY < 5) {

            warehouse.setChargingStation(chargeX, chargeY);
            break;
        }

        std::cout << "Invalid position! Please try again.\n\n";
    }

    // Create robots
    warehouse.addRobot(new Picker("Picker-1", 0, 0, 25));
    warehouse.addRobot(new Packer("Packer-1", 0, 1, 100));
    warehouse.addRobot(new Picker("Picker-2", 0, 2, 100));

    // Create tasks
    warehouse.addTask(Task(1, "Pick Item A", "Picking", 1, 0));
    warehouse.addTask(Task(2, "Pack Item A", "Packing", 1, 1));
    warehouse.addTask(Task(3, "Pick Item B", "Picking", 1, 2));
    warehouse.addTask(Task(4, "Pick Item C", "Picking", 2, 1));
    warehouse.addTask(Task(5, "Pack Item B", "Packing", 2, 2));

    warehouse.runSimulation();

    return 0;
}
