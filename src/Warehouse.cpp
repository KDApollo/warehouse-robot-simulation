#include <cstdlib>
#include "Warehouse.h"
#include <iostream>
#include <map>
#include <iomanip>

Warehouse::Warehouse(int width, int height)
    : width(width),
      height(height),
      chargingStationX(-1),
      chargingStationY(-1),
      simulationFailed(false) {
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

// Round Robin Task Assignment
void Warehouse::assignTasks() {

    if (robots.empty()) {
        std::cout << "No robots available!\n";
        return;
    }

    robotTaskQueues.clear();
    robotTaskQueues.resize(robots.size());

    currentTaskIndices.assign(robots.size(), 0);
    taskStarted.assign(tasks.size(), false);
    returningToCharge.assign(robots.size(), false);

    std::map<std::string, int> lastAssigned;

    for (int i = 0; i < tasks.size(); i++) {

        std::string taskType = tasks[i].getType();

        int startIndex = 0;

        if (lastAssigned.find(taskType) != lastAssigned.end()) {
            startIndex = (lastAssigned[taskType] + 1) % robots.size();
        }

        bool assigned = false;

        for (int j = 0; j < robots.size(); j++) {

            int robotIndex = (startIndex + j) % robots.size();

            if (robots[robotIndex]->getType() == taskType) {

                robotTaskQueues[robotIndex].push_back(i);

                lastAssigned[taskType] = robotIndex;

                std::cout << "Task " << tasks[i].getId()
                          << " assigned to "
                          << robots[robotIndex]->getName()
                          << "\n";

                assigned = true;
                break;
            }
        }

        if (!assigned) {
            std::cout << "No compatible robot for Task "
                      << tasks[i].getId() << "\n";
        }
    }

    std::cout << "Tasks assigned successfully!\n";
}


bool Warehouse::hasEnoughBattery(
    const Robot* robot,
    const Task& task
) const {

    // Distance from robot to task
    int distanceToTask = robot->distanceTo(
        task.getX(),
        task.getY()
    );

    // Distance from task to charging station
    int distanceToStation =
        abs(task.getX() - chargingStationX) +
        abs(task.getY() - chargingStationY);

    // Battery required for working
    int workingBattery = task.getDuration() * 2;

    int requiredBattery =
        distanceToTask +
        workingBattery +
        distanceToStation;

    return robot->getBattery() >= requiredBattery;
}

// Update Simulation
void Warehouse::update() {

    for (int i = 0; i < robots.size(); i++) {

        Robot* robot = robots[i];

        // 1. Check low battery
        if (returningToCharge[i] || robot->isLowBattery()) {

            returningToCharge[i] = true;

            // Check charging station
            if (chargingStationX == -1 || chargingStationY == -1) {
                std::cout << "No charging station available!\n";
                simulationFailed = true;
                continue;
            }

            // Calculate required battery
            int requiredBattery = robot->distanceTo(
                chargingStationX, chargingStationY
            );

            // Check battery availability
            if (robot->getBattery() < requiredBattery) {

                std::cout << robot->getName()
                          << " cannot reach charging station!\n";

                std::cout << "Required battery: "
                          << requiredBattery << "%\n";

                std::cout << "Available battery: "
                          << robot->getBattery() << "%\n";

                simulationFailed = true;
                continue;
            }

            // Move to charging station
            if (robot->getX() != chargingStationX ||
                robot->getY() != chargingStationY) {

                robot->setStatus(RobotStatus::Moving);

                robot->moveOneStep(
                    chargingStationX,
                    chargingStationY
                );

                std::cout << robot->getName()
                          << " returning to charging station. Battery: "
                          << robot->getBattery() << "%\n";
            }
            else {

                // Charge battery
                robot->setStatus(RobotStatus::Charging);
                robot->chargeBattery(20);

                std::cout << robot->getName()
                          << " is charging. Battery: "
                          << robot->getBattery() << "%\n";

                // Resume task when battery is full
                if (robot->getBattery() == 100) {

                    returningToCharge[i] = false;
                    robot->setStatus(RobotStatus::Idle);

                    std::cout << robot->getName()
                              << " battery full. Resume task!\n";
                }
            }

            continue;
        }

        // 2. Check remaining tasks
        if (currentTaskIndices[i] >= robotTaskQueues[i].size()) {

            robot->setStatus(RobotStatus::Idle);
            continue;
        }

        // 3. Get current task
        int taskIndex = robotTaskQueues[i][currentTaskIndices[i]];
        Task& task = tasks[taskIndex];

        if (task.isCompleted()) {

            currentTaskIndices[i]++;
            robot->setStatus(RobotStatus::Idle);
            continue;
        }

        // Check Battery Reserve
        if (chargingStationX != -1 && chargingStationY != -1 &&
            !hasEnoughBattery(robot, task)) {

            std::cout << robot->getName()
                    << " does not have enough battery for Task "
                    << task.getId() << ". Returning to charge.\n";

            returningToCharge[i] = true;
            continue;
        }

        // 4. Move toward task
        if (robot->getX() != task.getX() ||
            robot->getY() != task.getY()) {

            robot->setStatus(RobotStatus::Moving);

            robot->moveOneStep(task.getX(), task.getY());

            std::cout << robot->getName()
                      << " moved to ("
                      << robot->getX() << ", "
                      << robot->getY() << ")\n";
        }
        else {

            // Start task once
            robot->setStatus(RobotStatus::Working);

            if (!taskStarted[taskIndex]) {

                task.start();
                taskStarted[taskIndex] = true;

                robot->work();
            }

            // Perform task
            task.updateProgress();

            // Working consumes 2% battery
            robot->decreaseBattery(2);

            std::cout << robot->getName()
                      << " is working on Task "
                      << task.getId() << "\n";

            // Complete task
            if (task.isCompleted()) {

                std::cout << "Task "
                          << task.getId()
                          << " completed!\n";

                currentTaskIndices[i]++;

                // No more tasks
                if (currentTaskIndices[i] >= robotTaskQueues[i].size()) {
                    robot->setStatus(RobotStatus::Idle);
                }
            }
        }
    }
}

// Display Warehouse Grid

void Warehouse::displayGrid() const {

    std::cout << "\n========== STEP GRID ==========\n";

    std::cout << "   ";
    for (int x = 0; x < width; x++) {
        std::cout << x << "  ";
    }
    std::cout << "\n";

    for (int y = 0; y < height; y++) {

        std::cout << y << "  ";

        for (int x = 0; x < width; x++) {

            bool found = false;

            // 1. Display Robot
            for (int i = 0; i < robots.size(); i++) {
                if (robots[i]->getX() == x &&
                    robots[i]->getY() == y) {

                    std::cout << "R" << i + 1 << " ";
                    found = true;
                    break;
                }
            }

            // 2. Display Charging Station
            if (!found &&
                x == chargingStationX &&
                y == chargingStationY) {

                std::cout << "C  ";
                found = true;
            }

            // 3. Display unfinished Tasks
            if (!found) {
                for (const Task& task : tasks) {

                    if (!task.isCompleted() &&
                        task.getX() == x &&
                        task.getY() == y) {

                        if (task.getType() == "Picking") {
                            std::cout << "P  ";
                        }
                        else if (task.getType() == "Packing") {
                            std::cout << "K  ";
                        }
                        else {
                            std::cout << "T  ";
                        }

                        found = true;
                        break;
                    }
                }
            }

            // 4. Empty cell
            if (!found) {
                std::cout << ".  ";
            }
        }

        std::cout << "\n";
    }
}


// Display Robot Information
void Warehouse::displayRobotStatus() const {

    std::cout << "\n========== ROBOT STATUS ==========\n";

    std::cout << std::left
              << std::setw(8)  << "Robot"
              << std::setw(12) << "Type"
              << std::setw(12) << "Position"
              << std::setw(12) << "Status"
              << std::setw(10) << "Battery"
              << "\n";

    std::cout << "--------------------------------------------------\n";

    for (int i = 0; i < robots.size(); i++) {

        Robot* robot = robots[i];

        std::string position =
            "(" + std::to_string(robot->getX()) +
            "," + std::to_string(robot->getY()) + ")";

        std::cout << std::left
                  << std::setw(8)  << ("R" + std::to_string(i + 1))
                  << std::setw(12) << robot->getType()
                  << std::setw(12) << position
                  << std::setw(12) << robot->getStatusName()
                  << std::setw(10) << (std::to_string(robot->getBattery()) + "%")
                  << "\n";
    }

    std::cout << "--------------------------------------------------\n";
}



// Check all tasks completed
bool Warehouse::allTasksCompleted() const {

    for (const Task& task : tasks) {

        if (!task.isCompleted()) {
            return false;
        }
    }

    return true;
}

// Run Simulation
void Warehouse::runSimulation() {

    std::cout << "Starting simulation...\n";

    simulationFailed = false;

    assignTasks();

    int step = 1;

    while (true) {

        std::cout << "\nStep " << step << "\n";

        update();

        // Stop immediately if failure occurs
        if (simulationFailed) {

            std::cout << "\nSimulation stopped due to battery failure!\n";
            break;
        }

        displayGrid();
        displayTaskProgress();
        // Display robot status
        displayRobotStatus();

        bool tasksDone = allTasksCompleted();

        bool chargingDone = true;

        for (int i = 0; i < robots.size(); i++) {

            if (returningToCharge[i]) {
                chargingDone = false;
                break;
            }
        }

        if (tasksDone && chargingDone) {
            break;
        }

        step++;
    }

    showSummary();
}

// Simulation Summary
void Warehouse::showSummary() const {

    std::cout << "\n========== SIMULATION SUMMARY ==========\n";

    std::cout << "Simulation finished.\n";

    std::cout << "Robot count: " << robots.size() << "\n";

    std::cout << "Task count: " << tasks.size() << "\n";

    std::cout << "Completed tasks: ";

    int completedCount = 0;

    for (const Task& task : tasks) {
        if (task.isCompleted()) {
            completedCount++;
        }
    }

    std::cout << completedCount << "/" << tasks.size() << "\n";

    std::cout << "\nFinal Robot Status:\n";

    displayRobotStatus();
}

// Set Charging Station
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


void Warehouse::displayTaskProgress() const {

    std::cout << "\n========== TASK STATUS ==========\n";

    std::cout << std::left
              << std::setw(6) << "Task"
              << std::setw(16) << "Name"
              << std::setw(12) << "Type"
              << "Progress\n";

    int waiting = 0;
    int inProgress = 0;
    int done = 0;

    for (const Task& task : tasks) {

        std::string status;

        if (task.isCompleted()) {
            status = "DONE";
            done++;
        }
        else if (task.getProgress() > 0) {
            status = "IN PROGRESS";
            inProgress++;
        }
        else {
            status = "WAITING";
            waiting++;
        }

        std::string progress =
            std::to_string(task.getProgress()) + "/" +
            std::to_string(task.getDuration());

        std::cout << std::left
                  << std::setw(6) << ("T" + std::to_string(task.getId()))
                  << std::setw(16) << task.getName()
                  << std::setw(12) << task.getType()
                  << progress << "\n";
    }

    std::cout << "\nTasks: Waiting " << waiting
              << " | In Progress " << inProgress
              << " | Done " << done << "\n";
}
