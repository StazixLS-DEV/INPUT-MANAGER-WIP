#pragma once
#include "CoreMinimal.h"

namespace Input
{
#pragma region Typedef & Using
	typedef optional<Event> EventInfo;
	using TypeIndex = type_index;
	using Key = Keyboard::Key;

	//Keyboard
	using PressedKey = Event::KeyPressed;
	using ReleasedKey = Event::KeyReleased;

	//Mouse Button
	using PressedMouseButton = Event::MouseButtonPressed;
	using ReleasedMouseButton = Event::MouseButtonReleased;
	//Mouse 
	using ScrolledMouseWheel = Event::MouseWheelScrolled;
	using MovedMouse = Event::MouseMoved;
	using MovedRawMouse = Event::MouseMovedRaw;
	using EnteredMouse= Event::MouseEntered;
	using LeftMouse = Event::MouseLeft;

	//Window
	using ResizedWindow = Event::Resized;
	using LostFocus = Event::FocusLost;
	using GainedFocus = Event::FocusGained;
	using EnteredText = Event::TextEntered;
	using ClosedWindow = Event::Closed;

	//Controler
	using ControllerJoystickPressed = Event::JoystickButtonPressed;
	using ControllerJoystickReleased = Event::JoystickButtonReleased;
	using ControllerJoystickMoved = Event::JoystickMoved;
	using ControllerJoystickConnected = Event::JoystickConnected;
	using ControllerJoystickDisconnected = Event::JoystickDisconnected;

	//Touch
	using BeganTouch = Event::TouchBegan;
	using MovedTouch = Event::TouchMoved;
	using EndedTouch = Event::TouchEnded;
	using ChangedSensor = Event::SensorChanged;
	
#pragma endregion

	

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
		//Keyboard
		KeyPressed,
		KeyHold,
		KeyReleased,

		//Mouse Button
		MouseButtonPressed,
		MouseButtonHold,
		MouseButtonReleased,
		//Mouse
		MouseWheelScrolled,
		MouseMoved,
		MouseMovedRaw,
		MouseEntered,
		MouseLeft,

		//Window
		Resized,
		FocusLost,
		FocusGained,
		TextEntered,
		Closed,

		//Controller
		JoystickPressed,
		JoystickReleased,
		JoystickMoved,
		JoystickConnected,
		JoystickDisconnected,

		//Touch
		TouchBegan,
		TouchMoved,
		TouchEnded,
		SensorChanged,
	};

		//chaque inputaction possède des callback start / performed / released
		//elles sont appellé quand c'est l'input associé
		//on peut accèder à tout moment à la valeur de l'inputaction

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

		template<typename EnumType, typename = enable_if_t<is_enum_v<EnumType>>>
		ActionData(const ActionType& _type, const EnumType& _key)
		{
			value = Digital;
			type = _type;
			key = CAST(int, _key);
		}
		ActionData(const ActionType& _type)
		{
			value = Digital;
			type = _type;
			key = -1;
		}
	};

	class Action
	{
		string name;
		multimap<TypeIndex, ActionData> allData;
		function<void()> callback;
		/*bool isKeyHolding;
		bool isButtonHolding;*/

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
		FORCEINLINE bool IsSpecificTypeInAllData(const TypeIndex& _type, const int _key, const ActionType& _specificType)
		{
			using Iterator = multimap<TypeIndex, ActionData>::iterator;
			const pair <Iterator, Iterator>& _actionsType = allData.equal_range(_type);
			for (Iterator it = _actionsType.first; it != _actionsType.second; ++it)
			{
				if (it->second.key == _key && it->second.type == _specificType) return true;
			}
			return false;
		}
	public:
		FORCEINLINE string GetName() const
		{
			return name;
		}
		FORCEINLINE void AddData(const ActionData& _actionData)
		{
			allData.insert({ ComputeTypeIndexByActionType(_actionData.type), _actionData });
		}
		/*template <typename Type>
		FORCEINLINE void SetTrigger()
		{
			data.trigger = typeid(Type);
		}*/

	public:
		Action(const string& _name, const ActionData& _data, const function<void()>& _callback);
		Action(const string& _name, const set<ActionData>& _allData, const function<void()>& _callback);

		template<typename Type>
		Type TryToExecute(const EventInfo& _event)
		{
			if (!callback) return;

			_event->visit([&](auto&& _element)
			{
				if (is_same_v<decay_t<decltype(_element) >>, Type > )
				{
					const type_index& _elementType = TYPE_ID(_element);
					if (allData.contains(_elementType))
					{
						if (const Type* _key = _event->getIf<Type>())
						{
							_key.
						}
					}
				}
			});
		}

	private:
		TypeIndex ComputeTypeIndexByActionType(const ActionType& _typeIndex);
	
	
	};
}