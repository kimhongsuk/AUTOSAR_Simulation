#ifndef __DAG_HPP__

#define __DAG_HPP__

#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <cmath>
#include <limits>
#include <cstring>
#include <time.h>
#include <stdlib.h>
#include <cstdio>
#include <numeric>
#include <fstream>
#include <stdexcept>
#include "runnable.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"


class Dag { 
private:
    // Member
    std::unique_ptr<RunnableSet> runnableSet_;

    // Special Runnables
    std::vector<std::shared_ptr<Runnable>> inputrunnables_;
    std::vector<std::shared_ptr<Runnable>> outputrunnables_;

    // Path
    std::vector<std::vector<std::shared_ptr<Runnable>> pathes_;
	
	// If Runnables are generated
    void RandomEdge();
    
	// Save Input/Output Runnable list
    void SetInputRunnableList();
    void SetOutputRunnableList();

    // Path
    void SetPathes();

    // Parse from .json
    void ParseDag(std::string jsonPath);

public:
    // Constructor
    DAG() { RandomEdge(); SetInputRunnableList(); SetOutputRunnableList(); SetPath(); }
    DAG(const std::string dagJsonPath) { SetInputRunnableList(); SetOutputRunnableList(); SetPath(); }

    // Destructor
    ~DAG() { runnableSet_.clear(); inputrunnableSet_.clear(); outputrunnableSet_.clear(); }

	// Get Values
    const std::vector<std::shared_ptr<Runnable>>& GetRunnables() const { return runnableSet_; }
    const std::shared_ptr<Runnable> GetRunnable(const int index) const { return runnableSet_[index]; }
    const int GetNumberOfRunnables() { return static_cast<int>(runnableSet_.size()); }

    const std::vector<std::shared_ptr<Runnable>>& GetInputRunnables() const { return inputrunnables_; }
    const std::shared_ptr<Runnable> GetInputRunnable(const int index) const { return inputrunnables_[index]; }
    const int GetNumberOfInputRunnables() { return static_cast<int>(inputrunnables_.size()); }

    const std::vector<std::shared_ptr<Runnable>>& GetOutputRunnables() const { return outputrunnables_; }
    const std::shared_ptr<Runnable> GetOutputRunnable(const int index) const { return outputrunnables_[index]; }
    const int GetNumberOfOutputRunnables() { return static_cast<int>(outputrunnables_.size()); }

    const std::vector<std::vector<std::shared_ptr<Runnable>>>& GetPathes() const { return pathes_; }
    const std::vector<std::shared_ptr<Runnable>>& GetPath(const int index) const { return pathes_[index]; }

    // Save to .json
    void SaveDag(const std::string dataDirectory);
};

#endif    // __DAG__HPP__