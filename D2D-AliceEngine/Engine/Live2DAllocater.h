#pragma once
#include <Live2D/Framework/src/CubismFramework.hpp>

class Live2DAllocater : public Csm::ICubismAllocator
{
public:
	Live2DAllocater() = default;
	virtual ~Live2DAllocater() = default;

	// 메모리 할당
	void* Allocate(Csm::csmSizeType size) override
	{
		return ::operator new(size);
	}
	// 메모리 해제
	void Deallocate(void* ptr) override
	{
		::operator delete(ptr);
	}

	void* AllocateAligned(const Csm::csmSizeType size, const Csm::csmUint32 alignment) override
	{
		size_t offset = alignment - 1 + sizeof(void*);
		void* ptr = ::operator new(size + offset);
		if (ptr == nullptr)
		{
			return nullptr;
		}
		void** alignedPtr = reinterpret_cast<void**>(
			(reinterpret_cast<size_t>(ptr) + offset) & ~(alignment - 1));
		alignedPtr[-1] = ptr; // 원래 포인터를 저장

		return alignedPtr;
	}
	void DeallocateAligned(void* alignedMemory) override
	{
		void* originalPtr = reinterpret_cast<void**>(alignedMemory)[-1];
		::operator delete(originalPtr);
	}
};
