/*
 * TomateTimer.cpp
 *
 *  Created on: Aug 26, 2018
 *      Author: diebm
 */

#include "TomateTimer.h"

void TomateTimer::work() {
	switch (_currentState) {
	case IDLE_STATE:
		if (startButton.isOn()) {
			timer.setTimeOut(ACTIVITY_LENGTH);
			timer.start();
			_currentState = ACTIVE_STATE;
		}
		break;
	case ACTIVE_STATE:
		if (timer.timedOut()) {
			if (count < 3) {
				timer.setTimeOut(NORMAL_BREAK_LENGTH);
			} else {
				timer.setTimeOut(LONG_BREAK_LENGTH);
			}
			timer.start();
			_currentState = BREAK_STATE;
		}
		break;
	case BREAK_STATE:
		if (timer.timedOut()) {
			timer.setOff(true);
			_currentState = IDLE_STATE;
			totalCount++;
			count = totalCount % 4;
		}
		break;
	default:
		break;
	}
}
