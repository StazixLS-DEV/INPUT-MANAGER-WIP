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
		/// <summary>
		/// Ajoute une action dont ValueType est Digital !
		/// </summary>
		/// <param name="_name">nom de l'action</param>
		/// <param name="_data">data de l'action</param>
		/// <param name="_callback"></param>
		void AddAction(const string& _name, const ActionData& _data, const function<void()>& _callback);
		/// <summary>
		/// Ajoute une action dont ValueType est Axis ou Axis2 !
		/// </summary>
		/// <typeparam name="Type">float ou Vector2f selon la ValueType de l'ActionType correspondante</typeparam>
		/// <typeparam name=""></typeparam>
		/// <param name="_name">nom de l'action</param>
		/// <param name="_data">data de l'action</param>
		/// <param name="_callback"></param>
		template <typename Type, enable_if_t<is_same_v<Type, float> || is_same_v<Type, Vector2f>>>
		void AddAction(const string& _name, const ActionData& _data, const function<void(const Type& _parameter)>& _callback)
		{
			assert((_data.value == Axis && is_same_v<Type, float>
				|| _data.value == Axis2 && is_same_v<Type, Vector2f>) &&
				"The callback must be a function with compatible parameter like ValueType return !");

			if (actions.contains(_name))
			{
				LOG(Error, "This Action's name (" + _name + ") already used in this ActionMap (" + name + ") !");
				return;
			}

			Action* _action = new Action(_name, _data, _callback);
			actions.insert({ _action->GetName(), _action });
		}
		void AddActions(const vector<Action*>& _actions);
		void RemoveAction(const string& _name);
		void Update(const EventInfo& _event);
	};
}