#include "Action.h"
#include "InputManager.h"

Input::Action::Action(const string& _name, const ActionData& _data, const function<void()>& _callback)
{
	name = _name;
	AddData(_data);
	callback = _callback;
}

Input::Action::Action(const string& _name, const set<ActionData>& _allData, const function<void()>& _callback)
{
	name = _name;
	for (const ActionData& _actionData : _allData)
	{
		AddData(_actionData);
	}
	callback = _callback;
}


void Input::Action::TryToExecute(const EventInfo& _event)
{
	if (!callback) return;

	_event->visit([&](auto&& _element)
	{
		const type_index& _elementType = TYPE_ID(_element);

		if (allData.contains(_elementType))
		{
#pragma region Keyboard

			if (const PressedKey* _key = _event->getIf<PressedKey>())
			{
				const bool _isKeyHolding = /*isKeyHolding*/ M_INPUT.GetIsKeyHolding();
				if (IsSpecificTypeInAllData(_elementType, 
					CAST(const int, _key->code), KeyHold) && 
					_isKeyHolding)
				{
					callback();
				}
				else if (IsSpecificTypeInAllData(_elementType,
					CAST(const int, _key->code), KeyPressed) &&
					!_isKeyHolding)
				{
					callback();
				}
			}

			if (const ReleasedKey* _key = _event->getIf<ReleasedKey>())
			{
				if (IsInAllData(_elementType, CAST(const int, _key->code)))
				{
					callback();
				}
			}
			
			
#pragma endregion

#pragma region Mouse
#pragma region Button

			else if (const PressedMouseButton* _key = _event->getIf<PressedMouseButton>())
			{
				const bool _isButtonHolding = /*isButtonHolding*/ M_INPUT.GetIsButtonHolding();
				if (IsSpecificTypeInAllData(_elementType,
					CAST(const int, _key->button), MouseButtonHold) &&
					_isButtonHolding)
				{
					callback();
				}
				else if (IsSpecificTypeInAllData(_elementType,
					CAST(const int, _key->button), MouseButtonPressed) &&
					!_isButtonHolding)
				{
					callback();
				}
			}

			else if (const ReleasedMouseButton* _key = _event->getIf<ReleasedMouseButton>())
			{
				if (IsInAllData(_elementType, CAST(const int, _key->button)))
				{
					callback();
				}
			}
#pragma endregion

			//TODO rajouter condition appel callback
			else if (const ScrolledMouseWheel* _key = _event->getIf<ScrolledMouseWheel>())
			{
				callback();
			}
			else if (const MovedMouse* _key = _event->getIf<MovedMouse>())
			{
				callback();
			}
			else if (const MovedRawMouse* _key = _event->getIf<MovedRawMouse>())
			{
				callback();
			}
			else if (_event->is<EnteredMouse>())
			{
				callback();
			}
			else if (_event->is<LeftMouse>())
			{
				callback();
			}
#pragma endregion

			//TODO rajouter condition appel callback
#pragma region Window
			else if (const ResizedWindow* _key = _event->getIf<ResizedWindow>())
			{
				callback();
			}
			else if (_event->is<LostFocus>())
			{
				callback();
			}
			else if (_event->is<GainedFocus>())
			{
				callback();
			}
			else if (const EnteredText* _key = _event->getIf<EnteredText>())
			{
				callback();
			}
			else if (const ClosedWindow* _key = _event->getIf<ClosedWindow>())
			{
				callback();
			}
#pragma endregion

			//TODO rajouter condition appel callback
#pragma region Controler
			else if (const ControllerJoystickPressed* _key = _event->getIf<ControllerJoystickPressed>())
			{
				callback();
			}
			else if (const ControllerJoystickReleased* _key = _event->getIf<ControllerJoystickReleased>())
			{
				callback();
			}
			else if (const ControllerJoystickMoved* _key = _event->getIf<ControllerJoystickMoved>())
			{
				callback();
			}
			else if (const ControllerJoystickConnected* _key = _event->getIf<ControllerJoystickConnected>())
			{
				callback();
			}
			else if (const ControllerJoystickDisconnected* _key = _event->getIf<ControllerJoystickDisconnected>())
			{
				callback();
			}
#pragma endregion

#pragma region Touch
			else if (const BeganTouch* _key = _event->getIf<BeganTouch>())
			{
				callback();
			}
			else if (const MovedTouch* _key = _event->getIf<MovedTouch>())
			{
				callback();
			}
			else if (const EndedTouch* _key = _event->getIf<EndedTouch>())
			{
				callback();
			}
			else if (const ChangedSensor* _key = _event->getIf<ChangedSensor>())
			{
				callback();
			}
#pragma endregion

		}
		//TODO remove test mettre is holding dans action mais même resultat..
		/*if (_event->is<PressedKey>() && !isKeyHolding)
		{
			isKeyHolding = true;
		}
		else if (_event->is<ReleasedKey>() && isKeyHolding)
		{
			isKeyHolding = false;
		}
		else if (_event->is<PressedMouseButton>() && !isButtonHolding)
		{
			isButtonHolding = true;
		}
		else if (_event->is<ReleasedMouseButton>() && isButtonHolding)
		{
			isButtonHolding = false;
		}*/
	});
}

Input::TypeIndex Input::Action::ComputeTypeIndexByActionType(const ActionType& _actionType)
{
	const vector<function<TypeIndex()>>& _actionsTypes =
	{
		//Keyboard
		[&]() { return type_index(typeid(PressedKey)); },			//KeyPressed
		[&]() { return type_index(typeid(PressedKey)); },			//KeyHold
		[&]() { return type_index(typeid(ReleasedKey)); },			//KeyReleased

		//Mouse Button
		[&]() { return type_index(typeid(PressedMouseButton)); },				//MouseButtonPressed
		[&]() { return type_index(typeid(PressedMouseButton)); },				//MouseButtonHold
		[&]() { return type_index(typeid(ReleasedMouseButton)); },				//MouseButtonReleased
		//Mouse 
		[&]() { return type_index(typeid(ScrolledMouseWheel)); },				//MouseWheelScrolled
		[&]() { return type_index(typeid(MovedMouse)); },						//MouseMoved
		[&]() { return type_index(typeid(MovedRawMouse)); },					//MouseMovedRaw
		[&]() { return type_index(typeid(EnteredMouse)); },						//MouseEntered
		[&]() { return type_index(typeid(LeftMouse)); },						//MouseLeft

		//Window
		[&]() { return type_index(typeid(ResizedWindow)); },					//Resized
		[&]() { return type_index(typeid(LostFocus)); },						//FocusLost
		[&]() { return type_index(typeid(GainedFocus)); },						//FocusGained
		[&]() { return type_index(typeid(EnteredText)); },						//TextEntered
		[&]() { return type_index(typeid(ClosedWindow)); },  					//Closed

		//Controler
		[&]() { return type_index(typeid(ControllerJoystickPressed)); },		//JoystickButtonPressed
		[&]() { return type_index(typeid(ControllerJoystickPressed)); },		//JoystickButtonHold
		[&]() { return type_index(typeid(ControllerJoystickReleased)); },		//JoystickButtonReleased
		[&]() { return type_index(typeid(ControllerJoystickMoved)); },			//JoystickMoved
		[&]() { return type_index(typeid(ControllerJoystickConnected)); },		//JoystickConnected
		[&]() { return type_index(typeid(ControllerJoystickDisconnected)); },	//JoystickDisconnected

		//Touch
		[&]() { return type_index(typeid(BeganTouch)); },						//TouchBegan
		[&]() { return type_index(typeid(MovedTouch)); },						//TouchMoved
		[&]() { return type_index(typeid(EndedTouch)); },						//TouchEnded
		[&]() { return type_index(typeid(ChangedSensor)); }						//SensorChanged
	};

	return _actionsTypes[_actionType]();
}
