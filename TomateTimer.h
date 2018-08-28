/*
 * TomateTimer.h
 *
 *  Created on: Aug 26, 2018
 *      Author: diebm
 */

#ifndef TOMATETIMER_H_
#define TOMATETIMER_H_

#include <ITask.h>
#include <Timer.h>
#include <IGpio.h>

class TomateTimer: public ranetos::ITask {
	ranetos::Timer & timer;
	ranetos::IGpio & startButton;
	unsigned char count;
	unsigned char totalCount;
public:
	static const unsigned long ACTIVITY_LENGTH_MS = 1500000;
	static const unsigned long NORMAL_BREAK_LENGTH_MS = 300000;
	static const unsigned long LONG_BREAK_LENGTH_MS = 1200000;
	enum State {
		IDLE_STATE,
		ACTIVE_STATE,
		BREAK_STATE
	};
	TomateTimer(ranetos::Timer & timer, ranetos::IGpio & startButton):
		timer(timer), startButton(startButton) {
		_currentState = IDLE_STATE;
		count = 0;
		totalCount = 0;
	}
	virtual ~TomateTimer() {}

	virtual void work() override;

	State currentState() {
		return _currentState;
	}

	unsigned char getCount() const {
		return count;
	}

	void setCount(unsigned char count) {
		this->count = count;
	}

	unsigned char getTotalCount() const {
		return totalCount;
	}

	void setTotalCount(unsigned char totalCount) {
		this->totalCount = totalCount;
	}

private:
	State _currentState;
};

#endif /* TOMATETIMER_H_ */
