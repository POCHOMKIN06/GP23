//#include "MoveType.h"
//
//#include <iostream>
//#include <cstdio>
//
//MOVE_TYPE& operator ++ (MOVE_TYPE& type)
//{
//	if (static_cast<unsigned int>(type) >= sizeof(MOVE_TYPE) - 1) {
//		return type;
//	}
//
//
//	//return static_cast<MOVE_TYPE>(static_cast<unsigned int>(type) + 1);
//	type =  MOVE_TYPE(static_cast<std::underlying_type<MOVE_TYPE>::type>(type) + 1);
//	return type;
//	//switch (type) {
//	//case MOVE_TYPE::Constant_Velocity:
//	//	return type = MOVE_TYPE::Constant_Acceleration;
//	//	break;
//	//case MOVE_TYPE::Constant_Acceleration:
//	//	return type = MOVE_TYPE::Free_Fall;
//	//	break;
//	//case MOVE_TYPE::Free_Fall:
//	//	return type = MOVE_TYPE::Vertical_Throw_Up;
//	//	break;
//	//case MOVE_TYPE::Vertical_Throw_Up:
//	//	return type = MOVE_TYPE::Oblique_Throw;
//	//	break;
//	//case MOVE_TYPE::Oblique_Throw:
//	//	break;
//	//default:
//	//	return type = MOVE_TYPE::Constant_Acceleration;
//	//	break;
//	//}
//}
//
////MOVE_TYPE& operator--(MOVE_TYPE& type)
////{
////	if (static_cast<unsigned int>(type) <= 0) {
////		return type;
////	}
////
////	type = static_cast<MOVE_TYPE>(static_cast<unsigned int>(type) - 1);
////	return type;
////
////	//switch (type) {
////	//case MOVE_TYPE::Constant_Velocity:
////	//	break;
////	//case MOVE_TYPE::Constant_Acceleration:
////	//	return type = MOVE_TYPE::Constant_Velocity;
////	//	break;
////	//case MOVE_TYPE::Free_Fall:
////	//	return type = MOVE_TYPE::Constant_Acceleration;
////	//	break;
////	//case MOVE_TYPE::Vertical_Throw_Up:
////	//	return type = MOVE_TYPE::Free_Fall;
////	//	break;
////	//case MOVE_TYPE::Oblique_Throw:
////	//	return type = MOVE_TYPE::Vertical_Throw_Up;
////	//	break;
////	//default:
////	//	return type = MOVE_TYPE::Constant_Velocity;
////	//	break;
////	//}
////}