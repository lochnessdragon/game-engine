#pragma once

namespace Math {
	template <typename T> int Sign(T a) {
		return (T(0) < a) - (a < T(0));
	}
}