#ifndef BUTTON_H_
#define BUTTON_H_
#include <avr/io.h>

class cButtonListner
{
protected:
    bool mEnabled;

public:
    cButtonListner();
    virtual ~cButtonListner(){};

    enum eButtons
    {
        MENU_UP,
        MENU_DOWN,
        MENU_LEFT,
        MENU_RIGHT,
        MENU_ENTER,
        MENU_BACK
    };

    void enable();
    void disable();

    virtual void pressed(eButtons button) = 0;
};

class cButtons
{
    uint8_t mPressed;
    cButtonListner *mListener;

public:
    cButtons();
    virtual ~cButtons();


    void setListener(cButtonListner *menu){ mListener = menu; };
    void handleButton(uint8_t button);

    void run();
};
#endif
extern cButtons Buttons;


