#pragma once
#include <spine/TextureLoader.h>
#include <spine/Atlas.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <wincodec.h>
#include <iostream>
#include <string>

class D2DSpine2DTextureLoader : public spine::TextureLoader 
{
public:
	D2DSpine2DTextureLoader(ID2D1Bitmap1* _bitmap): bitmap(_bitmap) {}
	~D2DSpine2DTextureLoader() {}

	void load(spine::AtlasPage& page, const spine::String& path) override {
		if (bitmap == nullptr) return;
		// path�� spine::String�̹Ƿ� std::wstring���� ��ȯ �ʿ�
		std::string utf8Path(path.buffer());
		std::wstring wpath(utf8Path.begin(), utf8Path.end()); // �ܼ� ��ȯ(ASCII�� ����)
		page.setRendererObject(bitmap);

		if (bitmap) {
			D2D1_SIZE_F size = bitmap->GetSize();
			page.width = static_cast<int>(size.width);
			page.height = static_cast<int>(size.height);
		}
	}

	void unload(void* texture) override {
		if (texture) {
			ID2D1Bitmap1* bitmap = static_cast<ID2D1Bitmap1*>(texture);
			bitmap->Release();
		}
	}

private:
	ID2D1Bitmap1* bitmap = nullptr;
};