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
};

#endif
