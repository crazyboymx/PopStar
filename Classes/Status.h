/*
 * Status.h
 *
 *  Created on: 2013-7-18
 *      Author: maxing
 */

#ifndef STATUS_H_
#define STATUS_H_

#define MAX_UNREGULAR_STAGE 10

class Status {
public:
	Status();
	virtual ~Status();
    bool mute;
    int highScore;
    int stage;
    int score;
    int target;
    int lastTarget;
};

#endif /* STATUS_H_ */
