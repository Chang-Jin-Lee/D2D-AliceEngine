#include "pch.h"
#include "Animator.h"
#include <Math/Transform.h>
#include <Animation/TextureLoader.h>
#include <Component/SpriteRenderer.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Manager/SceneManager.h>
#include <Manager/PackageResourceManager.h>
#include <Helpers/StringHelper.h>
#include <Helpers/FileHelper.h>
#include <Math/TColor.h>

Animator::Animator()
{
	sheet = std::make_unique<SpriteSheet>();
}

Animator::~Animator()
{

}

void Animator::Initialize()
{
	__super::Initialize();

	UpdateTaskManager::GetInstance().Enque(
		weak_from_this(),
		Define::ETickingGroup::TG_PostPhysics,
		[weak = weak_from_this()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			sp->Update(dt);
		}
	}
	);
}

void Animator::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	if (!bPlay) return;	// �÷��� ������ ���� �÷���
	if (IsEnd() && !bLoopping) return; // �������� ������ �ݺ����� ���� �Ǵ�
	if (curAnimationClip != nextAnimationClip)
	{
		curAnimationClip = nextAnimationClip;
		return;
	}

	m_accumTime += deltaSeconds;
	if (m_accumTime >= animationClips[curAnimationClip]->frames[m_curFrame].duration)
	{
		m_accumTime = animationClips[curAnimationClip]->frames[m_curFrame].duration;
		m_curFrame = (m_curFrame + 1) % animationClips[curAnimationClip]->frames.size();
	}
}

void Animator::Release()
{
	__super::Release();
}

void Animator::Render()
{
	__super::Render();
	if (sheet.get() == nullptr) return;
	if (m_bitmap == nullptr) return;
	if (curAnimationClip != nextAnimationClip) return;
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	Camera* camera = SceneManager::GetCamera();
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize(); // ��Ʈ�� ũ�� �� �ǹ�
	auto& sprite = sheet.get()->sprites[animationClips[curAnimationClip]->frames[m_curFrame].spriteSheetIndex];

	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F world = GetTransform()->ToMatrix();
	D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();

	if (D2DRenderManager::GetInstance().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity;
	}

	// ���� �ǹ� ���� ���� ��ȯ, ī�޶� ����� ����
	cameraInv.Invert();
	view = view * world * cameraInv;

	// Unity ��ǥ��� ��ȯ �߰�
	if (D2DRenderManager::GetInstance().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
	}

	// ���� ��ȯ ��Ʈ�� ������ ���� �׸��� (Src ��ü ���)
	float ScaleX = bmpSize.width / sheet.get()->textureWidth;
	float ScaleY = bmpSize.height / sheet.get()->textureHeight;
	float x = sprite.x * ScaleX;
	float y = bmpSize.height - sprite.y * ScaleY;
	float width = sprite.width * ScaleX;
	float height = sprite.height * ScaleY;
	D2D1_RECT_F SrcRect = { x, y - height, x + width, y };
	D2D1_RECT_F destRect = { -width * sprite.pivotX, -height * sprite.pivotY,  -width * sprite.pivotX + width,  -height * sprite.pivotY + height };

	if (bFlip)
	{
		D2D1::Matrix3x2F flipX =
			D2D1::Matrix3x2F::Scale(-1.f, 1.f);
		view = flipX * view;
	}
	context->SetTransform(view);
	context->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);
	D2DRenderManager::GetInstance().DrawDebugBox(-10, -10, 10, 10, 0, 0, 0, 255);
}

void Animator::PlayAnimation(std::weak_ptr<SpriteSheet> sheet, std::weak_ptr<AnimationClip> clip)
{

}

void Animator::LoadSpriteSheet(const std::string& filePath)
{
	if (sheet.get())
	{
		TextureLoader::LoadSpriteSheet(StringHelper::wstring_to_string(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + StringHelper::string_to_wstring(filePath))), *sheet.get());
		const std::wstring path = StringHelper::string_to_wstring(StringHelper::wstring_to_string(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + StringHelper::string_to_wstring(sheet.get()->texture))));
		m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(path.c_str());
	}
}

void Animator::LoadAnimationClip(const std::string& filePath)
{
	if (sheet.get())
	{
		std::unique_ptr<AnimationClip> clip = std::make_unique<AnimationClip>();
		TextureLoader::LoadAnimationClip(StringHelper::wstring_to_string(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + StringHelper::string_to_wstring(filePath))), *clip.get(), *sheet.get());
		animationClips.emplace(clip->clipName, std::move(clip));
	}
}

void Animator::Play()
{
	if (!CheckAnimationClip()) return;
	bPlay = true;
}

void Animator::Stop()
{
	if (!CheckAnimationClip()) return;
	m_curFrame = 0;
	bPlay = false;
}

void Animator::PlayAtFrame(const size_t& _frame)
{
	if (!CheckAnimationClip()) return;
	m_curFrame = _frame;
	bPlay = true;
}

void Animator::PlayAtStart()
{
	if (!CheckAnimationClip()) return;
	m_curFrame = 0;
	bPlay = true;
}

void Animator::StopAndResetFrame()
{
	if (!CheckAnimationClip()) return;
	bPlay = false;
}

bool Animator::IsEnd()
{
	if (!CheckAnimationClip()) return false;
	return m_curFrame >= animationClips[curAnimationClip]->frames.size() - 1;
}

bool Animator::CheckAnimationClip()
{
	return curAnimationClip.empty() == false;
}