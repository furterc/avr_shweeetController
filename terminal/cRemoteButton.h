/*
 * cRemoteButton.h
 *
 *  Created on: 19 Feb 2017
 *      Author: christo
 */

#ifndef CREMOTEBUTTON_H_
#define CREMOTEBUTTON_H_

class cRemoteButton {
	void (*buttonCallback)(void) = 0;
	bool mEnabled = false;
	bool mBtnState = false;

public:
	cRemoteButton();
	void setCB(void (*cb)(void));
	void runCB();
	void setState(bool);
	bool getState();
	~cRemoteButton();
};

#endif /* CREMOTEBUTTON_H_ */
