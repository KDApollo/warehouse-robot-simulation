#include <iostream>
#include "Warehouse.h"
#include "Picker.h"
#include "Packer.h"

int main() {

    

    // Create warehouse
    Warehouse warehouse(10, 5);
    
    // Get charging station position
    int chargeX, chargeY;

    std::cout << "Enter charging station X: ";
    std::cin >> chargeX;

    std::cout << "Enter charging station Y: ";
    std::cin >> chargeY;

    warehouse.setChargingStation(chargeX, chargeY);

    // Create robots
    warehouse.addRobot(new Picker("Picker-1", 0, 0, 100));
    warehouse.addRobot(new Packer("Packer-1", 0, 1, 100));
    warehouse.addRobot(new Picker("Picker-2", 0, 2, 100));

    // Create tasks
    warehouse.addTask(Task(1, "Pick Item A", "Picking", 1, 0, 1));
    warehouse.addTask(Task(2, "Pack Item A", "Packing", 1, 1, 1));
    warehouse.addTask(Task(3, "Pick Item B", "Picking", 1, 2, 1));
    warehouse.addTask(Task(4, "Pick Item C", "Picking", 2, 1, 1));
    warehouse.addTask(Task(5, "Pack Item B", "Packing", 2, 2, 1));

    // Start simulation
    warehouse.runSimulation();

    return 0;
}
