// ��ƼŬ Ư�� ȿ�� ���̴�
// ����, ����, ����, ���� ȿ��

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float time : TEXCOORD1;
    float lifeRatio : TEXCOORD2;
    float2 worldPos : TEXCOORD3;
    float3 normal : TEXCOORD4;
    float effectType : TEXCOORD5;
};

// ���̴� ��� ����
cbuffer SpecialEffectConstants : register(b0)
{
    float4 effectData;      // x: magicIntensity, y: explosionPower, z: electricCharge, w: portalStrength
    float4 timeData;        // x: time, y: deltaTime, z: emissionTime, w: unused
    float4 transformData;   // x: scale, y: rotation, z: unused, w: unused
    float4 colorData;       // x: hueShift, y: saturation, z: brightness, w: contrast
};

// Ư�� ȿ�� �Լ���
float3 calculateMagicEffect(float2 pos, float time, float intensity)
{
    float2 magicUV = pos * 0.02 + time * float2(0.3, 0.7);
    float magic = sin(magicUV.x * 20.0) * cos(magicUV.y * 20.0) * 0.5 + 0.5;
    magic *= intensity;
    
    // ���� ���� ȿ��
    float aura = sin(time * 2.0) * 0.3 + 0.7;
    magic *= aura;
    
    return float3(magic * 0.5, magic * 0.3, magic);
}

float3 calculateExplosionEffect(float2 pos, float time, float power)
{
    float distance = length(pos);
    float explosion = exp(-distance * 0.01) * power;
    
    // ���� �ĵ�
    float wave = sin(distance * 0.1 - time * 10.0) * 0.5 + 0.5;
    explosion *= wave;
    
    // �߽ɿ��� �ٱ����� ������ ȿ��
    float radial = 1.0 - smoothstep(0.0, 100.0, distance);
    explosion *= radial;
    
    return float3(explosion, explosion * 0.7, explosion * 0.3);
}

float3 calculateElectricEffect(float2 pos, float time, float charge)
{
    float2 electricUV = pos * 0.05 + time * float2(1.0, 0.5);
    float electric = sin(electricUV.x * 50.0) * sin(electricUV.y * 50.0) * 0.5 + 0.5;
    electric *= charge;
    
    // ���� ��ũ ȿ��
    float arc = sin(pos.x * 0.1 + time * 20.0) * sin(pos.y * 0.1 + time * 15.0);
    electric += arc * 0.3;
    
    return float3(electric, electric, 1.0);
}

float3 calculatePortalEffect(float2 pos, float time, float strength)
{
    float2 portalUV = pos * 0.01 + time * float2(0.2, 0.4);
    float portal = sin(portalUV.x * 30.0) * cos(portalUV.y * 30.0) * 0.5 + 0.5;
    portal *= strength;
    
    // ���� ȸ�� ȿ��
    float rotation = sin(time * 0.5) * 0.5 + 0.5;
    portal *= rotation;
    
    // ���� �߽� ȿ��
    float center = 1.0 - smoothstep(0.0, 50.0, length(pos));
    portal *= center;
    
    return float3(portal * 0.3, portal * 0.7, portal);
}

// ���� ��ȯ �Լ���
float3 hsv2rgb(float3 c)
{
    float4 K = float4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    float3 p = abs(frac(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * lerp(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float3 rgb2hsv(float3 c)
{
    float4 K = float4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    float4 p = lerp(float4(c.bg, K.wz), float4(c.gb, K.xy), step(c.b, c.g));
    float4 q = lerp(float4(p.xyw, c.r), float4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return float3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

// ���� �ȼ� ���̴�
float4 D2DMain(PSInput input) : SV_TARGET
{
    float time = timeData.x;
    float lifeRatio = input.lifeRatio;
    float effectType = input.effectType;
    
    // �⺻ ����
    float4 color = input.color;
    
    // ȿ�� Ÿ�Կ� ���� Ư�� ȿ�� ����
    float3 specialEffect = float3(0, 0, 0);
    
    if (effectType < 0.25) // ���� ȿ��
    {
        specialEffect = calculateMagicEffect(input.worldPos, time, effectData.x);
    }
    else if (effectType < 0.5) // ���� ȿ��
    {
        specialEffect = calculateExplosionEffect(input.worldPos, time, effectData.y);
    }
    else if (effectType < 0.75) // ���� ȿ��
    {
        specialEffect = calculateElectricEffect(input.worldPos, time, effectData.z);
    }
    else // ���� ȿ��
    {
        specialEffect = calculatePortalEffect(input.worldPos, time, effectData.w);
    }
    
    // Ư�� ȿ���� �⺻ ���� �ռ�
    color.rgb += specialEffect;
    
    // ���� ��ȯ ȿ��
    float3 hsv = rgb2hsv(color.rgb);
    hsv.x += colorData.x * time * 0.1; // ���� ��ȭ
    hsv.y *= colorData.y; // ä�� ����
    hsv.z *= colorData.z; // ���� ����
    color.rgb = hsv2rgb(hsv);
    
    // ��� ����
    color.rgb = (color.rgb - 0.5) * colorData.w + 0.5;
    
    // �ð��� ���� ���� ��ȭ
    float timeShift = sin(time * 3.0) * 0.1;
    color.rgb += timeShift * float3(0.1, 0.05, 0.2);
    
    // ������ ���� ������
    color.a *= lifeRatio;
    
    // ȿ�� ������ ���� ������ ����
    float effectIntensity = length(specialEffect);
    color.a *= (1.0 + effectIntensity * 0.5);
    
    // ���� ���� ��ȯ
    return color;
}