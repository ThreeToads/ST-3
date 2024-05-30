// Copyright 2024 Kokin Ivan

#include <stdexcept>
#include <thread> // NOLINT [build/c++11]
#include <chrono> // NOLINT [build/c++11]

#include "TimedDoor.h"

void Timer::sleep(int sec) {
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}

void Timer::tregister(int sec, TimerClient* timer) {
    client = timer;
    sleep(sec);
    client->Timeout();
    client = nullptr;
}

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& door) : door(door) {}

void DoorTimerAdapter::Timeout() {
    if (door.isDoorOpened()) {
        throw std::runtime_error("Door open so looooooong");
    }
}

TimedDoor::TimedDoor(int timeout) : iTimeout(timeout), isOpened(false) {
    adapter = new DoorTimerAdapter(*this);
}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::lock() {
    if (!isOpened) {
        throw std::logic_error("Error! Door locked");
    }
    isOpened = false;
}

void TimedDoor::unlock() {
    if (isOpened) {
        throw std::logic_error("Error! Door unlocked");
    }
    isOpened = true;
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void TimedDoor::throwState() {
    if (isOpened) {
        throw std::runtime_error("Error! Opened");
    } else {
        throw std::runtime_error("Error! Closed");
    }
}
