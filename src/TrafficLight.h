#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

template <class T>
class MessageQueue
{
public:
    void send(T&& msg);
    T&& receive();

private:
    std::deque<T> _queue;
    std::condition_variable _condition_variable;
    std::mutex _mtx;
    
};

class TrafficLight : public TrafficObject
{

enum class TrafficLightPhase 
{
    Red,
    Green,
};

public:
    // constructor / desctructor

    // getters / setters

    // typical behaviour methods
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();

private:
    // typical behaviour methods
    void cycleThroughPhases();
    MessageQueue<TrafficLightPhase> _queue;
    std::condition_variable _condition;
    std::mutex _mutex;
    TrafficLightPhase _currentPhase;
};

#endif