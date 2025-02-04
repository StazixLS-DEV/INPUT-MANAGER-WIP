#pragma once
#include "Singleton.h"
#include "ActionMap.h"

namespace Input
{
	class InputManager : public Singleton<InputManager>
	{
		map<string, ActionMap*> actionsMaps;
		bool isKeyHolding;
		bool isButtonHolding;

	private:
		FORCEINLINE void AddActionMap(const pair<string, ActionMap*>& _actionMap)
		{
			actionsMaps.insert(_actionMap);
		}
	public:
		FORCEINLINE bool GetIsKeyHolding() const
		{
			return isKeyHolding;
		}
		FORCEINLINE bool GetIsButtonHolding() const
		{
			return isButtonHolding;
		}
		FORCEINLINE void SetIsButtonHolding(const bool _isButtonHolding)
		{
			isButtonHolding = _isButtonHolding;
		}
		FORCEINLINE void SetIsKeyHolding(const bool _isKeyHolding)
		{
			isKeyHolding = _isKeyHolding;
		}
		FORCEINLINE ActionMap* GetActionMapByName(const string& _name)
		{
			if (!actionsMaps.contains(_name)) return nullptr;
			return actionsMaps.at(_name);
		}
		FORCEINLINE ActionMap* CreateActionMap(const string& _name)
		{
			ActionMap* _actionMap = new ActionMap(_name);
			AddActionMap({ _name, _actionMap });
			return _actionMap;
		}

	public:
		InputManager();
		~InputManager();

	private:

	public:
		void Update(RenderWindow& _window);
		void UpdateActionMaps(const EventInfo& _event);
	};
}