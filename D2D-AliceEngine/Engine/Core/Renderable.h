#pragma once
#include <Define/Define.h>

class IRenderable
{
public:
	virtual ~IRenderable() = default;
	virtual void Render() = 0;

};

class Renderable : public IRenderable
{
public:
	int  GetLayer() const { return m_layer; }
	void SetLayer(int v) { m_layer = v; }
	void AddLayer(int v) { m_layer += v; }

	virtual void Render() override {}

	Define::EDrawType GetDrawType() const { return m_drawType; }
	void SetDrawType(Define::EDrawType t) { m_drawType = t; }

	bool GetVisible() const { return bVisible; }
	void SetVisible(bool v) { bVisible = v; }

protected:
	int m_layer{ -999 };
	Define::EDrawType m_drawType{ Define::EDrawType::WorldSpace };
	bool bVisible = true;
};
