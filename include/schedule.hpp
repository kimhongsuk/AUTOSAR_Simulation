#ifndef __SCHEDULE__HPP__

#define __SCHEDULE__HPP__

#include <vector>
#include <stdexcept>
#include "dag.hpp"
#include "task.hpp"


class Schedule : public std::enable_shared_from_this<Schedule> {
private:
    // Command Pattern
    friend class Mapping;
    std::unique_ptr<Mapping> mapping_;

    friend class Sequencing;
    std::unique_ptr<Sequencing> sequencing_;

    // Members
    std::shared_ptr<Dag> dag_;
    std::shared_ptr<TaskSet> taskSet_;

    // Mapping
    void SelectMappingCommand();
    void DoMapping() { mapping_->DoMapping(); }

    // Sequencing
    void SelectSequencingCommand();
    void DoSequencing() { sequencing_->DoSequencing(); }

    // Parse from .json
    void ParseMapping(std::string jsonPath);

    // Parse from .json
    void ParseSequencing(std::string jsonPath);

public:
    // Constructor
    Schedule() { SelectMappingCommand(); DoMapping(); SelectSequencingCommand(); DoSequencing(); }

    // Destructor
    ~Schedule() {}

    // Save to .json
    void SaveDag(std::string dataDirectory);
    void SaveMapping(std::string dataDirectory);
    void SaveSequencing(std::string dataDirectory);

    // Pointer Function
    std::shared_ptr<Runnable> GetSharedPtr() { return shared_from_this(); }
};

#endif   // __SCHEDULE__HPP__