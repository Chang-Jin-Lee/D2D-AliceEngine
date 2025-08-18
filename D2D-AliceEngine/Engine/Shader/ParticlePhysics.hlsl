// 파티클 물리 시뮬레이션 쉐이더
// 입자 상호작용, 유체 효과, 물리 기반 변형

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float time : TEXCOORD1;
    float lifeRatio : TEXCOORD2;
    float2 worldPos : TEXCOORD3;
    float2 velocity : TEXCOORD4;
    float mass : TEXCOORD5;
};

// 쉐이더 상수 버퍼
cbuffer PhysicsConstants : register(b0)
{
    float4 physicsData;     // x: gravity, y: viscosity, z: surfaceTension, w: pressure
    float4 timeData;        // x: time, y: deltaTime, z: emissionTime, w: unused
    float4 effectData;      // x: turbulence, y: vorticity, z: buoyancy, w: drag
    float4 transformData;   // x: scale, y: rotation, z: unused, w: unused
};

// 물리 시뮬레이션 함수들
float2 calculateGravity(float2 pos, float2 center, float gravity)
{
    float2 direction = center - pos;
    float distance = length(direction);
    if (distance < 0.001) return float2(0, 0);
    
    float force = gravity / (distance * distance + 1.0);
    return normalize(direction) * force;
}

float2 calculateViscosity(float2 velocity, float viscosity)
{
    return -velocity * viscosity;
}

float2 calculateSurfaceTension(float2 pos, float2 neighborPos, float tension)
{
    float2 direction = neighborPos - pos;
    float distance = length(direction);
    if (distance < 0.001) return float2(0, 0);
    
    return normalize(direction) * tension * distance;
}

float2 calculatePressure(float2 pos, float2 center, float pressure, float radius)
{
    float2 direction = pos - center;
    float distance = length(direction);
    if (distance > radius) return float2(0, 0);
    
    float force = pressure * (1.0 - distance / radius);
    return normalize(direction) * force;
}

// 터빌런스 효과
float2 calculateTurbulence(float2 pos, float time, float intensity)
{
    float2 turbulence = float2(
        sin(pos.x * 0.1 + time * 0.5) * cos(pos.y * 0.1 + time * 0.3),
        cos(pos.x * 0.1 + time * 0.4) * sin(pos.y * 0.1 + time * 0.6)
    );
    return turbulence * intensity;
}

// 소용돌이 효과
float2 calculateVorticity(float2 pos, float2 center, float time, float intensity)
{
    float2 direction = pos - center;
    float distance = length(direction);
    if (distance < 0.001) return float2(0, 0);
    
    float angle = atan2(direction.y, direction.x) + time * intensity;
    return float2(
        cos(angle) * distance * 0.1,
        sin(angle) * distance * 0.1
    );
}

// 부력 효과
float2 calculateBuoyancy(float2 pos, float2 surface, float buoyancy)
{
    float2 direction = surface - pos;
    float distance = length(direction);
    if (distance < 0.001) return float2(0, 0);
    
    return normalize(direction) * buoyancy * distance;
}

// 메인 픽셀 셰이더
float4 D2DMain(PSInput input) : SV_TARGET
{
    float time = timeData.x;
    float lifeRatio = input.lifeRatio;
    
    // 물리 시뮬레이션 계산
    float2 pos = input.worldPos;
    float2 velocity = input.velocity;
    float mass = input.mass;
    
    // 중력
    float2 gravity = calculateGravity(pos, float2(0, 0), physicsData.x);
    
    // 점성
    float2 viscosity = calculateViscosity(velocity, physicsData.y);
    
    // 표면 장력 (가상의 이웃 입자와의 상호작용)
    float2 neighborPos = pos + float2(10.0, 0.0); // 가상 이웃
    float2 surfaceTension = calculateSurfaceTension(pos, neighborPos, physicsData.z);
    
    // 압력
    float2 pressure = calculatePressure(pos, float2(0, 0), physicsData.w, 100.0);
    
    // 터빌런스
    float2 turbulence = calculateTurbulence(pos, time, effectData.x);
    
    // 소용돌이
    float2 vorticity = calculateVorticity(pos, float2(0, 0), time, effectData.y);
    
    // 부력
    float2 buoyancy = calculateBuoyancy(pos, float2(0, 50.0), effectData.z);
    
    // 총 힘 계산
    float2 totalForce = gravity + viscosity + surfaceTension + pressure + 
                        turbulence + vorticity + buoyancy;
    
    // 질량에 따른 가속도
    float2 acceleration = totalForce / max(mass, 0.1);
    
    // 속도 업데이트 (간단한 오일러 적분)
    velocity += acceleration * timeData.y;
    
    // 위치 업데이트
    pos += velocity * timeData.y;
    
    // 색상 계산
    float4 color = input.color;
    
    // 속도에 따른 색상 변화
    float speed = length(velocity);
    color.rgb += speed * 0.01 * float3(1.0, 0.5, 0.0);
    
    // 물리 효과에 따른 색상 변화
    float forceMagnitude = length(totalForce);
    color.rgb += forceMagnitude * 0.001 * float3(0.0, 0.5, 1.0);
    
    // 생명에 따른 투명도
    color.a *= lifeRatio;
    
    // 질량에 따른 크기 변화
    float massScale = 1.0 + mass * 0.1;
    color.a *= massScale;
    
    // 최종 색상 반환
    return color;
}