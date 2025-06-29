#pragma once

#include <spine/spine.h>
#include <d2d1_1.h>
#include <wrl/client.h>
#include <Vector>
#include <iostream>

namespace spine {
	class SpineExtension;
	SpineExtension* getDefaultExtension();
}

using Microsoft::WRL::ComPtr;

class SkeletonDrawable_D2D
{
public:
	SkeletonDrawable_D2D(ID2D1Bitmap1* _bitmap, const std::wstring& path);
	SkeletonDrawable_D2D(spine::SkeletonData* skeletonData, spine::AnimationStateData* stateData = nullptr);
	~SkeletonDrawable_D2D();

	void Update(float deltaTime);
	void Draw(ID2D1DeviceContext* context);

	void SetPremultipliedAlpha(bool enabled) { usePremultipliedAlpha = enabled; }
	bool GetPremultipliedAlpha() const { return usePremultipliedAlpha; }

private:
	spine::Skeleton* skeleton;
	spine::AnimationState* state;
	spine::AnimationStateData* stateData;
	bool ownsStateData;

	float timeScale = 1.0f;
	bool usePremultipliedAlpha = false;

	spine::Vector<float> worldVertices;
	spine::Vector<float> tempUvs;
	spine::Vector<spine::Color> tempColors;
	spine::Vector<unsigned short> quadIndices;
	spine::SkeletonClipping clipper;

	ComPtr<ID2D1SolidColorBrush> debugBrush;

	void InitializeBrush(ID2D1DeviceContext* context);
};