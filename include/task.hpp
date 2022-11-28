#ifndef __TASK_HPP__

#define __TASK_HPP__

#include "RUNNABLE.hpp"

#include <vector>
#include <memory>
#include <algorithm>


class TASK : public std::enable_shared_from_this<TASK> {
private:
	// Dynamic characters
	int priority_ = -1;
	long long int executionTime_ = -1;
    int core_ = -1;

    std::vector<std::shared_ptr<RUNNABLE>> runnables_;
	
	void SetExecutionTime();

public:
	// Inherent characters
    const int id_;
    const long long int period_;
    const long long int offset_;

    TASK(int id, int period, int offset) : id_(id), period_(period), offset_(offset) {}
    ~TASK() {}

    // Const Values
    const int GetId() const { return id_; }
    const long long int GetPeriod() const { return period_; }
    const long long int GetOffset() const { return offset_; }
    
    // Dynamic Values
    const int GetPriority() const { return priority_; }
    const long long int GetExecutionTime() { return executionTime_; }
    const int GetCore() const { return core_; }

    const int GetNumberOfRunnables() { return static_cast<int>(runnables_.size()); }

    const std::vector<std::shared_ptr<RUNNABLE>>& GetRunnables() const { return runnables_; }
	const std::shared_ptr<RUNNABLE>& GetRunnable(const int index) const { return runnables_[index]->GetSharedPtr(); }
	
	void SetPriority(const int priority) { priority_ = priority; }
    void SetCore(const int number) { core_ = number; }

    void AddRunnable(const std::shared_ptr<RUNNABLE> runnable);

    void ClearMapping();

    std::shared_ptr<TASK> GetSharedPtr() { return shared_from_this(); }
    std::shared_ptr<TASK> GetWeakPtr() { return weak_from_this(); }
};

class TaskSet : public Task {
private:
    // Member
    std::vector<std::shared_ptr<Task>> tasks_;

    // Generate Tasks
    void GenerateTasks();
    void SetTaskPriority();
    void ClearTaskSet();
    
    // Parse from .json
    void ParseTask(std::string jsonPath);

public:
    // Constructor
    TaskSet() { GenerateTasks(); }
    TaskSet(const std::string dagJsonPath) { ParseTask(dagJsonPath); }

    // Destructor
    ~TaskSet() {}

    // Get Values
    const std::vector<std::shared_ptr<Task>>& GetTasks() const { return tasks_; }
    const std::shared_ptr<Task> GetTask(const int index) const { return tasks_[index]->GetSharedPtr(); }
    const int GetNumberOfTasks() { return static_cast<int>(tasks_.size()); }

    // Save to .json
    void SaveTask(std::string jsonPath);
};

#endif   // __TASK__HPP__