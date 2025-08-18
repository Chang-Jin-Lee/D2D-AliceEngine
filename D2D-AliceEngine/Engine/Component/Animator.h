#pragma once
#include <Component/RenderComponent.h>
#include <unordered_map>
#include <memory>

/*
* @briefs :
	Sprite Animation을 실행하기 위한 Animator입니다.
	비트맵을 json에 명세되어 있는 크기로 잘라서 그립니다.
	연습을 위해 Bitmap을 새로 만들어 Render 함수를 다시 만들어 보았습니다.
	더 쉽게 만들고 싶다면 Render() 함수를 빼고 SpriteRenderer*를 선언하고 LoadData(L"") 함수를 사용해야합니다.
* @details :
	// AnimatorInstance를 상속받고 사용할 MyAnimatorInstance를 새로 만들 경우의 사용법은 다음과 같습니다.

	// 1. AnimatorController 정보를 불러옵니다.
	AnimatorController::LoadAnimatorController(L"Zero/Zero_AnimController.json", animController);
	// 2. 상속받아 새로 만든 PlayerAnimatorInstance를 컴포넌트로 추가합니다.
	animInstance = m_owner->AddComponent<PlayerAnimatorInstance>();
	animInstance->m_layer = 3;
	animInstance->SetAnimatorController(&animController);

	// FSM - state를 사용하는 경우의 사용법은 다음과 같습니다. 
	1. SpriteSheet와 AnimationClip 오브젝트를 생성합니다. 
	(예시에서는 std::shared_ptr을 사용했지만 관리를 잘할 자신이 있다면 rawPointer로도 가능합니다.)
Texture = std::make_shared<SpriteSheet>();
idle = std::make_shared<AnimationClip>();
  
	2. Animator의 static 함수로 해당하는 경로에서 json 파일을 읽어옵니다.
	(경로는 Resource/ 의 하위 디렉토리)
Animator::LoadSpriteSheet("ken_sprites.json", Texture);
Animator::LoadAnimationClip("ken_idle_anim.json", idle, Texture);

	3. Animator 컴포넌트를 추가하고 클립을 설정한 다음 Texture를 Load해주고 그 이후 기능을 사용합니다.
m_animator = m_owner->AddComponent<Animator>();
m_animator->SetAnimationClip(idle.get());
m_animator->LoadSpriteRenderer(Texture);
	4. 기능
m_animator->SetLooping(true);
m_animator->Play();

* 
*/

struct AnimationClip;
struct SpriteSheet;
class SpriteRenderer;
struct ID2D1Bitmap1;
class Animator : public RenderComponent
{
public:
	Animator();
	~Animator();

	virtual void Initialize() override;
	virtual void Update(const float& deltaSeconds) override;
	virtual void Release() override;
	virtual void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	virtual float GetSpriteSizeX();
	virtual float GetSpriteSizeY();

	// 애니메이션 사전 준비 함수
	void PlayAnimation(std::weak_ptr<SpriteSheet> sheet, std::weak_ptr<AnimationClip> clip);
	void LoadSpriteSheet(const std::wstring& filePath);
	void LoadAnimationClip(const std::wstring& filePath);

	// 애니메이션 실행, 중지, 변경 등의 함수
	void Play();
	void Stop();
	void PlayAtFrame(const size_t& _frame);
	void PlayAtStart();
	void StopAndResetFrame();
	bool IsEnd();
	void SetLooping(const bool& loop) { bLoopping = loop; }
	bool IsLooping() { return bLoopping; }
  // Visibility (Unreal-style): when false, Render skips drawing
  void SetVisible(bool visible) { m_visible = visible; }
  bool IsVisible() const { return m_visible; }

	float time = -1;
	std::string curAnimationClip;
	std::string nextAnimationClip;
	std::unique_ptr<SpriteSheet> sheet;
	std::unordered_map<std::string, std::unique_ptr<AnimationClip>> animationClips;
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
	size_t m_curTime = 0;
	size_t m_curFrame = 0;
	bool bPlay = false;
	bool bLoopping = false;
	float m_accumTime = 0;
  bool m_visible = true;
};