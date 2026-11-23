#include <iostream>
#include <cassert>

#include "Task.h"

int main() {

    std::cout << "========== TASK UNIT TEST ==========\n";

    // Test 1: Picking Duration
    {
        Task task(1, "Pick_Item", "Picking", 2, 3);

        assert(task.getDuration() == 2);

        std::cout << "[PASS] Picking Duration\n";
    }

    // Test 2: Packing Duration
    {
        Task task(2, "Pack_Item", "Packing", 4, 2);

        assert(task.getDuration() == 3);

        std::cout << "[PASS] Packing Duration\n";
    }

    // Test 3: Initial Task Status
    {
        Task task(3, "Pick_Item", "Picking", 1, 1);

        assert(task.getProgress() == 0);
        assert(task.isCompleted() == false);

        std::cout << "[PASS] Initial Task Status\n";
    }

    // Test 4: Task Progress
    {
        Task task(4, "Pick_Item", "Picking", 1, 1);

        task.updateProgress();

        assert(task.getProgress() == 1);
        assert(task.isCompleted() == false);

        std::cout << "[PASS] Task Progress\n";
    }

    // Test 5: Task Completion
    {
        Task task(5, "Pick_Item", "Picking", 1, 1);

        task.updateProgress();
        task.updateProgress();

        assert(task.getProgress() == 2);
        assert(task.isCompleted() == true);

        std::cout << "[PASS] Task Completion\n";
    }

    std::cout << "\nAll Task Tests Passed!\n";

    return 0;
}
