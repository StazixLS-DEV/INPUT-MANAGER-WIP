#include "Action.h"
#include "InputManager.h"

Input::Action::Action(const string& _name, const ActionData& _data, const function<void(const Vector2f& _parameter)>& _callback)
{
	assert(_data.value == Axis2 &&
		"The callback must be a function with compatible parameter like ValueType return !");
	SimpleConstruct(_name, _data);
	callback = CallBackType(_callback);
}
Input::Action::Action(const string& _name, const ActionData& _data, const function<void(const float _parameter)>& _callback)
{
	assert(_data.value == Axis &&
		"The callback must be a function with compatible parameter like ValueType return !");
	SimpleConstruct(_name, _data);
	callback = CallBackType(_callback);
}
Input::Action::Action(const string& _name, const ActionData& _data, const function<void()>& _callback)
{
	assert(_data.value == Digital &&
		"The callback must be a function with compatible parameter like ValueType return !");

	SimpleConstruct(_name, _data);
	callback = CallBackType(_callback);
}
Input::Action::Action(const string& _name, const vector<ActionData>& _allData, const function<void()>& _callback)
{
	if (_allData.empty()) return;

	assert(_allData[0].value == Digital &&
		"The callback must be a function with compatible parameter like ValueType return !");

	MultipleConstruct(_name, _allData);
	callback = CallBackType(_callback);
}
Input::Action::Action(const string& _name, const vector<ActionData>& _allData, const function<void(const Vector2f& _parameter)>& _callback)
{
	if (_allData.empty()) return;

	assert(_allData[0].value == Axis2 &&
		"The callback must be a function with compatible parameter like ValueType return !");

	MultipleConstruct(_name, _allData);
	callback = CallBackType(_callback);
}
Input::Action::Action(const string& _name, const vector<ActionData>& _allData, const function<void(const float _parameter)>& _callback)
{
	if (_allData.empty()) return;

	assert(_allData[0].value == Axis2 &&
		"The callback must be a function with compatible parameter like ValueType return !");

	MultipleConstruct(_name, _allData);
	callback = CallBackType(_callback);
}

void Input::Action::TryToExecute(const EventInfo& _event)
{
	_event->visit([&](auto&& _element)
		{
			const type_index& _elementType = TYPE_ID(_element);

			if (allData.contains(_elementType))
			{
#pragma region Keyboard


				if (_event->is<PressedKey>())
				{
					using Iterator = multimap<TypeIndex, ActionData>::iterator;
					const pair <Iterator, Iterator>& _actionsType = allData.equal_range(_elementType);
					for (Iterator _it = _actionsType.first; _it != _actionsType.second; ++_it)
					{
						const Key& _key = CAST(Key, _it->second.key);
						if (Keyboard::isKeyPressed(_key))
						{
							if (!M_INPUT.GetIsKeyHolding(_key))
							{
								(*callback.digitalCallback.get())();
								break;//TODO REPLACE BY RETURN ?
							}
						}
					}
				}

				else if (const ReleasedKey* _key = _event->getIf<ReleasedKey>())
				{
					if (IsInAllData(_elementType, CAST(const int, _key->code)))
					{
						(*callback.digitalCallback.get())();
					}
					//return;
				}


#pragma endregion

#pragma region Mouse
#pragma region Button

				else if (_event->is<PressedMouseButton>())
				{
					using Iterator = multimap<TypeIndex, ActionData>::iterator;
					const pair <Iterator, Iterator>& _actionsType = allData.equal_range(_elementType);
					for (Iterator _it = _actionsType.first; _it != _actionsType.second; ++_it)
					{
						const Button& _mouseButton = CAST(Button, _it->second.key);

						if (Mouse::isButtonPressed(_mouseButton))
						{
							if (!M_INPUT.GetIsMouseButtonHolding(_mouseButton))
							{
								(*callback.digitalCallback.get())();
								break;//TODO REPLACE BY RETURN ?
							}
						}
					}
				}

				else if (const ReleasedMouseButton* _key = _event->getIf<ReleasedMouseButton>())
				{
					if (IsInAllData(_elementType, CAST(const int, _key->button)))
					{
						(*callback.digitalCallback.get())();
					}
					return;
				}
#pragma endregion

				else if (const ScrolledMouseWheel* _key = _event->getIf<ScrolledMouseWheel>())
				{
					if (IsInAllData(_elementType, _key->wheel))
					{
						(*callback.axisCallback.get())(_key->delta);
					}
					return;
				}
				else if (const MovedMouse* _key = _event->getIf<MovedMouse>())
				{
					(*callback.axis2Callback.get())(CAST(Vector2f, _key->position));
					return;
				}
				else if (const MovedRawMouse* _key = _event->getIf<MovedRawMouse>())
				{
					(*callback.axis2Callback.get())(CAST(Vector2f, _key->delta));
					return;
				}
				else if (_event->is<EnteredMouse>())
				{
					(*callback.digitalCallback.get())();
					return;
				}
				else if (_event->is<LeftMouse>())
				{
					(*callback.digitalCallback.get())();
					return;
				}
#pragma endregion

#pragma region Window
				else if (const ResizedWindow* _key = _event->getIf<ResizedWindow>())
				{
					(*callback.axis2Callback.get())(CAST(Vector2f, _key->size));
					return;
				}
				else if (_event->is<LostFocus>())
				{
					(*callback.digitalCallback.get())();
					return;
				}
				else if (_event->is<GainedFocus>())
				{
					(*callback.digitalCallback.get())();
					return;
				}
				else if (const EnteredText* _key = _event->getIf<EnteredText>())
				{
					(*callback.digitalCallback.get())();
					return;
				}
				else if (const ClosedWindow* _key = _event->getIf<ClosedWindow>())
				{
					(*callback.digitalCallback.get())();
					return;
				}
#pragma endregion

#pragma region Controler
				else if (const ControllerJoystickPressed* _key = _event->getIf<ControllerJoystickPressed>())
				{
					if (HasJoystickIDInAllData(_elementType, _key->joystickId))
					{
						using Iterator = multimap<TypeIndex, ActionData>::iterator;
						const pair <Iterator, Iterator>& _actionsType = allData.equal_range(_elementType);
						for (Iterator _it = _actionsType.first; _it != _actionsType.second; ++_it)
						{
							const int _joystickID = _key->joystickId;
							const int _joystickButton = _it->second.key;
							if (Joystick::isButtonPressed(_joystickID, _joystickButton))
							{
								if (!M_INPUT.GetIsJoystickButtonHolding(_joystickID, _joystickButton))
								{
									(*callback.digitalCallback.get())();
									break; //TODO REPLACE BY RETURN ?
								}
							}
						}
					}
					return;
				}
				else if (const ControllerJoystickReleased* _key = _event->getIf<ControllerJoystickReleased>())
				{
					if (HasJoystickIDInAllData(_elementType, _key->joystickId) && 
						IsInAllData(_elementType, _key->button))
					{
						(*callback.digitalCallback.get())();
					}
					return;
				}
				else if (const ControllerJoystickMoved* _key = _event->getIf<ControllerJoystickMoved>())
				{
					if (HasJoystickIDInAllData(_elementType, _key->joystickId) &&
						IsInAllData(_elementType, _key->axis))
					{
						(*callback.axisCallback.get())(_key->position / 100.0f);
					}
					return;
				}
				else if (const ControllerJoystickConnected* _key = _event->getIf<ControllerJoystickConnected>())
				{
					(*callback.axisCallback.get())(CAST(float, _key->joystickId));
					return;
				}
				else if (const ControllerJoystickDisconnected* _key = _event->getIf<ControllerJoystickDisconnected>())
				{
					(*callback.axisCallback.get())(CAST(float, _key->joystickId));
					return;
				}
#pragma endregion

#pragma region Screen
				else if (const BeganTouch* _key = _event->getIf<BeganTouch>())
				{
					(*callback.axis2Callback.get())(CAST(Vector2f, _key->position));
					return;
				}
				else if (const MovedTouch* _key = _event->getIf<MovedTouch>())
				{
					(*callback.axis2Callback.get())(CAST(Vector2f, _key->position));
					return;
				}
				else if (const EndedTouch* _key = _event->getIf<EndedTouch>())
				{
					(*callback.axis2Callback.get())(CAST(Vector2f, _key->position));
					return;
				}
				else if (const ChangedSensor* _key = _event->getIf<ChangedSensor>())
				{
					if (IsInAllData(_elementType, _key->type))
					{
						(*callback.digitalCallback.get())();
					}
					return;
				}
#pragma endregion


				using Iterator = multimap<TypeIndex, ActionData>::iterator;
				const pair <Iterator, Iterator>& _keyActions = allData.equal_range(TYPE_ID(PressedKey));
				for (Iterator _it = _keyActions.first; _it != _keyActions.second; ++_it)
				{
					if (_it->second.type == KeyHold && M_INPUT.GetIsKeyHolding(CAST(Key, _it->second.key)))
					{

						(*callback.digitalCallback.get())();
						break;
					}
				}
				const pair <Iterator, Iterator>& _mouseButtonActions = allData.equal_range(TYPE_ID(PressedMouseButton));
				for (Iterator _it = _mouseButtonActions.first; _it != _mouseButtonActions.second; ++_it)
				{
					if (_it->second.type == MouseButtonHold && M_INPUT.GetIsMouseButtonHolding(CAST(Button, _it->second.key)))
					{
						(*callback.digitalCallback.get())();
						break;
					}
				}
				const pair <Iterator, Iterator>& _joystickButtonActions = allData.equal_range(TYPE_ID(ControllerJoystickPressed));
				for (Iterator _it = _joystickButtonActions.first; _it != _joystickButtonActions.second; ++_it)
				{
					if (_it->second.type == JoystickButtonHold && M_INPUT.GetIsJoystickButtonHolding(_it->second.joystickId , _it->second.key))
					{
						(*callback.digitalCallback.get())();
						break;
					}
				}
			}
		});
}

Input::TypeIndex Input::Action::ComputeTypeIndexByActionType(const ActionType& _actionType)
{
	const vector<function<TypeIndex()>>& _actionsTypes =
	{
		//Keyboard
		[&]() { return type_index(TYPE_ID(PressedKey)); },						//KeyPressed
		[&]() { return type_index(TYPE_ID(PressedKey)); },						//KeyHold
		[&]() { return type_index(TYPE_ID(ReleasedKey)); },						//KeyReleased

		//Mouse Button
		[&]() { return type_index(TYPE_ID(PressedMouseButton)); },				//MouseButtonPressed
		[&]() { return type_index(TYPE_ID(PressedMouseButton)); },				//MouseButtonHold
		[&]() { return type_index(TYPE_ID(ReleasedMouseButton)); },				//MouseButtonReleased
		//Mouse 
		[&]() { return type_index(TYPE_ID(ScrolledMouseWheel)); },				//MouseWheelScrolled
		[&]() { return type_index(TYPE_ID(MovedMouse)); },						//MouseMoved
		[&]() { return type_index(TYPE_ID(MovedRawMouse)); },					//MouseMovedRaw
		[&]() { return type_index(TYPE_ID(EnteredMouse)); },					//MouseEntered
		[&]() { return type_index(TYPE_ID(LeftMouse)); },						//MouseLeft

		//Window
		[&]() { return type_index(TYPE_ID(ResizedWindow)); },					//Resized
		[&]() { return type_index(TYPE_ID(LostFocus)); },						//FocusLost
		[&]() { return type_index(TYPE_ID(GainedFocus)); },						//FocusGained
		[&]() { return type_index(TYPE_ID(EnteredText)); },						//TextEntered
		[&]() { return type_index(TYPE_ID(ClosedWindow)); },  					//Closed

		//Controler
		[&]() { return type_index(TYPE_ID(ControllerJoystickPressed)); },		//JoystickButtonPressed
		[&]() { return type_index(TYPE_ID(ControllerJoystickPressed)); },		//JoystickButtonHold
		[&]() { return type_index(TYPE_ID(ControllerJoystickReleased)); },		//JoystickButtonReleased
		[&]() { return type_index(TYPE_ID(ControllerJoystickMoved)); },			//JoystickMoved
		[&]() { return type_index(TYPE_ID(ControllerJoystickConnected)); },		//JoystickConnected
		[&]() { return type_index(TYPE_ID(ControllerJoystickDisconnected)); },	//JoystickDisconnected

		//Screen
		[&]() { return type_index(TYPE_ID(BeganTouch)); },						//TouchBegan
		[&]() { return type_index(TYPE_ID(MovedTouch)); },						//TouchMoved
		[&]() { return type_index(TYPE_ID(EndedTouch)); },						//TouchEnded
		[&]() { return type_index(TYPE_ID(ChangedSensor)); }					//SensorChanged
	};

	return _actionsTypes[_actionType]();
}