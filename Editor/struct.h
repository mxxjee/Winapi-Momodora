#pragma once

struct Vec2
{
	float x, y;
public:
	static Vec2 Up() { return { 0.f, -1.f }; }
	static Vec2 Down() { return { 0.f, 1.f }; }
	static Vec2 Left() { return { -1.f, 0.f }; }
	static Vec2 Right() { return { 1.f, 0.f }; }
	static Vec2 Zero() { return { 0.f, 0.f }; }

	float Length()
	{
		return sqrt(x * x + y * y);
	}
	Vec2& Normalize()
	{
		float fLen = Length();

		if (fLen == 0.f)
			return *this;

		x /= fLen;
		y /= fLen;

		return *this;
	}

	static float Distance(const Vec2& _a, const Vec2& _b)
	{
		float dx = _a.x - _b.x;
		float dy = _a.y - _b.y;
		return sqrt(dx * dx + dy * dy);
	}

public:
	void operator -=(float other)
	{
		x -= other;
		y -= other;
	}
	void operator -=(Vec2 other)
	{
		x -= other.x;
		y -= other.y;
	}
	void operator +=(Vec2 other)
	{
		x += other.x;
		y += other.y;
	}
	void operator =(Vec2 other)
	{
		x = other.x;
		y = other.y;
	}
	Vec2 operator +(Vec2 other) const
	{
		return Vec2(x + other.x, y + other.y);
	}
	Vec2 operator +(float other) const
	{
		return Vec2(x + other, y + other);
	}
	Vec2 operator -(Vec2 other) const
	{
		return Vec2(x - other.x, y - other.y);
	}
	Vec2 operator *(Vec2 other)  const
	{
		return Vec2(x * other.x, y * other.y);
	}
	Vec2 operator *(float f) const
	{
		return Vec2(x * f, y * f);
	}

	Vec2 operator / (float f) const
	{
		return Vec2(x / f, y / f);
	}
	Vec2& operator*=(float f) {
		x *= f;
		y *= f;
		return *this;
	}
	Vec2 operator / (Vec2 other)
	{
		//0나누기 방지
		if (other.x == 0.f && other.y == 0.f)
			return Vec2(x / other.x, y / other.y);
	}
public:
	Vec2()
		:x(0.f)
		, y(0.f)
	{
	}
	Vec2(float _x, float _y)
		:x(_x)
		, y(_y)
	{
	}
	Vec2(int _x, int _y)	//explicit으로 암묵적 형변환 방지
		:x((float)_x)
		, y((float)_y)
	{
	}
	Vec2(UINT _x, UINT _y)	//explicit으로 암묵적 형변환 방지
		:x((float)_x)
		, y((float)_y)
	{
	}
};