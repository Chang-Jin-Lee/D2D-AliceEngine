#pragma once
#include "Component/ScriptComponent.h"
#include <functional>

class Health : public ScriptComponent
{
public:
	enum ActionType {
		MIN = 0,
		LOW,
		MAX,
		END
	};

	int maxSize = 100;
	int minSize = 0;
	int lowSize = 15;

	void AddHealth(int _val);
	void RemoveSlots(ObjectHandle& _handle, ActionType _type);
	std::string GetCurrentStat();

	template<typename T>
	void AddSlot(ObjectHandle _handle, T&& _action, ActionType _type) {
		if (ObjectHandler::GetInstance().IsValid(_handle)) {
			Slot temp = { _handle, _action };
			slots.push_back(std::make_pair(_type, temp));
		}
	}

	void Update() override;
private:
	struct Slot {
		ObjectHandle handle;
		std::function<void()> func;
	};

	int hp = 100;
	ActionType currStat = MAX;
	ActionType prevStat = MAX;

	std::vector<std::pair<ActionType, Slot>> slots;

	void CheckStat();
	void BroadCast(ActionType _type);
};

