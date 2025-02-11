#pragma once
#include "Singleton.h"
#include "ActionMap.h"

namespace Input
{

	class InputManager : public Singleton<InputManager>
	{
		map<string, ActionMap*> actionsMaps;
		map<Key, bool> keysIsHolding;
		map<Button, bool> mouseButtonsIsHolding;
		vector<vector<bool>> joysticksButtonsIsHolding;

	private:
		FORCEINLINE void AddActionMap(const pair<string, ActionMap*>& _actionMap)
		{
			actionsMaps.insert(_actionMap);
		}
	public:
		FORCEINLINE bool GetIsKeyHolding(const Key& _key) const
		{
			return keysIsHolding.at(_key);
		}
		FORCEINLINE bool GetIsMouseButtonHolding(const Button& _mouseButton) const
		{
			return mouseButtonsIsHolding.at(_mouseButton);
		}
		FORCEINLINE bool GetIsJoystickButtonHolding(const int _joystickId, const int _joystickButton) const
		{
			return joysticksButtonsIsHolding[_joystickId][_joystickButton];
		}
		FORCEINLINE ActionMap* GetActionMapByName(const string& _name)
		{
			if (!actionsMaps.contains(_name)) return nullptr;
			return actionsMaps.at(_name);
		}
		FORCEINLINE void RemoveActionMap(const string& _name)
		{
			if (!actionsMaps.contains(_name)) return;

			delete actionsMaps[_name];
			actionsMaps.erase(_name);
		}
		FORCEINLINE ActionMap* CreateActionMap(const string& _name)
		{
			if (actionsMaps.contains(_name))
			{
				LOG(Warning, "This ActionMap's name (" + _name + ") already used,\
					I return the ActionMap with name asked to create !");
				return GetActionMapByName(_name);
			}

			ActionMap* _actionMap = new ActionMap(_name);
			AddActionMap({ _name, _actionMap });
			return _actionMap;
		}

	public:
		InputManager();
		~InputManager();

	private:
		void Init();
		void InitKeysHolding();
		void InitMouseButtonHolding();
		void InitJoystickButtonHolding();
		void UpdateActionMaps(const EventInfo& _event);

	public:
		void Update(RenderWindow& _window);
	};
}

#pragma region Exemple
/*
ActionMap* _actionMap = M_INPUT.CreateActionMap("Demo");
//DIGITAL
_actionMap->AddAction("Test", ActionData(MouseEntered), [&]() { LOG(Error, "ButtonEntered"); });
_actionMap->AddAction("Test1", ActionData(KeyPressed, KeyType::A), [&]() { LOG(Display, "coucou"); });
_actionMap->AddAction("Test2", ActionData(KeyReleased, KeyType::A), [&]() { LOG(Warning, "coucou"); });
_actionMap->AddAction("Test3", ActionData(KeyHold, KeyType::A), [&]() { LOG(Error, "coucou"); });
_actionMap->AddAction("Test4", ActionData(KeyPressed, KeyType::B), [&]() { LOG(Display, "salut"); });
_actionMap->AddAction("Test5", ActionData(KeyReleased, KeyType::B), [&]() { LOG(Warning, "salut"); });
_actionMap->AddAction("Test6", ActionData(KeyHold, KeyType::B), [&]() { LOG(Error, "salut"); });
_actionMap->AddAction("Test9", ActionData(KeyHold, KeyType::T), [&]() { LOG(Error, "Thomas"); });
_actionMap->AddAction("Test8", ActionData(KeyReleased, KeyType::T), [&]() { LOG(Warning, "Thomas"); });
_actionMap->AddAction("Test7", ActionData(KeyPressed, KeyType::T), [&]() { LOG(Display, "Thomas"); });
//AXIS2
_actionMap->AddAction("Test10", ActionData(MouseMoved), [&](const Vector2f& _position) { LOG(Error, "X: " + to_string(_position.x) + " Y: " + to_string(_position.y)); });
_actionMap->AddAction("Test11", ActionData(MouseMovedRaw), [&](const Vector2f& _position) { LOG(Error, "MouseMovedRaw"); });
//AXIS
_actionMap->AddAction("Test12", ActionData(MouseWheelScrolled, KeyType::Vertical), [&](const float _position) { LOG(Error, to_string(_position)); });
_actionMap->AddAction("Test99", ActionData(JoystickMoved, KeyType::LeftJoystickX), [&](const float _position) { LOG(Error, "manette :" + to_string(_position)); });

_actionMap->Enable();
*/
#pragma endregion
