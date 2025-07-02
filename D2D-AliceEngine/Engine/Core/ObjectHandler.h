#pragma once
#include <assert.h>
#include <vector>
#include <queue>
#include <cstdint>
#include <unordered_set>
#include <core/Singleton.h>

class UObject;
class ObjectHandler : public Singleton<ObjectHandler>
{
	//ObjectTable�� ������ / �Ҹ��ڸ� private���� �����߱� ������, 
// �Ϲ� �ڵ�� new MyManager()�� delete�� ���� ȣ���� �� �����ϴ�.
	friend class Singleton<ObjectHandler>;// �� ������ �Ʒ� Create()���� ������ ����
private:
	ObjectHandler() = default;
	~ObjectHandler() = default;

	std::unordered_set<UObject*> m_objects;      // ������ �ν��Ͻ� �ּ� ����
public:

	// �ν��Ͻ� �ּҰ� �����ϴ��� �˻�
	bool IsValid(UObject* instance) {
		return m_objects.find(instance) != m_objects.end();
	}
	void Register(UObject* instance) {
		m_objects.insert(instance);
	}
	void Unregister(UObject* instance) {
		m_objects.erase(instance);
	}
};