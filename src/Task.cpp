#include "Task.h"

Task::Task(int id, std::string name, std::string type,
           int x, int y, int duration)
    : id(id), name(name), type(type), x(x), y(y),
      duration(duration), progress(0), completed(false) {
}

void Task::start() {
    progress = 0;
}

void Task::updateProgress() {
    if (progress < duration) {
        progress++;
    }

    if (progress >= duration) {
        complete();
    }
}

void Task::complete() {
    completed = true;
}

int Task::getId() const {
    return id;
}

std::string Task::getType() const {
    return type;
}

bool Task::isCompleted() const {
    return completed;
}
