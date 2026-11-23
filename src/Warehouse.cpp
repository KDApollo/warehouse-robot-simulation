
#include <cstdlib>
#include "Warehouse.h"
#include <iostream>
#include <map>
#include <iomanip>
#include <thread>
#include <chrono>

Warehouse::Warehouse(int width, int height)
    : width(width),
      height(height),
      chargingStationX(-1),
      chargingStationY(-1),
      simulationFailed(false),
      assignmentFailed(false),
      totalTimesteps(0) {
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

    assignmentFailed = false;

    if (robots.empty()) {
        std::cout << "No robots available!\n";
        assignmentFailed = true;
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
            std::cout << "ERROR: No compatible robot for Task "
                      << tasks[i].getId()
                      << " (" << taskType << ")\n";

            assignmentFailed = true;
        }
    }

    if (assignmentFailed) {
        std::cout << "Task assignment failed!\n";
    }
    else {
        std::cout << "Tasks assigned successfully!\n";
    }
}

bool Warehouse::hasEnoughBattery(
    const Robot* robot,
    const Task& task
) const {

    int distanceToTask = robot->distanceTo(
        task.getX(),
        task.getY()
    );

    int distanceToStation =
        abs(task.getX() - chargingStationX) +
        abs(task.getY() - chargingStationY);

    int workingBattery = task.getDuration() * 2;

    int requiredBattery =
        distanceToTask +
        workingBattery +
        distanceToStation;

    return robot->getBattery() >= requiredBattery;
}

// Update Simulation
void Warehouse::update(bool animationMode) {

    for (int i = 0; i < robots.size(); i++) {

        Robot* robot = robots[i];

        // 1. Check low battery
        if (returningToCharge[i] || robot->isLowBattery()) {

            returningToCharge[i] = true;

            if (chargingStationX == -1 || chargingStationY == -1) {

                if (!animationMode) {
                    std::cout << "No charging station available!\n";
                }

                simulationFailed = true;
                continue;
            }

            int requiredBattery = robot->distanceTo(
                chargingStationX, chargingStationY
            );

            if (robot->getBattery() < requiredBattery) {

                if (!animationMode) {
                    std::cout << robot->getName()
                              << " cannot reach charging station!\n";

                    std::cout << "Required battery: "
                              << requiredBattery << "%\n";

                    std::cout << "Available battery: "
                              << robot->getBattery() << "%\n";
                }

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

                if (!animationMode) {
                    std::cout << robot->getName()
                              << " returning to charging station. Battery: "
                              << robot->getBattery() << "%\n";
                }
            }
            else {

                // Charge battery
                robot->setStatus(RobotStatus::Charging);
                robot->chargeBattery(20);

                if (!animationMode) {
                    std::cout << robot->getName()
                              << " is charging. Battery: "
                              << robot->getBattery() << "%\n";
                }

                // Resume task when battery is full
                if (robot->getBattery() == 100) {

                    returningToCharge[i] = false;
                    robot->setStatus(RobotStatus::Idle);

                    if (!animationMode) {
                        std::cout << robot->getName()
                                  << " battery full. Resume task!\n";
                    }
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

            if (!animationMode) {
                std::cout << robot->getName()
                          << " does not have enough battery for Task "
                          << task.getId()
                          << ". Returning to charge.\n";
            }

            returningToCharge[i] = true;
            continue;
        }

        // 4. Move toward task
        if (robot->getX() != task.getX() ||
            robot->getY() != task.getY()) {

            robot->setStatus(RobotStatus::Moving);

            robot->moveOneStep(task.getX(), task.getY());

            if (!animationMode) {
                std::cout << robot->getName()
                          << " moved to ("
                          << robot->getX() << ", "
                          << robot->getY() << ")\n";
            }
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

            if (!animationMode) {
                std::cout << robot->getName()
                          << " is working on Task "
                          << task.getId() << "\n";
            }

            // Complete task
            if (task.isCompleted()) {

                if (!animationMode) {
                    std::cout << "Task "
                              << task.getId()
                              << " completed!\n";
                }

                currentTaskIndices[i]++;

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

            // Display Robot
            for (int i = 0; i < robots.size(); i++) {

                if (robots[i]->getX() == x &&
                    robots[i]->getY() == y) {

                    std::cout << "R" << i + 1 << " ";
                    found = true;
                    break;
                }
            }

            // Display Charging Station
            if (!found &&
                x == chargingStationX &&
                y == chargingStationY) {

                std::cout << "C  ";
                found = true;
            }

            // Display unfinished Tasks
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

            // Empty cell
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
                  << std::setw(8) << ("R" + std::to_string(i + 1))
                  << std::setw(12) << robot->getType()
                  << std::setw(12) << position
                  << std::setw(12) << robot->getStatusName()
                  << std::setw(10)
                  << (std::to_string(robot->getBattery()) + "%")
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
void Warehouse::runSimulation(bool animationMode) {

    if (animationMode) {
        system("cls");
    }

    std::cout << "Starting simulation...\n";

    simulationFailed = false;

    assignTasks();

    if (assignmentFailed) {
        std::cout << "\nSimulation cannot start!\n";
        std::cout << "Please check robot and task types.\n";

        simulationFailed = true;
        showSummary();
        return;
    }

    int step = 1;
    totalTimesteps = 0;

    while (true) {

        if (animationMode) {
            system("cls");
            std::cout << "Timestep: " << step << "\n";
        }
        else {
            std::cout << "\nStep " << step << "\n";
        }

        // Update simulation
        update(animationMode);
        totalTimesteps++;

        // Stop immediately if failure occurs
        if (simulationFailed) {

            std::cout << "\nSimulation stopped due to battery failure!\n";
            break;
        }

        // Display current state
        displayGrid();
        displayTaskProgress();
        displayRobotStatus();

        // Animation delay
        if (animationMode) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(1000)
            );
        }

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

    if (animationMode) {
        system("cls");

        std::cout << "========== FINAL RESULT ==========\n";

        displayGrid();
        displayTaskProgress();
        displayRobotStatus();
    }

    showSummary();
}

// Simulation Summary
void Warehouse::showSummary() const {

    std::cout << "\n========== SIMULATION SUMMARY ==========\n";

    if (simulationFailed) {
        std::cout << "Simulation stopped due to failure.\n";
    }
    else {
        std::cout << "Simulation completed successfully!\n";
    }

    std::cout << "Total timesteps: " << totalTimesteps << "\n";
    std::cout << "Robot count: " << robots.size() << "\n";
    std::cout << "Task count: " << tasks.size() << "\n";

    // Count completed tasks
    int completedCount = 0;

    for (const Task& task : tasks) {

        if (task.isCompleted()) {
            completedCount++;
        }
    }

    std::cout << "Completed tasks: "
              << completedCount << "/" << tasks.size() << "\n";

    std::cout << "\n========== ROBOT PERFORMANCE ==========\n";

    for (int i = 0; i < robots.size(); i++) {

        int robotCompletedTasks = 0;

        for (int taskIndex : robotTaskQueues[i]) {

            if (tasks[taskIndex].isCompleted()) {
                robotCompletedTasks++;
            }
        }

        std::cout << robots[i]->getName()
                  << " (" << robots[i]->getType() << ")"
                  << " completed "
                  << robotCompletedTasks
                  << " task(s).\n";
    }

    std::cout << "\n========== FINAL ROBOT STATUS ==========\n";

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

// Display Task Progress
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

        if (task.isCompleted()) {
            done++;
        }
        else if (task.getProgress() > 0) {
            inProgress++;
        }
        else {
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
