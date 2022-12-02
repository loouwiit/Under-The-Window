#pragma once
#include <Windows.h>
#include "SFML/System.hpp"

#pragma region SFML_Vector
template <typename T1,typename T2>
sf::Vector2<T1> operator-(sf::Vector2<T1> Sub1, const sf::Vector2<T2> Sub2);
template <typename T1, typename T2>
sf::Vector2<T1> operator+(sf::Vector2<T1> Add1, const sf::Vector2<T2> Add2);
template <typename T1, typename T2>
sf::Vector2<T1> operator*(sf::Vector2<T1> Mul1, const T2 Mul2);
template <typename T1, typename T2>
sf::Vector2<T1> operator/(sf::Vector2<T1> Div1, const T2 Div2);
#pragma endregion

//struct MOVE_INT
//{
//	MOVE_INT()
//	{
//		Move_Int = 0;
//	}
//	MOVE_INT(const int Move_Int)
//	{
//		this->Move_Int = Move_Int;
//	}
//	int Move_Int = 0;
//};

RECT operator-(RECT Sub1, const RECT Sub2);//¼õ·¨
RECT& operator-=(RECT& Sub1, const RECT Sub2);
RECT operator-(RECT Sub1, const RECT* Sub2);//Æ«ÒÆ¼õ·¨
RECT& operator-=(RECT& Sub1, const RECT* Sub2);

//HWND& operator<<=(HWND& Hwnd, const MOVE_INT Move_Int);
//HWND& operator>>=(HWND& Hwnd, const MOVE_INT Move_Int);
//HWND& operator*=(HWND& Hwnd, const MOVE_INT Move_Int);
//HWND& operator/=(HWND& Hwnd, const MOVE_INT Move_Int);
//HWND& operator+=(HWND& Hwnd, const MOVE_INT Move_Int);
