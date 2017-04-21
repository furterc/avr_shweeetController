#include "Lights.h"
#include "Bluetooth.h"
#include "Packet.h"

#include <terminal.h>

cPacket packet;
void kitchenTop(cPacket packet)
{
    if (packet.getType() == packet.TYPE_GET)
    {
        packet.setType(packet.TYPE_SET);
        uint8_t data = mLights.getDuty(mLights.LIGHT_KITCHEN_TOP);

        packet.setData(data);
        uint8_t bytes[4];
        packet.toBytes(bytes);
        Bluetooth.transmit_packet(bytes, 4);
        return;
    }
    else
        mLights.setSoft(mLights.LIGHT_KITCHEN_TOP, 5, packet.getData());
}
extern const bt_dbg_entry btKitchenTop =
{ kitchenTop, packet.BT_KITCH_TOP };

void kitchenBot(cPacket packet)
{
    if (packet.getType() == packet.TYPE_GET)
    {
        packet.setType(packet.TYPE_SET);

        packet.setData(mLights.getDuty(mLights.LIGHT_KITCHEN_BOT));
        uint8_t bytes[4];
        packet.toBytes(bytes);
        Bluetooth.transmit_packet(bytes, 4);
        return;
    }
    else
        mLights.setSoft(mLights.LIGHT_KITCHEN_BOT, 5, packet.getData());
}
extern const bt_dbg_entry btKitchenBot =
{ kitchenBot, packet.BT_KITCH_BOT };

void studyTop(cPacket packet)
{
    if (packet.getType() == packet.TYPE_GET)
    {
        packet.setType(packet.TYPE_SET);
        uint8_t data = mLights.getDuty(mLights.LIGHT_STUDY_TOP);

        packet.setData(data);
        uint8_t bytes[4];
        packet.toBytes(bytes);
        Bluetooth.transmit_packet(bytes, 4);
        return;
    }
    else
        mLights.setSoft(mLights.LIGHT_STUDY_TOP, 5, packet.getData());
}
extern const bt_dbg_entry btStudyTop=
{ studyTop, packet.BT_STUDY_TOP};

void studyBot(cPacket packet)
{
    if (packet.getType() == packet.TYPE_GET)
    {
        packet.setType(packet.TYPE_SET);
        uint8_t data = mLights.getDuty(mLights.LIGHT_STUDY_BOT);

        packet.setData(data);
        uint8_t bytes[4];
        packet.toBytes(bytes);
        Bluetooth.transmit_packet(bytes, 4);
        return;
    }
    else
        mLights.setSoft(mLights.LIGHT_STUDY_BOT, 5, packet.getData());
}
extern const bt_dbg_entry btStudyBot=
{ studyBot, packet.BT_STUDY_BOT};

cLights::cLights()
{
    tFourPwm.init();
    tFivePwm.init();

    kitchenTop.setDutyC(0);
    kitchenBot.setDutyC(0);
    studyTop.setDutyC(0);
    studyBot.setDutyC(0);
    studyAlt.setDutyC(0);
}

void cLights::setSoft(Lights light, uint8_t delay, uint8_t duty)
{
    switch (light)
    {
    case LIGHT_KITCHEN_TOP:
        kitchenTopRun.setDuty(delay, duty);
        break;
    case LIGHT_KITCHEN_BOT:
        kitchenBotRun.setDuty(delay, duty);
        break;
    case LIGHT_STUDY_TOP:
        studyTopRun.setDuty(delay, duty);
        break;

    case LIGHT_STUDY_BOT:
        studyBotRun.setDuty(delay, duty);
        break;
    }
}

uint8_t cLights::getDuty(Lights light)
{
    switch (light)
    {
    case LIGHT_KITCHEN_TOP:
        return kitchenTop.getDutyC();
        break;
    case LIGHT_KITCHEN_BOT:
        return kitchenBot.getDutyC();
        break;
    case LIGHT_STUDY_TOP:
        return studyTop.getDutyC();
        break;
    case LIGHT_STUDY_BOT:
        return studyBot.getDutyC();
        break;
    default:
        return 0;
        break;
    }

    return 0;
}

void cLights::incLevel(Lights light)
{
    if ((uint8_t) light > LIGHT_COUNT - 1)
        return;

    uint8_t l = mLightLevels[(uint8_t) light];

    if (l < LEVEL_COUNT - 1)
        l++;
    else
        l = 0;

    setLevel(light, l, 6);
}

void cLights::setLevel(Lights light, Levels level)
{
    if ((uint8_t) level < LEVEL_COUNT)
        switch ((uint8_t) level)
        {
        case LEVEL_0:
            setSoft(light, 6, LEVEL_0_DUTY);
            break;
        case LEVEL_1:
            setSoft(light, 1, LEVEL_1_DUTY);
            break;
        case LEVEL_2:
            setSoft(light, 1, LEVEL_2_DUTY);
            break;
        case LEVEL_3:
            setSoft(light, 1, LEVEL_3_DUTY);
            break;
        case LEVEL_4:
            setSoft(light, 1, LEVEL_4_DUTY);
            break;
        case LEVEL_5:
            setSoft(light, 1, LEVEL_5_DUTY);
            break;
        }
    mLightLevels[(uint8_t) light] = (uint8_t) level;
}

void cLights::setLevel(Lights light, uint8_t level, uint8_t delay)
{
    if ((uint8_t) level < LEVEL_COUNT)
        switch (level)
        {
        case LEVEL_0:
            setSoft(light, delay, LEVEL_0_DUTY);
            break;
        case LEVEL_1:
            setSoft(light, delay, LEVEL_1_DUTY);
            break;
        case LEVEL_2:
            setSoft(light, delay, LEVEL_2_DUTY);
            break;
        case LEVEL_3:
            setSoft(light, delay, LEVEL_3_DUTY);
            break;
        case LEVEL_4:
            setSoft(light, delay, LEVEL_4_DUTY);
            break;
        case LEVEL_5:
            setSoft(light, delay, LEVEL_5_DUTY);
            break;
        }
    mLightLevels[(uint8_t) light] = level;
}

void cLights::setLevel(Lights light, uint8_t level)
{
    if ((uint8_t) level < LEVEL_COUNT)
        switch (level)
        {
        case 0:
            setSoft(light, 1, LEVEL_0_DUTY);
            break;
        case 1:
            setSoft(light, 1, LEVEL_1_DUTY);
            break;
        case 2:
            setSoft(light, 1, LEVEL_2_DUTY);
            break;
        case 3:
            setSoft(light, 1, LEVEL_3_DUTY);
            break;
        case 4:
            setSoft(light, 1, LEVEL_4_DUTY);
            break;
        case 5:
            setSoft(light, 1, LEVEL_5_DUTY);
            break;
        }

    mLightLevels[(uint8_t) light] = level;
}
uint8_t cLights::getLevel(Lights light)
{
    return mLightLevels[(uint8_t) light];
}

void cLights::runDelay()
{
    kitchenTopRun.run();
    kitchenBotRun.run();
    studyTopRun.run();
    studyBotRun.run();
}

cLights::~cLights()
{
}

cLights mLights;

