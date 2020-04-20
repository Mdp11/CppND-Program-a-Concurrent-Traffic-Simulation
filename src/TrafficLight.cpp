#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock lock{_mtx};
    _condition_variable.wait(lock, [this]{ return !_queue.size() != 0; });
    T msg = std::move(_queue.front());
    _queue.pop_front();
    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard lock{_mtx};
    _queue.emplace_back(msg);
    _condition_variable.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    while(true)
    {
        if(_queue.receive() == TrafficLightPhase::green)
        {
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}


// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distribution(4,6);
    TrafficLightPhase last_phase = _currentPhase;
    while(true)
    {
        auto phase_duration = distribution(rng);

        auto start = std::chrono::steady_clock::now();
        while(std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count() < phase_duration)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

        }
        _currentPhase = last_phase = last_phase == TrafficLightPhase::red ? TrafficLightPhase::green : TrafficLightPhase::red;
        _queue.send(std::move(last_phase));
    }   
}
