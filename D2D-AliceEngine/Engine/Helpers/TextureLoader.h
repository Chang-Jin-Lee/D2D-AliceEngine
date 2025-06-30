#pragma once
/*
* @briefs : �ؽ�ó�� �ε��ϱ� ���� Helper�Լ����� ���ִ� Ŭ���� �Դϴ�.
*/
#include <Extension/json.hpp>
using json = nlohmann::json;

struct SpriteInfo
{
	std::string name; // sprite �̸�
	float x, y; // sprite x ��ǥ
	float width, height; // sprite �ʺ�
	float pivotX, pivotY; // sprite �ǹ� ��ǥ (�߽���)
};

struct FrameInfo	// �ִϸ��̼��� �������� sprite�Ӹ� �ƴ϶� �ٸ� ������ ���Ե� �� �ִ�.
{
	std::string sprite; // sprite �̸�
	int spriteSheetIndex; // sprite �ε���
	float duration; // ������ ���� �ð�	
};

struct AnimationClip {
	std::string clipName; // �ִϸ��̼� Ŭ�� �̸�
	std::string texturePath;// �ִϸ��̼� Ŭ���� ���Ǵ� �ؽ�ó ���
	bool loop; // �ִϸ��̼��� �ݺ��Ǵ��� ����
	float duration; // ��ü
	std::vector<FrameInfo> frames; // ������ ������
};

struct SpriteSheet {
	std::string texture; // ��������Ʈ ��Ʈ �̸�
	float textureWidth, textureHeight; // ��������Ʈ ��Ʈ�� �ʺ�� ����
	std::vector<SpriteInfo> sprites; // Sprite ���
	std::unordered_map<std::string, int> spriteIndexMap; // �̸����� Sprite�� ã�� ���� �ε��� ��
};

class TextureLoader
{
public:
	static void LoadSpriteSheetAndAnimationClip(const std::string& filePath, AnimationClip& clip, SpriteSheet& SpriteSheet);
	static void LoadSpriteSheet(const std::string& filePath, SpriteSheet& spriteSheet);
	static void LoadAnimationClip(const std::string& filePath, AnimationClip& clip, const SpriteSheet& spriteSheet);
};

/*
* json���� ����� �Լ��̱� ������ �������� �����ؾ���
*/

// SpriteInfo
void from_json(const nlohmann::json& j, SpriteInfo& sprite);
void to_json(nlohmann::json& j, const SpriteInfo& sprite);

// FrameInfo
void from_json(const nlohmann::json& j, FrameInfo& frame);
void to_json(nlohmann::json& j, const FrameInfo& frame);

// AnimationClip
void from_json(const nlohmann::json& j, AnimationClip& clip);
void to_json(nlohmann::json& j, const AnimationClip& clip);

// SpriteSheet
void from_json(const nlohmann::json& j, SpriteSheet& sheet);
void to_json(nlohmann::json& j, const SpriteSheet& sheet);
