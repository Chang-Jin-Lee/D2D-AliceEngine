#include "pch.h"
#include "Animator.h"
#include <Math/Transform.h>
#include <Animation/SpriteAnimationTextureLoader.h>
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
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PostPhysics);
}

void Animator::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	if (curAnimationClip != nextAnimationClip)
	{
		curAnimationClip = nextAnimationClip;
		m_curFrame = 0;
		return;
	}
	if (!bPlay) return;	// 플레이 가능할 때만 플레이
	//if (IsEnd()) return; // 프레임의 끝에서 반복할지 말지 판단
	//if (IsEnd() && !bLoopping) return; // 프레임의 끝에서 반복할지 말지 판단

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
	if (sheet.get() == nullptr) return;
	if (m_bitmap == nullptr) return;
    if (!m_visible) return;
	if (curAnimationClip != nextAnimationClip) return;
	auto& sprite = sheet.get()->sprites[animationClips[curAnimationClip]->frames[m_curFrame].spriteSheetIndex];

	// 최종 변환 비트맵 원점에 맞춰 그리기 (Src 전체 사용)
	float ScaleX = GetBitmapSizeX() / sheet.get()->textureWidth;
	float ScaleY = GetBitmapSizeY() / sheet.get()->textureHeight;
	float x = sprite.x * ScaleX;
	float y = sprite.y * ScaleY;
	float width = sprite.width * ScaleX;
	float height = sprite.height * ScaleY;
	D2D1_RECT_F SrcRect = { x, y - height, x + width, y };
	D2D1_RECT_F destRect = { -width * sprite.pivotX, -height * sprite.pivotY,  -width * sprite.pivotX + width,  -height * sprite.pivotY + height };

	__super::Render();

	D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);
	if (D2DRenderManager::GetInstance().bRenderedBoxRect)
		D2DRenderManager::GetInstance().DrawDebugBox(-10, -10, 10, 10, 0, 0, 0, 255);
}

float Animator::GetBitmapSizeX()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.width);
}

float Animator::GetBitmapSizeY()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.height);
}

float Animator::GetSpriteSizeX()
{
	return sheet->sprites[0].width;
}

float Animator::GetSpriteSizeY()
{
	return sheet->sprites[0].height;
}

void Animator::PlayAnimation(std::weak_ptr<SpriteSheet> sheet, std::weak_ptr<AnimationClip> clip)
{

}

void Animator::LoadSpriteSheet(const std::wstring& filePath)
{
	if (sheet.get())
	{
		SpriteAnimationTextureLoader::LoadSpriteSheet(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + filePath), *sheet.get());
		const std::wstring path = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + StringHelper::string_to_wstring(sheet.get()->texture));
		m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(path.c_str());
	}
}

void Animator::LoadAnimationClip(const std::wstring& filePath)
{
	if (sheet.get())
	{
		std::unique_ptr<AnimationClip> clip = std::make_unique<AnimationClip>();
		SpriteAnimationTextureLoader::LoadAnimationClip(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + filePath), *clip.get(), *sheet.get());
		animationClips.emplace(clip->clipName, std::move(clip));
	}
}

void Animator::Play()
{
	bPlay = true;
}

void Animator::Stop()
{
	m_curFrame = 0;
	bPlay = false;
}

void Animator::PlayAtFrame(const size_t& _frame)
{
	m_curFrame = _frame;
	bPlay = true;
}

void Animator::PlayAtStart()
{
	m_curFrame = 0;
	bPlay = true;
}

void Animator::StopAndResetFrame()
{
	bPlay = false;
}

bool Animator::IsEnd()
{
	return m_curFrame >= animationClips[curAnimationClip]->frames.size() - 1;
}
