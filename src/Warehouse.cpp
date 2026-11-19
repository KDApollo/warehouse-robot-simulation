#include "Warehouse.h"
#include <iostream>

Warehouse::Warehouse(int width, int height)
    : width(width),
      height(height),
      chargingStationX(-1),
      chargingStationY(-1) {
}

Warehouse::~Warehouse() {
    for (Robot* robot : robots) {
        delete robot;
    }
}

void Warehouse::addRobot(Robot* robot) {
    robots.push_back(robot);
}

void Warehouse::addTask(const Task& task) {
    tasks.push_back(task);
}


void Warehouse::assignTasks() {

    if (robots.empty()) {
        std::cout << "No robots available!\n";
        return;
    }
    robotTaskQueues.clear();
    
    robotTaskQueues.resize(robots.size());
    currentTaskIndices.assign(robots.size(), 0);

    for (int i = 0; i < tasks.size(); i++) {
        int robotIndex = i % robots.size();

        robotTaskQueues[robotIndex].push_back(i);
    }

    taskStarted.assign(tasks.size(), false);
    returningToCharge.assign(robots.size(), false);
    
    std::cout << "Tasks assigned successfully!\n";

}





void Warehouse::update() {

    for (int i = 0; i < robots.size(); i++) {

        Robot* robot = robots[i];

        // 1. Check low battery first
        if (returningToCharge[i] || robot->isLowBattery()) {

            returningToCharge[i] = true;

            if (chargingStationX == -1 || chargingStationY == -1) {
                std::cout << "No charging station available!\n";
                continue;
            }

            // Move to charging station
            if (robot->getX() != chargingStationX ||
                robot->getY() != chargingStationY) {

                robot->moveOneStep(chargingStationX, chargingStationY);

                std::cout << robot->getName()
                          << " returning to charging station. Battery: "
                          << robot->getBattery() << "%\n";
            }
            else {
                // Charge battery by 20%
                robot->chargeBattery(20);

                std::cout << robot->getName()
                          << " is charging. Battery: "
                          << robot->getBattery() << "%\n";

                // Resume when battery is full
                if (robot->getBattery() == 100) {
                    returningToCharge[i] = false;

                    std::cout << robot->getName()
                              << " battery full. Resume task!\n";
                }
            }

            continue;
        }

        // 2. Check whether this robot has remaining tasks
        if (currentTaskIndices[i] >= robotTaskQueues[i].size()) {
            continue;
        }

        // 3. Get current task
        int taskIndex = robotTaskQueues[i][currentTaskIndices[i]];
        Task& task = tasks[taskIndex];

        if (task.isCompleted()) {
            currentTaskIndices[i]++;
            continue;
        }

        // 4. Move toward task
        if (robot->getX() != task.getX() ||
            robot->getY() != task.getY()) {

            robot->moveOneStep(task.getX(), task.getY());

            std::cout << robot->getName()
                      << " moved to ("
                      << robot->getX() << ", "
                      << robot->getY() << ")\n";
        }
        else {

            if (!taskStarted[taskIndex]) {
                task.start();
                taskStarted[taskIndex] = true;
                robot->work();
            }

            task.updateProgress();

            // Working consumes 2% battery
            robot->decreaseBattery(2);

            std::cout << robot->getName()
                      << " is working on Task "
                      << task.getId() << "\n";

            if (task.isCompleted()) {
                std::cout << "Task "
                          << task.getId()
                          << " completed!\n";

                currentTaskIndices[i]++;
            }
        }
    }
}






void Warehouse::displayGrid() const {

    std::cout << "\nWarehouse Grid ("
              << width << " x " << height << ")\n";

    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {

            bool robotFound = false;

            // Check robot position
            for (int i = 0; i < robots.size(); i++) {

                if (robots[i]->getX() == x &&
                    robots[i]->getY() == y) {

                    std::cout << "R" << i + 1 << " ";
                    robotFound = true;
                    break;
                }
            }

            // Check charging station position
            if (!robotFound &&
                x == chargingStationX &&
                y == chargingStationY) {

                std::cout << "C  ";
                robotFound = true;
            }

            // Empty cell
            if (!robotFound) {
                std::cout << ".  ";
            }
        }

        std::cout << "\n";
    }
}

bool Warehouse::allTasksCompleted() const {
    for (const Task& task : tasks) {
        if (!task.isCompleted()) {
            return false;
        }
    }

    return true;
}


void Warehouse::runSimulation() {
    std::cout << "Starting simulation...\n";

    assignTasks();

    int step = 1;

    while (true) {

        std::cout << "\nStep " << step << "\n";

        update();
        displayGrid();

        // Check whether all tasks are completed
        bool tasksDone = allTasksCompleted();

        // Check whether all robots have finished charging
        bool chargingDone = true;

        for (int i = 0; i < robots.size(); i++) {
            if (returningToCharge[i]) {
                chargingDone = false;
                break;
            }
        }

        // Stop when all tasks are done and charging is complete
        if (tasksDone && chargingDone) {
            break;
        }

        step++;
    }

    showSummary();
}


void Warehouse::showSummary() const {
    std::cout << "Simulation finished.\n";
}

void Warehouse::setChargingStation(int x, int y) {

    if (x < 0 || x >= width ||
        y < 0 || y >= height) {

        std::cout << "Invalid charging station position!\n";
        return;
    }

    chargingStationX = x;
    chargingStationY = y;

    std::cout << "Charging Station set at ("
              << x << ", " << y << ")\n";
}