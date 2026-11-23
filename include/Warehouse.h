#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <vector>
#include "Robot.h"
#include "Task.h"

class Warehouse {
private:
    int width;
    int height;

    std::vector<Robot*> robots;
    std::vector<Task> tasks;

    // Task assignment
    std::vector<std::vector<int>> robotTaskQueues;
    std::vector<int> currentTaskIndices;
    std::vector<bool> taskStarted;

    int chargingStationX;
    int chargingStationY;
    std::vector<bool> returningToCharge;
    bool simulationFailed;
    int totalTimesteps;
    bool hasEnoughBattery(const Robot* robot, const Task& task) const;

    bool allTasksCompleted() const;

public:
    Warehouse(int width, int height);
    ~Warehouse();

    void addRobot(Robot* robot);
    void addTask(const Task& task);

    void assignTasks();
    void update();
    void displayGrid() const;
    void runSimulation();
    void showSummary() const;
    void displayRobotStatus() const;

    void setChargingStation(int x, int y);

    void displayTaskProgress() const;
};

#endif
