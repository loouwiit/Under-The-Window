#include "Operator.h"
template <typename T1, typename T2>
sf::Vector2<T1> operator-(sf::Vector2<T1> Sub1, const sf::Vector2<T2> Sub2)
{
	Sub1.x -= (T1)Sub2.x;
	Sub1.y -= (T1)Sub2.y;
	return Sub1;
}
template <typename T1, typename T2>
sf::Vector2<T1> operator+(sf::Vector2<T1> Add1, const sf::Vector2<T2> Add2)
{
	Add1.x += (T1)Add2.x;
	Add1.y += (T1)Add2.y;
	return Add1;
}
template <typename T1, typename T2>
sf::Vector2<T1> operator*(sf::Vector2<T1> Mul1, const T2 Mul2)
{
	Mul1.x *= (T1)Mul2.x;
	Mul1.y *= (T1)Mul2.y;
	return Mul1;
}
template <typename T1, typename T2>
sf::Vector2<T1> operator/(sf::Vector2<T1> Div1, const T2 Div2)
{
	Div1.x /= (T1)Div2.x;
	Div1.y /= (T1)Div2.y;
	return Div1;
}

RECT operator-(RECT Sub1, const RECT Sub2)
{
	Sub1.top -= Sub2.top;
	Sub1.bottom -= Sub2.bottom;
	Sub1.left -= Sub2.left;
	Sub1.right -= Sub2.right;
	return Sub1;
}
RECT& operator-=(RECT& Sub1, const RECT Sub2)
{
	Sub1.top -= Sub2.top;
	Sub1.bottom -= Sub2.bottom;
	Sub1.left -= Sub2.left;
	Sub1.right -= Sub2.right;
	return Sub1;
}
RECT operator-(RECT Sub1, const RECT* Sub2)
{
	Sub1.top -= Sub2->top;
	Sub1.bottom -= Sub2->top;
	Sub1.left -= Sub2->left;
	Sub1.right -= Sub2->left;
	return Sub1;
}
RECT& operator-=(RECT &Sub1, const RECT* Sub2)
{
	Sub1.top -= Sub2->top;
	Sub1.bottom -= Sub2->top;
	Sub1.left -= Sub2->left;
	Sub1.right -= Sub2->left;
	return Sub1;
}

//HWND& operator<<=(HWND& Hwnd, const MOVE_INT Move_Int)
//{
//	long long HWND_L = (long long)Hwnd;
//	HWND_L <<= Move_Int.Move_Int;
//	Hwnd = (HWND)HWND_L;
//	return Hwnd;
//}
//HWND& operator>>=(HWND& Hwnd, const MOVE_INT Move_Int)
//{
//	long long HWND_L = (long long)Hwnd;
//	HWND_L >>= Move_Int.Move_Int;
//	Hwnd = (HWND)HWND_L;
//	return Hwnd;
//}
//HWND& operator*=(HWND& Hwnd, const MOVE_INT Move_Int)
//{
//	long long HWND_L = (long long)Hwnd;
//	HWND_L *= Move_Int.Move_Int;
//	Hwnd = (HWND)HWND_L;
//	return Hwnd;
//}
//HWND& operator/=(HWND& Hwnd, const MOVE_INT Move_Int)
//{
//	long long HWND_L = (long long)Hwnd;
//	HWND_L /= Move_Int.Move_Int;
//	Hwnd = (HWND)HWND_L;
//	return Hwnd;
//}
//HWND& operator+=(HWND& Hwnd, const MOVE_INT Move_Int)
//{
//	long long HWND_L = (long long)Hwnd;
//	HWND_L += Move_Int.Move_Int;
//	Hwnd = (HWND)HWND_L;
//	return Hwnd;
//}