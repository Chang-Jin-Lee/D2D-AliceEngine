#pragma once
#include <vector>
#include <queue>
#include <cstdint>
#include <cassert>
#include "Singleton.h"

class UObject;

struct ObjectHandle 
{
    uint32_t index = 0;
    uint32_t generation = 0;

    bool operator==(const ObjectHandle& other) const 
    {
        return index == other.index && generation == other.generation;
    }
};

class ObjectHandler : public Singleton<ObjectHandler>
{
    friend class Singleton<ObjectHandler>;
private:
    ObjectHandler() = default;
    ~ObjectHandler() = default;

    std::vector<UObject*> objects;      // �ν��Ͻ� ������ ����
    std::vector<uint32_t> generations; // �ε����� generation
    std::queue<uint32_t> freeIndexes;  // �ı��� �ε��� ����

public:
    // �ڵ�� ������Ʈ ������ ��ȯ
    UObject* GetObjectPtr(const ObjectHandle& h) 
    {
        if (h.index >= objects.size() || !objects[h.index]) return nullptr;
        if (generations[h.index] != h.generation) return nullptr;
        return objects[h.index];
    }

    // Ÿ�� ���� �ٿ�ĳ����
    template<typename T>
    T* GetTypePtr(const ObjectHandle& h) 
    {
        UObject* base = GetObjectPtr(h);
        return dynamic_cast<T*>(base);
    }

    // �ڵ� ���� (�� ������Ʈ ���)
    ObjectHandle CreateHandle(UObject* allocated)
    {
        uint32_t index = 0;
        if (freeIndexes.empty()) {
            index = static_cast<uint32_t>(objects.size());
            generations.push_back(0);
            objects.push_back(allocated);
        }
        else {
            index = freeIndexes.front();
            freeIndexes.pop();
            generations[index]++;
            assert(objects[index] == nullptr);
            objects[index] = allocated;
        }
        return ObjectHandle{ index, generations[index] };
    }

    // �ڵ� �ı� (������Ʈ ����)
    void DestroyHandle(const ObjectHandle& h) {
        if (IsValid(h)) {
            objects[h.index] = nullptr;
            freeIndexes.push(h.index);
            // generation�� ���� �������� ����
        }
    }

    // �ڵ� ��ȿ�� �˻�
    bool IsValid(const ObjectHandle& h) const {
        return h.index < objects.size()
            && objects[h.index] != nullptr
            && generations[h.index] == h.generation;
    }

    // ������ ������Ʈ ������ ��ȯ
    UObject* Get(const ObjectHandle& h) {
        return IsValid(h) ? objects[h.index] : nullptr;
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
        ptr = instance;
        // UObject*���� Handle�� �����Ұű� ������ �̷��� ĳ����
        if (auto uobj = dynamic_cast<UObject*>(instance))
            handle = uobj->GetHandle();
        else
            handle = ObjectHandle{}; // invalid handle
    }

    bool IsValid() const {
        return ptr != nullptr && ObjectHandler::GetInstance().IsValid(handle);
    }

    T* lock() const {
        return IsValid() ? ptr : nullptr;
    }

    T* Get() const {
        return lock();
    }

    T* operator->() const {
        return lock(); // �ݵ�� ��ȿ�� üũ �� ���
    }

    bool expired() const {
        return !IsValid();
    }

    void reset() {
        ptr = nullptr;
        handle = ObjectHandle{};
    }

    explicit operator bool() const {
        return IsValid();
    }
};
