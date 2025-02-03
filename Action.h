#pragma once
#include "CoreMinimal.h"

namespace Input
{
	typedef optional<Event> EventInfo;
	using TypeIndex = type_index;
	using Key = Keyboard::Key;

	using KeyPressed = Event::KeyPressed;
	using KeyReleased = Event::KeyReleased;

	using MouseButtonPressed = Event::MouseButtonPressed;
	using MouseButtonReleased = Event::MouseButtonReleased;

	enum ControllerButtonsType
	{
		Start,
		Select,
		Cross,
		Circle,
		Triangle,
		Square,
		L1,
		R1,
		L2,
		R2,
		LeftStick,
		RightStick
	};

	enum ActionType
	{
		KeyboardPressed,
		KeyboardHold,
		KeyboarReleased,
		ButtonPressed,
		ButtonHold,
		ButtonReleased,
	};

	enum ControllerAxesType
	{
		LeftStickX,
		LeftStickY,
		RightStickX,
		RightStickY,
	};


	enum ValueType
	{
		Digital, // bool
		Axis,	 // float
		Axis2,	 // Vector2f
	};

	struct ActionData
	{
		ValueType value;
		ActionType type;
		int key;

		ActionData(const ActionType& _type, const int _key)
		{
			value = Digital;
			type = _type;
			key = _key;
		}

	};

	class Action
	{
		string name;
		multimap<TypeIndex, ActionData> allData;
		function<void()> callback;
		

	private:
		FORCEINLINE bool IsInAllData(const TypeIndex& _type, const int _key)
		{
			using Iterator = multimap<TypeIndex, ActionData>::iterator;
			const pair <Iterator, Iterator>& _actionsType = allData.equal_range(_type);
			for (Iterator it = _actionsType.first; it != _actionsType.second; ++it)
			{
				if (it->second.key == _key) return true;
			}
			return false;
		}

	public:
		FORCEINLINE string GetName() const
		{
			return name;
		}
		/*template <typename Type>
		FORCEINLINE void SetTrigger()
		{
			data.trigger = typeid(Type);
		}*/

	public:
		Action(const string& _name, const ActionData& _data, const function<void()>& _callback);
		Action(const string& _name, const set<ActionData>& _allData, const function<void()>& _callback);

		void TryToExecute(const EventInfo& _event);

	private:
		TypeIndex ComputeTypeIndexByActionType(const ActionType& _actionType);
	};
}