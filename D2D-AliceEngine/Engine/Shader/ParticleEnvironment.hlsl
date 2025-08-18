// 파티클 환경 효과 쉐이더
// 날씨, 조명, 반사/굴절, 환경 맵핑

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float time : TEXCOORD1;
    float lifeRatio : TEXCOORD2;
    float2 worldPos : TEXCOORD3;
    float3 normal : TEXCOORD4;
    float depth : TEXCOORD5;
};

// 쉐이더 상수 버퍼
cbuffer EnvironmentConstants : register(b0)
{
    float4 environmentData; // x: weather, y: lighting, z: reflection, w: refraction
    float4 timeData;        // x: time, y: deltaTime, z: emissionTime, w: unused
    float4 effectData;      // x: rainIntensity, y: snowIntensity, z: fogDensity, w: windStrength
    float4 transformData;   // x: scale, y: rotation, z: unused, w: unused
};

// 환경 효과 함수들
float3 calculateRainEffect(float2 pos, float time, float intensity)
{
    float2 rainUV = pos * 0.01 + time * float2(0.5, 1.0);
    float rain = sin(rainUV.x * 100.0) * sin(rainUV.y * 100.0) * 0.5 + 0.5;
    rain *= intensity;
    
    return float3(0.0, 0.0, rain);
}

float3 calculateSnowEffect(float2 pos, float time, float intensity)
{
    float2 snowUV = pos * 0.005 + time * float2(0.3, 0.8);
    float snow = sin(snowUV.x * 50.0) * cos(snowUV.y * 50.0) * 0.5 + 0.5;
    snow *= intensity;
    
    return float3(snow, snow, snow);
}

float3 calculateFogEffect(float depth, float density)
{
    float fog = 1.0 - exp(-depth * density);
    return float3(fog, fog, fog);
}

float3 calculateWindEffect(float2 pos, float time, float strength)
{
    float2 windUV = pos * 0.02 + time * float2(0.2, 0.1);
    float wind = sin(windUV.x * 30.0) * cos(windUV.y * 30.0) * strength;
    
    return float3(wind * 0.1, wind * 0.05, 0.0);
}

// 조명 계산
float3 calculateLighting(float3 normal, float3 lightDir, float3 lightColor, float intensity)
{
    float diffuse = max(dot(normal, lightDir), 0.0);
    return lightColor * diffuse * intensity;
}

// 반사 효과
float3 calculateReflection(float3 viewDir, float3 normal, float reflectivity)
{
    float3 reflectDir = reflect(-viewDir, normal);
    return reflectDir * reflectivity;
}

// 굴절 효과
float3 calculateRefraction(float3 viewDir, float3 normal, float ior)
{
    float3 refractDir = refract(-viewDir, normal, ior);
    return refractDir;
}

// 메인 픽셀 셰이더
float4 D2DMain(PSInput input) : SV_TARGET
{
    float time = timeData.x;
    float lifeRatio = input.lifeRatio;
    
    // 기본 색상
    float4 color = input.color;
    
    // 환경 효과 적용
    float weather = environmentData.x;
    float lighting = environmentData.y;
    float reflection = environmentData.z;
    float refraction = environmentData.w;
    
    // 비 효과
    float3 rainEffect = calculateRainEffect(input.worldPos, time, effectData.x);
    color.rgb += rainEffect * weather;
    
    // 눈 효과
    float3 snowEffect = calculateSnowEffect(input.worldPos, time, effectData.y);
    color.rgb += snowEffect * weather;
    
    // 안개 효과
    float3 fogEffect = calculateFogEffect(input.depth, effectData.z);
    color.rgb = lerp(color.rgb, float3(0.7, 0.7, 0.8), fogEffect * 0.3);
    
    // 바람 효과
    float3 windEffect = calculateWindEffect(input.worldPos, time, effectData.w);
    color.rgb += windEffect;
    
    // 조명 계산
    float3 lightDir = normalize(float3(0.5, 1.0, 0.3));
    float3 lightColor = float3(1.0, 0.95, 0.8);
    float3 lightingEffect = calculateLighting(input.normal, lightDir, lightColor, lighting);
    color.rgb += lightingEffect * 0.3;
    
    // 반사 효과
    float3 viewDir = normalize(float3(0.0, 0.0, 1.0));
    float3 reflectionEffect = calculateReflection(viewDir, input.normal, reflection);
    color.rgb += reflectionEffect * 0.2;
    
    // 굴절 효과
    float3 refractionEffect = calculateRefraction(viewDir, input.normal, 1.33); // 물의 굴절률
    color.rgb += refractionEffect * 0.1;
    
    // 시간에 따른 색상 변화
    float timeShift = sin(time * 0.5) * 0.05;
    color.rgb += timeShift * float3(0.1, 0.05, 0.1);
    
    // 생명에 따른 투명도
    color.a *= lifeRatio;
    
    // 환경에 따른 투명도 조정
    float environmentOpacity = 1.0 - (weather * 0.3 + effectData.z * 0.2);
    color.a *= environmentOpacity;
    
    // 최종 색상 반환
    return color;
}