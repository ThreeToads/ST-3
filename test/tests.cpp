// Copyright 2024 Kokin Ivan

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include <thread>
#include <chrono>
#include "TimedDoor.h"

class MockTimerClient : public TimerClient {
public:
    MOCK_METHOD(void, Timeout, (), (override));
};

class TimedDoorTest : public ::testing::Test {
protected:
    TimedDoor door;
    MockTimerClient *timerC;
    Timer timer;
public:
    TimedDoorTest() : door(1), timer() {}
protected:
    void SetUp() override {
        timerC = new MockTimerClient();
    }

    void TearDown() override {
        delete timerC;
    }
};

TEST_F(TimedDoorTest, TimedDoorLockedDef) {
    EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, DoorTimerAdapter) {
    DoorTimerAdapter adapter(door);
    EXPECT_NO_THROW(adapter.Timeout());
}

TEST_F(TimedDoorTest, TimedDoorLock) {
    EXPECT_THROW(door.lock(), std::logic_error);
}

TEST_F(TimedDoorTest, Lock) {
    door.unlock();
    door.lock();
    EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, opened_exception_throw) {
    door.unlock();
    EXPECT_THROW(door.throwState(), std::runtime_error);
}

TEST_F(TimedDoorTest, throw_lock) {
    door.unlock();
    door.lock();
    EXPECT_THROW(door.throwState(), std::runtime_error);
}

TEST_F(TimedDoorTest, Unclock_test) {
    door.unlock();
    EXPECT_THROW(door.unlock(), std::logic_error);
}

TEST_F(TimedDoorTest, TimerTimeoutCall) {
    EXPECT_CALL(*timerC, Timeout()).Times(1);
    timer.tregister(100, timerC);
}

TEST_F(TimedDoorTest, TimeoutThrowsOnUnlocked) {
    DoorTimerAdapter adapter(door);
    door.unlock();
    EXPECT_THROW(adapter.Timeout(), std::runtime_error);
}

TEST_F(TimedDoorTest, unlock_door_test) {
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());
}
