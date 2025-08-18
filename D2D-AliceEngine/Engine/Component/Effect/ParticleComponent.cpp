#include "pch.h"
#include "ParticleComponent.h"
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <System/RenderSystem.h>
#include <Manager/SceneManager.h>
#include <Core/Input.h>
#include <Manager/UpdateTaskManager.h>
#include <Helpers/CoordHelper.h>
#include <Helpers/FileHelper.h>

// WorldSpace 입력 좌표를 내부 시뮬레이션 좌표로 변환 (Y 반전 옵션)
FVector2 ParticleComponent::ToSimPos(const FVector2& in) const
{
    if (GetDrawType() == Define::EDrawType::WorldSpace && m_flipWorldYInput)
        return FVector2(in.x, -in.y);
    return in;
}


ParticleComponent::ParticleComponent()
{
    std::random_device rd;
    m_rng.seed(rd());
}

ParticleComponent::~ParticleComponent()
{
}

void ParticleComponent::Initialize()
{
    __super::Initialize();
    ensureParticleTexture();
    m_layer = 987654322; // 기본 레이어 (UI보다 뒤, 월드보다 위 조정 필요시 변경)
    REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_NewlySpawned);
}

void ParticleComponent::Release()
{
    __super::Release();
    m_particles.clear();
    m_particleBitmap.reset();
}

void ParticleComponent::Update(const float& deltaSeconds)
{
    __super::Update(deltaSeconds);

    // 자동 삭제 타이머
    if (m_selfDestructActive)
    {
        m_selfDestructTimer += deltaSeconds;
        if (m_selfDestructTimer >= m_selfDestructAfterSeconds)
        {
            if (Scene* world = SceneManager::GetInstance().GetWorld())
            {
                world->RemoveObject(GetOwner());
            }
            m_selfDestructActive = false;
            return;
        }
    }

    // 마우스 트레일
    if (m_mouseTrail)
    {
        m_trailAccumulator += deltaSeconds;
        const float spawnRate = 60.0f; // 초당 60개
        const float interval = 1.0f / spawnRate;
        while (m_trailAccumulator >= interval)
        {
            m_trailAccumulator -= interval;
            FVector2 mp = (GetDrawType() == Define::EDrawType::WorldSpace)
                ? ToSimPos(CoordHelper::ConvertD2DToUnity(Input::GetMousePosition()))
                : Input::GetMousePosition();

            emitBurstCommon(mp, 4,
                10.0f, 30.0f, // speed
                24.0f, 30.0f,  // size
                0.25f, 0.45f, // life
                D2D1::ColorF(1.0f, 1.0f, 0.8f, 0.9f),
                D2D1::ColorF(0.9f, 0.6f, 0.2f, 0.0f),
                2.0f, 0.0f);
        }
    }

    // 파티클 적분 업데이트
    for (auto it = m_particles.begin(); it != m_particles.end(); )
    {
        Particle& p = *it;
        p.life += deltaSeconds;
        if (p.life >= p.maxLife) { it = m_particles.erase(it); continue; }

        if (p.isOrbital)
        {
            // 오오라: 중심을 기준으로 공전하면서 회전
            p.orbitalAngleDeg += p.orbitalAngularSpeed * deltaSeconds;
            const float ang = p.orbitalAngleDeg * 3.14159265f / 180.0f;
            p.position.x = p.orbitalCenter.x + cosf(ang) * p.orbitalRadius;
            p.position.y = p.orbitalCenter.y + sinf(ang) * p.orbitalRadius;
            p.rotation += p.angularVelocity * deltaSeconds;
        }
        else
        {
            // 기본 물리 감쇠/중력
            p.velocity.x *= (1.0f / (1.0f + p.drag * deltaSeconds));
            p.velocity.y *= (1.0f / (1.0f + p.drag * deltaSeconds));
            p.velocity.y += p.gravity * deltaSeconds;
            p.position.x += p.velocity.x * deltaSeconds;
            p.position.y += p.velocity.y * deltaSeconds;
            p.rotation += p.angularVelocity * deltaSeconds;
        }
        ++it;
    }
}

void ParticleComponent::Render()
{
    ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
    ComPtr<ID2D1SpriteBatch> spriteBatch = D2DRenderManager::GetInstance().m_spriteBatch;
    if (!context || !spriteBatch) return;

    // 부모 클래스에서 좌표계 변환 설정
    __super::Render();

    ensureParticleTexture();
    if (!m_particleBitmap) return;

    // 블렌딩 모드 설정
    context->SetPrimitiveBlend(m_useAdditive ? D2D1_PRIMITIVE_BLEND_ADD : D2D1_PRIMITIVE_BLEND_SOURCE_OVER);

    // SpriteBatch가 있으면 최적, 없으면 개별 DrawBitmap 사용
    
    const bool useBatch = spriteBatch.Get() != nullptr;

    if (useBatch)
    {
        spriteBatch->Clear();
        for (const Particle& p : m_particles)
        {
            const float t = p.maxLife > 0.0f ? (p.life / p.maxLife) : 1.0f;
            const D2D1_COLOR_F c = lerpColor(p.colorStart, p.colorEnd, t);

            const float half = p.size * 0.5f;
            D2D1_RECT_F dest = D2D1::RectF(p.position.x - half, p.position.y - half, p.position.x + half, p.position.y + half);
            const D2D1_RECT_U srcU = D2D1::RectU(0, 0, m_srcWidth, m_srcHeight);
            const D2D1_MATRIX_3X2_F xform = D2D1::Matrix3x2F::Rotation(p.rotation, D2D1::Point2F(p.position.x, p.position.y));

            // AddSprites(signature): dest(F), src(U), colors, transforms, counts...
            spriteBatch->AddSprites(
                1,
                &dest,
                &srcU,
                &c,
                &xform,
                1, 1, 1, 1);
        }

        context->DrawSpriteBatch(
            spriteBatch.Get(),
            m_particleBitmap.get(),
            D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
            D2D1_SPRITE_OPTIONS_NONE);
    }
    else
    {
        for (const Particle& p : m_particles)
        {
            const float t = p.maxLife > 0.0f ? (p.life / p.maxLife) : 1.0f;
            const D2D1_COLOR_F c = lerpColor(p.colorStart, p.colorEnd, t);
            const float half = p.size * 0.5f;
            D2D1_RECT_F dest = D2D1::RectF(p.position.x - half, p.position.y - half, p.position.x + half, p.position.y + half);
            context->DrawBitmap(m_particleBitmap.get(), dest, c.a, D2D1_INTERPOLATION_MODE_LINEAR, nullptr);
        }
    }

    // 원복
    context->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
}

float ParticleComponent::GetBitmapSizeX() { return 128.0f; }
float ParticleComponent::GetBitmapSizeY() { return 128.0f; }

// 프리셋 구현 ---------------------------------------------------------------
void ParticleComponent::EmitExplosion(const FVector2& pos, int count)
{
    emitBurstCommon(ToSimPos(pos), count,
        200.0f, 520.0f,
        10.0f, 28.0f,
        0.45f, 0.85f,
        D2D1::ColorF(1.0f, 0.55f, 0.15f, 1.0f), // 주황
        D2D1::ColorF(0.9f, 0.2f, 0.0f, 0.0f),   // 붉은 꼬리
        4.0f,
        200.0f);

    // 쇼크웨이브 링
    spawnShockwaveRing(ToSimPos(pos), 20.0f, 8.0f, 0.5f, D2D1::ColorF(1.0f, 0.8f, 0.4f, 0.8f));
}

void ParticleComponent::EmitExplosionByColor(
	const FVector2& pos,
	int count,
	const D2D1::ColorF& colorA,
	const D2D1::ColorF& colorB)
{
	// 본체 + 꼬리 색을 외부에서 주입
	emitBurstCommon(
		ToSimPos(pos), count,
		200.0f, 520.0f,   // speedMin, speedMax
		34.0f, 56.0f,     // sizeMin, sizeMax
		0.45f, 0.85f,     // lifeMin, lifeMax
		colorA,           // 본체 색
		colorB,           // 꼬리 색
		4.0f,             // drag
		200.0f            // gravity
	);

	// 쇼크웨이브 링 (색은 기존 고정값 유지)
	spawnShockwaveRing( 
		ToSimPos(pos),
		30.0f,   // radius
		14.0f,    // thickness
		0.5f,    // life
		D2D1::ColorF(1.0f, 0.8f, 0.4f, 0.8f)
	);
}

void ParticleComponent::EmitImpact(const FVector2& pos, int count)
{
    // 중앙에서 방사형으로 강하게 퍼지는 튜닝
    emitBurstCommon(ToSimPos(pos), count,
        260.0f, 520.0f,   // 더 강한 반경 속도
        10.0f, 22.0f,
        0.28f, 0.55f,
        D2D1::ColorF(1.0f, 0.95f, 0.85f, 1.0f),
        D2D1::ColorF(1.0f, 0.5f, 0.2f, 0.0f),
        3.5f,
        0.0f);

    // 강한 방사 링 추가
    spawnShockwaveRing(ToSimPos(pos), 28.0f, 10.0f, 0.4f, D2D1::ColorF(1.0f, 0.9f, 0.6f, 0.9f));
}

void ParticleComponent::EmitImpactByColor(const FVector2& pos, int count, D2D1::ColorF colorA, D2D1::ColorF colorB)
{
	// 중앙에서 방사형으로 강하게 퍼지는 튜닝
	emitBurstCommon(ToSimPos(pos), count,
		260.0f, 620.0f,   // 더 강한 반경 속도
		14.0f, 22.0f,
		0.28f, 0.55f,
		colorA,
        colorB,
		3.5f,
		0.0f);

	// 강한 방사 링 추가
	spawnShockwaveRing(ToSimPos(pos), 28.0f, 10.0f, 0.4f, D2D1::ColorF(1.0f, 0.9f, 0.6f, 0.9f));
}

void ParticleComponent::EmitClickBurst(const FVector2& pos, bool rightClick)
{
    emitBurstCommon(ToSimPos(pos), rightClick ? 30 : 18,
        120.0f, rightClick ? 380.0f : 240.0f,
        10.0f, 18.0f,
        0.25f, 0.5f,
        rightClick ? D2D1::ColorF(0.3f, 1.0f, 1.0f, 1.0f) : D2D1::ColorF(1.0f, 0.9f, 0.6f, 1.0f),
        rightClick ? D2D1::ColorF(0.0f, 0.6f, 1.0f, 0.0f) : D2D1::ColorF(1.0f, 0.4f, 0.1f, 0.0f),
        3.0f, 0.0f);

    // 2초 후 자동 삭제
    m_selfDestructActive = true;
    m_selfDestructTimer = 0.0f;
    m_selfDestructAfterSeconds = 2.0f;
}

void ParticleComponent::ToggleMouseTrail(bool enabled)
{
    m_mouseTrail = enabled;
}

void ParticleComponent::EmitAura(const FVector2& center, float radius, int count)
{
    // 원형으로 배치된 파티클들이 중심을 기준으로 공전하며 회전
    for (int i = 0; i < count; ++i)
    {
        const float ang = (float)i / (float)count * 6.2831853f;
        Particle p;
        p.isOrbital = true;
        p.orbitalCenter = ToSimPos(center);
        p.orbitalRadius = radius + randRange(-6.0f, 6.0f);
        p.orbitalAngularSpeed = randRange(60.0f, 140.0f); // deg/sec
        p.orbitalAngleDeg = ang * 180.0f / 3.14159265f;

        p.position = FVector2(p.orbitalCenter.x + cosf(ang) * p.orbitalRadius, p.orbitalCenter.y + sinf(ang) * p.orbitalRadius);
        p.velocity = FVector2(0.0f, 0.0f);
        p.size = randRange(10.0f, 18.0f);
        p.rotation = randRange(0.0f, 360.0f);
        p.angularVelocity = randRange(-90.0f, 90.0f);
        p.life = 0.0f;
        p.maxLife = randRange(1.2f, 2.0f);
        p.drag = 0.8f;
        p.gravity = 0.0f;
        p.colorStart = D2D1::ColorF(0.6f, 0.9f, 1.0f, 0.85f);
        p.colorEnd = D2D1::ColorF(0.2f, 0.6f, 1.0f, 0.0f);
        m_particles.push_back(p);
    }
}

void ParticleComponent::EmitElectric(const FVector2& pos, int count)
{
    emitBurstCommon(ToSimPos(pos), count,
        220.0f, 480.0f,
        6.0f, 12.0f,
        0.25f, 0.55f,
        D2D1::ColorF(0.6f, 0.8f, 1.0f, 1.0f),
        D2D1::ColorF(0.2f, 0.6f, 1.0f, 0.0f),
        2.0f, 0.0f);
}

void ParticleComponent::EmitPortalSwirl(const FVector2& pos, int count)
{
    emitBurstCommon(ToSimPos(pos), count,
        90.0f, 220.0f,
        10.0f, 20.0f,
        0.8f, 1.2f,
        D2D1::ColorF(0.7f, 0.2f, 1.0f, 1.0f),
        D2D1::ColorF(0.2f, 0.1f, 0.5f, 0.0f),
        0.8f, 0.0f);
}

void ParticleComponent::EmitLocationIndicator(const FVector2& pos, int count, D2D1::ColorF colorA, D2D1::ColorF colorB)
{
    emitBurstCommon(ToSimPos(pos), 4,
        10.0f, 30.0f, // speed
        24.0f, 48.0f,  // size
        0.25f, 0.45f, // life
        colorA,
        colorB,
        2.0f, 0.0f);
}

// 커스텀 프리셋 오버로드 구현 ----------------------------------------------
void ParticleComponent::EmitExplosion(
    const FVector2& pos,
    int count,
    float speedMin, float speedMax,
    float sizeMin, float sizeMax,
    float lifeMin, float lifeMax,
    const D2D1_COLOR_F& colorA, const D2D1_COLOR_F& colorB,
    float drag, float gravity,
    bool shockwaveEnabled,
    float shockwaveRadius, float shockwaveThickness, float shockwaveLife,
    const D2D1_COLOR_F& shockwaveColor)
{
    emitBurstCommon(ToSimPos(pos), count,
        speedMin, speedMax,
        sizeMin, sizeMax,
        lifeMin, lifeMax,
        colorA, colorB,
        drag, gravity);

    if (shockwaveEnabled)
    {
        spawnShockwaveRing(ToSimPos(pos), shockwaveRadius, shockwaveThickness, shockwaveLife, shockwaveColor);
    }
}

void ParticleComponent::EmitImpact(
    const FVector2& pos,
    int count,
    float speedMin, float speedMax,
    float sizeMin, float sizeMax,
    float lifeMin, float lifeMax,
    const D2D1_COLOR_F& colorA, const D2D1_COLOR_F& colorB,
    float drag, float gravity,
    bool shockwaveEnabled,
    float shockwaveRadius, float shockwaveThickness, float shockwaveLife,
    const D2D1_COLOR_F& shockwaveColor)
{
    emitBurstCommon(ToSimPos(pos), count,
        speedMin, speedMax,
        sizeMin, sizeMax,
        lifeMin, lifeMax,
        colorA, colorB,
        drag, gravity);

    if (shockwaveEnabled)
    {
        spawnShockwaveRing(ToSimPos(pos), shockwaveRadius, shockwaveThickness, shockwaveLife, shockwaveColor);
    }
}

void ParticleComponent::EmitClickBurst(
    const FVector2& pos,
    int count,
    float speedMin, float speedMax,
    float sizeMin, float sizeMax,
    float lifeMin, float lifeMax,
    const D2D1_COLOR_F& colorA, const D2D1_COLOR_F& colorB,
    float drag, float gravity,
    bool enableSelfDestruct, float selfDestructAfterSeconds)
{
    emitBurstCommon(ToSimPos(pos), count,
        speedMin, speedMax,
        sizeMin, sizeMax,
        lifeMin, lifeMax,
        colorA, colorB,
        drag, gravity);

    if (enableSelfDestruct)
    {
        m_selfDestructActive = true;
        m_selfDestructTimer = 0.0f;
        m_selfDestructAfterSeconds = selfDestructAfterSeconds;
    }
}

void ParticleComponent::EmitAura(
    const FVector2& center,
    float radius,
    int count,
    float radiusJitter,
    float angularSpeedMin, float angularSpeedMax,
    float sizeMin, float sizeMax,
    float lifeMin, float lifeMax,
    float drag, float gravity,
    const D2D1_COLOR_F& colorA, const D2D1_COLOR_F& colorB)
{
    for (int i = 0; i < count; ++i)
    {
        const float ang = (float)i / (float)count * 6.2831853f;
        Particle p;
        p.isOrbital = true;
        p.orbitalCenter = ToSimPos(center);
        p.orbitalRadius = radius + randRange(-radiusJitter, radiusJitter);
        p.orbitalAngularSpeed = randRange(angularSpeedMin, angularSpeedMax); // deg/sec
        p.orbitalAngleDeg = ang * 180.0f / 3.14159265f;

        p.position = FVector2(
            p.orbitalCenter.x + cosf(ang) * p.orbitalRadius,
            p.orbitalCenter.y + sinf(ang) * p.orbitalRadius);
        p.velocity = FVector2(0.0f, 0.0f);
        p.size = randRange(sizeMin, sizeMax);
        p.rotation = randRange(0.0f, 360.0f);
        p.angularVelocity = randRange(-90.0f, 90.0f);
        p.life = 0.0f;
        p.maxLife = randRange(lifeMin, lifeMax);
        p.drag = drag;
        p.gravity = gravity;
        p.colorStart = colorA;
        p.colorEnd = colorB;
        m_particles.push_back(p);
    }
}

void ParticleComponent::EmitElectric(
    const FVector2& pos,
    int count,
    float speedMin, float speedMax,
    float sizeMin, float sizeMax,
    float lifeMin, float lifeMax,
    const D2D1_COLOR_F& colorA, const D2D1_COLOR_F& colorB,
    float drag, float gravity,
    float spreadRadians)
{
    emitBurstCommon(ToSimPos(pos), count,
        speedMin, speedMax,
        sizeMin, sizeMax,
        lifeMin, lifeMax,
        colorA, colorB,
        drag, gravity,
        spreadRadians);
}

void ParticleComponent::EmitPortalSwirl(
    const FVector2& pos,
    int count,
    float speedMin, float speedMax,
    float sizeMin, float sizeMax,
    float lifeMin, float lifeMax,
    const D2D1_COLOR_F& colorA, const D2D1_COLOR_F& colorB,
    float drag, float gravity,
    float spreadRadians)
{
    emitBurstCommon(ToSimPos(pos), count,
        speedMin, speedMax,
        sizeMin, sizeMax,
        lifeMin, lifeMax,
        colorA, colorB,
        drag, gravity,
        spreadRadians);
}

// 내부 구현 -----------------------------------------------------------------
void ParticleComponent::emitBurstCommon(const FVector2& pos,
    int count,
    float speedMin,
    float speedMax,
    float sizeMin,
    float sizeMax,
    float lifeMin,
    float lifeMax,
    const D2D1_COLOR_F& colorA,
    const D2D1_COLOR_F& colorB,
    float drag,
    float gravity,
    float spreadRadians)
{
    for (int i = 0; i < count; ++i)
    {
        Particle p;
        p.position = pos;
        const FVector2 dir = randDir();
        const float speed = randRange(speedMin, speedMax);
        const float angleJitter = randRange(-spreadRadians * 0.5f, spreadRadians * 0.5f);
        const float cs = cosf(angleJitter), sn = sinf(angleJitter);
        FVector2 d2(cs * dir.x - sn * dir.y, sn * dir.x + cs * dir.y);

        p.velocity = FVector2(d2.x * speed, d2.y * speed);
        p.size = randRange(sizeMin, sizeMax);
        p.rotation = randRange(0.0f, 360.0f);
        p.angularVelocity = randRange(-180.0f, 180.0f);
        p.life = 0.0f;
        p.maxLife = randRange(lifeMin, lifeMax);
        p.colorStart = colorA;
        p.colorEnd = colorB;
        p.drag = drag;
        p.gravity = gravity;
        m_particles.push_back(p);
    }
}

void ParticleComponent::spawnShockwaveRing(const FVector2& pos, float radius, float thickness, float life, const D2D1_COLOR_F& color)
{
    const int seg = 36;
    for (int i = 0; i < seg; ++i)
    {
        const float t = (float)i / (float)seg;
        const float ang = t * 6.2831853f;
        Particle p;
        const float r = randRange(radius * 0.9f, radius * 1.1f);
        p.position = FVector2(pos.x + cosf(ang) * r, pos.y + sinf(ang) * r);
        p.velocity = FVector2(cosf(ang) * 120.0f, sinf(ang) * 120.0f);
        p.size = randRange(thickness * 0.8f, thickness * 1.2f);
        p.rotation = randRange(0.0f, 360.0f);
        p.angularVelocity = randRange(-45.0f, 45.0f);
        p.life = 0.0f;
        p.maxLife = life;
        p.colorStart = color;
        p.colorEnd = D2D1::ColorF(color.r, color.g, color.b, 0.0f);
        p.drag = 1.0f;
        p.gravity = 0.0f;
        m_particles.push_back(p);
    }
}

void ParticleComponent::ensureParticleTexture()
{
    if (m_particleBitmap) return;
    createSoftCircleBitmap();
}

void ParticleComponent::createSoftCircleBitmap()
{
    // 64x64 부드러운 원 알파 텍스처를 CPU에서 생성 후 D2D Bitmap으로 올림
    ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
    if (!context) return;

    const UINT width = 64, height = 64;
    std::vector<uint8_t> pixels(width * height * 4, 0);
    const float cx = (width - 1) * 0.5f;
    const float cy = (height - 1) * 0.5f;
    const float r = min(cx, cy);
    for (UINT y = 0; y < height; ++y)
    {
        for (UINT x = 0; x < width; ++x)
        {
            const float dx = (float)x - cx;
            const float dy = (float)y - cy;
            float d = sqrtf(dx * dx + dy * dy) / r; // 0..1
            float a = 1.0f - d;
            a = (a < 0.0f) ? 0.0f : (a > 1.0f ? 1.0f : a);
            a = powf(a, 1.5f); // 가장자리 소프트닝

            const size_t i = (y * width + x) * 4;
            pixels[i + 0] = 255; // B
            pixels[i + 1] = 255; // G
            pixels[i + 2] = 255; // R
            pixels[i + 3] = (uint8_t)(a * 255.0f); // A
        }
    }

    D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_NONE,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

    ComPtr<ID2D1Bitmap1> bmp;
    HRESULT hr = context->CreateBitmap(
        D2D1::SizeU(width, height),
        pixels.data(),
        width * 4,
        &props,
        &bmp);
    if (SUCCEEDED(hr))
    {
        m_particleBitmap = std::shared_ptr<ID2D1Bitmap1>(bmp.Detach(), [](ID2D1Bitmap1* p) { if (p) p->Release(); });
        m_srcWidth = width;
        m_srcHeight = height;
    }
}

void ParticleComponent::LoadData(const std::wstring& relativeOrAbsolutePath)
{
    std::wstring filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + relativeOrAbsolutePath); // 파일 이름만 저장
    // 파일에서 텍스처 로드하여 파티클 비트맵 교체
	std::shared_ptr<ID2D1Bitmap1> loaded = PackageResourceManager::GetInstance().CreateBitmapFromFile(
			filePath.c_str());
    if (loaded)
    {
        m_particleBitmap = loaded;
        D2D1_SIZE_U s = loaded->GetPixelSize();
        m_srcWidth = s.width;
        m_srcHeight = s.height;
    }
}

float ParticleComponent::randRange(float a, float b)
{
    std::uniform_real_distribution<float> dist(a, b);
    return dist(m_rng);
}

FVector2 ParticleComponent::randDir()
{
    float ang = randRange(0.0f, 6.2831853f);
    return FVector2(cosf(ang), sinf(ang));
}

D2D1_COLOR_F ParticleComponent::lerpColor(const D2D1_COLOR_F& a, const D2D1_COLOR_F& b, float t)
{
    if (t < 0.0f) t = 0.0f; else if (t > 1.0f) t = 1.0f;
    return D2D1::ColorF(
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t,
        a.a + (b.a - a.a) * t);
}


