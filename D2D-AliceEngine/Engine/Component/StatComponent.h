#pragma once
#include <Component/Component.h>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <functional>
#include <iostream>
#include <cassert>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>  // 이 파일을 통해 BuildOffsetMap 분리

/*
* @briefs : Stat을 관리하는 Component 입니다.
* @details : 
* 원하는 Stat을 구조체로 만들어 관리가 가능합니다. 
* 기본 Stat는 DefaultStat입니다.
* 다음처럼 구현해 사용이 가능합니다.
 	struct MyStat {
	float HP = 100.f;
	float MP = 50.f;
};
DEFINE_STAT_TRAITS_2(MyStat, HP, MP, STR, DEX, INT)
m_Stat = m_obj->AddComponent<StatComponent<MyStat>>();
*/

struct DefaultStat {
	float HP = 100.f;
	float MAXHP = 100.f;
	float MP = 50.f;
	float STR = 10.f;
	float DEX = 20.f;
	float INT = 30.f;
};

DEFINE_STAT_TRAITS_6(DefaultStat, HP, MAXHP, MP, STR, DEX, INT)

template<typename T = DefaultStat>
class StatComponent : public Component
{
public:
	T value;
	T prevValue;

	MultiDelegate<std::string, float, float> OnChangeStat;
	std::unordered_map<std::string, MultiDelegate<float, float>> OnChangeStatMap;

	virtual void Initialize() override {}
	virtual void Update()  override {}
	virtual void Release()  override {}

	StatComponent() {
		memset(&value, 0, sizeof(T));
		memset(&prevValue, 0, sizeof(T));
	}

	// Get, Set으로 직접 사용할 수도 있음.

	void SetStat(const std::string& statName, float newVal) {
		float* valPtr = GetFieldPtr(value, statName);
		float* prevPtr = GetFieldPtr(prevValue, statName);
		if (valPtr && prevPtr && *valPtr != newVal) {
			*prevPtr = *valPtr;
			*valPtr = newVal;
			MultiDelegate<std::string, float, float> OnChangeStat;
			auto it = OnChangeStatMap.find(statName);
			if (it != OnChangeStatMap.end()) {
				it->second.BroadCast(*prevPtr, *valPtr);
			}
		}
	}

	float GetStat(const std::string& statName) const {
		const float* ptr = GetFieldPtr(value, statName);
		return ptr ? *ptr : 0.f;
	}

	void TakeDamage(const std::string& statName, float val)
	{
		float result = std::max<float>(0, GetStat(statName) - val);
		SetStat(statName, result);
	}

	void Heal(const std::string& statMAXName, const std::string& statName , float val)
	{
		float result = std::min<float>(GetStat(statMAXName), GetStat(statName) + val);
		SetStat(statName, result);
	}

private:
	float* GetFieldPtr(T& instance, const std::string& fieldName) const {
		static const std::unordered_map<std::string, size_t> offsetMap = StatTraits<T>::GetOffsetMap();
		auto it = offsetMap.find(fieldName);
		if (it != offsetMap.end()) {
			return reinterpret_cast<float*>(
				reinterpret_cast<char*>(&instance) + it->second
				);
		}
		return nullptr;
	}

	const float* GetFieldPtr(const T& instance, const std::string& fieldName) const {
		return const_cast<StatComponent<T>*>(this)->GetFieldPtr(const_cast<T&>(instance), fieldName);
	}
};
