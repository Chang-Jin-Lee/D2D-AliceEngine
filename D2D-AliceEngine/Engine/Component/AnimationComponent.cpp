#include "pch.h"
#include "AnimationComponent.h"
#include "SpriteRenderer.h"
#include "Manager/D2DRenderManager.h"
#include "Application.h"
#include <Helpers/FFmpegHelper.h>
#include <Core/Time.h>


void AnimationComponent::Initialize()
{
	__super::Initialize();
}

void AnimationComponent::Update()
{
	__super::Update();

	ReleaseFrames();
	UpdateFrames();
}

void AnimationComponent::UpdateFrames()
{
	if (bPlay)	// �÷��� ������ ���� �÷���
	{
		m_fFPSLastTime = Time::GetTotalTime() - m_fcountOneSecond;
		if (m_fFPSLastTime >= m_fFPSTime)	// 1/60 �ʿ� �� ����
		{
			if (m_maxClipSize != 0)
			{
				if (m_curClip + 1 >= m_maxClipSize)
				{
					m_bitmaps.clear();
				}
				m_curClip = (m_curClip + 1) % m_maxClipSize;
			}
			m_fcountOneSecond = Time::GetTotalTime();
		}
	}
}

void AnimationComponent::ReleaseFrames()
{
	// ���� ������ �ֺ��� ĳ�� ����
	size_t start = (m_curClip > cacheSize) ? m_curClip - cacheSize : 0;
	size_t end = min(m_bitmaps.size(), m_curClip + cacheSize);

	for (size_t i = 0; i < m_bitmaps.size(); ++i) {
		if (i < start || i > end) {
			m_bitmaps[i].Reset(); // ������� �ʴ� ������ ����
		}
	}
}

void AnimationComponent::LoadData(const std::wstring& path, const int& fps)
{
	files = FFmpegHelper::GetFramesFromVideo(L"Resource\\" + path, 24);
	if (files.empty()) return;

	m_fFPSTime = 1.0f / fps;
	m_maxClipSize = files.size();
	m_curClip = 0;
	m_bitmaps.clear();
}

void AnimationComponent::LoadFrame(size_t frameIndex) {
	ComPtr<ID2D1Bitmap1> bitmap;
	D2DRenderManager::Get().CreateBitmapFromFile(files[frameIndex].c_str(), &bitmap);
	if (m_bitmaps.size() <= frameIndex) 
	{
		m_bitmaps.resize(frameIndex + 1);
	}
	m_bitmaps[frameIndex] = bitmap;
}

void AnimationComponent::Release()
{
	__super::Release();
}

/*
* @briefs : ��ȯ����� �����Ͽ� ��Ʈ���� ���ϴ� ��ġ�� �׸��ϴ�.
* @details
*	ETransformType : ��ǥ�� ����
*/

void AnimationComponent::Render()
{
	if (files.empty()) return;
	if (m_bitmaps.size() <= m_curClip) 
	{
		// �ʿ��� �����Ӹ� �ε�
		LoadFrame(m_curClip);
	}
	if (m_bitmaps.empty()) return;

	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	Camera* camera = SceneManager::GetCamera();
	D2D1_SIZE_U bmpSize = m_bitmaps[m_curClip]->GetPixelSize(); // ��Ʈ�� ũ�� �� �ǹ�
	D2D1_POINT_2F pivotOffset = {
		bmpSize.width * m_pivot->x,
		bmpSize.height * m_pivot->y
	};
	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);
	D2D1::Matrix3x2F world = m_pTransform->ToMatrix();
	D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();

	if (D2DRenderManager::Get().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity;
	}

	// ���� �ǹ� ���� ���� ��ȯ, ī�޶� ����� ����
	cameraInv.Invert();
	view = view * world * cameraInv;

	// Unity ��ǥ��� ��ȯ �߰�
	if (D2DRenderManager::Get().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
	}

	// ���� ��ȯ ��Ʈ�� ������ ���� �׸��� (Src ��ü ���)
	context->SetTransform(view);
	context->DrawBitmap(m_bitmaps[m_curClip].Get());
}
