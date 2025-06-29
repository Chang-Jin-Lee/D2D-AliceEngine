#include <spine/spine-alice.h>
#include <spine/Slot.h>
#include <spine/Attachment.h>
#include <spine/RegionAttachment.h>
#include <spine/MeshAttachment.h>
#include <spine/ClippingAttachment.h>
#include <spine/Spine2DTextureLoader.h>
#include <spine/extension.h>

namespace spine {
	SpineExtension* getDefaultExtension() {
		return new DefaultSpineExtension();
	}
}

SkeletonDrawable_D2D::SkeletonDrawable_D2D(ID2D1Bitmap1* _bitmap, const std::wstring& path)
{
	D2DSpine2DTextureLoader* textureLoader = new D2DSpine2DTextureLoader(_bitmap);
	// 1. Atlas 로드
	std::string spath;
	spath.assign(path.begin(), path.end());
	spine::Atlas* atlas = new spine::Atlas((spath + ".atlas").c_str(), textureLoader);

	spine::SkeletonBinary binary(atlas);
	binary.setScale(1.0f); // 스케일 조정

	spine::SkeletonData* skeletonData = binary.readSkeletonDataFile("Resource\\yuuka_spr\\yuuka_spr.skel");
	if (!skeletonData)
	{
		// 에러 처리: binary.getError().buffer()
	}
}
SkeletonDrawable_D2D::SkeletonDrawable_D2D(spine::SkeletonData* skeletonData, spine::AnimationStateData* stateData)
	: skeleton(new spine::Skeleton(skeletonData)), stateData(stateData), ownsStateData(false) {

	if (!stateData) {
		this->stateData = new spine::AnimationStateData(skeletonData);
		ownsStateData = true;
	}

	state = new spine::AnimationState(this->stateData);

	worldVertices.setSize(1000, 0);  // 대충 여유롭게 잡아둠
	tempUvs.ensureCapacity(1000);
	tempColors.ensureCapacity(1000);
	quadIndices.ensureCapacity(1000);
}

SkeletonDrawable_D2D::~SkeletonDrawable_D2D() {
	delete skeleton;
	delete state;
	if (ownsStateData) delete stateData;
}

void SkeletonDrawable_D2D::Update(float deltaTime) {
	state->update(deltaTime * timeScale);
	state->apply(*skeleton);
	skeleton->updateWorldTransform();
}

void SkeletonDrawable_D2D::Draw(ID2D1DeviceContext* context) {
	if (!context) return;

	if (!debugBrush) {
		InitializeBrush(context);
	}

	context->SetTransform(D2D1::Matrix3x2F::Identity());

	spine::Vector<spine::Slot*>& drawOrder = skeleton->getDrawOrder();
	for (int i = 0, n = drawOrder.size(); i < n; ++i) {
		spine::Slot* slot = drawOrder[i];
		spine::Attachment* attachment = slot->getAttachment();
		if (!attachment) continue;

		if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti)) {
			spine::RegionAttachment* region = (spine::RegionAttachment*)attachment;
			worldVertices.setSize(8, 0);
			region->computeWorldVertices(slot->getBone(), worldVertices, 0, 2);

			for (int j = 0; j < 4; ++j) {
				D2D1_POINT_2F pt = {
					worldVertices[j * 2],
					worldVertices[j * 2 + 1]
				};

				D2D1_ELLIPSE ellipse = D2D1::Ellipse(pt, 2.0f, 2.0f);
				context->FillEllipse(&ellipse, debugBrush.Get());
			}

			// Draw connecting lines (outline)
			for (int j = 0; j < 4; ++j) {
				D2D1_POINT_2F p1 = { worldVertices[j * 2], worldVertices[j * 2 + 1] };
				D2D1_POINT_2F p2 = { worldVertices[((j + 1) % 4) * 2], worldVertices[((j + 1) % 4) * 2 + 1] };
				context->DrawLine(p1, p2, debugBrush.Get(), 1.0f);
			}
		}
	}
}

void SkeletonDrawable_D2D::InitializeBrush(ID2D1DeviceContext* context) {
	D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::Red);
	context->CreateSolidColorBrush(color, &debugBrush);
}