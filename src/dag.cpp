#include "dag.hpp"


void DAG::GenerateRunnables() {
    std::cout << "\033[H\033[2J\033[3J";
    std::cout << "*** How many Runnables to Generate? ***" << "\n";
    std::cout << "Enter Number : ";
    int numberOfRunnables;
    std::cin >> numberOfRunnables;

    std::clog << "\033[H\033[2J\033[3J";
    std::clog << "[Runnable Generation] Random Generation Start" << std::endl;

	std::clog << "===============================================[Debug : Runnable Generation}===============================================" << std::endl;
    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        std::shared_ptr<Runnable> runnable(new Runnable(runnableIndex, runnableIndex, std::rand() % 1000));
        this->runnableSet_.push_back(runnable);
        std::clog << "[DAG.cpp] Runnable ID : " << runnable->id_ << ", Execution Time : " << runnable->GetExecutionTime() << std::endl;
    }

    this->RandomEdge();
    this->SetInputRunnableList();
    this->SetOutputRunnableList();
	
	std::clog << "[Runnable Generation] Random Generation End" << std::endl;
	std::clog << "===========================================================================================================================" << std::endl;
}

void Dag::SetInputRunnableList() {
    // inputRunnables clear
    std::vector<std::shared_ptr<Runnable>> tmpList;
    tmpList.swap(this->inputrunnableSet_);

    for (auto &runnable : this->runnableSet_) {
		if (runnable->GetStatus() == 0) {
			inputrunnableSet_.push_back(runnable->GetSharedPtr());
		}
	}
}

void Dag::SetOutputRunnableList() {
    // outputRunnables clear
    std::vector<std::shared_ptr<Runnable>> tmpList;
    tmpList.swap(this->outputrunnableSet_);

    for (auto &runnable : this->runnableSet_) {
		if (runnable->GetStatus() == 1) {
			outputrunnableSet_.push_back(runnable->GetSharedPtr());
		}
	}
}

void DAG::RandomEdge() { //Runnable edge random generation
    std::cout << "*** Enter Runnables' Edge Rate (Recommend : 20 ~ 40 (%)) ***" << "\n";
    std::cout << "Enter Number : ";
    int rate = -1;
    std::cin >> rate; 

    for (auto &runnable : this->runnableSet_) {
        for (auto &outputRunnable : this->runnableSet_) {
            if ((rand() % 100) < rate && runnable->id_ < outputRunnable->id_) {
                runnable->LinkOutputRunnable(outputRunnable->GetSharedPtr());
            }
        }
    }
}

/* Save File & Parsing File Section */
void DAG::SaveDag(std::string dataDirectory) {
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value dagObject(rapidjson::kObjectType);
    rapidjson::Value runnableArray(rapidjson::kArrayType);

    for (auto &runnable : this->runnableSet_) {
        rapidjson::Value runnableObject(rapidjson::kObjectType);
        rapidjson::Value inputRunnableArray(rapidjson::kArrayType);
        rapidjson::Value outputRunnableArray(rapidjson::kArrayType);

        runnableObject.AddMember("ID", runnable->GetId(), allocator);
        runnableObject.AddMember("Execution Time", static_cast<double>(runnable->GetExecutionTime()) / 1000.0, allocator);
        runnableObject.AddMember("Status", runnable->GetStatus(), allocator);

        for (auto &inputRunnable : runnable->GetInputRunnables()) {
            inputRunnableArray.PushBack(inputRunnable->GetId(), allocator);
        }
        runnableObject.AddMember("Input Runnables's ID", inputRunnableArray, allocator);

        for (auto &outputRunnable : runnable->GetOutputRunnables()) {
            outputRunnableArray.PushBack(outputRunnable->GetId(), allocator);
        }
        runnableObject.AddMember("Output Runnable's ID", outputRunnableArray, allocator);

        runnableArray.PushBack(runnableObject, allocator);
    }
    dagObject.AddMember("Runnables", runnableArray, allocator);

    // Save to json
    std::string fileName = dataDirectory + "/DAG.json";

    std::ofstream ofs(fileName.c_str());
    rapidjson::OStreamWrapper osw(ofs);

    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
    dagObject.Accept(writer);

    ofs.close();
}

void DAG::ParseDag(std::string jsonPath) {
    std::ifstream ifs(jsonPath.c_str());

    // Exception
    if (ifs.fail()) {
        throw std::runtime_error("File doesn't exist.");
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    std::cout << "Parse Runnables Start" << std::endl;

    // Generate Runnable from .json
    for (auto &runnable : doc["Runnables"].GetArray()) {
        std::shared_ptr<Runnable> tmpRunnable(new Runnable(runnable["ID"].GetInt(), static_cast<int>(runnable["Execution Time"].GetDouble() * 1000.0)));
        this->runnableSet_.push_back(tmpRunnable);
    }

    // Link Edge
    for (auto &runnable : doc["Runnables"].GetArray()) {
        std::shared_ptr<Runnable> tmpRunnable = this->runnableSet_[runnable["ID"].GetInt()];

        for (auto &outputRunnable : runnable["Output Runnable's ID"].GetArray()) {
            tmpRunnable->LinkOutputRunnable(this->runnableSet_[outputRunnable.GetInt()]->GetSharedPtr());
        }
    }

    // Set Status
    this->SetInputRunnableList();
    this->SetOutputRunnableList();

    ifs.close();
}