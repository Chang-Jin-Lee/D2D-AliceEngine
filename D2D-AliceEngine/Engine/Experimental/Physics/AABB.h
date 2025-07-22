#pragma once


namespace Physics
{
	template<typename T>
	struct TAABB
	{
		TVector2<T> minVector = TVector2<T>(FLT_MAX, FLT_MAX);
		TVector2<T> maxVector = TVector2<T>(FLT_MIN, FLT_MIN);

		//float x, y;
		inline TAABB() : minVector(FLT_MAX), maxVector(FLT_MIN) {}
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
		// ToString 메서드 수정
		// 보여주려고 만든거.
		inline std::string ToString() const
		{
			std::ostringstream oss;
			oss << "minVector : ( X = " << minVector.x << ", Y = " << minVector.y << ", Z = " << minVector.z << " )\n";
			oss << "maxVector : ( X = " << maxVector.x << ", Y = " << maxVector.y << ", Z = " << maxVector.z << " )\n";
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