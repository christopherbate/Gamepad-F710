/**
 * Gamepad Class
 * For rapid prototyping with Logitch F720 Gamepad
 * Author: Christopher Bate March, 2019
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code package.
 * christopherbate.github.com/gamepad
 */
#include "Gamepad.hpp"

#include <linux/input.h>
#include <linux/joystick.h>
#include <cerrno>
#include <cstring>
#include <iostream>

#include <unistd.h>

#include <sstream>

using namespace ctbate;

/** Default files for controller input in linux evdev and js and js in xinput mode **/
const std::string f710File_evdev = "/dev/input/by-id/usb-Logitech_Logitech_Cordless_RumblePad_2-event-joystick";
const std::string f710File_js = "/dev/input/by-id/usb-Logitech_Logitech_Cordless_RumblePad_2-joystick";
const std::string f710File_js_xmode = "/dev/input/js0";

Gamepad::Gamepad() : m_enabled(false)
{
}

Gamepad::~Gamepad()
{
    m_input.close();
}

void Gamepad::Start()
{
    // Reset controller if necessary.
    if (m_input.is_open())
    {
        m_input.close();
    }

    // Open controller in x-input mode.
    m_input.open(f710File_js_xmode, std::ifstream::binary | std::ifstream::in);

    // If controller failed to open, set our status to false.
    if (!m_input.is_open())
    {
        m_enabled = false;
    }
    m_enabled = true;
}

bool Gamepad::ReadRaw(js_event &event)
{
    if (!m_enabled)
    {
        return false;
    }

    m_input.read((char *)&event, sizeof(struct js_event));

    // Check to make sure we got a full read.
    if (m_input.gcount() != sizeof(struct js_event))
    {
        m_enabled = false;
        return false;
    }

    return true;
}

bool Gamepad::Read(Gamepad::GamepadEvent &event)
{
    if (!m_enabled)
    {
        return false;
    }

    js_event js;
    if (!ReadRaw(js))
    {
        return false;
    }

    // Process this event into a gamepad event.
    GamepadEvent event;
    event.type = js.type == 1 ? BUTTON : AXIS;
    if (event.type == BUTTON)
    {
        event.bid = (BUTTON_ID)js.number;
        event.down = (js.value == 1);
    }
    else
    {
        event.aid = (AXIS_ID)js.number;
        event.axisValue = (((double)js.value / ((double)INT16_MAX)));

        // For the triggers, we convert all values to lie in a positive range
        // Otherwise by default, half the trigger value depressed will be zero.
        if (event.aid == RIGHT_TRIGGER || event.aid == LEFT_TRIGGER)
        {
            event.axisValue = (event.axisValue + 1.0) / 2.0;
        }
    }

    return true;
}

void Gamepad::PrintEvent(js_event event)
{
    std::stringstream ss;
    ss << "Event: type " << int(event.type) << " time " << event.time << " number " << int(event.number) << " value "
       << event.value;
    std::cout << ss.str() << std::endl;
}

void Gamepad::PrintEvent(Gamepad::GamepadEvent event)
{
    std::stringstream ss;
    if (event.type == Gamepad::AXIS)
    {
        ss << "Event: AXIS " << int(event.type) << " id " << event.aid << " value " << event.axisValue;
    }
    else
    {
        ss << "Event: BUTTON " << int(event.type) << " id " << event.bid << " down " << event.down;
    }

    std::cout << ss.str() << std::endl;
}