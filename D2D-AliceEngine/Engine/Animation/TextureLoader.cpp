#include "pch.h"
#include "TextureLoader.h"
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

/*
from_json�� JSON �����͸� C++ ��ü(Sprite)�� ������ �����ϴ� ���� �Լ�
nlohmann::json::get<T>() ȣ�� �� �ڵ����� ����
�ݴ�� JSON���� ����ȭ�Ϸ��� to_json(json&, const Sprite&)�� �ʿ�
*/

void TextureLoader::LoadSpriteSheetAndAnimationClip(const std::string& filePath, AnimationClip& clip, SpriteSheet& SpriteSheet)
{
	LoadSpriteSheet(filePath, SpriteSheet);
	LoadAnimationClip(filePath, clip, SpriteSheet);
}

void TextureLoader::LoadSpriteSheet(const std::string& filePath, SpriteSheet& spriteSheet)
{
	std::ifstream inFile(filePath);
	if (inFile.is_open()) {
		json j;
		inFile >> j;
		inFile.close();
		// JSON ��ü���� ������ ����
		spriteSheet.texture = j["texture"];
		spriteSheet.textureWidth = j["textureWidth"];
		spriteSheet.textureHeight = j["textureHeight"];
		spriteSheet.sprites = j["sprites"].get<std::vector<SpriteInfo>>();

		// ��������Ʈ �̸����� �ε��� �� ����
		for (int i = 0; i < spriteSheet.sprites.size(); ++i) {
			spriteSheet.spriteIndexMap[spriteSheet.sprites[i].name] = i;
		}
	}
	else {
		throw std::runtime_error("Failed to open animation clip file: " + filePath);
	}
}

void TextureLoader::LoadAnimationClip(const std::string& filePath, AnimationClip& clip, const SpriteSheet& SpriteSheet)
{
	std::ifstream inFile(filePath);
	if (inFile.is_open()) {
		json j;
		inFile >> j;
		inFile.close();
		// JSON ��ü���� ������ ����
		clip.clipName = j["clipName"];
		clip.texturePath = j["texturePath"];
		clip.loop = j["loop"];
		clip.duration = j["duration"];
		clip.frames = j["frames"].get<std::vector<FrameInfo>>();
		clip.events = j["events"].get<std::vector<EventInfo>>();

		// �������� sprite �̸��� �ε����� ��ȯ
		for (auto& frame : clip.frames) {
			auto it = SpriteSheet.spriteIndexMap.find(frame.sprite);
			if (it != SpriteSheet.spriteIndexMap.end()) {
				frame.spriteSheetIndex = it->second; // sprite �̸����� �ε��� ã��
			}
			else {
				throw std::runtime_error("Sprite not found in sprite sheet: " + frame.sprite);
			}
		}
	}
	else {
		throw std::runtime_error("Failed to open animation clip file: " + filePath);
	}
}
// SpriteInfo
void from_json(const nlohmann::json& j, SpriteInfo& sprite)
{
	j.at("name").get_to(sprite.name);
	j.at("x").get_to(sprite.x);
	j.at("y").get_to(sprite.y);
	j.at("width").get_to(sprite.width);
	j.at("height").get_to(sprite.height);
	j.at("pivotX").get_to(sprite.pivotX);
	j.at("pivotY").get_to(sprite.pivotY);
}
void to_json(nlohmann::json& j, const SpriteInfo& sprite)
{
	j["name"] = sprite.name;
	j["x"] = sprite.x;
	j["y"] = sprite.y;
	j["width"] = sprite.width;
	j["height"] = sprite.height;
	j["pivotX"] = sprite.pivotX;
	j["pivotY"] = sprite.pivotY;
}

// FrameInfo
void from_json(const nlohmann::json& j, FrameInfo& frame)
{
	j.at("sprite").get_to(frame.sprite);
	j.at("time").get_to(frame.duration);
}
void to_json(nlohmann::json& j, const FrameInfo& frame)
{
	j["sprite"] = frame.sprite;
	j["spriteSheetIndex"] = frame.spriteSheetIndex;
	j["duration"] = frame.duration;
}

// AnimationClip
void from_json(const nlohmann::json& j, AnimationClip& clip)
{
	j.at("clipName").get_to(clip.clipName);
	j.at("texturePath").get_to(clip.texturePath);
	j.at("loop").get_to(clip.loop);
	j.at("duration").get_to(clip.duration);
	j.at("frames").get_to(clip.frames);
	j.at("events").get_to(clip.events);
}
void to_json(nlohmann::json& j, const AnimationClip& clip)
{
	j["clipName"] = clip.clipName;
	j["texturePath"] = clip.texturePath;
	j["loop"] = clip.loop;
	j["duration"] = clip.duration;
	j["frames"] = clip.frames;
}

// SpriteSheet
void from_json(const nlohmann::json& j, SpriteSheet& sheet)
{
	j.at("texture").get_to(sheet.texture);
	j.at("textureWidth").get_to(sheet.textureWidth);
	j.at("textureHeight").get_to(sheet.textureHeight);
	j.at("sprites").get_to(sheet.sprites);
	// spriteIndexMap�� �ε� �� ���� ó��
}
void to_json(nlohmann::json& j, const SpriteSheet& sheet)
{
	j["texture"] = sheet.texture;
	j["textureWidth"] = sheet.textureWidth;
	j["textureHeight"] = sheet.textureHeight;
	j["sprites"] = sheet.sprites;
	// spriteIndexMap�� ���� ����ȭ���� ����
}

// SpriteSheet
void from_json(const nlohmann::json& j, EventInfo& event)
{
	j.at("function").get_to(event.function);
	j.at("parameter").get_to(event.parameter);
	j.at("time").get_to(event.time);
}
void to_json(nlohmann::json& j, const EventInfo& event)
{
	j["function"] = event.function;
	j["parameter"] = event.parameter;
	j["time"] = event.time;
}
