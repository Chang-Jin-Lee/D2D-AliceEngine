#pragma once
#include <spine/spine.h>
#include <d2d1_1.h>
#include <wincodec.h>
#include <iostream>
#include <string>

namespace spine
{
	class D2DSpine2DTextureLoader : public TextureLoader
	{
	public:
		D2DSpine2DTextureLoader(ID2D1Bitmap1* _bitmap) : bitmap(_bitmap) {}
		~D2DSpine2DTextureLoader() {}

		void load(AtlasPage& page, const String& path) override;
		void unload(void* texture) override;

	private:
		ID2D1Bitmap1* bitmap = nullptr;
	};
}
