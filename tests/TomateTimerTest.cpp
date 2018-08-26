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
};

TEST(TomateTimer, StartIdle) {
	CHECK(tomateTimer->currentState() == TomateTimer::IDLE_STATE);
	tomateTimer->work();
	CHECK(tomateTimer->currentState() == TomateTimer::IDLE_STATE);
}

TEST(TomateTimer, StartActive) {
	startButton->setOn(true);
	tomateTimer->work();
	CHECK(tomateTimer->currentState() == TomateTimer::ACTIVE_STATE);
	CHECK_FALSE(timer->isOff());
	LONGS_EQUAL(90000000, timer->getTimeSet());
}

TEST(TomateTimer, ActiveToBreak) {
	startButton->setOn(true);
	tomateTimer->work();
	mockSystemClock->setMillisecondsValue(mockSystemClock->milliseconds() + 90000001);

	tomateTimer->work();
	CHECK(tomateTimer->currentState() == TomateTimer::BREAK_STATE);
	CHECK_FALSE(timer->isOff());
	LONGS_EQUAL(18000000, timer->getTimeSet());
}

