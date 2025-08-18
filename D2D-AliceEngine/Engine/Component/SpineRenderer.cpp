#include "pch.h"
#include "SpineRenderer.h"
#include <Extension/json.hpp>// nlohmann::json 사용
#include <wincodec.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <spine/Atlas.h>
#include <spine/SkeletonJson.h>
#include <spine/AnimationState.h>
#include <spine/AnimationStateData.h>
#include <spine/Skeleton.h>
#include <spine/AtlasAttachmentLoader.h>
#include <spine/TextureLoader.h>
#include <spine/Slot.h>
#include <spine/RegionAttachment.h>
#include <spine/SkeletonClipping.h>
#include <spine/Bone.h>
#include <spine/Animation.h>
#include <spine/BoneData.h>
#include <algorithm>
#include <spine/MeshAttachment.h>

#pragma comment(lib, "d3d11.lib")

#include <d2d1_3.h> //ID2D1Factory8,ID2D1DeviceContext7
#pragma comment(lib, "d2d1.lib")

#include <dxgi1_6.h> // IDXGIFactory7
#pragma comment(lib, "dxgi.lib")

#pragma comment(lib,"dwrite.lib")

#include <Core/Singleton.h>
#include <Manager/D2DRenderManager.h>
#include <Helpers/FileHelper.h>
#include <Helpers/StringHelper.h>
#include <System/RenderSystem.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>

static inline float clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

// Direct2DTextureLoader 구현
Direct2DTextureLoader::Direct2DTextureLoader(ID2D1RenderTarget* renderTarget)
	: m_renderTarget(renderTarget) {
}
Direct2DTextureLoader::~Direct2DTextureLoader() {}
void Direct2DTextureLoader::load(spine::AtlasPage& page, const spine::String& path)
{
	std::wstring wPath(path.buffer(), path.buffer() + path.length());

	std::shared_ptr<ID2D1Bitmap1> bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(wPath.c_str());

	page.texture = bitmap.get();
	m_bitmapMap[path.buffer()] = bitmap;
	page.width = static_cast<int>(bitmap->GetSize().width);
	page.height = static_cast<int>(bitmap->GetSize().height);
}
void Direct2DTextureLoader::unload(void* texture)
{
	// ComPr 삭제
	m_bitmapMap.clear();
}


SpineRenderer::SpineRenderer() : m_textureLoader(nullptr), m_atlas(nullptr), m_skeletonData(nullptr), m_skeleton(nullptr), m_stateData(nullptr), m_state(nullptr) 
{

}

SpineRenderer::~SpineRenderer() 
{
	RenderSystem::GetInstance().UnRegist(WeakFromThis<RenderComponent>());
	Shutdown(); 
}

float SpineRenderer::GetBitmapSizeX()
{
	return 0.0f;
}

float SpineRenderer::GetBitmapSizeY()
{
	return 0.0f;
}

void SpineRenderer::RegistSystem(WeakObjectPtr<UObject> object)
{
	if (!ObjectHandler::GetInstance().IsValid(object.Get()->GetHandle())) return;
}

void SpineRenderer::Initialize()
{
	// 상대 좌표 적용
	__super::Initialize();

	RenderSystem::GetInstance().Regist(WeakFromThis<RenderComponent>());
	LoadTextureLoader();
	if (!m_textureLoader)
	{
		OutputDebugStringW(L"spineRenderer Initialize() Error!\n");
	}

	// 디버그 렌더링 리소스 생성
	HRESULT hr = S_OK;
	if (!m_brush)
	{
		hr = D2DRenderManager::GetInstance().m_d2dDeviceContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Lime, 1.0f), m_brush.ReleaseAndGetAddressOf());
	}
	if (!m_dwriteFactory)
	{
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)m_dwriteFactory.ReleaseAndGetAddressOf());
	}
	if (!m_textFormat && m_dwriteFactory)
	{
		hr = m_dwriteFactory->CreateTextFormat(L"Consolas", nullptr, DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"ko-kr",
			m_textFormat.ReleaseAndGetAddressOf());
	}

}

void SpineRenderer::LoadTextureLoader()
{
	m_textureLoader = std::make_unique<Direct2DTextureLoader>(D2DRenderManager::GetInstance().m_d2dDeviceContext.Get());

	FVector2 screenSize = D2DRenderManager::GetInstance().GetApplicationSize();
	m_UnityScreen = D2D1::Matrix3x2F::Scale(1.0f, -1.0f) *
		D2D1::Matrix3x2F::Translation((float)screenSize.x / 2, (float)screenSize.y / 2);
}

void SpineRenderer::Shutdown()
{
	ReleaseSpine();
	ReleaseDirect2D();
}

void SpineRenderer::ReleaseResources() {

}

void SpineRenderer::SetAnimation(const std::string& animationName) {
	if (m_state && m_skeletonData) {
		m_state->setAnimation(0, animationName.c_str(), true);
		m_currentAnimation = animationName;
		m_currentAnimationTime = 0.0f;
		m_animationTime = 0.0f;
		for (size_t i = 0; i < m_animationList.size(); ++i) {
			if (m_animationList[i] == animationName) {
				m_currentAnimationIndex = static_cast<int>(i);
				break;
			}
		}
		std::cout << "Animation changed: " << animationName << " (Index: " << m_currentAnimationIndex << ")" << std::endl;
	}
}

void SpineRenderer::UpdateAnimation(float deltaTime) {
	if (m_state && m_skeleton) {
		m_state->update(deltaTime);
		m_state->apply(*m_skeleton.get());
		m_skeleton->updateWorldTransform(spine::Physics::Physics_Pose);
		m_animationTime += deltaTime;
		m_currentAnimationTime += deltaTime;
	}
}

void SpineRenderer::SetAnimationTime(float time) {
	m_animationTime = time;
}


D2D1_SIZE_F SpineRenderer::GetRenderTargetSize() const {
	if (D2DRenderManager::GetInstance().m_d2dDeviceContext) {
		return D2DRenderManager::GetInstance().m_d2dDeviceContext->GetSize();
	}
	FVector2 screenSize = D2DRenderManager::GetInstance().GetApplicationSize();
	return D2D1::SizeF(static_cast<float>(screenSize.x), static_cast<float>(screenSize.y)); // 기본 크기
}

void SpineRenderer::Clear(const D2D1_COLOR_F& color) {
	if (!m_initialized) return;

	D2DRenderManager::GetInstance().m_d2dDeviceContext->Clear(color);
}

// Spine에서는 본 구조를 그려야 하기 때문에 RenderComponent의 기본 좌표축 계산인
// Render() 함수를 실행하지 않고, 독립적으로 좌표를 설정하고 그려냅니다
void SpineRenderer::Render()
{
	if (!m_bRendered) return;
	if (!m_skeleton || !m_atlas || !D2DRenderManager::GetInstance().m_d2dDeviceContext)
		return;

	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	Camera* camera = SceneManager::GetCamera();
	if (!context || !camera) return;

	D2DRenderManager::GetInstance().DrawDebugBox(-10, -10, 10, 10, 0, 0, 0, 255);

	D2D1::Matrix3x2F worldTransform = relativeTransform.m_worldTransform.ToMatrix();
	D2D1::Matrix3x2F flipY = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F cameraInv = camera->relativeTransform.m_worldTransform.ToMatrix();
	cameraInv.Invert();

	// 슬롯별로 렌더링
	const auto& slotOrder = m_useSetupSlotOrder ? m_skeleton->getSlots() : m_skeleton->getDrawOrder();
	for (size_t i = 0; i < slotOrder.size(); ++i)
	{
		spine::Slot* slot = slotOrder[i];
		spine::Attachment* attachment = slot->getAttachment();
		if (!attachment)
			continue;

		if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti))
		{
			spine::RegionAttachment* regionAtt = static_cast<spine::RegionAttachment*>(attachment);
			spine::TextureRegion* region = regionAtt->getRegion();
			if (!region)
				continue;

			spine::AtlasRegion* atlasRegion = static_cast<spine::AtlasRegion*>(region);
			if (!atlasRegion)
				continue;

			ID2D1Bitmap* bitmap = reinterpret_cast<ID2D1Bitmap*>(region->rendererObject);
			if (!bitmap)
				continue;

			// region.rotate 확인
			assert(atlasRegion->degrees == 0 || atlasRegion->degrees == 90);

			// 텍스처 이미지 배치 최적화로 이미지가 회전된 경우는 예외처리가 필요하다.
			bool rotated = (atlasRegion->degrees == 90);
			float srcW = rotated ? atlasRegion->height : atlasRegion->width;
			float srcH = rotated ? atlasRegion->width : atlasRegion->height;
			D2D1_RECT_F srcRect;
			srcRect.left = (float)atlasRegion->x;
			srcRect.top = (float)atlasRegion->y;
			srcRect.right = srcRect.left + srcW;
			srcRect.bottom = srcRect.top + srcH;

			// destRect + offset 적용
			float destW = rotated ? atlasRegion->originalHeight : atlasRegion->originalWidth;
			float destH = rotated ? atlasRegion->originalWidth : atlasRegion->originalHeight;
			float offsetX = rotated ? atlasRegion->offsetY : atlasRegion->offsetX;
			float offsetY = rotated ? atlasRegion->offsetX : atlasRegion->offsetY;
			float attachX = regionAtt->getX() + offsetX;
			float attachY = regionAtt->getY() + offsetY;
			float attachRot = regionAtt->getRotation() + (rotated ? -90.0f : 0.0f);

			D2D1::Matrix3x2F attachmentMatrix =
				D2D1::Matrix3x2F::Scale(regionAtt->getScaleX(), regionAtt->getScaleY()) *
				D2D1::Matrix3x2F::Rotation(attachRot) *
				D2D1::Matrix3x2F::Translation(attachX, attachY);

			D2D1_RECT_F destRect;
			destRect.left = -destW / 2;
			destRect.top = -destH / 2;
			destRect.right = destRect.left + destW;
			destRect.bottom = destRect.top + destH;

			// 본(Bone)의 월드 변환
			spine::Bone& bone = slot->getBone();
			D2D1::Matrix3x2F boneWorldMatrix =
				D2D1::Matrix3x2F::Scale(bone.getScaleX(), bone.getScaleY()) *
				D2D1::Matrix3x2F::Rotation(bone.getWorldRotationX()) *
				D2D1::Matrix3x2F::Translation(bone.getWorldX(), bone.getWorldY());

			D2D1::Matrix3x2F finalMatrix = flipY * attachmentMatrix * boneWorldMatrix * worldTransform * cameraInv * m_UnityScreen;
			D2DRenderManager::GetInstance().m_d2dDeviceContext->SetTransform(finalMatrix);

			// 이미지 그리기
			D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawBitmap(bitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
		}
		else if (attachment->getRTTI().isExactly(spine::MeshAttachment::rtti))
		{
			// MeshAttachment 정확 렌더(2D 변환 상에서의 삼각형 텍스처 매핑):
			// 각 삼각형에 대해 UV→World의 어파인 변환을 계산하여 BitmapBrush로 채운다.
			spine::MeshAttachment* mesh = static_cast<spine::MeshAttachment*>(attachment);
			spine::TextureRegion* region = mesh->getRegion();
			if (!region) continue;
			spine::AtlasRegion* atlasRegion = static_cast<spine::AtlasRegion*>(region);
			ID2D1Bitmap* pageBitmap = reinterpret_cast<ID2D1Bitmap*>(region->rendererObject);
			if (!pageBitmap) continue;

			// 월드 버텍스 및 UV 준비
			spine::Vector<float> worldVerts; worldVerts.setSize(mesh->getWorldVerticesLength(), 0);
			mesh->computeWorldVertices(*slot, 0, mesh->getWorldVerticesLength(), worldVerts, 0, 2);
			const float* uvs = mesh->getUVs().buffer();
			const spine::Vector<unsigned short>& tris = mesh->getTriangles();
			if (tris.size() < 3) continue;

			// 페이지 크기 (UV는 0..1 기준이므로 픽셀 좌표로 변환)
			float pageW = (float)atlasRegion->page->width;
			float pageH = (float)atlasRegion->page->height;

			// 고정 변환: 월드→스크린 (Spine 좌표를 D2D 화면에)
			D2D1::Matrix3x2F finalMatrix = worldTransform * cameraInv * m_UnityScreen;
			D2DRenderManager::GetInstance().m_d2dDeviceContext->SetTransform(finalMatrix);

			auto solveAffine = [](float sx0, float sy0, float sx1, float sy1, float sx2, float sy2,
							   float dx0, float dy0, float dx1, float dy1, float dx2, float dy2,
							   D2D1_MATRIX_3X2_F& outM) -> bool
			{
				// Solve for m11,m21,dx from sx,sy -> x; and m12,m22,dy from sx,sy -> y
				auto solve3 = [](float a0,float b0,float c0,float r0,
							  float a1,float b1,float c1,float r1,
							  float a2,float b2,float c2,float r2,
							  float& x,float& y,float& z)->bool{
					float det = a0*(b1*c2-b2*c1) - b0*(a1*c2-a2*c1) + c0*(a1*b2-a2*b1);
					if (fabsf(det) < 1e-6f) return false;
					float detX = r0*(b1*c2-b2*c1) - b0*(r1*c2-r2*c1) + c0*(r1*b2-r2*b1);
					float detY = a0*(r1*c2-r2*c1) - r0*(a1*c2-a2*c1) + c0*(a1*r2-a2*r1);
					float detZ = a0*(b1*r2-b2*r1) - b0*(a1*r2-a2*r1) + r0*(a1*b2-a2*b1);
					x = detX/det; y = detY/det; z = detZ/det; return true; };

				float m11,m21,dx; // x = m11*sx + m21*sy + dx
				float m12,m22,dy; // y = m12*sx + m22*sy + dy
				bool okx = solve3(sx0, sy0, 1.0f, dx0,
							  sx1, sy1, 1.0f, dx1,
							  sx2, sy2, 1.0f, dx2,
							  m11, m21, dx);
				bool oky = solve3(sx0, sy0, 1.0f, dy0,
							  sx1, sy1, 1.0f, dy1,
							  sx2, sy2, 1.0f, dy2,
							  m12, m22, dy);
				if (!okx || !oky) return false;
				outM = D2D1::Matrix3x2F(m11, m12, m21, m22, dx, dy);
				return true;
			};

			// 브러시 생성 (페이지 비트맵 기반)
			ComPtr<ID2D1BitmapBrush> brush;
			D2D1_BITMAP_BRUSH_PROPERTIES bbp{};
			bbp.extendModeX = D2D1_EXTEND_MODE_CLAMP;
			bbp.extendModeY = D2D1_EXTEND_MODE_CLAMP;
			bbp.interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
			D2DRenderManager::GetD2DDevice()->CreateBitmapBrush(pageBitmap, &bbp, nullptr, &brush);
			if (!brush) continue;

			// 삼각형 단위로 그리기
			for (size_t ti = 0; ti + 2 < tris.size(); ti += 3)
			{
				unsigned short i0 = tris[ti];
				unsigned short i1 = tris[ti+1];
				unsigned short i2 = tris[ti+2];
				// 목적지(월드) 좌표
				D2D1_POINT_2F p0{ worldVerts[2*i0], worldVerts[2*i0+1] };
				D2D1_POINT_2F p1{ worldVerts[2*i1], worldVerts[2*i1+1] };
				D2D1_POINT_2F p2{ worldVerts[2*i2], worldVerts[2*i2+1] };
				// 소스(페이지 픽셀) 좌표 (UV*pageSize)
				D2D1_POINT_2F s0{ uvs[2*i0]*pageW, uvs[2*i0+1]*pageH };
				D2D1_POINT_2F s1{ uvs[2*i1]*pageW, uvs[2*i1+1]*pageH };
				D2D1_POINT_2F s2{ uvs[2*i2]*pageW, uvs[2*i2+1]*pageH };

				// 어파인 변환(소스→월드) 계산
				D2D1_MATRIX_3X2_F M;
				if (!solveAffine(s0.x, s0.y, s1.x, s1.y, s2.x, s2.y,
								 p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, M))
				{
					continue;
				}
				brush->SetTransform(M);

				// 목적 삼각형 지오메트리 생성
				ComPtr<ID2D1PathGeometry> geom;
				D2DRenderManager::GetInstance().m_d2dFactory->CreatePathGeometry(&geom);
				if (!geom) continue;
				ComPtr<ID2D1GeometrySink> sink;
				geom->Open(&sink);
				sink->BeginFigure(p0, D2D1_FIGURE_BEGIN_FILLED);
				sink->AddLine(p1);
				sink->AddLine(p2);
				sink->EndFigure(D2D1_FIGURE_END_CLOSED);
				sink->Close();

				D2DRenderManager::GetD2DDevice()->FillGeometry(geom.Get(), brush.Get());
			}
		}
		else
		{
			continue;
		}
	}

	// 본의 원점에 + 라인과 이름 표시
	if (m_skeleton)
	{
		auto& bones = m_skeleton->getBones();
		//m_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red, 1.0f));
		for (size_t i = 0; i < bones.size(); ++i) 
		{
			spine::Bone& bone = *bones[i];
			D2D1::Matrix3x2F boneWorldMatrix =
				D2D1::Matrix3x2F::Scale(bone.getScaleX(), bone.getScaleY()) *
				D2D1::Matrix3x2F::Rotation(bone.getWorldRotationX()) *
				D2D1::Matrix3x2F::Translation(bone.getWorldX(), bone.getWorldY());
		}
	}
}

D2D1_RECT_F SpineRenderer::MakeSrcRect(spine::AtlasRegion* atlasRegion, bool rotated, float srcW, float srcH)
{
	D2D1_RECT_F srcRect;
	srcRect.left = (float)atlasRegion->x;
	srcRect.top = (float)atlasRegion->y;
	srcRect.right = srcRect.left + srcW;
	srcRect.bottom = srcRect.top + srcH;
	return srcRect;
}

void SpineRenderer::ComputeRegionPlacement(
	spine::RegionAttachment* regionAtt,
	spine::AtlasRegion* atlasRegion,
	float srcW, float srcH, bool rotated,
	float& outDestW, float& outDestH,
	float& outAttachX, float& outAttachY,
	float& outAttachRot)
{
	// 원본 크기(트리밍 이전)
	float origW = (float)atlasRegion->originalWidth;
	float origH = (float)atlasRegion->originalHeight;
	if (origW <= 0.0f) origW = rotated ? srcH : srcW;
	if (origH <= 0.0f) origH = rotated ? srcW : srcH;

	// 회전 전 기준의 실제 잘린 영역 크기
	float regionWUnrot = rotated ? srcH : srcW;
	float regionHUnrot = rotated ? srcW : srcH;

	// 스케일 팩터(트리밍/스케일 반영)
	float scaleX = (origW > 0.0f) ? (regionWUnrot / origW) : 1.0f;
	float scaleY = (origH > 0.0f) ? (regionHUnrot / origH) : 1.0f;

	// 오프셋 스케일링 및 회전 보정
	float offX = (float)atlasRegion->offsetX * scaleX;
	float offY = (float)atlasRegion->offsetY * scaleY;
	float offsetX = rotated ? offY : offX;
	float offsetY = rotated ? offX : offY;

	outDestW = origW * scaleX;
	outDestH = origH * scaleY;
	outAttachX = regionAtt->getX() + offsetX;
	outAttachY = regionAtt->getY() + offsetY;
	outAttachRot = regionAtt->getRotation() + (rotated ? -90.0f : 0.0f);
}

// --- 키보드 입력 처리 ---
void SpineRenderer::HandleKeyInput(int keyCode) 
{
	std::cout << "Key pressed: " << keyCode << std::endl;
	// 1~9: 해당 인덱스 애니메이션 재생
	if (keyCode >= '1' && keyCode <= '9') 
	{
		int idx = keyCode - '1';
		if (idx >= 0 && idx < (int)m_animationList.size()) 
		{
			SetAnimation(m_animationList[idx]);
		}
		return;
	}
	switch (keyCode) {
	case '0': // 0번 키: 애니메이션 없이 기본 포즈
		if (m_state) m_state->clearTracks();
		if (m_skeleton) m_skeleton->setToSetupPose();
		SetAnimationTime(0.0f);
		break;
	case VK_LEFT: // 왼쪽 화살표: 이전 애니메이션
		SetPreviousAnimation();
		break;
	case VK_RIGHT: // 오른쪽 화살표: 다음 애니메이션
		SetNextAnimation();
		break;
	default:
		std::cout << "Unhandled key: " << keyCode << std::endl;
		break;
	}
}

void SpineRenderer::SetNextAnimation() 
{
	if (m_animationList.empty()) return;

	m_currentAnimationIndex = (m_currentAnimationIndex + 1) % m_animationList.size();
	std::string newAnimation = m_animationList[m_currentAnimationIndex];
	SetAnimation(newAnimation);

	std::cout << "Next animation: " << newAnimation << " (Index: " << m_currentAnimationIndex << ")" << std::endl;
}

void SpineRenderer::SetPreviousAnimation() {
	if (m_animationList.empty()) return;

	m_currentAnimationIndex = (m_currentAnimationIndex - 1 + m_animationList.size()) % m_animationList.size();
	std::string newAnimation = m_animationList[m_currentAnimationIndex];
	SetAnimation(newAnimation);

	std::cout << "Previous animation: " << newAnimation << " (Index: " << m_currentAnimationIndex << ")" << std::endl;
}


void SpineRenderer::LoadSpine(const std::wstring& atlasPath, const std::wstring& jsonPath)
{
	std::wstring atlasFilePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + atlasPath); // 파일 이름만 저장
	std::wstring jsonFilePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + jsonPath); // 파일 이름만 저장
	std::string atlasFileStrPath = StringHelper::wstring_to_string(atlasFilePath);
	std::string jsonFileStrPath = StringHelper::wstring_to_string(jsonFilePath);

	m_atlas.reset();
	m_atlas = std::make_unique<spine::Atlas>(atlasFileStrPath.c_str(), m_textureLoader.get());
	spine::SkeletonJson json(m_atlas.get());

	m_skeletonData.reset(json.readSkeletonDataFile(jsonFileStrPath.c_str())); // 내부 new
	if (!m_skeletonData)
	{
		OutputDebugStringW(L"spineRenderer LoadSpine()  m_skeletonDataError!\n");
		return;
	}

	m_skeleton.reset(new spine::Skeleton(m_skeletonData.get()));
	m_stateData.reset(new spine::AnimationStateData(m_skeletonData.get()));
	m_state.reset(new  spine::AnimationState(m_stateData.get()));
	// 애니메이션 목록 채우기
	m_animationList.clear();
	for (int i = 0; i < m_skeletonData->getAnimations().size(); ++i) {
		m_animationList.push_back(m_skeletonData->getAnimations()[i]->getName().buffer());
	}

	m_initialized = true;
}

void SpineRenderer::ReleaseSpine()
{
	// spine-cpp 객체들 해제
	m_state.reset();
	m_stateData.reset();
	m_skeleton.reset();
	m_skeletonData.reset();
	m_atlas.reset();

	if (m_textureLoader)
		m_textureLoader->unload(nullptr); // 비트맵 해제)
}

void SpineRenderer::ReleaseDirect2D()
{
	m_textureLoader.reset();
}

