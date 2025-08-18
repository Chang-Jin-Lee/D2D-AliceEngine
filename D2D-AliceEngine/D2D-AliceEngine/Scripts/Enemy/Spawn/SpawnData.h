#pragma once
#include "Component/ScriptComponent.h"
#include "TileMap/TileMapLoader.h"

class SpawnData : public ScriptComponent
{
public:
	const CollData& GetCollData() { return colldata; }
	void SetCollData(CollData _colldata) { colldata = _colldata; }
private:
	CollData colldata;
};

