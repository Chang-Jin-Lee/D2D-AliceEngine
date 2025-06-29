#include <spine/Spine2DTextureLoader.h>
#include <spine/TextureLoader.h>
#include <spine/Atlas.h>

namespace spine
{
	void D2DSpine2DTextureLoader::load(spine::AtlasPage& page, const spine::String& path)
	{
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

	void D2DSpine2DTextureLoader::unload(void* texture)
	{
		if (texture) {
			ID2D1Bitmap1* bitmap = static_cast<ID2D1Bitmap1*>(texture);
			bitmap->Release();
		}
	}
}