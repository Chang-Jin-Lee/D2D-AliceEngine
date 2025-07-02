#pragma once
#include <functional>
#include <Core/ObjectHandler.h>
class Delegate
{
};

template<typename... Args>
class MultiDelegate {
	struct Slot {
		UObject* handle; // �ݹ� ���п� ������(�ַ� this)
		std::function<void(Args...)> func;
	};
	std::vector<Slot> slots;

public:
	// Handle�� ���� �޴´�.
	void Add(UObject* handle, const std::function<void(Args...)>& f) {
		slots.push_back({ handle, f });
	}
	// tag(������)�� ����
	void Remove(UObject* handle) {
		slots.erase(
			std::remove_if(slots.begin(), slots.end(),
				[handle](const Slot& s) { return s.handle == handle; }),
			slots.end());
	}
	void Clear() { slots.clear(); }
	void BroadCast(Args... args) const {
		for (const auto& s : slots)
		{
			// Handle�� ���̺��� ��ȿ���� �˻�
			if (ObjectHandler::GetInstance().IsValid(s.handle))
				s.func(args...);
		}

	}
};

