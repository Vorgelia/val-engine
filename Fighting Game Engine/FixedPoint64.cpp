#include "FixedPoint64.h"

const FixedPoint64 FixedPoint64::zero = FromRawBits(Fixed64::Zero);
const FixedPoint64 FixedPoint64::one = FromRawBits(Fixed64::One);
const FixedPoint64 FixedPoint64::half = FromRawBits(Fixed64::Half);

const FixedPoint64 FixedPoint64::pi = FromRawBits(Fixed64::Pi);
const FixedPoint64 FixedPoint64::doublePi = FromRawBits(Fixed64::Pi2);
const FixedPoint64 FixedPoint64::halfPi = FromRawBits(Fixed64::PiHalf);
const FixedPoint64 FixedPoint64::e = FromRawBits(Fixed64::E);

const FixedPoint64 FixedPoint64::minValue = FromRawBits(Fixed64::MinValue);
const FixedPoint64 FixedPoint64::maxValue = FromRawBits(Fixed64::MaxValue);

FixedPoint64 FixedPoint64::operator+(const FixedPoint64& rhs) const
{
	return FromRawBits(_bits) += rhs;
}

FixedPoint64 FixedPoint64::operator-(const FixedPoint64& rhs) const
{
	return FromRawBits(_bits) -= rhs;
}

FixedPoint64 FixedPoint64::operator*(const FixedPoint64& rhs) const
{
	return FromRawBits(_bits) *= rhs;
}

FixedPoint64 FixedPoint64::operator/(const FixedPoint64& rhs) const
{
	return FromRawBits(_bits) /= rhs;
}

FixedPoint64 FixedPoint64::operator%(const FixedPoint64& rhs) const
{
	return FromRawBits(_bits) %= rhs;
}

FixedPoint64& FixedPoint64::operator+=(const FixedPoint64& rhs)
{
	_bits = Fixed64::Add(_bits, rhs._bits);
	return *this;
}

FixedPoint64& FixedPoint64::operator-=(const FixedPoint64& rhs)
{
	_bits = Fixed64::Sub(_bits, rhs._bits);
	return *this;
}

FixedPoint64& FixedPoint64::operator*=(const FixedPoint64& rhs)
{
	_bits = Fixed64::Mul(_bits, rhs._bits);
	return *this;
}

FixedPoint64& FixedPoint64::operator/=(const FixedPoint64& rhs)
{
	_bits = Fixed64::DivPrecise(_bits, rhs._bits);
	return *this;
}

FixedPoint64& FixedPoint64::operator%=(const FixedPoint64& rhs)
{
	_bits = Fixed64::Mod(_bits, rhs._bits);
	return *this;
}

FixedPoint64 FixedPoint64::operator|(const FixedPoint64& rhs) const
{
	return FromRawBits(_bits) |= rhs;
}

FixedPoint64 FixedPoint64::operator&(const FixedPoint64& rhs) const
{
	return FromRawBits(_bits) &= rhs;
}

FixedPoint64 FixedPoint64::operator^(const FixedPoint64& rhs) const
{
	return FromRawBits(_bits) ^= rhs;
}

FixedPoint64& FixedPoint64::operator|=(const FixedPoint64& rhs)
{
	_bits |= rhs._bits;
	return *this;
}

FixedPoint64& FixedPoint64::operator&=(const FixedPoint64& rhs)
{
	_bits &= rhs._bits;
	return *this;
}

FixedPoint64& FixedPoint64::operator^=(const FixedPoint64& rhs)
{
	_bits ^= rhs._bits;
	return *this;
}

FixedPoint64 FixedPoint64::operator<<(int rhs) const
{
	return FromRawBits(_bits << rhs);
}

FixedPoint64 FixedPoint64::operator>>(int rhs) const
{
	return FromRawBits(_bits >> rhs);
}

FixedPoint64 FixedPoint64::operator-() const
{
	return FromRawBits(-_bits);
}

FixedPoint64 FixedPoint64::operator~() const
{
	return FromRawBits(~_bits);
}

bool FixedPoint64::operator==(const FixedPoint64& rhs) const
{
	return _bits == rhs._bits;
}

bool FixedPoint64::operator!=(const FixedPoint64& rhs) const
{
	return _bits != rhs._bits;
}

bool FixedPoint64::operator>(const FixedPoint64& rhs) const
{
	return _bits > rhs._bits;
}

bool FixedPoint64::operator<(const FixedPoint64& rhs) const
{
	return _bits < rhs._bits;
}

bool FixedPoint64::operator>=(const FixedPoint64& rhs) const
{
	return _bits >= rhs._bits;
}

bool FixedPoint64::operator<=(const FixedPoint64& rhs) const
{
	return _bits <= rhs._bits;
}

FixedPoint64::operator bool() const
{
	return _bits != 0;
}

FixedPoint64::operator int() const
{
	return Fixed64::FloorToInt(_bits);
}

FixedPoint64::operator float() const
{
	return Fixed64::ToFloat(_bits);
}

FixedPoint64::operator double() const
{
	return Fixed64::ToDouble(_bits);
}

FixedPoint64 FixedPoint64::Abs(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Abs(lhs._bits));
}

FixedPoint64 FixedPoint64::Round(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Round(lhs._bits));
}

FixedPoint64 FixedPoint64::Floor(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Floor(lhs._bits));
}

FixedPoint64 FixedPoint64::Ceil(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Ceil(lhs._bits));
}

int FixedPoint64::RoundToInt(const FixedPoint64 & lhs)
{
	return Fixed64::RoundToInt(lhs._bits);
}

int FixedPoint64::FloorToInt(const FixedPoint64& lhs)
{
	return Fixed64::FloorToInt(lhs._bits);
}

int FixedPoint64::CeilToInt(const FixedPoint64& lhs)
{
	return Fixed64::FloorToInt(lhs._bits);
}

FixedPoint64 FixedPoint64::Sqrt(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Sqrt(lhs._bits));
}

FixedPoint64 FixedPoint64::RSqrt(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::RSqrt(lhs._bits));
}

FixedPoint64 FixedPoint64::Exp(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Exp(lhs._bits));
}

FixedPoint64 FixedPoint64::Exp2(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Exp2(lhs._bits));
}

FixedPoint64 FixedPoint64::Log(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Log(lhs._bits));
}

FixedPoint64 FixedPoint64::Log2(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Log2(lhs._bits));
}

FixedPoint64 FixedPoint64::Pow(const FixedPoint64& lhs, const FixedPoint64& rhs)
{
	return FromRawBits(Fixed64::Pow(lhs._bits, rhs._bits));
}

FixedPoint64 FixedPoint64::Sin(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Sin(lhs._bits));
}

FixedPoint64 FixedPoint64::Cos(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Cos(lhs._bits));
}

FixedPoint64 FixedPoint64::Tan(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Tan(lhs._bits));
}

FixedPoint64 FixedPoint64::Asin(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Asin(lhs._bits));
}

FixedPoint64 FixedPoint64::Acos(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Acos(lhs._bits));
}

FixedPoint64 FixedPoint64::Atan(const FixedPoint64& lhs)
{
	return FromRawBits(Fixed64::Atan(lhs._bits));
}

FixedPoint64 FixedPoint64::Atan2(const FixedPoint64& lhs, const FixedPoint64& rhs)
{
	return FromRawBits(Fixed64::Atan2(lhs._bits, rhs._bits));
}

FixedPoint64 FixedPoint64::FromRawBits(std::int64_t bits)
{
	FixedPoint64 fixed;
	fixed._bits = bits;
	return fixed;
}

//TODO: Slow and bad bad bad. Come up with something better.
FixedPoint64 FixedPoint64::FromString(const std::string & str)
{
	std::int32_t preDecimalNumber = 0;
	std::int32_t postDecimalNumber = 0;

	int integerBase = 1;
	int decimalBase = 1;

	bool isPreDecimal = true;
	bool isNegative = false;

	for(int i = str.length() - 1; i >= 0; --i)
	{
		const unsigned char number = str[i];
		if(number == '-')
		{
			isNegative = true;
			break;
		}

		if(number == '.')
		{
			isPreDecimal = false;
			continue;
		}
		
		if(number < '0' || number > '9')
		{
			break;
		}

		if(isPreDecimal)
		{
			preDecimalNumber += decimalBase * (number - '0');
			decimalBase *= 10;
		}
		else
		{
			postDecimalNumber += integerBase * (number - '0');
			integerBase *= 10;
		}
	}

	if(isPreDecimal)
	{
		postDecimalNumber = preDecimalNumber;
		preDecimalNumber = 0;
	}

	if(isNegative)
	{
		postDecimalNumber = -postDecimalNumber;
		preDecimalNumber = -preDecimalNumber;
	}

	FixedPoint64 fixedNum = FixedPoint64(postDecimalNumber);
	if(preDecimalNumber != 0)
	{
		fixedNum += FixedPoint64(preDecimalNumber) / FixedPoint64(decimalBase);
	}

	return fixedNum;
}

FixedPoint64::FixedPoint64(int value)
	: _bits(Fixed64::FromInt(value))
{
}

FixedPoint64::FixedPoint64(float value)
	: _bits(Fixed64::FromFloat(value))
{
}

FixedPoint64::FixedPoint64(double value)
	: _bits(Fixed64::FromDouble(value))
{
}

FixedPoint64::FixedPoint64()
	: FixedPoint64(zero)
{
}
