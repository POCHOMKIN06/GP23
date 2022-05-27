#pragma once

#include <type_traits>

enum class MOVE_TYPE {
	Constant_Velocity,
	Constant_Acceleration,
	Free_Fall,
	Vertical_Throw_Up,
	Oblique_Throw,
	END
};

//operator
//前方インクリメント
inline MOVE_TYPE& operator ++ (MOVE_TYPE& type)
{
	if (static_cast<unsigned int>(type) >= sizeof(MOVE_TYPE)) {
		return type;
	}

	type = MOVE_TYPE(static_cast<std::underlying_type<MOVE_TYPE>::type>(type) + 1);
	return type;
}
inline MOVE_TYPE& operator -- (MOVE_TYPE& type)
{
	if (static_cast<unsigned int>(type) <= 0) {
		return type;
	}

	type = MOVE_TYPE(static_cast<std::underlying_type<MOVE_TYPE>::type>(type) - 1);
	return type;
}
//後方インクメント
inline MOVE_TYPE operator ++ (MOVE_TYPE& type, int)
{
	MOVE_TYPE result = type;
	++result;

	return result;
}
inline MOVE_TYPE operator -- (MOVE_TYPE& type, int)
{
	MOVE_TYPE result = type;
	--result;

	return result;
}
