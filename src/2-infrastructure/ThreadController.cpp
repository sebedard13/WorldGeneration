#include "ThreadController.h"
#include "../1-foundation/MathUtils.h"
#include <thread>

ThreadController::ThreadController()
        : threadCount(ThreadController::THREADS) {

}

ThreadController::ThreadController(Output *output)
        : threadCount(ThreadController::THREADS){
    delete this->output;
    this->output = output;
}

ThreadController::~ThreadController() {
    delete[] counters;
}

void
ThreadController::runIteration(const unsigned min, const unsigned max, const std::function<void(unsigned i)> &func) {
    std::vector<std::thread> threads(threadCount);
    delete[] counters;
    counters = new std::atomic<Counters>[threadCount];
    const unsigned range = (max - min) / threads.size(); // may floor and miss some
    for (unsigned i = 0; i < threads.size() - 1; ++i) {
        threads[i] = std::thread(&ThreadController::run, this, &counters[i], i * range + min, (i + 1) * range + min,
                                 func);
    }

    //last thread do to max to not miss any;
    const unsigned i = static_cast<unsigned>(threads.size() - 1);
    threads[i] = std::thread(&ThreadController::run,
                             this,
                             &counters[i],
                             i * range + min,
                             max,
                             func);

    for (auto &thread: threads) {
        thread.detach();
    }
}

void ThreadController::runIterationOutput(const unsigned min, const unsigned max,
                                          const std::function<void(unsigned i)> &func) {
    runIteration(min, max, func);

    if (output != nullptr) {
        int time = 1;
        while (!isDone()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(time));
            output->setPercent(percent(getDoneOperations(), max));
            if (time < 128) {
                time *= 2;
            }
        }
    }
}

bool ThreadController::isDone() const {
    for (int i = 0; i < threadCount; ++i) {
        if (!counters[i].load().done) {
            return false;
        }
    }
    return true;
}

unsigned ThreadController::getDoneOperations() const {
    unsigned operations = 0;
    for (int i = 0; i < threadCount; ++i) {
        operations += counters[i].load().currentOperation;
    }
    return operations;
}

void ThreadController::run(std::atomic<Counters> *counter, const unsigned min, const unsigned max,
                           const std::function<void(unsigned i)> &func) {
    int m;
    if((max - min)<(128 * threadCount)){
        m = (max - min)*2;
    }
    else{
        m = (max - min) / (128 * threadCount);
    }


    for (unsigned i = min; i < max; ++i) {
        func(i);
        if ((i - min) % m) {
            counter->store({i - min, false});
        }
    }
    counter->store({max - min, true});
}

unsigned ThreadController::THREADS = std::thread::hardware_concurrency();

void ThreadController::setThreadNumber(unsigned int nb) {
    THREADS = nb;
}
