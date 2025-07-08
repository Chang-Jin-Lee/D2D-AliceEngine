#pragma once
#include <Experimental/Physics/AABB.h>

class Collider;
namespace Physics
{
	// �浹 �˻� ����ƽ �Լ����� ���ִ� Ŭ����.
	class FCollisionDetector
	{
	public:
		FCollisionDetector() {};
		~FCollisionDetector() {};

		static bool BruteForceOverlapCheck(const std::vector<WeakObjectPtr<Collider>>& objects);
	};

	struct NodeEntry
	{
		UObject* obj;
		NodeEntry* prev;
		NodeEntry* next;

		NodeEntry()
		{
			prev = nullptr;
			next = nullptr;
			obj = nullptr;
		}
	};

	struct Rectangle
	{
		float x, y, length;
	};

	//class QuadTree
	//{
	//public:
	//	QuadTree();
	//	~QuadTree();
	//
	//	UObject* head;
	//	UINT nodeID = -1;
	//	//  ------------------- 
	//	//  |        |        |
	//	//  |    1   |   2    |
	//	//  |--------|--------|
	//	//  |    3   |   4    |
	//	//  |        |        |
	//	//  ------------------- 
	//	QuadTree* child[2][2];
	//	Rectangle rect;
	//
	//	static void MakeQuadTree(QuadTree* tree, int& nodeID, const int& level);
	//	static void FindNode(QuadTree* tree, const int& level, UObject** Node, const FVector3& minVector, const FVector3& maxVector);
	//	static void UpdateNode(QuadTree* tree, const int& level, UObject* object, const FVector3& minVector, const FVector3& maxVector);
	//	static void ClearNode(QuadTree* tree, const int& level);
	//};
}

