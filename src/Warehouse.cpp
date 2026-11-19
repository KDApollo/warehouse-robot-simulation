#include "Warehouse.h"
#include <iostream>

Warehouse::Warehouse(int width, int height)
    : width(width), height(height) {
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

    std::cout << "Tasks assigned successfully!\n";
}



void Warehouse::update() {

    for (int i = 0; i < robots.size(); i++) {

        // Check whether this robot has any remaining tasks
        if (currentTaskIndices[i] >= robotTaskQueues[i].size()) {
            continue;
        }

        // Get the current task assigned to this robot
        int taskIndex = robotTaskQueues[i][currentTaskIndices[i]];

        Task& task = tasks[taskIndex];
        Robot* robot = robots[i];

        // Skip completed tasks
        if (task.isCompleted()) {
            currentTaskIndices[i]++;
            continue;
        }

        // Move toward the task location
        if (robot->getX() != task.getX() ||
            robot->getY() != task.getY()) {

            robot->moveOneStep(task.getX(), task.getY());

            std::cout << robot->getName()
                      << " moved to ("
                      << robot->getX() << ", "
                      << robot->getY() << ")\n";
        }
        else {

            // Start the task only once
            if (!taskStarted[taskIndex]) {
                task.start();
                taskStarted[taskIndex] = true;
                robot->work();
            }

            // Perform the task
            task.updateProgress();

            std::cout << robot->getName()
                      << " is working on Task "
                      << task.getId() << "\n";

            // Move to the next task when completed
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
    std::cout << "Warehouse size: "
              << width << " x " << height << "\n";
}

void Warehouse::runSimulation() {
    std::cout << "Starting simulation...\n";

    assignTasks();

    for (int step = 0; step < 5; step++) {
        std::cout << "Step " << step + 1 << "\n";
        update();
    }

    showSummary();
}

void Warehouse::showSummary() const {
    std::cout << "Simulation finished.\n";
}
