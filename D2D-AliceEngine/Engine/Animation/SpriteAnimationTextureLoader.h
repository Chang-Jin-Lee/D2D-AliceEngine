#pragma once
/*
* @briefs : 텍스처를 로드하기 위한 Helper함수들이 모여있는 클래스 입니다.
*/
#include <Extension/json.hpp>
using json = nlohmann::json;

struct SpriteInfo
{
	std::string name; // sprite 이름
	float x, y; // sprite x 좌표
	float width, height; // sprite 너비
	float pivotX, pivotY; // sprite 피벗 좌표 (중심점)
};

struct FrameInfo	// 애니메이션의 프레임은 sprite뿐만 아니라 다른 정보도 포함될 수 있다.
{
	std::string sprite; // sprite 이름
	int spriteSheetIndex; // sprite 인덱스
	float duration; // 프레임 지속 시간	
};

struct EventInfo {
	std::string function; // 호출할 함수 이름
	std::string parameter; // 문자열 파라미터
	float time; // 이벤트가 발생하는 시간 (초 단위)
};

struct AnimationClip {
	std::string clipName; // 애니메이션 클립 이름
	std::string texturePath;// 애니메이션 클립에 사용되는 텍스처 경로
	bool loop; // 애니메이션이 반복되는지 여부
	float duration; // 전체
	std::vector<FrameInfo> frames; // 프레임 데이터
	std::vector<EventInfo> events; // 이벤트 데이터
};

struct SpriteSheet {
	std::string texture; // 스프라이트 시트 이름
	float textureWidth, textureHeight; // 스프라이트 시트의 너비와 높이
	std::vector<SpriteInfo> sprites; // Sprite 목록
	std::unordered_map<std::string, int> spriteIndexMap; // 이름으로 Sprite를 찾기 위한 인덱스 맵
};

class SpriteAnimationTextureLoader
{
public:
	static void LoadSpriteSheetAndAnimationClip(const std::wstring& filePath, AnimationClip& clip, SpriteSheet& SpriteSheet);
	static void LoadSpriteSheet(const std::wstring& filePath, SpriteSheet& spriteSheet);
	static void LoadAnimationClip(const std::wstring& filePath, AnimationClip& clip, const SpriteSheet& spriteSheet);
};

/*
* json에서 사용할 함수이기 때문에 전역으로 선언해야함
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

// EventInfo
void from_json(const nlohmann::json& j, EventInfo& event);
void to_json(nlohmann::json& j, const EventInfo& event);
