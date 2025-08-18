#pragma once

#include <math.h>
#include <algorithm>

template<typename T>
class TColor
{
public:
	T r, g, b, a;

	// 기본 생성자 (반투명 검정색)
	TColor() : r(0), g(0), b(0), a(255) {}

	// RGB 생성자 (알파 기본값: 255)
	TColor(T _r, T _g, T _b, T _a = 0) : r(_r), g(_g), b(_b), a(_a) {}

	// 단일 값 생성자 (그레이스케일)
	TColor(T _all) : r(_all), g(_all), b(_all), a(_all) {}

	// 복사 생성자
	TColor(const TColor<T>& other) = default;


	// 연산자 오버로딩
	TColor operator*(const float& scalar) const {
		return TColor(r * scalar, g * scalar, b * scalar, a * scalar);
	}

	TColor operator/(const float& scalar) const {
		return TColor(r / scalar, g / scalar, b / scalar, a / scalar);
	}

	TColor operator+(const TColor& rhs) const {
		return TColor(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
	}

	TColor operator-(const TColor& rhs) const {
		return TColor(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
	}

	// 비교 연산자
	bool operator==(const TColor& rhs) const {
		return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
	}

	bool operator!=(const TColor& rhs) const {
		return !(*this == rhs);
	}

	// 복합 할당 연산자
	void operator+=(const TColor& rhs) {
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
	}

	void operator-=(const TColor& rhs) {
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		a -= rhs.a;
	}

	void operator*=(float scalar) {
		r *= scalar;
		g *= scalar;
		b *= scalar;
		a *= scalar;
	}

	template<typename T>
	T clamp(const T& v, const T& lo, const T& hi) {
		return (v < lo) ? lo : (hi < v) ? hi : v;
	}

	// 클램핑 함수 (0~255 범위 제한)
	TColor Clamp() const {
		return TColor(
			clamp(r, T(0), T(255)),
			clamp(g, T(0), T(255)),
			clamp(b, T(0), T(255)),
			clamp(a, T(0), T(255))
		);
	}

	// 선형 보간
	static TColor Lerp(const TColor& A, const TColor& B, float Alpha) {
		return A * (1.0f - Alpha) + B * Alpha;
	}

	// 미리 정의된 색상들 (static const)
	static const TColor<T> Red;
	static const TColor<T> Green;
	static const TColor<T> Blue;
	static const TColor<T> White;
	static const TColor<T> Black;
	static const TColor<T> Yellow;
	static const TColor<T> Cyan;
	static const TColor<T> Magenta;
	static const TColor<T> Gray;
	static const TColor<T> LightGray;
	static const TColor<T> DarkGray;
	static const TColor<T> Orange;
	static const TColor<T> Pink;
	static const TColor<T> Purple;
	static const TColor<T> Brown;
	static const TColor<T> Lime;
	static const TColor<T> Navy;
	static const TColor<T> Teal;
	static const TColor<T> Olive;
	static const TColor<T> Maroon;
	static const TColor<T> Silver;
	static const TColor<T> Gold;
	static const TColor<T> Transparent;
};
// static 멤버 정의
template<typename T> const TColor<T> TColor<T>::Red = TColor<T>(255, 0, 0, 255);
template<typename T> const TColor<T> TColor<T>::Green = TColor<T>(0, 255, 0, 255);
template<typename T> const TColor<T> TColor<T>::Blue = TColor<T>(0, 0, 255, 255);
template<typename T> const TColor<T> TColor<T>::White = TColor<T>(255, 255, 255, 255);
template<typename T> const TColor<T> TColor<T>::Black = TColor<T>(0, 0, 0, 255);
template<typename T> const TColor<T> TColor<T>::Yellow = TColor<T>(255, 255, 0, 255);
template<typename T> const TColor<T> TColor<T>::Cyan = TColor<T>(0, 255, 255, 255);
template<typename T> const TColor<T> TColor<T>::Magenta = TColor<T>(255, 0, 255, 255);
template<typename T> const TColor<T> TColor<T>::Gray = TColor<T>(128, 128, 128, 255);
template<typename T> const TColor<T> TColor<T>::LightGray = TColor<T>(191, 191, 191, 255);
template<typename T> const TColor<T> TColor<T>::DarkGray = TColor<T>(64, 64, 64, 255);
template<typename T> const TColor<T> TColor<T>::Orange = TColor<T>(255, 128, 0, 255);
template<typename T> const TColor<T> TColor<T>::Pink = TColor<T>(255, 191, 204, 255);
template<typename T> const TColor<T> TColor<T>::Purple = TColor<T>(128, 0, 128, 255);
template<typename T> const TColor<T> TColor<T>::Brown = TColor<T>(153, 77, 0, 255);
template<typename T> const TColor<T> TColor<T>::Lime = TColor<T>(191, 255, 0, 255);
template<typename T> const TColor<T> TColor<T>::Navy = TColor<T>(0, 0, 128, 255);
template<typename T> const TColor<T> TColor<T>::Teal = TColor<T>(0, 128, 128, 255);
template<typename T> const TColor<T> TColor<T>::Olive = TColor<T>(128, 128, 0, 255);
template<typename T> const TColor<T> TColor<T>::Maroon = TColor<T>(128, 0, 0, 255);
template<typename T> const TColor<T> TColor<T>::Silver = TColor<T>(191, 191, 191, 255);
template<typename T> const TColor<T> TColor<T>::Gold = TColor<T>(255, 214, 0, 255);
template<typename T> const TColor<T> TColor<T>::Transparent = TColor<T>(0, 0, 0, 0);

using FColor = TColor<uint8_t>;
