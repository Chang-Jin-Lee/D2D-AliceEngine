#pragma once
#include <spine/spine.h>
#include <spine/TextureLoader.h>
#include <d2d1_1.h>
#include <string>

namespace spine
{
	class Spine2DTextureLoader : public TextureLoader
	{
	public:
		Spine2DTextureLoader(ID2D1Bitmap1* _bitmap) : bitmap(_bitmap) {}
		~Spine2DTextureLoader() {}

		void load(AtlasPage& page, const String& path) override {
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
}