//#include "MoveType.h"
//
//#include <cstdio>
//#include <iostream>

//operator
//前方インクリメント
//MOVE_TYPE& operator ++ (MOVE_TYPE& type)
//{
//	if (static_cast<unsigned int>(type) >= sizeof(MOVE_TYPE)) {
//		return type;
//	}
//
//	type = MOVE_TYPE(static_cast<std::underlying_type<MOVE_TYPE>::type>(type) + 1);
//	return type;
//}
//MOVE_TYPE& operator--(MOVE_TYPE& type)
//{
//	if (static_cast<unsigned int>(type) <= 0) {
//		return type;
//	}
//
//	type = MOVE_TYPE(static_cast<std::underlying_type<MOVE_TYPE>::type>(type) - 1);
//	return type;
//}
////後方インクリメント
//MOVE_TYPE operator ++ (MOVE_TYPE& type, int)
//{
//	MOVE_TYPE result = type;
//	++type;
//	return result;
//}
//MOVE_TYPE operator -- (MOVE_TYPE& type, int)
//{
//	MOVE_TYPE result = type;
//	--type;
//	return result;
//}
//
