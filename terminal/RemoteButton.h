#ifndef REMOTEBUTTON_H_
#define REMOTEBUTTON_H_

class cRemoteButton
{
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

#endif /* REMOTEBUTTON_H_ */
