#ifndef __RUNNABLE__HPP__

#define __RUNNABLE__HPP__

#include <vector>
#include <memory>
#include "simulation_types.hpp"


class TASK;

class Runnable : public std::enable_shared_from_this<Runnable> { 
private:
	// Dynamic characters
    int status_ = 0;   // 0 = input runnable, 1 = output runnable, 2 = middle runnable
	int precedence_ = -1;
    int maxCycle_ = -1;

    // relative Runnables
    std::vector<std::weak_ptr<Runnable>> inputRunnables_;
    std::vector<std::shared_ptr<Runnable>> outputRunnables_;
    
    // mapped Task
    std::weak_ptr<TASK> task_;
    
    // time line
    std::vector<RequiredTime> executionTimes_;

    void SetStatus() { status_ = (inputRunnables_.size()) ? ((outputRunnables_.size()) ? 2 : 1) : 0; }
	
    void AddInputRunnable(const std::shared_ptr<Runnable> inputRunnable) { this->inputRunnables_.push_back(inputRunnable->GetWeakPtr()); this->SetStatus(); }
    void AddOutputRunnable(const std::shared_ptr<Runnable> outputRunnable) { this->outputRunnables_.push_back(outputRunnable->GetSharedPtr()); this->SetStatus(); }

public:
	// Inherent characters
    const int id_;
    const long long int executionTime_;   // ns (nano second)

    Runnable(int id, long long int executionTime) : id_(id), executionTime_((executionTime <= 0) ? 1 : executionTime) {}
    ~Runnable() {}

    // Get Const Values
    const int GetId() const { return id_; }
    long long int GetExecutionTime() const { return executionTime_; }

    // Get Dynamic Values
    const int GetStatus() const { return status_; }
    const int GetPrecedence() const { return precedence_; }
    const int GetMaxCycle() const { return maxCycle_; }
	
    const int GetNumberOfInputRunnables() { return static_cast<int>(inputRunnables_.size()); }
    const int GetNumberOfOutputRunnables() { return static_cast<int>(outputRunnables_.size()); }

    const std::shared_ptr<Runnable> GetInputRunnable(int index) { return inputRunnables_[index].lock(); }
    const std::shared_ptr<Runnable> GetOutputRunnable(int index) const { return outputRunnables_[index]->GetSharedPtr(); }

    const std::vector<std::shared_ptr<Runnable>> GetInputRunnables();
    const std::vector<std::shared_ptr<Runnable>> GetOutputRunnables() const { return outputRunnables_; }

    const std::shared_ptr<TASK> GetTask() { return task_.lock(); }
	
    // Set Dynamic Values
	void SetPrecedence(const int precedence) { precedence_ = precedence; }
    void SetMaxCycle(const int maxCycle) { maxCycle_ = maxCycle; }

    void SetTask(const std::shared_ptr<TASK> task) { task_ = task->GetWeakPtr(); }

    void LinkInputRunnable(const std::shared_ptr<Runnable> inputRunnable);
    void LinkOutputRunnable(const std::shared_ptr<Runnable> outputRunnable);

    // Pointer Function
    std::shared_ptr<Runnable> GetSharedPtr() { return shared_from_this(); }
    std::weak_ptr<Runnable> GetWeakPtr() { return weak_from_this(); }
};

class RunnableSet : public Runnable {
private:
    // Member
    std::vector<std::shared_ptr<Runnable>> runnables_;

    // Generate Runnables
    void GenerateRunnables();
    
    // Parse from .json
    void ParseRunnable(std::string jsonPath);

public:
    // Constructor
    RunnableSet() { GenerateRunnables(); }
    RunnableSet(const std::string dagJsonPath) { ParseRunnable(dagJsonPath); }

    // Destructor
    ~RunnableSet() {}

    // Get Values
    const std::vector<std::shared_ptr<Runnable>>& GetRunnables() const { return runnables_; }
    const std::shared_ptr<Runnable> GetRunnable(const int index) const { return runnables_[index]->GetSharedPtr(); }
    const int GetNumberOfRunnables() { return static_cast<int>(runnables_.size()); }

    // Save to .json
    void SaveRunnable(std::string jsonPath);
};

#endif   // __RUNNABLE__HPP__