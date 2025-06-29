#include <spine/Spine2DTextureLoader.h>
#include <spine/TextureLoader.h>
#include <spine/Atlas.h>

namespace spine
{
	void D2DSpine2DTextureLoader::load(spine::AtlasPage& page, const spine::String& path)
	{
		if (bitmap == nullptr) return;
		// path는 spine::String이므로 std::wstring으로 변환 필요
		std::string utf8Path(path.buffer());
		std::wstring wpath(utf8Path.begin(), utf8Path.end()); // 단순 변환(ASCII만 안전)
		page.setRendererObject(bitmap);

		if (bitmap) {
			D2D1_SIZE_F size = bitmap->GetSize();
			page.width = static_cast<int>(size.width);
			page.height = static_cast<int>(size.height);
		}
	}

	void D2DSpine2DTextureLoader::unload(void* texture)
	{
		if (texture) {
			ID2D1Bitmap1* bitmap = static_cast<ID2D1Bitmap1*>(texture);
			bitmap->Release();
		}
	}
}