#ifndef LIGHTS_H_
#define LIGHTS_H_

#include "pwm.h"
#include "PwmRun.h"
#include "TimerPwm.h"

#define LEVEL_0_DUTY 0
#define LEVEL_1_DUTY 0xA
#define LEVEL_2_DUTY 0x15
#define LEVEL_3_DUTY 0x30
#define LEVEL_4_DUTY 0x50
#define LEVEL_5_DUTY 0xFF

#define LEVEL_COUNT 6
#define LIGHT_COUNT 4



class cLights
{
    typedef enum
    {
        LEVEL_0,
        LEVEL_1,
        LEVEL_2,
        LEVEL_3,
        LEVEL_4,
        LEVEL_5
    } Levels;

    uint8_t mLightLevels[LEVEL_COUNT];

    cPWM kitchenTop;
    cPWM kitchenBot;
    cPWM kitchenAlt;
    cPWM studyTop;
    cPWM studyBot;
    cPWM studyAlt;
    cPwmRun kitchenTopRun = cPwmRun(&kitchenTop);
    cPwmRun kitchenBotRun = cPwmRun(&kitchenBot);
    cPwmRun studyTopRun = cPwmRun(&studyTop);
    cPwmRun studyBotRun = cPwmRun(&studyBot);
    cTimerPwm tFivePwm = cTimerPwm(1, 5, &kitchenTop, &kitchenAlt, &kitchenBot);
    cTimerPwm tFourPwm = cTimerPwm(1, 4, &studyTop, &studyBot, &studyAlt);

public:
    typedef enum
    {
        LIGHT_KITCHEN_TOP,
        LIGHT_KITCHEN_BOT,
        LIGHT_STUDY_BOT,
        LIGHT_STUDY_TOP
    } Lights;

    cLights();
    void setSoft(Lights, uint8_t, uint8_t);
    uint8_t getDuty(Lights);
    void incLevel(Lights);
    void setLevel(Lights, Levels);
    void setLevel(Lights, uint8_t);
    void setLevel(Lights, uint8_t, uint8_t);
    uint8_t getLevel(Lights);
    void runDelay();
    virtual ~cLights();
};

extern cLights mLights;

#endif /* LIGHTS_H_ */
