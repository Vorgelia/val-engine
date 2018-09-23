#pragma once
#include <Fixed64.h>
#include <cstdint>
#include <string>

//OO Wrapper for Fixed64.
class FixedPoint64
{
private:
	std::int64_t _bits;

public:
	static const FixedPoint64 zero;
	static const FixedPoint64 one;
	static const FixedPoint64 half;

	static const FixedPoint64 pi;
	static const FixedPoint64 doublePi;
	static const FixedPoint64 halfPi;
	static const FixedPoint64 e;

	static const FixedPoint64 minValue;
	static const FixedPoint64 maxValue;

	std::int64_t bits() const;

	FixedPoint64 operator+(const FixedPoint64& rhs) const;
	FixedPoint64 operator-(const FixedPoint64& rhs) const;
	FixedPoint64 operator*(const FixedPoint64& rhs) const;
	FixedPoint64 operator/(const FixedPoint64& rhs) const;
	FixedPoint64 operator%(const FixedPoint64& rhs) const;
	FixedPoint64& operator+=(const FixedPoint64& rhs);
	FixedPoint64& operator-=(const FixedPoint64& rhs);
	FixedPoint64& operator*=(const FixedPoint64& rhs);
	FixedPoint64& operator/=(const FixedPoint64& rhs);
	FixedPoint64& operator%=(const FixedPoint64& rhs);

	FixedPoint64 operator|(const FixedPoint64& rhs) const;
	FixedPoint64 operator&(const FixedPoint64& rhs) const;
	FixedPoint64 operator^(const FixedPoint64& rhs) const;
	FixedPoint64& operator|=(const FixedPoint64& rhs);
	FixedPoint64& operator&=(const FixedPoint64& rhs);
	FixedPoint64& operator^=(const FixedPoint64& rhs);

	FixedPoint64 operator<<(int rhs) const;
	FixedPoint64 operator>>(int rhs) const;

	FixedPoint64 operator-() const;
	FixedPoint64 operator~() const;

	bool operator==(const FixedPoint64& rhs) const;
	bool operator!=(const FixedPoint64& rhs) const;
	bool operator>(const FixedPoint64& rhs) const;
	bool operator<(const FixedPoint64& rhs) const;
	bool operator>=(const FixedPoint64& rhs) const;
	bool operator<=(const FixedPoint64& rhs) const;

	explicit operator bool() const;
	explicit operator int() const;
	explicit operator float() const;
	explicit operator double() const;

	static FixedPoint64 Abs(const FixedPoint64& lhs);

	static FixedPoint64 Round(const FixedPoint64& lhs);
	static FixedPoint64 Floor(const FixedPoint64& lhs);
	static FixedPoint64 Ceil(const FixedPoint64& lhs);
	static int RoundToInt(const FixedPoint64& lhs);
	static int FloorToInt(const FixedPoint64& lhs);
	static int CeilToInt(const FixedPoint64& lhs);

	static FixedPoint64 Sqrt(const FixedPoint64& lhs);
	static FixedPoint64 RSqrt(const FixedPoint64& lhs);

	static FixedPoint64 Exp(const FixedPoint64& lhs);
	static FixedPoint64 Exp2(const FixedPoint64& lhs);
	static FixedPoint64 Log(const FixedPoint64& lhs);
	static FixedPoint64 Log2(const FixedPoint64& lhs);
	static FixedPoint64 Pow(const FixedPoint64& lhs, const FixedPoint64& rhs);

	static FixedPoint64 Sin(const FixedPoint64& lhs);
	static FixedPoint64 Cos(const FixedPoint64& lhs);
	static FixedPoint64 Tan(const FixedPoint64& lhs);
	static FixedPoint64 Asin(const FixedPoint64& lhs);
	static FixedPoint64 Acos(const FixedPoint64& lhs);
	static FixedPoint64 Atan(const FixedPoint64& lhs);
	static FixedPoint64 Atan2(const FixedPoint64& lhs, const FixedPoint64& rhs);

	static FixedPoint64 FromRawBits(std::int64_t bits);
	static FixedPoint64 FromString(const std::string& str);

	FixedPoint64(int value);
	FixedPoint64(float value);
	FixedPoint64(double value);
	FixedPoint64();
};

//TODO: Actually fill in
namespace std
{
	template<>
	class numeric_limits<FixedPoint64> : public numeric_limits<double>{};
}