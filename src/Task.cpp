#include "Task.h"

Task::Task(int id, std::string name, std::string type,
           int x, int y)
    : id(id),
      name(name),
      type(type),
      x(x),
      y(y),
      duration(type == "Picking" ? 2 : 3),
      progress(0),
      completed(false) {}

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

int Task::getX() const {
    return x;
}

int Task::getY() const {
    return y;
}

int Task::getDuration() const {
    return duration;
}