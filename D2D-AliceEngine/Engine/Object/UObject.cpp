#include "pch.h"
#include "UObject.h"
#include <Scene/Scene.h>
#include <Manager/SceneManager.h>

UObject::UObject()
{
	objectHandle = ObjectHandler::GetInstance().CreateHandle(this);
}

UObject::~UObject()
{
	ObjectHandler::GetInstance().DestroyHandle(objectHandle);
}

Scene* UObject::GetWorld()
{
	if (Scene* scene = SceneManager::GetInstance().GetWorld())
	{
		return scene; // 월드가 없을 경우 nullptr 반환
	}
	return nullptr;
}
