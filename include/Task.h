#ifndef TASK_H
#define TASK_H

#include <string>

class Task {
private:
    int id;
    std::string name;
    std::string type;
    int x;
    int y;
    int duration;
    int progress;
    bool completed;

public:
    Task(int id, std::string name, std::string type,
         int x, int y, int duration);

    void start();
    void updateProgress();
    void complete();

    int getId() const;
    std::string getType() const;
    bool isCompleted() const;
};

#endif
