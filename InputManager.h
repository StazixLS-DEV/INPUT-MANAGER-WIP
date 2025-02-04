#pragma once
#include "Singleton.h"
#include "ActionMap.h"

namespace Input
{
	/*template <typename ValueType>
	struct InputAction
	{
		vector<function<ValueType()>> started;
		vector<function<ValueType()>> performed;
		vector<function<ValueType()>> cancel;

		void BindPerformed(const function<ValueType()>& _callback)
		{
			performed.push_back(_callback);
		}
		ValueType OnPerformed()
		{
			for (const function<ValueType()>& _callback : performed)
			{
				return _callback();
			}
		}
	};*/

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
				LOG(Error, "This ActionMap's name (" + _name + ") already used !");
				return nullptr;
			}

			ActionMap* _actionMap = new ActionMap(_name);
			AddActionMap({ _name, _actionMap });
			return _actionMap;
		}

	public:
		InputManager();
		~InputManager();

	private:
		void UpdateActionMaps(const EventInfo& _event);

	public:
		void Update(RenderWindow& _window);
	};
}