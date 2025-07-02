#pragma once
#include <Component/RenderComponent.h>

/*
* @briefs :
*	Sprite Animation�� �����ϱ� ���� Animator�Դϴ�.
*	��Ʈ���� json�� ���Ǿ� �ִ� ũ��� �߶� �׸��ϴ�.
* @details :

	������ ������ �����ϴ�. 
	1. SpriteSheet�� AnimationClip ������Ʈ�� �����մϴ�. 
	(���ÿ����� std::shared_ptr�� ��������� ������ ���� �ڽ��� �ִٸ� rawPointer�ε� �����մϴ�.)
Texture = std::make_shared<SpriteSheet>();
idle = std::make_shared<AnimationClip>();
  
	2. Animator�� static �Լ��� �ش��ϴ� ��ο��� json ������ �о�ɴϴ�.
	(��δ� Resource/ �� ���� ���丮)
Animator::LoadSpriteSheet("ken_sprites.json", Texture);
Animator::LoadAnimationClip("ken_idle_anim.json", idle, Texture);

	3. Animator ������Ʈ�� �߰��ϰ� Ŭ���� ������ ���� Texture�� Load���ְ� �� ���� ����� ����մϴ�.
m_animator = m_owner->AddComponent<Animator>();
m_animator->SetAnimationClip(idle.get());
m_animator->LoadSpriteRenderer(Texture);
	4. ���
m_animator->SetLooping(true);
m_animator->Play();

* 
*/

struct SpriteRenderer;
struct AnimationClip;
struct SpriteSheet;
class ID2D1Bitmap1;
class Animator : public RenderComponent
{
public:
	Animator();
	~Animator();

	virtual void Initialize() override;
	virtual void Update(const float& deltaSeconds) override;
	virtual void Release() override;
	virtual void Render() override;

	// �ִϸ��̼� ���� �غ� �Լ�
	void SetAnimationClip(AnimationClip* clip);
	void PlayAnimation(std::weak_ptr<SpriteSheet> sheet, std::weak_ptr<AnimationClip> clip);
	void LoadSpriteData(const std::string& filePath);
	static void LoadSpriteSheet(const std::string& filePath, std::weak_ptr<SpriteSheet> spriteSheet);
	static void LoadAnimationClip(const std::string& filePath, std::weak_ptr<AnimationClip> clip, const std::weak_ptr<SpriteSheet> spriteSheet);
	void LoadSpriteRenderer(std::weak_ptr<SpriteSheet> sheet);

	// �ִϸ��̼� ����, ����, ���� ���� �Լ�
	void Play();
	void Stop();
	void PlayAtFrame(const size_t& _frame);
	void PlayAtStart();
	void StopAndResetFrame();
	bool IsEnd();
	void SetLooping(const bool& loop) { bLoopping = loop; }
	bool IsLooping() { return bLoopping; }
	void ChangeAnimation(const std::string& _key, bool _loop = true);
	void SetFlip(const bool& _flip) { bFlip = _flip; }

	float time = -1;
	AnimationClip* pAnimationClip;
	std::weak_ptr<SpriteSheet> sheet;
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
	size_t m_curTime = 0;
	size_t m_curFrame = 0;
	bool bFlip = false;
	bool bPlay = false;
	bool bLoopping = false;
	float m_accumTime = 0;
};