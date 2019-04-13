/**
 * GamepadF710 Class
 * For rapid prototyping with Logitch F720 Gamepad
 * Author: Christopher Bate March, 2019
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 * christopherbate.github.com/Gamepad-F710
 * 
 * Usage: 
 * Gamepad gamepad;
 * gamepad.Start();
 * 
 * Gamepad::GamepadEvent event;
 * bool success = gamepad.Read(event);
 * 
 * Exception handeling is not used for speed and to avoid too many
 * issues with ROS transfering between sim and real environments. On failiure, you
 * need to re-call Start()
 */
#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <linux/joystick.h>
#include <fstream>
#include <string>

namespace ctbate
{
class Gamepad
{
  public:
    enum EVENT_TYPE
    {
        AXIS = 2,
        BUTTON = 1
    };

    enum AXIS_ID : uint8_t
    {
        LEFT_THUMB_X = 0,
        LEFT_THUMB_Y = 1,
        LEFT_TRIGGER = 2,
        RIGHT_THUMB_X = 3,
        RIGHT_THUMB_Y = 4,
        RIGHT_TRIGGER = 5,
        DPAD_X = 6,
        DPAD_Y = 7
    };

    enum BUTTON_ID : uint8_t
    {
        A_BUTTON = 0,
        B_BUTTON = 1,
        X_BUTTON = 2,
        Y_BUTTON = 3,
        LEFT_BUMPER = 4,
        RIGHT_BUMPER = 5
    };

    struct GamepadEvent
    {
        EVENT_TYPE type;
        bool down;
        double axisValue;
        AXIS_ID aid;
        BUTTON_ID bid;
    };

  public:
    Gamepad();
    ~Gamepad();

    std::string GetInfo();

    void Start();

    /** 
     * Attempts to read a js event 
     * On failure, return false
     * **/
    bool ReadRaw( js_event &event );

    /**
     * Attempts to read an event and process the event
     * into a "GamepadEvent"
     * On failure, return false
     */
    bool Read( GamepadEvent &event );

    /**
     * Prints the linux js_event type event
     */
    void PrintEvent(js_event event);

    /**
     * Prints the gamepad style event 
     * of this class
     */
    void PrintEvent(GamepadEvent event);

  private:
    std::ifstream m_input;
    bool m_enabled;
};
} // namespace ctbate

#endif // !GAMEPAD_H