#include "InputManager.h"

Input::InputManager::InputManager()
{
    actionsMaps = map<string, ActionMap*>();
    Init();
}

Input::InputManager::~InputManager()
{
    for (const pair<string, ActionMap*>& _pair : actionsMaps)
    {
        delete _pair.second;
    }
}

void Input::InputManager::Init()
{
    InitKeysHolding();
    InitMouseButtonHolding();
    InitJoystickButtonHolding();
}

void Input::InputManager::InitKeysHolding()
{
    for (int _keyIndex = 0; _keyIndex < Keyboard::KeyCount; _keyIndex++)
    {
        const Key& _key = CAST(Key, _keyIndex);
        keysIsHolding[_key] = false;
    }
}

void Input::InputManager::InitMouseButtonHolding()
{
    for (int _keyIndex = 0; _keyIndex < Mouse::ButtonCount; _keyIndex++)
    {
        const Button& _mouseButton = CAST(Button, _keyIndex);
        mouseButtonsIsHolding[_mouseButton] = false;
    }
}

void Input::InputManager::InitJoystickButtonHolding()
{
    for (int _joystickId = 0; _joystickId < Joystick::Count; _joystickId++)
    {
        joysticksButtonsIsHolding.push_back(vector<bool>());
        for (int _keyIndex = 0; _keyIndex < Joystick::ButtonCount; _keyIndex++)
        {
            joysticksButtonsIsHolding[_joystickId].push_back(false);
        }
    }
}


void Input::InputManager::UpdateActionMaps(const EventInfo& _event)
{
    for (const pair<string, ActionMap*>& _map : actionsMaps)
    {
        _map.second->Update(_event);
    }
}

void Input::InputManager::Update(RenderWindow& _window)
{
    while (const EventInfo& _event = _window.pollEvent())
    {
        LOG(Display, "IN");
        if (_event->is<Event::Closed>())
        {
            _window.close();
            return;
        }

        UpdateActionMaps(_event);
        if (_event->is<PressedKey>())
        {
            for (int _keyIndex = 0; _keyIndex < Keyboard::KeyCount; _keyIndex++)
            {
                const Key& _key = CAST(Key, _keyIndex);
                if (Keyboard::isKeyPressed(_key) && !keysIsHolding.at(_key))
                {
                    keysIsHolding[_key] = true;
                }
            }
        }
        else if (_event->is<ReleasedKey>())
        {
            for (int _keyIndex = 0; _keyIndex < Keyboard::KeyCount; _keyIndex++)
            {
                const Key& _key = CAST(Key, _keyIndex);
                if (!Keyboard::isKeyPressed(_key) && keysIsHolding.at(_key))
                {
                    keysIsHolding[_key] = false;
                }
            }
        }
        else if (_event->is<PressedMouseButton>())
        {
            for (int _keyIndex = 0; _keyIndex < Mouse::ButtonCount; _keyIndex++)
            {
                const Button& _mouseButton = CAST(Button, _keyIndex);
                if (Mouse::isButtonPressed(_mouseButton) && !mouseButtonsIsHolding.at(_mouseButton))
                {
                    mouseButtonsIsHolding[_mouseButton] = true;
                }
            }
        }
        else if (_event->is<ReleasedMouseButton>())
        {
            for (int _keyIndex = 0; _keyIndex < Mouse::ButtonCount; _keyIndex++)
            {
                const Button& _mouseButton = CAST(Button, _keyIndex);
                if (!Mouse::isButtonPressed(_mouseButton) && mouseButtonsIsHolding.at(_mouseButton))
                {
                    mouseButtonsIsHolding[_mouseButton] = false;
                }
            }
        }
        else if (_event->is<ControllerJoystickPressed>())
        {
            for (int _joystickId = 0; _joystickId < Joystick::Count; _joystickId++)
            {
                if (!Joystick::isConnected(_joystickId)) continue;
                for (int _keyIndex = 0; _keyIndex < Joystick::ButtonCount; _keyIndex++)
                {
                    if (Joystick::isButtonPressed(_joystickId, _keyIndex) && !joysticksButtonsIsHolding[_joystickId][_keyIndex])
                    {
                        joysticksButtonsIsHolding[_joystickId][_keyIndex] = true;
                    }
                }
            }
        }
        else if (_event->is<ControllerJoystickReleased>())
        {
            for (int _joystickId = 0; _joystickId < Joystick::Count; _joystickId++)
            {
                if (!Joystick::isConnected(_joystickId)) continue;
                for (int _keyIndex = 0; _keyIndex < Joystick::ButtonCount; _keyIndex++)
                {
                    if (!Joystick::isButtonPressed(_joystickId, _keyIndex) && joysticksButtonsIsHolding[_joystickId][_keyIndex])
                    {
                        joysticksButtonsIsHolding[_joystickId][_keyIndex] = false;
                    }
                }
            }
        }
    }

}