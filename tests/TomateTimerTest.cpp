#include "TomateTimer.h"
#include <mocks/MockSystemClock.h>
#include <mocks/MockGpio.h>
#include <Timer.h>

//CppUTest includes should be after your and system includes
#include "CppUTest/TestHarness.h"

TEST_GROUP(TomateTimer) {
	TomateTimer* tomateTimer;
	ranetos::MockSystemClock* mockSystemClock;
	ranetos::MockGpio* startButton;
	ranetos::Timer* timer;

	void setup() {
		mockSystemClock = new ranetos::MockSystemClock();
		startButton = new ranetos::MockGpio();
		timer = new ranetos::Timer(*mockSystemClock);
		tomateTimer = new TomateTimer(*timer, *startButton);
	}
	void teardown() {
		delete tomateTimer;
		delete timer;
		delete startButton;
		delete mockSystemClock;
	}

	void idleToActive() {
		startButton->setOn(true);
		tomateTimer->work();
	}

	void activeToBreak() {
		mockSystemClock->setMillisecondsValue(mockSystemClock->milliseconds() + 90000001);
		tomateTimer->work();
	}

	void breakToIdle() {
		mockSystemClock->setMillisecondsValue(mockSystemClock->milliseconds() + 18000001);
		tomateTimer->work();
	}

	void longBreakToIdle() {
		mockSystemClock->setMillisecondsValue(mockSystemClock->milliseconds() + 72000001);
		tomateTimer->work();
	}
};

TEST(TomateTimer, StartIdle) {
	CHECK(tomateTimer->currentState() == TomateTimer::IDLE_STATE);
	tomateTimer->work();
	CHECK(tomateTimer->currentState() == TomateTimer::IDLE_STATE);
}

TEST(TomateTimer, IdleToActive) {
	startButton->setOn(true);
	tomateTimer->work();
	CHECK(tomateTimer->currentState() == TomateTimer::ACTIVE_STATE);
	CHECK_FALSE(timer->isOff());
	LONGS_EQUAL(90000000, timer->getTimeOut());
}

TEST(TomateTimer, ActiveToBreak) {
	idleToActive();
	activeToBreak();

	CHECK(tomateTimer->currentState() == TomateTimer::BREAK_STATE);
	CHECK_FALSE(timer->isOff());
	LONGS_EQUAL(18000000, timer->getTimeOut());
}

TEST(TomateTimer, BreakToIdle) {
	idleToActive();
	activeToBreak();
	breakToIdle();

	CHECK(tomateTimer->currentState() == TomateTimer::IDLE_STATE);
	CHECK_TRUE(timer->isOff());
	LONGS_EQUAL(1, tomateTimer->getCount());
	LONGS_EQUAL(1, tomateTimer->getTotalCount());
}

TEST(TomateTimer, countIncrement) {
	idleToActive();
	activeToBreak();
	breakToIdle();

	idleToActive();
	activeToBreak();
	breakToIdle();

	LONGS_EQUAL(2, tomateTimer->getCount());
	LONGS_EQUAL(2, tomateTimer->getTotalCount());
}

TEST(TomateTimer, longBreakAfterFourActives) {
	idleToActive();
	activeToBreak();
	breakToIdle();

	idleToActive();
	activeToBreak();
	breakToIdle();

	idleToActive();
	activeToBreak();
	breakToIdle();

	idleToActive();
	activeToBreak();

	CHECK(tomateTimer->currentState() == TomateTimer::BREAK_STATE);
	CHECK_FALSE(timer->isOff());
	LONGS_EQUAL(72000000, timer->getTimeOut());
}

TEST(TomateTimer, countResetAfterLongBreak) {
	idleToActive();
	activeToBreak();
	breakToIdle();

	idleToActive();
	activeToBreak();
	breakToIdle();

	idleToActive();
	activeToBreak();
	breakToIdle();

	idleToActive();
	activeToBreak();
	longBreakToIdle();

	CHECK(tomateTimer->currentState() == TomateTimer::IDLE_STATE);
	CHECK_TRUE(timer->isOff());
	LONGS_EQUAL(0, tomateTimer->getCount());

	LONGS_EQUAL(4, tomateTimer->getTotalCount());
}
