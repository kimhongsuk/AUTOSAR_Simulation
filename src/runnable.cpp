#include "runnable.hpp"


const std::vector<std::shared_ptr<Runnable>> Runnable::GetInputRunnables() {
    std::vector<std::shared_ptr<Runnable>> tmpRunnables;
    tmpRunnables.reserve(this->GetNumberOfInputRunnables());

    for (auto &inputRunnable : this->inputRunnables_) {
        tmpRunnables.emplace_back(inputRunnable.lock());
    }

    return tmpRunnables;
}

void Runnable::LinkInputRunnable(const std::shared_ptr<Runnable> inputRunnable) {
    bool searchFlag = false; 

    if (tmpRunnable) {
        for (auto &existInputRunnable : this->inputRunnables_) {
            if (tmpRunnable->GetId() == existInputRunnable->GetId()) {
                searchFlag = true;
                break;
            }
        }

        if (!searchFlag) {
            this->AddInputRunnable(inputRunnable->GetWeakPtr());
            tmpRunnable->AddOutputRunnable(this->GetSharedPtr());
        }
    }
}

void Runnable::LinkOutputRunnable(const std::shared_ptr<Runnable> outputRunnable) {
    bool searchFlag = false;
    
    for (auto &existOutputRunnable : this->outputRunnables_) {
        if (outputRunnable->GetId() == existOutputRunnable->GetId()) {
            searchFlag = true;
            break;
        }
    }

    if (!searchFlag) {
        this->AddOutputRunnable(outputRunnable);
        outputRunnable->AddInputRunnable(this->GetSharedPtr());
    }
}