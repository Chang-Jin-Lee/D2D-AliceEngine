#include "pch.h"
#include "Spine2DRenderComponent.h"
#include "Manager/D2DRenderManager.h"
#include <Manager/PackageResourceManager.h>

#include <spine/Spine2DTextureLoader.h>
//#include <spine/SkeletonBinary.h>
//#include <spine/spine-alice.h>

Spine2DRenderComponent::Spine2DRenderComponent()
{
}

Spine2DRenderComponent::~Spine2DRenderComponent()
{
	for (auto m_bitmap : m_bitmaps)
	{
		m_bitmap = nullptr;
	}
	files.clear();
	m_bitmaps.clear();
}

void Spine2DRenderComponent::Initialize()
{
}

void Spine2DRenderComponent::Update(const float& deltaSeconds)
{
}

void Spine2DRenderComponent::UpdateFrames()
{
}

void Spine2DRenderComponent::ReleaseFrames()
{
}

void Spine2DRenderComponent::LoadData(const std::wstring& path)
{
	std::shared_ptr<ID2D1Bitmap1> _bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(path.c_str());
	spine::D2DSpine2DTextureLoader * textureLoader = new spine::D2DSpine2DTextureLoader(_bitmap.get());
	// 1. Atlas 로드
	spine::Atlas* atlas = new spine::Atlas("Resource\\yuuka_spr\\yuuka_spr.atlas", textureLoader);

	//spine::SkeletonBinary binary(atlas);
	//binary.setScale(1.0f); // 스케일 조정
	//
	//spine::SkeletonData* skeletonData = binary.readSkeletonDataFile("Resource\\yuuka_spr\\yuuka_spr.skel");
	//if (!skeletonData)
	//{
	//	// 에러 처리: binary.getError().buffer()
	//}



	//SkeletonDrawable_D2D* skeletonDrawable = new SkeletonDrawable_D2D(_bitmap.get(), L"Resource\\yuuka_spr\\yuuka_spr");
	
	//// 2. SkeletonBinary 인스턴스 생성
	//SkeletonBinary binary(atlas);
	//binary.setScale(1.0f); // 스케일 조정
	//
	//// 3. .skel 파일 로드
	//spine::SkeletonData* skeletonData = binary.readSkeletonDataFile("Resource\\yuuka_spr\\yuuka_spr.skel");
	//if (!skeletonData) {
	//	// 에러 처리: binary.getError().buffer()
	//}
}

void Spine2DRenderComponent::LoadFrame(size_t frameIndex)
{
}

void Spine2DRenderComponent::Release()
{
}

void Spine2DRenderComponent::Render()
{
}

FVector2 Spine2DRenderComponent::GetSize()
{
	if (m_bitmaps.empty() == false)
	{
		D2D1_SIZE_U bmpSize = m_bitmaps[0]->GetPixelSize();
		return FVector2(bmpSize.width, bmpSize.height);
	}
	return FVector2();
}
