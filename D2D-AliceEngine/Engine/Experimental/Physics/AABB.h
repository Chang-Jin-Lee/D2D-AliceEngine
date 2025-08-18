#pragma once

#include <limits>
#include <sstream>
#include <vector>
#include <Math/TMath.h>

namespace Physics
{
	template<typename T>
	struct TAABB
	{
		TVector2<T> minVector = TVector2<T>((std::numeric_limits<T>::max)(), (std::numeric_limits<T>::max)());
		TVector2<T> maxVector = TVector2<T>((std::numeric_limits<T>::lowest)(), (std::numeric_limits<T>::lowest)());

		inline TAABB() :
			minVector((std::numeric_limits<T>::max)()),
			maxVector((std::numeric_limits<T>::lowest)())
		{}
		inline TAABB(std::vector<TVector2<T>>& _vertices)
		{
			for (const TVector2<T>& vertex : _vertices)
			{
				minVector.x = min(minVector.x, vertex.x);
				minVector.y = min(minVector.y, vertex.y);
				maxVector.x = max(maxVector.x, vertex.x);
				maxVector.y = max(maxVector.y, vertex.y);
			}
		};
		// ToString 메서드 수정 (2D 전용)
		inline std::string ToString() const
		{
			std::ostringstream oss;
			oss << "minVector : ( X = " << minVector.x << ", Y = " << minVector.y << " )\n";
			oss << "maxVector : ( X = " << maxVector.x << ", Y = " << maxVector.y << " )\n";
			return oss.str();
		}

		inline bool IsOverlap(const TAABB* aabb)
		{
			if (minVector.x > aabb->maxVector.x ||
				maxVector.x  < aabb->minVector.x ||
				minVector.y >  aabb->maxVector.y ||
				maxVector.y < aabb->minVector.y)
				return false;
			return true;
		}

		inline TVector2<T> GetSize()
		{
			return TVector2<T>(maxVector.x - minVector.x, maxVector.y - minVector.y);
		}
	};
}
using FAABB = Physics::TAABB<float>;
