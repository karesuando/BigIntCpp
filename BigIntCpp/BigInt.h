#pragma once

#include <iostream>

using namespace std;

// class BigInt<int Size>
//
// Class for handling arithmetic operations on integers of arbitrary size.
//
template <int Size>
class BigInt
{
public:
	BigInt();
	BigInt(long Tmp);
	bool operator> (long Int) const;
	bool operator< (long Int) const;
	bool operator== (long Int) const;
	bool operator<= (long Int) const;
	bool operator>= (long Int) const;
	bool operator!= (long Int) const;
	bool operator> (const BigInt& Int) const;
	bool operator< (const BigInt& Int) const;
	bool operator== (const BigInt& Int) const;
	bool operator<= (const BigInt& Int) const;
	bool operator>= (const BigInt& Int) const;
	bool operator!= (const BigInt& Int) const;
	BigInt operator- () const;
	BigInt operator+ (const BigInt& Int) const;
	BigInt operator- (const BigInt& Int) const;
	BigInt operator* (const BigInt& Int) const;
	BigInt operator/ (const BigInt& Int) const;
	BigInt operator% (const BigInt& Int) const;
	long operator% (long Int) const;
	BigInt operator+ (long Int) const;
	BigInt operator- (long Int) const;
	BigInt operator* (long Int) const;
	BigInt operator| (const BigInt& Int) const;
	BigInt operator& (const BigInt& Int) const;
	BigInt operator^ (const BigInt& Int) const;
	BigInt operator~ () const;
	long operator& (long Int) const;
	BigInt operator<< (int Bits) const;
	BigInt operator>> (int Bits) const;
	BigInt& operator= (const BigInt& Int);
	BigInt& operator= (long Int);
	void operator += (const BigInt& Int);
	void operator -= (const BigInt& Int);
	void operator *= (const BigInt& Int);
	void operator /= (const BigInt& Int);
	void operator %= (const BigInt& Int);
	void operator |= (const BigInt& Int);
	void operator &= (const BigInt& Int);
	void operator ^= (const BigInt& Int);
	void operator += (long Int);
	void operator -= (long Int);
	void operator *= (long Int);
	void operator>>= (int Bits);
	void operator<<= (int Bits);
	const BigInt& operator++();
	BigInt operator++(int);
	const BigInt& operator--();
	BigInt operator--(int);
	void Div(const BigInt& Denom, BigInt& Quot, BigInt& Rem) const;
	BigInt Power(unsigned long) const;
	BigInt MulMod(const BigInt& b, const BigInt& m);
	BigInt PowerMod(const BigInt& e, const BigInt& n);
	BigInt PowerMod(const long e, const BigInt& n);
	void UDiv(const BigInt& Denom, BigInt& Quot, BigInt& Rem) const;
	bool XGCD(BigInt b, BigInt& x0, BigInt& y0);
	string ToString() const;
	unsigned short GetUShort() const;
	void SetBit(int BitPos);
	int GetBit(int BitPos) const;
	int IsNeg() const;
	int IsZero() const;
	void Abs();
	unsigned short GetShort() const;
	int IsOdd() const;
	unsigned int BitCount(int Start = Size - 1) const;
	const unsigned short *GetData() const;

private:
	template <int T>
	friend ostream& operator<< (ostream& OutStream, const BigInt<T>& Int);
	friend istream& operator>> (istream& InStream, BigInt& Int);
	friend string BigIntToString(BigInt Int);
	friend bool XGCD(BigInt a,BigInt b,BigInt& x0,BigInt& y0);
	friend BigInt HexStringToBigInt(const char Str[]);
	friend BigInt Abs(const BigInt& a);
	friend BigInt PowerMod(const BigInt&, long, const BigInt&);
	friend BigInt MulMod (const BigInt&, const BigInt&);

	void RShOne();
	void RSh(int NumBits);
	void LSh(int NumBits);	
	void TwosComplement();
	void Add(long Int);
	void Sub(long Int);
	void Mul(long Int);
	void Add(const BigInt&);
	void Sub(const BigInt&);
	void XOr(const BigInt&);	
	void IOr(const BigInt&);
	void And(const BigInt&);
	void PrintHexNum(ostream&) const;
	void PrintOctNum(ostream&) const;
	void PrintDecNum(ostream&) const;
	unsigned int NumDigits() const;
	int Compare(const BigInt& Int) const;
	int UCompare(const BigInt& Int) const;

	// The number is stored as an array of ushort.
	unsigned short m_Digits[Size];
};

template <int Size>
inline unsigned short BigInt<Size>::GetUShort() const
{
	return m_Digits[0];
}

template <int Size>
inline int BigInt<Size>::IsNeg() const
{
	return m_Digits[Size - 1] & 0x8000;
}

template <int Size>
inline int BigInt<Size>::IsOdd() const
{
	return m_Digits[0] & 1;
}

template <int Size>
inline BigInt<Size> operator+ (long Int, const BigInt<Size>& Num)
{
	return Num + Int;
}

template <int Size>
inline BigInt<Size> operator- (long Int, const BigInt<Size>& Num)
{
	return -(Num - Int);
}

template <int Size>
inline BigInt<Size> operator* (long Int, const BigInt<Size>& Num)
{
	return Num * Int;
}

template <int Size>
inline BigInt<Size> operator/ (long Int, const BigInt<Size>& Den)
{
	BigInt Num;

	Num = Int;
	return Num / Den;
}

template <int Size>
inline bool operator> (long Int, const BigInt<Size>& Num)
{
	return Num < Int;
}
template <int Size>
inline bool operator< (long Int, const BigInt<Size>& Num)
{
	return Num > Int;
}

template <int Size>
inline bool operator== (long Int, const BigInt<Size>& Num)
{
	return Num == Int;
}

template <int Size>
inline bool operator<= (long Int, const BigInt<Size>& Num)
{
	return ! (Int > Num);
}

template <int Size>
inline bool operator>= (long Int, const BigInt<Size>& Num)
{
	return ! (Int < Num);
}

template <int Size>
inline bool BigInt<Size>::operator< (const BigInt<Size>& Int) const
{
	return Compare(Int) == -1;
}

template <int Size>
inline bool BigInt<Size>::operator> (const BigInt<Size>& Int) const
{
	return Compare(Int) == 1;
}

template <int Size>
inline bool BigInt<Size>::operator>= (const BigInt<Size>& Int) const
{
	return ! (*this < Int);
}

template <int Size>
inline bool BigInt<Size>::operator<= (const BigInt<Size>& Int) const
{
	return ! (*this > Int);
}

template <int Size>
inline bool BigInt<Size>::operator>= (long Int) const
{
	return ! (*this < Int);
}

template <int Size>
inline bool BigInt<Size>::operator<= (long Int) const
{
	return ! (*this > Int);
}

template <int Size>
inline bool BigInt<Size>::operator!= (const BigInt<Size>& Int) const
{
	return ! (*this == Int);
}

template <int Size>
inline bool BigInt<Size>::operator != (long Int) const
{
	return ! (*this == Int);
}

template <int Size>
inline bool BigInt<Size>::operator== (const BigInt<Size>& Int) const
{
	return memcmp(m_Digits, Int.m_Digits, sizeof(m_Digits)) == 0;
}

template <int Size>
inline bool operator!= (long Int, const BigInt<Size>& Num)
{
	return Num != Int;
}

template <int Size>
inline void BigInt<Size>::operator += (const BigInt<Size>& Int)
{
	Add(Int);
}

template <int Size>
inline void BigInt<Size>::operator -= (const BigInt<Size>& Int)
{
	Sub(Int);
}

template <int Size>
inline void BigInt<Size>::operator /= (const BigInt<Size>& Int)
{
	*this = *this / Int;
}

template <int Size>
inline void BigInt<Size>::operator %= (const BigInt<Size>& Int)
{
	*this = *this % Int;
}

template <int Size>
inline void BigInt<Size>::operator *= (const BigInt<Size>& Int)
{
	*this = *this * Int;
}

template <int Size>
inline void BigInt<Size>::operator += (long Int)
{
	Add(Int);
}

template <int Size>
inline void BigInt<Size>::operator -= (long Int)
{
	Sub(Int);
}

template <int Size>
inline BigInt<Size> operator& (long Int, const BigInt<Size>& Tmp)
{
	return Tmp & Int;
}

template <int Size>
inline BigInt<Size> operator^ (long Int, const BigInt<Size>& Tmp)
{
	return Tmp ^ Int;
}

template <int Size>
inline BigInt<Size> operator| (long Int, const BigInt<Size>& Tmp)
{
	return Tmp | Int;
}

template <int Size>
inline void BigInt<Size>::operator |= (const BigInt<Size>& Int)
{
	IOr(Int);
}

template <int Size>
inline void BigInt<Size>::operator &= (const BigInt<Size>& Int)
{
	And(Int);
}

template <int Size>
inline void BigInt<Size>::operator ^= (const BigInt<Size>& Int)
{
	XOr(Int);
}

template <int Size>
inline void BigInt<Size>::operator>>= (int Bits)
{
	RSh(Bits);
}

template <int Size>
inline void BigInt<Size>::operator<<= (int Bits)
{
	LSh(Bits);
}

template <int Size>
inline const unsigned short *BigInt<Size>::GetData() const
{
	return m_Digits;
}

template <int Size>
inline int BigInt<Size>::IsZero() const
{
	const unsigned short *Digit = m_Digits;
	for (int i = Size - 1; i >= 0; i--)
		if (*Digit++)
			return 0;
	return 1;
}

template <int Size>
inline int BigInt<Size>::UCompare(const BigInt<Size>& Int) const
{
	unsigned short a,b;
	register const unsigned short *ThisDigit = m_Digits + Size - 1;
	register const unsigned short *ThatDigit = Int.m_Digits + Size - 1;

	for (int i = Size; i > 0; i--)
	{
		a = *ThisDigit--;
		b = *ThatDigit--;
		if (a > b)
			return 1;
		else if (a < b)
			return -1;
	}
	return 0;
}

// void BigInt::SetBit(int BitPos)
//
// Inverts the bit at position 'BitPos'.

template <int Size>
inline void BigInt<Size>::SetBit(int BitPos)
{
	int i             = BitPos >> 4;
	unsigned long Bit = BitPos & 0xf;
	if (i < Size)
		m_Digits[i] ^= 1 << Bit;
}

template <int Size>
inline unsigned int BigInt<Size>::BitCount(int Start) const
{
	register int i;

	for (i = Start; i > 0; i--)
		if (m_Digits[i])
			return ::BitCount(m_Digits[i]) + (i << 4);
	return ::BitCount(m_Digits[0]);
}

template <int Size>
inline void BigInt<Size>::Sub(const BigInt<Size>& Int)
{
	BigInt<Size> Tmp;

	Tmp = Int;
	Tmp.TwosComplement();
	Add(Tmp);
}

template <int Size>
inline BigInt<Size> BigInt<Size>::operator+ (const BigInt<Size>& Int) const
{
	BigInt<Size> Tmp;

	Tmp = Int;
	Tmp.Add(*this);
	return Tmp;
}

template <int Size>
inline BigInt<Size> BigInt<Size>::operator<< (int Bits) const
{
	BigInt<Size> Tmp = *this;
	if (Bits < 0)
		Tmp.RSh(-Bits);
	else
		Tmp.LSh(Bits);
	return Tmp;	
}

template <int Size>
inline BigInt<Size> BigInt<Size>::operator>> (int Bits) const
{
	BigInt<Size> Tmp = *this;
	if (Bits < 0)
		Tmp.LSh(-Bits);
	else
		Tmp.RSh(Bits);
	return Tmp;
}

template <int Size>
inline BigInt<Size> BigInt<Size>::operator- () const
{
	BigInt<Size> Tmp = *this;
	Tmp.TwosComplement();
	return Tmp;
}

template <int Size>
inline BigInt<Size> BigInt<Size>::operator- (const BigInt<Size>& Int) const
{
	BigInt<Size> Tmp = *this;
	Tmp.Sub(Int);
	return Tmp;
}

template <int Size>
inline BigInt<Size> BigInt<Size>::operator/ (const BigInt<Size>& Int) const
{
	BigInt<Size> Quot,Rem;

	Div(Int, Quot, Rem);
	return Quot;
}

template <int Size>
inline BigInt<Size> BigInt<Size>::operator+ (long Int) const
{
	BigInt<Size> Tmp = *this;
	Tmp.Add(Int);
	return Tmp;
}

template <int Size>
inline BigInt<Size> BigInt<Size>::operator- (long Int) const
{
	BigInt<Size> Tmp = *this;
	Tmp.Sub(Int);
	return Tmp;
}

template <int Size>
inline BigInt<Size> BigInt<Size>::operator* (long Int) const
{
	BigInt<Size> Tmp = *this;
	Tmp.Mul(Int);
	return Tmp;
}

template <int Size>
BigInt<Size> Abs(const BigInt<Size>& a);
template <int Size>
BigInt<Size> GCD(BigInt<Size> A, BigInt<Size> B);
template <int Size>
BigInt<Size> Power(const BigInt<Size>& a, long e);
template <int Size>
BigInt<Size> Power(const BigInt<Size>& a, const BigInt<Size>& e);
template <int Size>
BigInt<Size> PowerMod(const BigInt<Size>& a, long e, const BigInt<Size>& n);
template <int Size>
BigInt<Size> PowerMod(const BigInt<Size>& a,const BigInt<Size>& e,const BigInt<Size>& n);
template <int Size>
bool XGCD(BigInt<Size> a, BigInt<Size> b, BigInt<Size>& x0, BigInt<Size>& y0);
template <int Size>
bool InvMod(const BigInt<Size>& r,const BigInt<Size>& p,BigInt<Size>& m);
template <int Size>
BigInt<Size> InvMod(const BigInt<Size>& r,const BigInt<Size>& p);
template <int Size>
BigInt<Size> HexStringToBigInt(const char Str[]);
template <int Size>
void BigIntToHexString(BigInt<Size> Int, char Buffer[],size_t BufferSize);
template <int Size>
ostream& operator<< (ostream& OutStream, const BigInt<Size>& Int);
template <int Size>
istream& operator>> (istream& InStream, BigInt<Size>& Int);

class DivisionByZero
{
public:
	DivisionByZero()
	{
		cout << "Exception: Division by zero." << endl;
	}
};

#include "BigInt.cpp"
