#pragma once
#include "CoreMinimal.h"
#include "Action.h"

namespace Input
{
	class ActionMap
	{
		bool isActive;
		string name;
		map<string, Action*> actions;

	public:
		FORCEINLINE void Enable()
		{
			isActive = true;
		}
		FORCEINLINE void Disable()
		{
			isActive = false;
		}

	public:
		ActionMap(const string& _name = "DefaultActionMap");
		~ActionMap();

	public:
		void AddAction(const string& _name, const ActionData& _data, const function<void()>& _callback);
		void AddActions(const vector<Action*>& _actions);
		void Update(const EventInfo& _event);
	};
}