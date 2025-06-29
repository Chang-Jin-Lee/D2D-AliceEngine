#pragma once
#include <assert.h>
#include <vector>
#include <queue>
#include <cstdint>
#include <core/Singleton.h>

class UObject;
struct ObjectHandle {
	uint32_t index = 0;
	uint32_t generation = 0;
};
class ObjectHandler : public Singleton<ObjectHandler>
{
	//ObjectHandler�� ������ / �Ҹ��ڸ� private���� �����߱� ������, 
	// �Ϲ� �ڵ�� new MyManager()�� delete�� ���� ȣ���� �� �����ϴ�.
	friend class Singleton<ObjectHandler>;// �� ������ �Ʒ� Create()���� ������ ����
private:
	ObjectHandler() = default;
	~ObjectHandler() = default;

	std::vector<UObject*> objects;      // ������ �ν��Ͻ� �ּ� ����
	std::vector<uint32_t> generations;     // �� ��ü�� Generation
	std::queue<uint32_t> freeIndexes;          // �ı��� ��ü�� index�� ����
public:
	template<typename T>
	T* GetTypePtr(const ObjectHandle& h) {
		UObject* base = GetObjectPtr(h);
		return dynamic_cast<T*>(base);  // typeid ��� RTTI �ٿ�ĳ����
	}

	UObject* GetObjectPtr(const ObjectHandle& h) {
		if (h.index >= objects.size() || !objects[h.index]) return nullptr;
		if (generations[h.index] != h.generation) return nullptr;
		return objects[h.index];
	}
	ObjectHandle CreateHandle(UObject* allocated) {
		uint32_t index = 0;
		if (freeIndexes.empty()) {	//��Ȱ�� ������ �ε����� ���ٸ�
			index = static_cast<uint32_t>(objects.size());// �߰��� ����� �ε�������.
			generations.push_back(0);				// ���ο� �ε����� ���� ��0
			objects.push_back(allocated);	// ���ʿ� �ν��Ͻ� ������ ����
		}
		else { // ��Ȱ�� ������ �ε����� �ִٸ�
			index = freeIndexes.front();			// �������� index ���� 
			freeIndexes.pop();					// ť���� ����
			generations[index]++;				// ���� ���� (�� ������Ʈ �ǹ�)		
			assert(objects[index] == nullptr);
			objects[index] = allocated;		// ��Ȱ���ϴ� �ε����� ����
		}
		return ObjectHandle{ index, generations[index] };
	}
	void DestroyHandle(const ObjectHandle& h) {
		if (IsValid(h)) {
			objects[h.index] = nullptr;
			freeIndexes.push(h.index); // index ���� �����ϰ� ť�� ����
			// generation�� ���� �������� ����
		}
	}
	// �ڵ��� ����Ű�� ������Ʈ�� ��ȿ���� �˻�
	bool IsValid(const ObjectHandle& h) const {
		return h.index < objects.size()
			&& objects[h.index] != nullptr
			&& generations[h.index] == h.generation;
	}

	// ������Ʈ ������ ���
	UObject* Get(const ObjectHandle& h) {
		if (IsValid(h)) return objects[h.index];
		return nullptr;
	}
};

template<typename T>
class WeakObjectPtr {
public:
	T* ptr = nullptr;
	ObjectHandle handle;

	WeakObjectPtr() = default;
	WeakObjectPtr(T* instance)
	{
		Set(instance);
	}

	bool IsValid() const {
		return ptr != nullptr && ObjectHandler::Instance().IsValid(handle);
	}

	void Set(T* instance) { ptr = instance; handle = instance->GetHandle(); }
	T* Get() const {
		return ObjectHandler::Instance().IsValid(handle) ? ptr : nullptr;
	}

	T* operator->() const {
		return ptr; // ȣ�� ���� �ݵ�� ��ȿ�� �˻��� ��
	}
	explicit operator bool() const {
		return IsValid();
	}
	void Reset() {
		ptr = nullptr;
		handle = ObjectHandle{};
	}
};