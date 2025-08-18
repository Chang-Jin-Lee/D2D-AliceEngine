// 파티클 왜곡 및 변형 쉐이더
// 물결, 소용돌이, 확장/수축 효과

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float time : TEXCOORD1;
    float lifeRatio : TEXCOORD2;
    float2 worldPos : TEXCOORD3;
    float2 velocity : TEXCOORD4;
};

// 쉐이더 상수 버퍼
cbuffer DistortionConstants : register(b0)
{
    float4 distortionData;  // x: waveAmplitude, y: waveFrequency, z: swirlIntensity, w: expansion
    float4 timeData;        // x: time, y: deltaTime, z: emissionTime, w: unused
    float4 effectData;      // x: noiseScale, y: turbulence, z: rippleSpeed, w: distortionStrength
    float4 transformData;   // x: scale, y: rotation, z: shear, w: unused
};

// 복잡한 노이즈 함수
float2 hash22(float2 p)
{
    float3 p3 = frac(float3(p.xyx) * float3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return frac((p3.xx + p3.yz) * p3.zy);
}

float noise(float2 p)
{
    float2 i = floor(p);
    float2 f = frac(p);
    float2 u = f * f * (3.0 - 2.0 * f);
    
    return lerp(lerp(dot(hash22(i), f),
                     dot(hash22(i + float2(1.0, 0.0)), f - float2(1.0, 0.0)), u.x),
                lerp(dot(hash22(i + float2(0.0, 1.0)), f - float2(0.0, 1.0)),
                     dot(hash22(i + float2(1.0, 1.0)), f - float2(1.0, 1.0)), u.x), u.y);
}

// 프랙탈 노이즈
float fbm(float2 p)
{
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;
    
    for (int i = 0; i < 4; i++)
    {
        value += amplitude * noise(p * frequency);
        amplitude *= 0.5;
        frequency *= 2.0;
    }
    
    return value;
}

// 소용돌이 효과
float2 swirl(float2 p, float intensity, float time)
{
    float angle = length(p) * intensity + time;
    float2 rotated = float2(
        p.x * cos(angle) - p.y * sin(angle),
        p.x * sin(angle) + p.y * cos(angle)
    );
    return rotated;
}

// 물결 효과
float2 wave(float2 p, float amplitude, float frequency, float time)
{
    float wave1 = sin(p.x * frequency + time) * amplitude;
    float wave2 = cos(p.y * frequency + time * 0.7) * amplitude;
    return p + float2(wave1, wave2);
}

// 메인 픽셀 셰이더
float4 D2DMain(PSInput input) : SV_TARGET
{
    float time = timeData.x;
    float lifeRatio = input.lifeRatio;
    
    // 기본 UV 좌표
    float2 uv = input.uv - 0.5; // 중심을 0으로
    float2 distortedUV = uv;
    
    // 물결 왜곡
    float waveAmp = distortionData.x * (1.0 - lifeRatio);
    float waveFreq = distortionData.y;
    distortedUV = wave(distortedUV, waveAmp, waveFreq, time);
    
    // 소용돌이 효과
    float swirlIntensity = distortionData.z * lifeRatio;
    distortedUV = swirl(distortedUV, swirlIntensity, time * 0.5);
    
    // 확장/수축 효과
    float expansion = distortionData.w * sin(time * 2.0) * 0.1;
    distortedUV *= (1.0 + expansion);
    
    // 프랙탈 노이즈 기반 왜곡
    float2 noiseUV = input.worldPos * effectData.x + time * 0.1;
    float noiseValue = fbm(noiseUV);
    distortedUV += noiseValue * effectData.w * 0.1;
    
    // 터빌런스 효과
    float2 turbulence = float2(
        noise(input.worldPos * 0.02 + time * 0.3),
        noise(input.worldPos * 0.02 + time * 0.4)
    ) * effectData.y;
    distortedUV += turbulence * 0.05;
    
    // 리플 효과 (동심원 파동)
    float distance = length(uv);
    float ripple = sin(distance * 20.0 - time * effectData.z) * 0.02;
    distortedUV += normalize(uv) * ripple;
    
    // UV 범위 체크
    if (any(distortedUV < -0.5) || any(distortedUV > 0.5))
    {
        discard; // 범위 밖이면 렌더링하지 않음
    }
    
    // 색상 계산
    float4 color = input.color;
    
    // 왜곡 강도에 따른 색상 변화
    float distortionAmount = length(distortedUV - uv);
    color.rgb += distortionAmount * float3(0.2, 0.1, 0.3);
    
    // 생명에 따른 투명도
    color.a *= lifeRatio;
    
    // 속도에 따른 색상 변화
    float speed = length(input.velocity);
    color.rgb += speed * 0.01 * float3(1.0, 0.5, 0.0);
    
    return color;
}