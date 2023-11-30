#pragma once
#include <cstring>

namespace sc
{
	template <typename T = float>
	struct Rect
	{
		union
		{
			struct
			{
				T x, y;
				T width, height;
			};

			struct
			{
				T left, top;
				T right, bottom;
			};

			T points[4];
		};

		Rect(const T* arr)
		{
			memcpy(points, arr, sizeof(points));
		};

		Rect(T a = 0.0f, T b = 0.0f, T c = 0.0f, T d = 0.0f)
		{
			x = a;
			y = b;
			width = c;
			height = d;
		};
	};
}