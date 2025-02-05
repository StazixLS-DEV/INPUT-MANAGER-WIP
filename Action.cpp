#include "Action.h"
#include "InputManager.h"


void Input::Action::TryToExecute(const EventInfo& _event)
{
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
						 _key->code, KeyHold) &&
						_isKeyHolding)
					{
						(*callback.get()->digitalCallback.get())();
					}
					else if (IsSpecificTypeInAllData(_elementType,
						 _key->code, KeyPressed) &&
						!_isKeyHolding)
					{
						(*callback.get()->digitalCallback.get())();
					}
				}

				if (const ReleasedKey* _key = _event->getIf<ReleasedKey>())
				{
					if (IsInAllData(_elementType, CAST(const int, _key->code)))
					{
						(*callback.get()->digitalCallback.get())();
					}
				}


#pragma endregion

#pragma region Mouse
#pragma region Button

				else if (const PressedMouseButton* _key = _event->getIf<PressedMouseButton>())
				{
					const bool _isButtonHolding = /*isButtonHolding*/ M_INPUT.GetIsButtonHolding();
					if (IsSpecificTypeInAllData(_elementType,
						 _key->button, MouseButtonHold) &&
						_isButtonHolding)
					{
						(*callback.get()->digitalCallback.get())();
					}
					else if (IsSpecificTypeInAllData(_elementType,
						_key->button, MouseButtonPressed) &&
						!_isButtonHolding)
					{
						(*callback.get()->digitalCallback.get())();
					}
				}

				else if (const ReleasedMouseButton* _key = _event->getIf<ReleasedMouseButton>())
				{
					if (IsInAllData(_elementType, CAST(const int, _key->button)))
					{
						(*callback.get()->digitalCallback.get())();
					}
				}
#pragma endregion

				//TODO rajouter condition appel callback
				else if (const ScrolledMouseWheel* _key = _event->getIf<ScrolledMouseWheel>())
				{
					if (IsInAllData(_elementType, _key->wheel))
					{
						(*callback.get()->axisCallback.get())(_key->delta);
					}
				}
				else if (const MovedMouse* _key = _event->getIf<MovedMouse>())
				{
					(*callback.get()->axis2Callback.get())(CAST(Vector2f, _key->position));
				}
				else if (const MovedRawMouse* _key = _event->getIf<MovedRawMouse>())
				{
					(*callback.get()->axis2Callback.get())(CAST(Vector2f, _key->delta));
				}
				else if (_event->is<EnteredMouse>())
				{
					(*callback.get()->digitalCallback.get())();
				}
				else if (_event->is<LeftMouse>())
				{
					(*callback.get()->digitalCallback.get())();
				}
#pragma endregion

#pragma region Window
				else if (const ResizedWindow* _key = _event->getIf<ResizedWindow>())
				{
					(*callback.get()->axis2Callback.get())(CAST(Vector2f, _key->size));
				}
				else if (_event->is<LostFocus>())
				{
					(*callback.get()->digitalCallback.get())();
				}
				else if (_event->is<GainedFocus>())
				{
					(*callback.get()->digitalCallback.get())();
				}
				else if (const EnteredText* _key = _event->getIf<EnteredText>())
				{
					(*callback.get()->digitalCallback.get())();
				}
				else if (const ClosedWindow* _key = _event->getIf<ClosedWindow>())
				{
					(*callback.get()->digitalCallback.get())();
				}
#pragma endregion

#pragma region Controler
				else if (const ControllerJoystickPressed* _key = _event->getIf<ControllerJoystickPressed>())
				{
					if (IsInAllData(_elementType, _key->button))
					{
						(*callback.get()->digitalCallback.get())();
					}
				}
				else if (const ControllerJoystickReleased* _key = _event->getIf<ControllerJoystickReleased>())
				{
					if (IsInAllData(_elementType, _key->button))
					{
						(*callback.get()->digitalCallback.get())();
					}
				}
				else if (const ControllerJoystickMoved* _key = _event->getIf<ControllerJoystickMoved>())
				{
					//callback();
					//TODO faire un vector2f avec le float et l'axe
					//callback.axis2Callback();
				}
				else if (const ControllerJoystickConnected* _key = _event->getIf<ControllerJoystickConnected>())
				{
					(*callback.get()->digitalCallback.get())();
				}
				else if (const ControllerJoystickDisconnected* _key = _event->getIf<ControllerJoystickDisconnected>())
				{
					(*callback.get()->digitalCallback.get())();
				}
#pragma endregion

#pragma region Screen
				else if (const BeganTouch* _key = _event->getIf<BeganTouch>())
				{
					(*callback.get()->axis2Callback.get())(CAST(Vector2f, _key->position));
				}
				else if (const MovedTouch* _key = _event->getIf<MovedTouch>())
				{
					(*callback.get()->axis2Callback.get())(CAST(Vector2f, _key->position));
				}
				else if (const EndedTouch* _key = _event->getIf<EndedTouch>())
				{
					(*callback.get()->axis2Callback.get())(CAST(Vector2f, _key->position));
				}
				else if (const ChangedSensor* _key = _event->getIf<ChangedSensor>())
				{
					if (IsInAllData(_elementType, _key->type))
					{
						(*callback.get()->digitalCallback.get())();
					}
				}
#pragma endregion

			}
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

		//Screen
		[&]() { return type_index(typeid(BeganTouch)); },						//TouchBegan
		[&]() { return type_index(typeid(MovedTouch)); },						//TouchMoved
		[&]() { return type_index(typeid(EndedTouch)); },						//TouchEnded
		[&]() { return type_index(typeid(ChangedSensor)); }						//SensorChanged
	};

	return _actionsTypes[_actionType]();
}