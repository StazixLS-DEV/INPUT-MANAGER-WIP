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

	//Screen
	using BeganTouch = Event::TouchBegan;
	using MovedTouch = Event::TouchMoved;
	using EndedTouch = Event::TouchEnded;
	using ChangedSensor = Event::SensorChanged;
	
#pragma endregion

	union CallBackType
	{
		function<void()> digitalCallback; // bool
		function<void(const float& _axis)>	axisCallback;	 // float
		function<void(const Vector2f& _axis2)>	axis2Callback;
		CallBackType()
		{

		}
		CallBackType(const function<void()>& _digitalCallback)
		{
			digitalCallback = _digitalCallback;
		}
		CallBackType(const function<void(const float& _axis)>& _axisCallback)
		{
			axisCallback = _axisCallback;
		}
		CallBackType(const function<void(const Vector2f& _axis2)>& _axis2Callback)
		{
			axis2Callback = _axis2Callback;
		}
		~CallBackType()
		{

		}
	};

	enum class ControllerButtonsType
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

		//Screen
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
			assert((_type != JoystickPressed && _type != JoystickReleased && 
				_type != Closed && _type != MouseEntered && _type != MouseLeft &&
				_type != FocusLost && _type != FocusGained) &&
				"Invalid constructor to use this ActionType!");

			value = ComputeValueTypeWithActionType(_type);
			type = _type;
			key = CAST(int, _key);
		}
		ActionData(const ActionType& _type, const int _button)
		{
			assert((_type == JoystickPressed || _type == JoystickReleased) &&
				"Invalid constructor to use this ActionType!");

			value = ComputeValueTypeWithActionType(_type);
			type = _type;
			key = _button;
		}
		ActionData(const ActionType& _type)
		{
			assert((_type == Closed || _type == MouseEntered || _type == MouseLeft ||
				_type == FocusLost || _type == FocusGained) &&
				"Invalid constructor to use this ActionType!");

			value = Digital;
			type = _type;
			key = -1;
		}

		ValueType ComputeValueTypeWithActionType(const ActionType& _type)
		{
			switch (_type)
			{
			case MouseMoved:
			case MouseMovedRaw:
			case Resized:
			case JoystickMoved:
			case TouchBegan:
			case TouchMoved:
			case TouchEnded:
				return Axis2;
			case MouseWheelScrolled:
			case SensorChanged:
				return Axis;
			default:
				break;
			}
			return Digital;
		}
	};

	class Action
	{
		string name;
		multimap<TypeIndex, ActionData> allData;
		shared_ptr<CallBackType> callback;
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
		void SimpleConstruct(const string& _name, const ActionData& _data)
		{
			name = _name;
			AddData(_data);
		}
		void MultipleConstruct(const string& _name, const set<ActionData>& _allData)
		{
			name = _name;
			for (const ActionData& _actionData : _allData)
			{
				AddData(_actionData);
			}
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
		template <typename Type, enable_if_t<is_same_v<Type, float> || is_same_v<Type, Vector2f>>>
		Action(const string& _name, const ActionData& _data, const function<void(const Type& _parameter)>& _callback)
		{
			assert((_data.value == Axis && is_same_v<Type, float>
				|| _data.value == Axis2 && is_same_v<Type, Vector2f>) &&
				"The callback must be a function with compatible parameter like ValueType return !");
			SimpleConstruct(_name, _data);
			callback = make_shared<CallBackType>(_callback);
		}
		Action(const string& _name, const ActionData& _data, const function<void()>& _callback)
		{
			assert((_data.value == Axis || _data.value == Axis2) &&
				"The callback must be a function with compatible parameter like ValueType return !");
			
			SimpleConstruct(_name, _data);
			callback = make_shared<CallBackType>(_callback);
		}
		Action(const string& _name, const set<ActionData>& _allData, const function<void()>& _callback)
		{
			if (_allData.empty()) return;

			assert((_allData.begin()->value == Axis && _allData.begin()->value == Axis2) &&
				"The callback must be a function with compatible parameter like ValueType return !");

			MultipleConstruct(_name, _allData);
			callback = make_shared<CallBackType>(_callback);
		}
		template <typename Type, enable_if_t<is_same_v<Type, float> || is_same_v<Type, Vector2f>>>
		Action(const string& _name, const set<ActionData>& _allData, const function<void(const Type& _parameter)>& _callback)
		{
			if (_allData.empty()) return;

			assert((_allData.begin()->value == Axis && is_same_v<Type, float>
					|| _allData.begin()->value == Axis2 && is_same_v<Type, Vector2f>) &&
				"The callback must be a function with compatible parameter like ValueType return !");

			MultipleConstruct(_name, _allData);
			callback = _callback;
		}

	public:
		void TryToExecute(const EventInfo& _event);

	private:
		TypeIndex ComputeTypeIndexByActionType(const ActionType& _typeIndex);
		
	
	};
}