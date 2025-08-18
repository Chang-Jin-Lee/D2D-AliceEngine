// 파티클 고급 색상 변환 쉐이더
// 시간, 위치, 생명에 따른 동적 색상 변화

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float time : TEXCOORD1;
    float lifeRatio : TEXCOORD2;
    float2 worldPos : TEXCOORD3;
};

// 쉐이더 상수 버퍼
cbuffer ParticleConstants : register(b0)
{
    float4 timeData;        // x: time, y: deltaTime, z: emissionTime, w: unused
    float4 colorData;       // x: hueShift, y: saturation, z: brightness, w: contrast
    float4 effectData;      // x: noiseIntensity, y: waveSpeed, z: distortion, w: glow
    float4 transformData;   // x: scale, y: rotation, z: unused, w: unused
};

// 노이즈 함수
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

// HSV to RGB 변환
float3 hsv2rgb(float3 c)
{
    float4 K = float4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    float3 p = abs(frac(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * lerp(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

// 메인 픽셀 셰이더
float4 D2DMain(PSInput input) : SV_TARGET
{
    float time = timeData.x;
    float lifeRatio = input.lifeRatio;
    
    // 기본 색상
    float4 baseColor = input.color;
    
    // 시간에 따른 색상 변화
    float hueShift = colorData.x * time * 0.5;
    float3 hsv = float3(baseColor.r + hueShift, colorData.y, colorData.z);
    float3 shiftedColor = hsv2rgb(hsv);
    
    // 노이즈 기반 색상 변화
    float2 noiseUV = input.worldPos * 0.01 + time * 0.1;
    float noiseValue = noise(noiseUV) * effectData.x;
    shiftedColor += noiseValue * 0.3;
    
    // 파동 효과
    float wave = sin(input.worldPos.x * 0.02 + time * effectData.y) * 
                 cos(input.worldPos.y * 0.02 + time * effectData.y) * 0.2;
    shiftedColor += wave;
    
    // 생명에 따른 그라데이션
    float3 lifeGradient = lerp(float3(1.0, 0.5, 0.0), float3(0.0, 0.5, 1.0), lifeRatio);
    shiftedColor *= lifeGradient;
    
    // 글로우 효과
    float glow = effectData.w * (1.0 - lifeRatio);
    shiftedColor += glow * float3(1.0, 0.8, 0.6);
    
    // 투명도 계산
    float alpha = baseColor.a * lifeRatio;
    
    // 최종 색상 반환
    return float4(shiftedColor, alpha);
}