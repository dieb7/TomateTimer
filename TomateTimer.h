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
public:
	enum State {
		IDLE_STATE,
		ACTIVE_STATE,
		BREAK_STATE
	};
	TomateTimer(ranetos::Timer & timer, ranetos::IGpio & startButton):
		timer(timer), startButton(startButton) {
		_currentState = IDLE_STATE;
	}
	virtual ~TomateTimer() {}

	virtual void work() override;

	State currentState() {
		return _currentState;
	}
private:
	State _currentState;
};

#endif /* TOMATETIMER_H_ */
