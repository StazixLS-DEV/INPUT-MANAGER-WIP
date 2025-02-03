#include "Action.h"

Input::Action::Action(const string& _name, const ActionData& _data, const function<void()>& _callback)
{
	name = _name;
	allData.insert({ ComputeTypeIndexByActionType(_data.type), _data });
	callback = _callback;
}

Input::Action::Action(const string& _name, const set<ActionData>& _allData, const function<void()>& _callback)
{
	name = _name;
	for (const ActionData& _actionData : _allData)
	{
		allData.insert({ ComputeTypeIndexByActionType(_actionData.type), _actionData });
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
			if (const KeyPressed* _key = _event->getIf<KeyPressed>())
			{
				if (IsInAllData(_elementType, CAST(const int, _key->code)))
				{
					/*if (isholding)0
					{
					callbackpressed(); 1

					}

				isHolding true; 2 
					holdCallback() 3*/
				}
			}

			else if (const KeyReleased* _key = _event->getIf<KeyReleased>())
			{
				if (IsInAllData(_elementType, CAST(const int, _key->code)))
				{
					callback();
				}
			}

			else if (const MouseButtonPressed* _key = _event->getIf<MouseButtonPressed>())
			{
				if (IsInAllData(_elementType, CAST(const int, _key->button)))
				{
					callback();
				}
			}

			else if (const MouseButtonReleased* _key = _event->getIf<MouseButtonReleased>())
			{
				if (IsInAllData(_elementType, CAST(const int, _key->button)))
				{
					callback();
				}
			}


		}
	});
}

Input::TypeIndex Input::Action::ComputeTypeIndexByActionType(const ActionType& _actionType)
{
	const vector<function<TypeIndex()>>& _actionsTypes =
	{
		[&]() { return CAST(type_index, typeid(KeyPressed)); },
		[&]() { return CAST(type_index,typeid(KeyPressed)); },
		[&]() { return CAST(type_index,typeid(KeyReleased)); },
		[&]() { return CAST(type_index, typeid(MouseButtonPressed)); },
		[&]() { return CAST(type_index,typeid(MouseButtonPressed)); },
		[&]() { return CAST(type_index,typeid(MouseButtonReleased)); },
	};

	return _actionsTypes[_actionType]();
}
