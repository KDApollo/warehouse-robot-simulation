
#include <iostream>
#include <limits>
#include <string>

#include "Warehouse.h"
#include "Picker.h"
#include "Packer.h"
#include "Task.h"

// รับตัวเลขและตรวจสอบช่วง
int readInt(std::string message, int min, int max) {
    int value;

    while (true) {
        std::cout << message;

        if (std::cin >> value && value >= min && value <= max) {
            return value;
        }

        std::cout << "Invalid input! Please try again.\n";

        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );
    }
}

// เพิ่ม Robot
void setupRobots(Warehouse& warehouse) {
    std::cout << "\n========== ADD ROBOTS ==========\n";

    int robotCount = readInt("How many robots? ", 1, 20);

    for (int i = 0; i < robotCount; i++) {
        std::cout << "\nRobot " << i + 1 << "\n";

        std::cout << "1. Picker\n";
        std::cout << "2. Packer\n";

        int type = readInt("Select type: ", 1, 2);

        std::string name;
        std::cout << "Name (no spaces): ";
        std::cin >> name;

        int x = readInt("Position X (0-9): ", 0, 9);
        int y = readInt("Position Y (0-4): ", 0, 4);
        int battery = readInt("Battery (0-100): ", 0, 100);

        if (type == 1) {
            warehouse.addRobot(new Picker(name, x, y, battery));
        }
        else {
            warehouse.addRobot(new Packer(name, x, y, battery));
        }
    }
}

// เพิ่ม Task
void setupTasks(Warehouse& warehouse) {
    std::cout << "\n========== ADD TASKS ==========\n";

    int taskCount = readInt("How many tasks? ", 1, 100);

    for (int i = 0; i < taskCount; i++) {
        std::cout << "\nTask " << i + 1 << "\n";

        std::cout << "1. Picking\n";
        std::cout << "2. Packing\n";

        int type = readInt("Select type: ", 1, 2);

        std::string taskName;
        std::cout << "Task name (no spaces): ";
        std::cin >> taskName;

        int x = readInt("Position X (0-9): ", 0, 9);
        int y = readInt("Position Y (0-4): ", 0, 4);

        std::string taskType = (type == 1) ? "Picking" : "Packing";

        warehouse.addTask(
            Task(i + 1, taskName, taskType, x, y)
        );
    }
}

// ตั้งค่า Default Mode
void setupDefault(Warehouse& warehouse) {
    std::cout << "\nLoading default configuration...\n";

    warehouse.setChargingStation(9, 4);

    warehouse.addRobot(new Picker("Picker-1", 0, 0, 25));
    warehouse.addRobot(new Packer("Packer-1", 0, 1, 72));
    warehouse.addRobot(new Picker("Picker-2", 0, 2, 100));

    warehouse.addTask(Task(1, "Pick_Item_A", "Picking", 5, 2));
    warehouse.addTask(Task(2, "Pack_Item_A", "Packing", 9, 3));
    warehouse.addTask(Task(3, "Pick_Item_B", "Picking", 3, 2));
    warehouse.addTask(Task(4, "Pick_Item_C", "Picking", 1, 3));
    warehouse.addTask(Task(5, "Pack_Item_B", "Packing", 7, 0));

    std::cout << "Default configuration loaded!\n";
}

int main() {

    Warehouse warehouse(10, 5);

    std::cout << "====================================\n";
    std::cout << "     WAREHOUSE ROBOT SIMULATION\n";
    std::cout << "====================================\n";

    std::cout << "\n1. Default Mode\n";
    std::cout << "2. Custom Mode\n";

    int mode = readInt("\nSelect mode: ", 1, 2);

    if (mode == 1) {
        setupDefault(warehouse);
    }
    else {
        std::cout << "\n========== CUSTOM MODE ==========\n";

        int chargeX = readInt("Charging Station X (0-9): ", 0, 9);
        int chargeY = readInt("Charging Station Y (0-4): ", 0, 4);

        warehouse.setChargingStation(chargeX, chargeY);

        setupRobots(warehouse);
        setupTasks(warehouse);
    }

    // Display Initial Warehouse
    std::cout << "\n========== INITIAL WAREHOUSE ==========\n";

    warehouse.displayGrid();
    warehouse.displayRobotStatus();
    warehouse.displayTaskProgress();

    // Wait for user
    std::cout << "\nPress Enter to continue...";

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n'
    );
    std::cin.get();

    // Select Simulation Display Mode
    std::cout << "\n========== SIMULATION DISPLAY MODE ==========\n";
    std::cout << "1. Full Output\n";
    std::cout << "2. Animation Mode\n";

    int displayMode = readInt("\nSelect display mode: ", 1, 2);

    // Start Simulation
    if (displayMode == 1) {
        warehouse.runSimulation(false);
    }
    else {
        warehouse.runSimulation(true);
    }

    return 0;
}
