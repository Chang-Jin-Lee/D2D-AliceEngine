#pragma once
#include <functional>
class Delegate
{
};

template<typename... Args>
class MultiDelegate {
	struct Slot {
		void* instance; // �ݹ� ���п� ������(�ַ� this)
		std::function<void(Args...)> func;
	};
	std::vector<Slot> slots;

public:
	void Add(void* instance, const std::function<void(Args...)>& f) {
		slots.push_back({ instance, f });
	}
	// tag(������)�� ����
	void Remove(void* instance) {
		slots.erase(
			std::remove_if(slots.begin(), slots.end(),
				[instance](const Slot& s) { return s.instance == instance; }),
			slots.end());
	}
	void Clear() { slots.clear(); }
	void BroadCast(Args... args) const {
		for (const auto& s : slots)
			if (s.func) s.func(args...);
	}
};

