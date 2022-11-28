

void Schedule::SelectMappingCommand() {
    std::cout << "\033[H\033[2J\033[3J";
    std::cout << "*** Select Mapping Strategy ***" << "\n";
    std::cout << " 0 : Input Mapping" << "\n";
    std::cout << " 1 : Rate Mapping" << "\n";
    std::cout << " 2 : Random Mapping" << "\n";
    std::cout << "\n" << "Enter Number : ";
    int mappingMethod = -1;
    std::cin >> mappingMethod;
    
    switch (mappingMethod) {
        case 0 : {
            this->mapping_ = std::make_unique<InputMapping>();
            break;
        }

        case 1 : {
            this->mapping_ = std::make_unique<RateMapping>();
            break;
        }

        case 2 : {
            this->mapping_ = std::make_unique<RandomMapping>();
            break;
        }

        default : {
            throw std::out_of_range("you selected unexpected option.");;
        }
    }
}

void Schedule::GenerateTasks() {
    std::cout << "\033[H\033[2J\033[3J";
    std::cout << "*** How many Tasks to Generate? ***" << "\n";
    int numberOfTasks;
    std::cin >> numberOfTasks;

	std::clog << "=================================================[Debug : Task Generation}=================================================" << std::endl;
    std::cout << "\n[Task Generation] Generation Start" << std::endl;

    for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
        std::cout << "[Task Generation] " << taskIndex << " -th Task's Period : ";
        int tmpPeriod = -1;
        std::cin >> tmpPeriod;

        std::cout << "[Task Generation] " << taskIndex << " -th Task's Offset : ";
        int tmpOffset = 0;
        // std::cin >> tmpOffset;
        
        std::shared_ptr<Task> task(new Task(taskIndex, tmpPeriod * 1000, tmpOffset * 1000));
        this->tasks_.push_back(task);
        std::clog << "[DAG.cpp] Task ID : " << this->tasks_[taskIndex]->id_ << ", Period : " << this->tasks_[taskIndex]->period_ << ", Offset : " << this->tasks_[taskIndex]->offset_ << std::endl;
    }

    this->DoMapping();

    if (this->CheckMappable()) {
        std::cout << "This Mappable!" << std::endl;
        this->SetTaskPriority();
        mappableFlag = true;
    }
    else {
        std::cout << "Increse Tasks Period!" << std::endl;
        this->ClearTaskMapping();
    }
	
	std::cout << "[Task Generation] Generation End" << std::endl;

	std::clog << "===========================================================================================================================" << std::endl;
}

void Schedule::SetTaskPriority() {
    std::vector<std::shared_ptr<Task>> tmpTaskArray(this->taskSet_);

    std::clog << "\033[H\033[2J\033[3J";
	std::clog << "==================================================[Debug : Task Priority]==================================================" << std::endl;

    std::sort(tmpTaskArray.begin(), tmpTaskArray.end(), [](std::shared_ptr<Task> a, std::shared_ptr<Task> b) { return a->period_ < b->period_; });
    std::clog << "[DAG.cpp] Sort Fin " << std::endl;

    int numberOfTasks = static_cast<int>(this->taskSet_.size());
    for (int taskPriority = 0; taskPriority < numberOfTasks; taskPriority++) {
        tmpTaskArray[taskPriority]->SetPriority(taskPriority);
    }

    this->tasksInPriority_.swap(tmpTaskArray);
	
	std::clog << "===========================================================================================================================" << std::endl;
}

void Schedule::ClearTaskSet() {
    std::vector<std::shared_ptr<Task>>().swap(this->taskSet_);
}

/* Save File & Parsing File Section */
void DAG::SaveMapping(std::string dataDirectory) {
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value dagObject(rapidjson::kObjectType);
    rapidjson::Value runnableArray(rapidjson::kArrayType);
    rapidjson::Value taskArray(rapidjson::kArrayType);

    dagObject.AddMember("Utilization", this->utilization_, allocator);

    dagObject.AddMember("Utilization Bound", this->utilizationBound_, allocator);

    for (auto &task : this->tasks_) {
        rapidjson::Value taskObject(rapidjson::kObjectType);
        rapidjson::Value runnableArray(rapidjson::kArrayType);

        taskObject.AddMember("Period", static_cast<double>(task->period_) / 1000.0, allocator);
        taskObject.AddMember("Offset", static_cast<double>(task->offset_) / 1000.0, allocator);
        taskObject.AddMember("Core", task->GetCore(), allocator);

        for (auto &runnable : task->GetRunnables()) {
            runnableArray.PushBack(runnable->realId_, allocator);
        }
        taskObject.AddMember("Runnables", runnableArray, allocator);

        taskArray.PushBack(taskObject, allocator);
    }
    dagObject.AddMember("Tasks", taskArray, allocator);

    // Save to json
    std::string fileName = dataDirectory + "/Mapping.json";

    std::ofstream ofs(fileName.c_str());
    rapidjson::OStreamWrapper osw(ofs);

    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
    dagObject.Accept(writer);

    ofs.close();
}

void DAG::ParseMapping(std::string jsonPath) {
    std::ifstream ifs(jsonPath.c_str());

    if (ifs.fail()) {
        throw std::runtime_error("File doesn't exist.");
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    std::cout << "Parse Mapping Start" << std::endl;

    // Generate Task from .json
    this->ClearTaskMapping();
	
	int taskIndex = 0;
	for (auto &task : doc["Tasks"].GetArray()) {
        std::shared_ptr<TASK> tmpTask(new Task(taskIndex, static_cast<int>(task["Period"].GetDouble() * 1000.0), static_cast<int>(task["Offset"].GetDouble() * 1000.0)));
        tmpTask->SetCore(task["Core"].GetInt());
        this->tasks_.push_back(tmpTask);
        taskIndex++;

        for (auto &runnableRealId : task["Runnables"].GetArray()) {
            auto iter = std::find_if(this->runnables_.begin(), this->runnables_.end(), [&runnableRealId](std::shared_ptr<RUNNABLE> a) { return a->GetId() == runnableRealId.GetInt(); });
            if (this->runnables_.end() != iter) {
                tmpTask->AddRunnable(*iter);
            }
        }
    }

    this->SetTaskPriority();

    ifs.close();
}