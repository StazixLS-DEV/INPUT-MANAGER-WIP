#include "InputManager.h"

Input::InputManager::InputManager()
{
    actionsMaps = map<string, ActionMap*>();
    isKeyHolding = false;
    isButtonHolding = false;
}

Input::InputManager::~InputManager()
{
    for (const pair<string, ActionMap*>& _pair : actionsMaps)
    {
        delete _pair.second;
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
    _window.handleEvents(
        [&](const auto& _event)
        {
            if (is_same_v<decay_t<decltype(_event)>, ClosedWindow>)

            {
                _window.close();
                return;
            }
            M_INPUT.UpdateActionMaps(_event);
            const bool _isKeyHolding = M_INPUT.GetIsKeyHolding();
            const bool _isButtonHolding = M_INPUT.GetIsButtonHolding();

            if (is_same_v<decay_t<decltype(_event)>, PressedKey> && !_isKeyHolding)
            {
                M_INPUT.SetIsKeyHolding(true);
            }
            else if (is_same_v<decay_t<decltype(_event)>, ReleasedKey> && _isKeyHolding)
            {
                M_INPUT.SetIsKeyHolding(false);
            }
            if (is_same_v<decay_t<decltype(_event)>, PressedMouseButton> && !_isButtonHolding)
            {
                M_INPUT.SetIsButtonHolding(true);
            }
            else if (is_same_v<decay_t<decltype(_event)>, ReleasedMouseButton> && _isButtonHolding)
            {
                M_INPUT.SetIsButtonHolding(false);
            }
        });
}