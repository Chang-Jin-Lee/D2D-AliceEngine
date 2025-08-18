#include "Battery.h"

void UI_Battery::AddBattery(int _val)
{
	battery += _val;
	Math::clamp<int>(_val, minSize, maxSize);

	CheckStat();
}

void UI_Battery::RemoveSlots(ObjectHandle& _handle, ActionType _type)
{
	while (true) {
		auto it = std::find_if(slots.begin(), slots.end(),
			[_handle, _type](const std::pair<ActionType, Slot>& kv)
			{ return (kv.first == _type && kv.second.handle == _handle); });

		if (it == slots.end())
			break;
		else
			slots.erase(it);
	}
}

std::string UI_Battery::GetCurrentStat()
{
	switch (currStat)
	{
	case UI_Battery::MIN:
		return "MIN";
		break;
	case UI_Battery::LOW:
		return "LOW";
		break;
	case UI_Battery::MAX:
		return "MAX";
		break;
	default:
		break;
	}

	return "NULL";
}

void UI_Battery::Update()
{
	if (prevStat != currStat) {
		prevStat = currStat;

		switch (currStat)
		{
		case UI_Battery::MIN:
			BroadCast(MIN);
			break;
		case UI_Battery::LOW:
			BroadCast(LOW);
			break;
		case UI_Battery::MAX:
			BroadCast(MAX);
			break;
		default:
			break;
		}
	}

}

void UI_Battery::CheckStat()
{
	if (battery <= minSize)
		currStat = MIN;
	else if (battery <= lowSize)
		currStat = LOW;
	else if (battery >= maxSize)
		currStat = MAX;
}

void UI_Battery::BroadCast(ActionType _type)
{
	for (auto it = slots.begin(); it != slots.end();) {
		if (it->first == _type) {
			if (ObjectHandler::GetInstance().IsValid(it->second.handle)) {
				it->second.func();
				it++;
			}
			else
				it = slots.erase(it);
		}
		else
			it++;
	}
}
