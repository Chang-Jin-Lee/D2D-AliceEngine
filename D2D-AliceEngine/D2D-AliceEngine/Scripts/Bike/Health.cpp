#include "Health.h"

void Health::AddHealth(int _val)
{
	hp += _val;
	Math::clamp<int>(_val, minSize, maxSize);

	CheckStat();
}

void Health::RemoveSlots(ObjectHandle& _handle, ActionType _type)
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

std::string Health::GetCurrentStat()
{
	switch (currStat)
	{
	case Health::MIN:
		return "MIN";
		break;
	case Health::LOW:
		return "LOW";
		break;
	case Health::MAX:
		return "MAX";
		break;
	default:
		break;
	}

	return "NULL";
}

void Health::Update()
{
	if (prevStat != currStat) {
		prevStat = currStat;

		switch (currStat)
		{
		case Health::MIN:
			BroadCast(MIN);
			break;
		case Health::LOW:
			BroadCast(LOW);
			break;
		case Health::MAX:
			BroadCast(MAX);
			break;
		default:
			break;
		}
	}
}

void Health::CheckStat()
{
	if (hp <= minSize)
		currStat = MIN;
	else if (hp <= lowSize)
		currStat = LOW;
	else if (hp >= maxSize)
		currStat = MAX;
}

void Health::BroadCast(ActionType _type)
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
