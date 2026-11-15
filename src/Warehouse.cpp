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
    for (Robot* robot : robots) {
        robot->work();
    }

    for (Task& task : tasks) {
        if (!task.isCompleted()) {
            task.updateProgress();
        }
    }
}

void Warehouse::displayGrid() const {
    std::cout << "Warehouse size: "
              << width << " x " << height << "\n";
}

void Warehouse::runSimulation() {
    std::cout << "Starting simulation...\n";

    for (int step = 0; step < 5; step++) {
        std::cout << "Step " << step + 1 << "\n";
        update();
    }

    showSummary();
}

void Warehouse::showSummary() const {
    std::cout << "Simulation finished.\n";
}
