#pragma once
#include <unordered_map>
#include <string>
#include <cstddef>

/*
* @briefs : ����ü�� unordered_map�� ��� �����մϴ�
* @details : 
*	��ũ�η� ���ϴ� ������ŭ �߰��� �����մϴ�. 
*	struct myStruct(); ���
*	StstTraits<myStruct>�� �������� �˴ϴ�
* @example :
	struct MyStat {
	float HP = 100.f;
	float MP = 50.f;
	float STR = 10.f;
	float DEX = 20.f;
	float INT = 30.f;
};

DEFINE_STAT_TRAITS_5(MyStat, HP, MP, STR, DEX, INT)
*/
// 1������ 10������ ����� ����.

// �⺻ ���ø�
template<typename T>
struct StatTraits {
	static std::unordered_map<std::string, size_t> GetOffsetMap() {
		return {};
	}
};

// �ʵ� ��� ��ũ��
#define REGISTER_FIELD(TypeName, Field) result[#Field] = offsetof(TypeName, Field);

// DEFINE_STAT_TRAITS_1 ~ DEFINE_STAT_TRAITS_10
#define DEFINE_STAT_TRAITS_1(TypeName, F1) \
template<> \
struct StatTraits<TypeName> { \
	static std::unordered_map<std::string, size_t> GetOffsetMap() { \
		std::unordered_map<std::string, size_t> result; \
		REGISTER_FIELD(TypeName, F1); \
		return result; \
	} \
};

#define DEFINE_STAT_TRAITS_2(TypeName, F1, F2) \
template<> \
struct StatTraits<TypeName> { \
	static std::unordered_map<std::string, size_t> GetOffsetMap() { \
		std::unordered_map<std::string, size_t> result; \
		REGISTER_FIELD(TypeName, F1); \
		REGISTER_FIELD(TypeName, F2); \
		return result; \
	} \
};

#define DEFINE_STAT_TRAITS_3(TypeName, F1, F2, F3) \
template<> \
struct StatTraits<TypeName> { \
	static std::unordered_map<std::string, size_t> GetOffsetMap() { \
		std::unordered_map<std::string, size_t> result; \
		REGISTER_FIELD(TypeName, F1); \
		REGISTER_FIELD(TypeName, F2); \
		REGISTER_FIELD(TypeName, F3); \
		return result; \
	} \
};

#define DEFINE_STAT_TRAITS_4(TypeName, F1, F2, F3, F4) \
template<> \
struct StatTraits<TypeName> { \
	static std::unordered_map<std::string, size_t> GetOffsetMap() { \
		std::unordered_map<std::string, size_t> result; \
		REGISTER_FIELD(TypeName, F1); \
		REGISTER_FIELD(TypeName, F2); \
		REGISTER_FIELD(TypeName, F3); \
		REGISTER_FIELD(TypeName, F4); \
		return result; \
	} \
};

#define DEFINE_STAT_TRAITS_5(TypeName, F1, F2, F3, F4, F5) \
template<> \
struct StatTraits<TypeName> { \
	static std::unordered_map<std::string, size_t> GetOffsetMap() { \
		std::unordered_map<std::string, size_t> result; \
		REGISTER_FIELD(TypeName, F1); \
		REGISTER_FIELD(TypeName, F2); \
		REGISTER_FIELD(TypeName, F3); \
		REGISTER_FIELD(TypeName, F4); \
		REGISTER_FIELD(TypeName, F5); \
		return result; \
	} \
};

#define DEFINE_STAT_TRAITS_6(TypeName, F1, F2, F3, F4, F5, F6) \
template<> \
struct StatTraits<TypeName> { \
	static std::unordered_map<std::string, size_t> GetOffsetMap() { \
		std::unordered_map<std::string, size_t> result; \
		REGISTER_FIELD(TypeName, F1); \
		REGISTER_FIELD(TypeName, F2); \
		REGISTER_FIELD(TypeName, F3); \
		REGISTER_FIELD(TypeName, F4); \
		REGISTER_FIELD(TypeName, F5); \
		REGISTER_FIELD(TypeName, F6); \
		return result; \
	} \
};

#define DEFINE_STAT_TRAITS_7(TypeName, F1, F2, F3, F4, F5, F6, F7) \
template<> \
struct StatTraits<TypeName> { \
	static std::unordered_map<std::string, size_t> GetOffsetMap() { \
		std::unordered_map<std::string, size_t> result; \
		REGISTER_FIELD(TypeName, F1); \
		REGISTER_FIELD(TypeName, F2); \
		REGISTER_FIELD(TypeName, F3); \
		REGISTER_FIELD(TypeName, F4); \
		REGISTER_FIELD(TypeName, F5); \
		REGISTER_FIELD(TypeName, F6); \
		REGISTER_FIELD(TypeName, F7); \
		return result; \
	} \
};

#define DEFINE_STAT_TRAITS_8(TypeName, F1, F2, F3, F4, F5, F6, F7, F8) \
template<> \
struct StatTraits<TypeName> { \
	static std::unordered_map<std::string, size_t> GetOffsetMap() { \
		std::unordered_map<std::string, size_t> result; \
		REGISTER_FIELD(TypeName, F1); \
		REGISTER_FIELD(TypeName, F2); \
		REGISTER_FIELD(TypeName, F3); \
		REGISTER_FIELD(TypeName, F4); \
		REGISTER_FIELD(TypeName, F5); \
		REGISTER_FIELD(TypeName, F6); \
		REGISTER_FIELD(TypeName, F7); \
		REGISTER_FIELD(TypeName, F8); \
		return result; \
	} \
};

#define DEFINE_STAT_TRAITS_9(TypeName, F1, F2, F3, F4, F5, F6, F7, F8, F9) \
template<> \
struct StatTraits<TypeName> { \
	static std::unordered_map<std::string, size_t> GetOffsetMap() { \
		std::unordered_map<std::string, size_t> result; \
		REGISTER_FIELD(TypeName, F1); \
		REGISTER_FIELD(TypeName, F2); \
		REGISTER_FIELD(TypeName, F3); \
		REGISTER_FIELD(TypeName, F4); \
		REGISTER_FIELD(TypeName, F5); \
		REGISTER_FIELD(TypeName, F6); \
		REGISTER_FIELD(TypeName, F7); \
		REGISTER_FIELD(TypeName, F8); \
		REGISTER_FIELD(TypeName, F9); \
		return result; \
	} \
};

#define DEFINE_STAT_TRAITS_10(TypeName, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10) \
template<> \
struct StatTraits<TypeName> { \
	static std::unordered_map<std::string, size_t> GetOffsetMap() { \
		std::unordered_map<std::string, size_t> result; \
		REGISTER_FIELD(TypeName, F1); \
		REGISTER_FIELD(TypeName, F2); \
		REGISTER_FIELD(TypeName, F3); \
		REGISTER_FIELD(TypeName, F4); \
		REGISTER_FIELD(TypeName, F5); \
		REGISTER_FIELD(TypeName, F6); \
		REGISTER_FIELD(TypeName, F7); \
		REGISTER_FIELD(TypeName, F8); \
		REGISTER_FIELD(TypeName, F9); \
		REGISTER_FIELD(TypeName, F10); \
		return result; \
	} \
};
