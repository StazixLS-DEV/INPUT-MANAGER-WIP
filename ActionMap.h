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
		/// Ajoute une action d�j� cr�er auparavant 
		/// </summary>
		/// <param name="_action"></param>
		void AddAction(Action* _action);
		/// <summary>
		/// Ajoute une action dont ValueType est Digital !
		/// </summary>
		/// <param name="_name">nom de l'action</param>
		/// <param name="_data">ActionData dont la ValueType est Digital</param>
		/// <param name="_callback"></param>
		void AddAction(const string& _name, const ActionData& _data, const function<void()>& _callback);
		/// <summary>
		/// Ajoute une action dont ValueType est Axis2 !
		/// </summary>
		/// <param name="_name">nom de l'action</param>
		/// <param name="_data">ActionData dont la ValueType est Axis2</param>
		/// <param name="_callback"></param>
		void AddAction(const string& _name, const ActionData& _data, const function<void(const Vector2f& _parameter)>& _callback);
		/// <summary>
		/// Ajoute une action dont ValueType est Axis !
		/// </summary>
		/// <param name="_name">nom de l'action</param>
		/// <param name="_data">ActionData dont la ValueType est Axis</param>
		/// <param name="_callback"></param>
		void AddAction(const string& _name, const ActionData& _data, const function<void(const float _parameter)>& _callback);
		/// <summary>
		/// Ajoute une action (avec plusieurs ActionData) dont ValueType est Digital !
		/// </summary>
		/// <param name="_name">nom de l'action</param>
		/// <param name="_allData">les ActionData dont les ValueType sont Digital</param>
		/// <param name="_callback"></param>
		void AddAction(const string& _name, const vector<ActionData>& _allData, const function<void()>& _callback);
		/// <summary>
		/// Ajoute une action (avec plusieurs ActionData) dont ValueType est Axis2 !
		/// </summary>
		/// <param name="_name">nom de l'action</param>
		/// <param name="_allData">les ActionData dont les ValueType sont Axis2</param>
		/// <param name="_callback"></param>
		void AddAction(const string& _name, const vector<ActionData>& _allData, const function<void(const Vector2f& _parameter)>& _callback);
		// <summary>
		/// Ajoute une action (avec plusieurs ActionData) dont ValueType est Axis !
		/// </summary>
		/// <param name="_name">nom de l'action</param>
		/// <param name="_allData">les ActionData dont les ValueType sont Axis</param>
		/// <param name="_callback"></param>
		void AddAction(const string& _name, const vector<ActionData>& _allData, const function<void(const float _parameter)>& _callback);

		void AddActions(const vector<Action*>& _actions);
		void RemoveAction(const string& _name);
		void Update(const EventInfo& _event);
	};
}