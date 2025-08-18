#pragma once

#include <math.h>
#include <random>
#include <Define/Define.h>

/*
* 수학관련입니다. 
* nodiscard는 반환값을 강제로 사용하도록 하는 키워드입니다.
*/

template<typename T>
class TVector2
{
public:
	T x, y;

	TVector2() : x(0), y(0) {}
	TVector2(T _x, T _y) : x(_x), y(_y) {}
	TVector2(T _x) : x(_x), y(_x) {}

	TVector2(const TVector2<T>& left) { x = left.x; y = left.y; }
	TVector2(TVector2<T>& left) { x = left.x; y = left.y; }
	~TVector2() {}

	TVector2 operator*(const float& value)
	{
		return TVector2(x * value, y * value);
	}

	TVector2 operator/(const float& value)
	{
		return TVector2(x / value, y / value);
	}

	TVector2 operator-() const
	{
		return TVector2(-x, -y);
	}

	TVector2 operator+(const TVector2& rhs)
	{
		return TVector2(x + rhs.x, y + rhs.y);
	}
	TVector2 operator-(const TVector2& rhs)
	{
		return TVector2(x - rhs.x, y - rhs.y);
	}

	bool operator!=(const TVector2& rhs)
	{
		return x != rhs.x || y != rhs.y;
	}
	bool operator==(const TVector2& rhs)
	{
		return x == rhs.x && y == rhs.y;
	}
	void operator*=(const TVector2& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
	}
	void operator*=(const float& value)
	{
		x *= value;
		y *= value;
	}
	void operator+=(const TVector2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}
	void operator-=(const TVector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}
	void operator=(const TVector2& rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}
	bool IsZero()
	{
		return x == 0 && y == 0;
	}

	float Dot(const TVector2& rhs)
	{
		return x * rhs.x + y * rhs.y;
	}

	TVector2 Normalize()
	{
		float len = Length();
		if (len < 1e-6f)
			return TVector2(0, 0);
		return TVector2(x / len, y / len);
	}

	float Length()
	{
		return sqrt(LengthSQ());
	}

	float LengthSQ()
	{
		return x * x + y * y;
	}
};

template<typename T>
TVector2<T> operator-(const TVector2<T>& lhs, const TVector2<T>& rhs)
{
	return TVector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template<typename T>
TVector2<T> operator+(const TVector2<T>& lhs, const TVector2<T>& rhs)
{
	return TVector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template<typename T>
TVector2<T> operator*(const TVector2<T>& lhs, const TVector2<T>& rhs)
{
	return TVector2<T>(lhs.x * rhs.x, lhs.y * rhs.y);
}

//  float * TVector2 연산 (순서 바뀐 경우)
template<typename T>
TVector2<T> operator*(const float& value, const TVector2<T>& vec)
{
	return TVector2<T>(vec.x * value, vec.y * value);
}

// TVector2 * float 연산 (전역 함수로)
template<typename T>
TVector2<T> operator*(const TVector2<T>& vec, const float& value)
{
	return TVector2<T>(vec.x * value, vec.y * value);
}

template<typename T>
class TVector3
{
public:
	T x, y, z;

	TVector3() : x(0), y(0), z(0) {}
	TVector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	TVector3(T _x) : x(_x), y(_x), z(_x) {}

	TVector3(const TVector3<T>& left) { x = left.x; y = left.y; z = left.z; }
	TVector3(TVector3<T>& left) { x = left.x; y = left.y; z = left.z; }
	~TVector3() {}

	TVector3 operator*(const float& value)
	{
		return TVector3(x * value, y * value, z * value);
	}

	TVector3 operator/(const float& value)
	{
		return TVector3(x / value, y / value, z / value);
	}

	TVector3 operator-() const
	{
		return TVector3(-x, -y, -z);
	}

	TVector3 operator+(const TVector3& rhs)
	{
		return TVector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	TVector3 operator-(const TVector3& rhs)
	{
		return TVector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	bool operator!=(const TVector3& rhs)
	{
		return x != rhs.x || y != rhs.y || z != rhs.z;
	}
	bool operator==(const TVector3& rhs)
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	void operator*=(const TVector3& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
	}
	void operator*=(const float& value)
	{
		x *= value;
		y *= value;
		z *= value;
	}
	void operator+=(const TVector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}
	void operator-=(const TVector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}
	void operator=(const TVector3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}
	bool IsZero()
	{
		return x == 0 && y == 0 && z == 0;
	}

	float Dot(const TVector3& rhs)
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	TVector3 Cross(const TVector3& rhs)
	{
		return TVector3(
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x
		);
	}

	TVector3 Normalize()
	{
		float len = Length();
		if (len < 1e-6f)
			return TVector3(0, 0, 0);
		return TVector3(x / len, y / len, z / len);
	}

	float Length()
	{
		return sqrt(LengthSQ());
	}

	float LengthSQ()
	{
		return x * x + y * y + z * z;
	}
};

template<typename T>
class TVector4
{
public:
	T x, y, z, w;

	TVector4() : x(0), y(0), z(0), w(0) {}
	TVector4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
	TVector4(T _x) : x(_x), y(_x), z(_x), w(_x) {}

	TVector4(const TVector4<T>& left) { x = left.x; y = left.y; z = left.z; w = left.w; }
	TVector4(TVector4<T>& left) { x = left.x; y = left.y; z = left.z; w = left.w; }
	~TVector4() {}

	TVector4 operator*(const float& value)
	{
		return TVector4(x * value, y * value, z * value, w * value);
	}

	TVector4 operator/(const float& value)
	{
		return TVector4(x / value, y / value, z / value, w / value);
	}

	TVector4 operator-() const
	{
		return TVector4(-x, -y, -z, -w);
	}

	TVector4 operator+(const TVector4& rhs)
	{
		return TVector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}
	TVector4 operator-(const TVector4& rhs)
	{
		return TVector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	bool operator!=(const TVector4& rhs)
	{
		return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
	}
	bool operator==(const TVector4& rhs)
	{
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}
	void operator*=(const TVector4& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
	}
	void operator*=(const float& value)
	{
		x *= value;
		y *= value;
		z *= value;
		w *= value;
	}
	void operator+=(const TVector4& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
	}
	void operator-=(const TVector4& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
	}
	void operator=(const TVector4& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
	}
	bool IsZero()
	{
		return x == 0 && y == 0 && z == 0 && w == 0;
	}

	float Dot(const TVector4& rhs)
	{
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}

	float Length()
	{
		return sqrt(LengthSQ());
	}

	float LengthSQ()
	{
		return x * x + y * y + z * z + w * w;
	}
};

template<typename T>
struct TBox2
{
public:
	TVector2<T> Min;
	TVector2<T> Max;

	TBox2<T>()
	{

	}

	TBox2<T>(const T& minx, const T& miny, const T& maxx, const T& maxy)
	{
		Min.x = minx;
		Min.y = miny;
		Max.x = maxx;
		Max.y = maxy;
	}

	TBox2<T>(const TBox2<T>& inbox)
	{
		Min = inbox.Min;
		Max = inbox.Max;
	}

	TVector2<T> GetCenter()
	{
		return TVector2<T>((Min + Max) * 0.5f);
	}
};

namespace Math
{
	static std::random_device rd;

	template<typename T>
	struct TRandom
	{
		// 두 수 사이의 랜덤한 수를 반환. 반환값이 없으면 오류.
		[[nodiscard]] static float GetRandomInRange(T Min, T Max)
		{
			static std::mt19937 gen(rd());
			std::uniform_real_distribution<T> dis(Min, Max);
			return dis(gen);
		}

		// 도넛 모양의 구간에 임의의 점을 반환하는 함수.
		[[nodiscard]] static TVector2<T> GetRandomPointInTorus2D(const T& centerX, const T& centerY, const T& innerRadius, const T& outerRadius)
		{
			static std::mt19937 gen(rd());
			std::uniform_real_distribution<T>  angleRandom(0, 2 * static_cast<T>(Define::PI));
			std::uniform_real_distribution<T>  radiusRandom(innerRadius * innerRadius, outerRadius * outerRadius);

			T theta = angleRandom(gen);
			T newRadius = std::sqrt(radiusRandom(gen));
			return TVector2<T>(centerX + newRadius * std::cos(theta), centerY + newRadius * std::sin(theta));
		}

		// 원 모양의 구간에 임의의 점을 반환하는 함수.
		[[nodiscard]] static TVector2<T> GetRandomPointInCircle2D(const T& centerX, const T& centerY, const T& radius)
		{
			static std::mt19937 gen(rd());
			std::uniform_real_distribution<T>  angleRandom(0, 2 * static_cast<T>(Define::PI));
			std::uniform_real_distribution<T>  radiusRandom(1, radius*radius);

			T theta = angleRandom(gen);
			T newRadius = std::sqrt(radiusRandom(gen));
			return TVector2<T>(centerX + newRadius * std::cos(theta), centerY + newRadius * std::sin(theta));
		}
	};

	template <typename T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi) 
	{
		return (v < lo) ? lo : (hi < v) ? hi : v;
	}

	// Clamp 함수 (대소문자 구분)
	template <typename T>
	constexpr const T& Clamp(const T& v, const T& lo, const T& hi) 
	{
		return (v < lo) ? lo : (hi < v) ? hi : v;
	}

	// Lerp 함수 (선형 보간)
	template <typename T>
	constexpr T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}

	// EaseInOut
	inline float EaseInOut(float a, float b, float elapsedTime, float duration) {
		// 보간 계수 t 계산
		float t = elapsedTime / duration;
		t = clamp(t, 0.0f, 1.0f); // 범위 제한

		// EaseInOut 적용 (SmoothStep)
		float smoothT = t * t * (3.0f - 2.0f * t);

		// 선형 보간
		return a + (b - a) * smoothT;
	}

	// EaseOut
	inline float EaseOut(float a, float b, float elapsedTime, float duration) {
		float t = elapsedTime / duration;
		t = clamp(t, 0.0f, 1.0f); // 범위 제한

		// EaseOut: 느려지는 곡선 (예: 1 - (1 - t)^2)
		float easeT = 1.0f - (1.0f - t) * (1.0f - t);

		return a + (b - a) * easeT;
	}

	// Approximately
	inline bool Approximately(float a, float b, float epsilon = 1e-5f) {
		return std::fabs(a - b) <= epsilon;
	}

	// Deg2Rad
	inline float Deg2Rad(float _deg)
	{
		return (_deg / 180.0f) * Define::PI;
	}

	template<typename T>
	TVector2<T> QuadraticBezier(const TVector2<T>& p0, const TVector2<T>& p1, const TVector2<T>& p2, float t)
	{
		float oneMinusT = 1.0f - t;
		return oneMinusT * oneMinusT * p0 +
			2.0f * oneMinusT * t * p1 +
			t * t * p2;
	}

	// 0..1 범위 보정용
	inline float clamp01(float v) { return (v < 0.f) ? 0.f : (v > 1.f ? 1.f : v); }

	// Quintic Smoothstep (더 부드러운 S-curve)
	inline float smoothstep5(float t) {
		t = clamp01(t);
		return t * t * t * (t * (t * 6.f - 15.f) + 10.f);
	}

	// 주기 t∈[0,1)에서 삼각파(0..1)
	inline float triangle01(float u01) {
		return 1.f - std::fabs(2.f * u01 - 1.f); // 0..1
	}

	// 모양 제어 가능한 펄스 ([-1,1]로 반환)
	// sharpness: 정점 날카로움(>1이면 뾰족, <1이면 둥글), ease: 0..1 (정점/바닥을 얼마나 부드럽게 할지)
	inline float eased_triangle_pulse(float u01, float sharpness = 1.0f, float ease = 0.7f) {
		// 기본 삼각파 (0..1)
		float tri = triangle01(u01);

		// 정점 날카로움 조절 (pow로 형태 제어)
		if (sharpness != 1.0f) {
			tri = std::pow(tri, sharpness);
		}

		// 양쪽 가장자리 부드럽게 (quintic smoothstep로 살짝 라운딩)
		if (ease > 0.f) {
			// ease 비율만큼 tri를 부드럽게 섞음
			float s = smoothstep5(tri);
			tri = (1.f - ease) * tri + ease * s;
		}

		// -1..1로 변환
		return tri * 2.f - 1.f;
	}

	// 2D 벡터 회전 (degree 단위)
	template<typename T>
	inline TVector2<T> Rotate(const TVector2<T>& v, float degree)
	{
		float rad = degree * Define::PI / 180.0f;
		float cs = std::cos(rad);
		float sn = std::sin(rad);
		return TVector2<T>(
			v.x * cs - v.y * sn,
			v.x * sn + v.y * cs
		);
	}
}

using FVector2 = TVector2<float>;
using FVector3 = TVector3<float>;
using FVector4 = TVector4<float>;
using FBox = TBox2<float>;
using FRandom = Math::TRandom<float>;
