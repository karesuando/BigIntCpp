#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define NUMBITS_USHORT  16
#define IOSTREAM_BUFSZ 10000

static const char Symbol[] = "0123456789ABCDEF";

static unsigned int BitCount(unsigned short Number)
{
	unsigned int Count = 0;

	while (Number > 0)
	{
		Count++;
		Number >>= 1;
	}
	return Count;
}

static long GetValue(char Sym, long Radix)
{
	long Value;

	Sym = toupper(Sym);
	if (isdigit(Sym))
		Value = Sym - '0';
	else if (Sym < 'A' || Sym > 'F')
		return -1;
	else 
		Value = Sym - 'A' + 10;
	return Value < Radix ? Value : -1;
}

template <int Size>
BigInt<Size>::BigInt()
{
	memset(m_Digits, 0, sizeof(m_Digits));
}

template <int Size>
BigInt<Size>::BigInt(long Int)
{
	unsigned long Tmp = abs(Int);
	memset(m_Digits, 0, sizeof(m_Digits));
	m_Digits[0] = (unsigned short)(Tmp & 0xffff);
	m_Digits[1] = (unsigned short)(Tmp >> 16);
	if (Int < 0)
		TwosComplement();
}

template <int Size>
unsigned short BigInt<Size>::GetShort() const
{
	return m_Digits[0];
}

// int BigInt::GetBit(int BitPos) const
//
// Returns the bit at position 'BitPos'.

template <int Size>
int BigInt<Size>::GetBit(int BitPos) const
{
	int i;
	unsigned long Bit;

	i   = BitPos >> 4;
	Bit = BitPos & 0xf;
	if (i >= Size)
		return -1;
	else
		return m_Digits[i] & (1 << Bit);
}

template <int Size>
unsigned int BigInt<Size>::NumDigits() const
{
	int i;

	for (i = Size - 1; i > 0; i--)
		if (m_Digits[i] != 0)
			return i + 1;
	return 1;
}

template <int Size>
BigInt<Size>& BigInt<Size>::operator= (const BigInt<Size>& Int)
{
	if (&Int != this)
		memcpy(m_Digits, Int.m_Digits, sizeof(m_Digits));
	return *this;
}

template <int Size>
BigInt<Size>& BigInt<Size>::operator= (long Int)
{
	unsigned long Tmp = abs(Int);
	memset(m_Digits, 0, sizeof(m_Digits));
	m_Digits[0] = (unsigned short)(Tmp & 0xffff);
	m_Digits[1] = (unsigned short)(Tmp >> 16);
	if (Int < 0)
		TwosComplement();
	return *this;
}

template <int Size>
void BigInt<Size>::TwosComplement()
{
	int i;
	register unsigned short *Digit;

	i      = Size;
	Digit  = m_Digits;
	while (i > 0 && *Digit == 0)
	{
		Digit++;
		i--;
	}
	if (i > 0)
	{
		*Digit++ = 1 + (~*Digit);
		for (i--; i > 0; i--)
			*Digit++ = ~*Digit;
	}
}

template <int Size>
void BigInt<Size>::Add(const BigInt<Size>& Int)
{
	int i;
	unsigned long Sum,Carry;
	register unsigned short* ThisDigit;
	register const unsigned short *ThatDigit = Int.m_Digits;
	
	Carry     = 0;
	ThisDigit = m_Digits;
	for (i = Size; i > 0; i--) 
	{
		Sum          = Carry + *ThisDigit + *ThatDigit++;
		*ThisDigit++ = (unsigned short)(Sum);
		Carry        = Sum >> NUMBITS_USHORT;
	}
}

template <int Size>
void BigInt<Size>::Add(long Int)
{
	if (Int < 0)
		Sub(-Int);
	else {
		int i;
		unsigned short *ThisDigit;
		unsigned long Sum,Carry;
		
		i            = 1;
		ThisDigit    = m_Digits;
		Sum          = *ThisDigit + Int;
		Carry        = Sum >> NUMBITS_USHORT;
		*ThisDigit   = (unsigned short)(Sum);
		while (Carry > 0 && i < Size) 
		{
			Sum        = Carry + *++ThisDigit;
			*ThisDigit = (unsigned short)(Sum);
			Carry      = Sum >> NUMBITS_USHORT;
			i++;
		}
	}
}

template <int Size>
void BigInt<Size>::Sub(long Int)
{
	if (Int < 0)
		Add(-Int);
	else {
		BigInt<Size> tmp = Int;
		tmp.TwosComplement();
		Add(tmp);
	}
}

template <int Size>
void BigInt<Size>::Mul(long Int)
{
	int i;
	unsigned long Prod,Carry;
	register unsigned short *ThisData = m_Digits;
	const int NumDig                  = NumDigits();

	Carry = 0;
	for (i = NumDig; i > 0; i--) 
	{
		Prod        = Carry + (unsigned long)(*ThisData) * Int;
		Carry       = Prod >> NUMBITS_USHORT;
		*ThisData++ = (unsigned short)(Prod);
	}
	if (Carry > 0 && NumDig < Size)
		*ThisData = (unsigned short)(Carry);
}

template <int Size>
BigInt<Size> BigInt<Size>::operator* (const BigInt<Size>& Int) const
{
	int i,j,Shift;
	BigInt<Size> Tmp,Sum;
	unsigned short ThisDigit;
	unsigned long Prod,Carry;
	register unsigned short *TmpDigit,*ThatDigit;
	register const unsigned short *ThatData = Int.m_Digits;
	const unsigned short *ThisData = m_Digits;
	const int NumDigits1           = NumDigits();
	const int NumDigits2           = Int.NumDigits();
	unsigned short* TmpData        = Tmp.m_Digits;

	Shift = 0;
	for (i = NumDigits1; i > 0; i--,Shift++) 
	{
		Carry     = 0;
		TmpDigit  = TmpData;
		ThisDigit = *ThisData++;
		ThatDigit = (unsigned short *)ThatData;
		for (j = __min(NumDigits2, Size - Shift); j > 0; j--)
		{
			Prod        = Carry + ThisDigit * *ThatDigit++;
			Carry       = Prod >> NUMBITS_USHORT;
			*TmpDigit++ = (unsigned short)(Prod);
		}
		if (Carry > 0 && NumDigits2 + Shift < Size)
			*TmpDigit = (unsigned short)(Carry);
		Sum.Add(Tmp);
		*TmpData++ = 0;
	}
	return Sum;
}

template <int Size>
void BigInt<Size>::UDiv ( 
	const BigInt<Size>& Denom,
	BigInt<Size>& Quot, 
	BigInt<Size>& Rem) const
{
	BigInt<Size> Tmp;
	int Bits,Shift,RemLen;
	const int DenomLen = Denom.BitCount();

	Quot   = 0;
	Rem    = *this;
	Tmp    = Denom;
	RemLen = BitCount();
	Shift  = RemLen - DenomLen;
	Tmp  <<= Shift;
	while (RemLen > DenomLen || Rem.UCompare(Denom) >= 0)
	{
		if (Tmp.UCompare(Rem) == 1)
		{
			Tmp.RShOne();
			Shift--;
		}	
		Rem.Sub(Tmp);
		Quot.SetBit(Shift);
		RemLen = Rem.BitCount(RemLen >> 4);
		Bits   = Shift + DenomLen - RemLen;
		Shift -= Bits;
		Tmp.RSh(Bits);
	}
}

template <int Size>
void BigInt<Size>::Div (
	const BigInt<Size>& Denom,
	BigInt<Size>& Quot, 
	BigInt<Size>& Rem) const
{
	if (Denom.IsZero())
		throw DivisionByZero();
	else if (! IsNeg())
	{
		if (! Denom.IsNeg())
			UDiv(Denom, Quot, Rem);
		else {
			BigInt<Size> Den;

			Den = Denom;
			Den.TwosComplement();
			UDiv(Den, Quot, Rem);
			Quot.TwosComplement();
		}
	}
	else {
		BigInt<Size> Numer;

		Numer = *this;
		Numer.TwosComplement();
		if (! Denom.IsNeg())
		{
			Numer.UDiv(Denom, Quot, Rem);
			Quot.TwosComplement();
			Rem.TwosComplement();
		}
		else {
			BigInt<Size> Den;

			Den = Denom;
			Den.TwosComplement();
			Numer.UDiv(Den, Quot, Rem);
		}
	}
}

template <int Size>
BigInt<Size> BigInt<Size>::operator% (const BigInt<Size>& Int) const
{
	BigInt<Size> Quot,Rem;

	Div(Int, Quot, Rem);
	return Rem;
}

template <int Size>
long BigInt<Size>::operator% (long Int) const
{
	BigInt<Size> Quot,Rem,Tmp;

	Tmp = Int;
	Div(Tmp, Quot, Rem);
	return Rem.GetLong();
}

template <int Size>
BigInt<Size> BigInt<Size>::operator| (const BigInt<Size>& Int) const
{
	BigInt<Size> Tmp;

	Tmp = *this;
	Tmp.IOr(Int);
	return Tmp;
}

template <int Size>
BigInt<Size> BigInt<Size>::operator& (const BigInt<Size>& Int) const
{
	BigInt<Size> Tmp;

	Tmp = *this;
	Tmp.And(Int);
	return Tmp;
}

template <int Size>
long BigInt<Size>::operator& (long Int) const
{
	BigInt<Size> Tmp;

	Tmp = *this;
	Tmp.And(Int);
	return Tmp.GetLong();
}

template <int Size>
BigInt<Size> BigInt<Size>::operator^ (const BigInt<Size>& Int) const
{
	BigInt<Size> Tmp;
	
	Tmp = *this;
	Tmp.XOr(Int);
	return Tmp;
}

template <int Size>
BigInt<Size> BigInt<Size>::operator~ () const
{
	int i;
	BigInt<Size> Tmp;
	register unsigned long *Digit;
	
	Tmp   = *this;
	Digit = Tmp.m_Digits;
	for (i = Size; i > 0; i--)
		*Digit++ = ~*Digit;
	return Tmp;
}

template <int Size>
bool BigInt<Size>::operator== (long Int) const
{
	BigInt<Size> Tmp;

	Tmp = Int;
	return *this == Tmp;
}

template <int Size>
int BigInt<Size>::Compare(const BigInt<Size>& Int) const
{
	if (IsNeg())
	{
		if (! Int.IsNeg())
			return -1;
	}
	else if (Int.IsNeg())
		return 1;

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

template <int Size>
bool BigInt<Size>::operator< (long Int) const
{
	BigInt<Size> Tmp;

	Tmp = Int;
	return *this < Tmp;
}

template <int Size>
bool BigInt<Size>::operator> (long Int) const
{
	BigInt<Size> Tmp;

	Tmp = Int;
	return *this > Tmp;
}

template <int Size>
const BigInt<Size>& BigInt<Size>::operator++()
{
	int i = 0;

	while (i < Size && m_Digits[i] == 0xffff)
		m_Digits[i++] = 0;
	if (i < Size)
		m_Digits[i]++;
	return *this;
}

template <int Size>
BigInt<Size> BigInt<Size>::operator++(int)
{
	BigInt<Size> Tmp;

	Tmp = *this;
	++(*this);
	return Tmp;
}

template <int Size>
const BigInt<Size>& BigInt<Size>::operator--()
{
	int i = 0;

	while (i < Size && m_Digits[i] == 0)
		m_Digits[i++] = 0xffff;
	if (i < Size)
		m_Digits[i]--;
	return *this;
}

template <int Size>
BigInt<Size> BigInt<Size>::operator--(int)
{
	BigInt<Size> Tmp;

	Tmp = *this;
	--(*this);
	return Tmp;
}

template <int Size>
void BigInt<Size>::operator *= (long Int)
{
	if (Int >= 0)
		Mul(Int);
	else {
		Mul(-Int);
		TwosComplement();
	}
}

// void BigInt<Size>::RSh(int NumBits)
//
// Right shifts the bits in a BigInt.

template <int Size>
void BigInt<Size>::RSh(int Bits)
{
	const int Rem    = Bits & 0xf;
	const int Digits = Bits >> 4;
	
	if (Digits > 0)
	{
		if (Digits < Size)
		{
			int i,j;
			size_t Count;

			Count = sizeof(unsigned short)*(Size - Digits);
			memmove(m_Digits, m_Digits + Digits, Count);
			for (i = Size - 1 - Digits,j = Digits; j > 0; i++,j--)
				m_Digits[i] = 0;
		}
		else {
			memset(m_Digits, 0, sizeof(m_Digits));
			return;
		}
	}
	if (Rem > 0)
	{
		int i;
		unsigned short Carry,Value;
		const int BitCount             = NUMBITS_USHORT - Rem;
		register unsigned short *Digit = m_Digits + Size - 1;

		Carry = 0;
		for (i = Size; i > 0; i--)
		{
			Value    = *Digit;
			*Digit-- = Carry | (Value >> Rem);
			Carry    = Value << BitCount;
		}
	}
}

template <int Size>
void BigInt<Size>::RShOne()
{
	unsigned short Carry,Value;
	register unsigned short *Digit = m_Digits + Size - 1;

	Carry = 0;
	for (int i = Size; i > 0; i--,Digit--)
	{
		Value = *Digit;
		if (Carry)
			*Digit = 0x8000|(Value >> 1);
		else
			*Digit = Value >> 1;
		Carry = Value & 1;
	}
}

// void BigInt<Size>::LSh(int NumBits)
//
// Left shifts the bits in a BigInt.

template <int Size>
void BigInt<Size>::LSh(int Bits)
{
	const int Rem   = Bits & 0xf;
	const int Zeros = Bits >> 4;

	if (Rem > 0)
	{
		int i;
		unsigned short Carry;
		unsigned long Value;
		register unsigned short *Digit = m_Digits;

		Carry = 0;
		for (i = Size; i > 0; i--)
		{
			Value    = (unsigned long)(*Digit) << Rem;
			*Digit++ = (unsigned short)(Value | Carry);
			Carry    = (unsigned long)(Value >> NUMBITS_USHORT);
		}
	}
	if (Zeros > 0)
	{
		int i;
		size_t Count;

		Count = sizeof(unsigned short)*(Size - Zeros);
		memmove(m_Digits + Zeros, m_Digits, Count);
		for (i = 0; i < Zeros; i++)
			m_Digits[i] = 0;
	}
}

template <int Size>
void BigInt<Size>::XOr(const BigInt<Size>& Int)
{
    unsigned short *ThisDigit      = m_Digits;
	const unsigned short *IntDigit = Int.m_Digits;

	for (int i = Size; i > 0; i--)
		*ThisDigit++ ^= *IntDigit++;
}

template <int Size>
void BigInt<Size>::IOr(const BigInt<Size>& Int)
{
	unsigned short *ThisDigit      = m_Digits;
	const unsigned short *IntDigit = Int.m_Digits;

	for (int i = Size; i > 0; i--)
		*ThisDigit++ |= *IntDigit++;
}

template <int Size>
void BigInt<Size>::And(const BigInt<Size>& Int)
{
	int i;
	unsigned short *ThisDigit      = m_Digits;
	const unsigned short *IntDigit = Int.m_Digits;

	for (i = Size; i > 0; i--)
		*ThisDigit++ &= *IntDigit++;
}

template <int Size>
void BigInt<Size>::Abs()
{
	if (IsNeg())
		TwosComplement();
}

template <int Size>
string BigInt<Size>::ToString() const
{
	string Number;
	BigInt<Size> Int = *this;
	BigInt<Size> Rem, Quot;
	const BigInt<Size> Ten = 10;

	do {
		Int.UDiv(Ten, Quot, Rem);
		int i = Rem.GetShort();
		Number = Symbol[i] + Number;
		Int = Quot;
	} while (Int > 0);
	return Number;
}

// void PrintDecNum(ostream& OutStream, const BigInt& Int)
//
// Prints a BigInt as a decimal number. 

template <int Size>
void BigInt<Size>::PrintDecNum(ostream& OutStream) const
{
	string Number;
	BigInt<Size> Rem,Quot,Int;
	const BigInt<Size> Ten = 10;

	Int = *this;
	do {
		Int.UDiv(Ten, Quot, Rem);
		int i  = Rem.GetShort();
		Number = Symbol[i] + Number;
		Int    = Quot;
	}
	while (Int > 0);
	OutStream << Number;
}

template <int Size>
void BigInt<Size>::PrintOctNum(ostream& OutStream) const
{
	string Number;
	BigInt<Size> Int = *this;

	do {
		int i  = m_Digits[0] & 0x7;
		Number = Symbol[i] + Number;
		Int >>= 3;
	} while (Int > 0);
	OutStream << Number;
}

template <int Size>
void BigInt<Size>::PrintHexNum(ostream& OutStream) const
{
	string Number;
	BigInt<Size> Int = *this;

	do {
		int i = m_Digits[0] & 0xf;
		Number = Symbol[i] + Number;
		Int >>= 4;
	} while (Int > 0);
	OutStream << Number;
}

template <int Size>
BigInt<Size> Abs(const BigInt<Size>& a)
{
	if (! a.IsNeg())
		return a;
	else {
		BigInt<Size> Tmp;

		Tmp = a;
		Tmp.TwosComplement();
		return Tmp;
	}
}

// BigInt GCD(BigInt a, BigInt b)
//
// Returns the greatest common divisor of A and B.

template <int Size>
BigInt<Size> GCD (
	BigInt<Size> a, 
	BigInt<Size> b)
{
	BigInt<Size> Tmp;

	a.Abs();
	b.Abs();
	if (a < b)
	{
		Tmp = a;
		a   = b;
		b   = Tmp;
	}
	while (! b.IsZero())
	{
		Tmp = a % b;
		a   = b;
		b   = Tmp;
	}
	return a;
}

// void Power(const BigInt& a, long e, BigInt& res)
//
// Calculates a ^ e. The result is returned in res.

template <int Size>
BigInt<Size> BigInt<Size>::Power(unsigned long e) const
{
	if (e == 0)
		return 1;
	else {
		BigInt<Size> Res,Base;

		Res  = 1;
		Base = *this;
		while (e > 0)
		{
			if (e & 1)
				Res *= Base;
			Base *= Base;
			e   >>= 1;
		}
		return Res;
	}
}

template <int Size>
BigInt<Size> Power (
	const BigInt<Size>& a, 
	const BigInt<Size>& e)
{
	if (e.IsNeg())
		return 0;
	else {
		int i;
		int BitCount;
		unsigned short Value;
		BigInt<Size> Res,Base;
		const unsigned short *Digit = e.GetData();

		Res      = 1;
		Base     = a;
		BitCount = e.BitCount();
		while (BitCount > 0)
		{
			Value = *Digit++;
			for (i = __min(BitCount, NUMBITS_USHORT); i > 0; i--)
			{
				if (Value & 1)
					Res *= Base;
				Base   *= Base;
				Value >>= 1;
			}
			BitCount -= NUMBITS_USHORT;
		}
		return Res;
	}
}

template <int Size>
BigInt<Size> HexStringToBigInt(const char String[])
{
	int i,Shift;
	unsigned long Value;
	BigInt<Size> Number;
	const int StrLen = strlen(String);

	Shift   = 0;
	String += StrLen - 1;
	unsigned short *Digit = (unsigned short* )Number.GetData();
	for (i = 0; i < StrLen; i++)
	{
		Value = GetValue(*String--, 16);
		if (Value == -1)
			break;
		if (Shift > 28)
		{
			Digit++;
			Shift = 0;
		}
		*Digit |= Value << Shift;
		Shift  += 4;
	}
	return Number;
}

template <int Size>
void BigIntToHexString (
	BigInt<Size> Int, 
	char Buffer[],
	size_t BuffSize)
{
	unsigned short Value;
	const unsigned short *FirstDigit = Int.GetData();
	const unsigned short *Digit      = FirstDigit + Size - 1;

	while (*Digit == 0 && Digit > FirstDigit)
		Digit--;
	Value = *Digit;
	if (Value == 0)
		*Buffer++ = '0';
	else {
		int HexValue;

		while ((Value & 0xf000) == 0)
			Value <<= 4;
		do {
			do {
				HexValue  = (Value & 0xf000) >> 12;
				*Buffer++ = Symbol[HexValue];
				Value   <<= 4;
			}
			while (Value > 0 && BuffSize-- > 0);
			Value = *--Digit;
		}
		while (Digit >= FirstDigit && BuffSize > 0);
	}
	*Buffer = '\0';
}

template <int Size>
BigInt<Size> BigInt<Size>::MulMod (
	const BigInt<Size>& b, 
	const BigInt<Size>& m)
{
    /* return a*b (mod m).
     * take special care of unsigned quantities and overflow.
     */
    BigInt<Size> z;
    BigInt<Size> y = *this;
	BigInt<Size> c = b;

    while (! c.IsZero()) 
	{
		if (c.IsOdd()) 
		{
            z += y;
            if (z < y || z >= m) 
				z -=m;
        }
        c.RSh(1);
        y.LSh(1);
        if (y.IsNeg() || y >= m) 
			y -= m;
    }
    return z;
}

// Calculates a ^ e (mod n).

template <int Size>
BigInt<Size> BigInt<Size>::PowerMod (
	const long e, 
	const BigInt<Size>& n)
{
	if (n.IsZero())
		throw DivisionByZero();
	else if (e < 0)
		return InvMod(PowerMod(-e, n), n);
	else {
		long Exp;
		BigInt<Size> Base,Res,Quot;

		Res  = 1;
		Base = *this;
		Exp  = abs(e);
		while (Exp > 0)
		{
			if (Exp & 1)
			{
				Res *= Base;
				Res.UDiv(n, Quot, Res);
			}
			Base *= Base;
			Base.UDiv(n, Quot, Base);
			Exp >>= 1;
		}
		if (e < 0)
			return InvMod(Res, n);
		else
			return Res;
	}
}

template <int Size>
BigInt<Size> BigInt<Size>::PowerMod (
	const BigInt<Size>& e, 
	const BigInt<Size>& n)
{
	if (e.IsNeg())
		return InvMod(PowerMod(-e, n), n);
	else if (n.IsZero())
		throw DivisionByZero();
	else {
		int i;
		int BitCount;
		unsigned short Value;
		BigInt<Size> Res,Base,Quot;

		Res      = 1;
		Base     = *this;
		BitCount = e.BitCount();
		const unsigned short *Digit = e.GetData();
		while (BitCount > 0)
		{
			Value = *Digit++;
			for (i = __min(BitCount, NUMBITS_USHORT); i > 0; i--)
			{
				if (Value & 1)
				{
					Res *= Base;
					Res.UDiv(n, Quot, Res);
				}
				Base *= Base;
				Base.UDiv(n, Quot, Base);
				Value >>= 1;
			}
			BitCount -= NUMBITS_USHORT;
		}
		return Res;
	}
}

template <int Size>
BigInt<Size> PowerMod (
	const BigInt<Size>& a,
	const BigInt<Size>& e, 
	const BigInt<Size>& n)
{
	if (e.IsNeg())
		return InvMod(PowerMod(a, -e, n), n);
	else if (n.IsZero())
		throw DivisionByZero();
	else {
		int i;
		int BitCount;
		unsigned long Value;
		BigInt<Size> Res,Base,Quot;

		Res      = 1;
		Base     = a;
		BitCount = e.BitCount();
		const unsigned short *Digit = e.GetData();
		while (BitCount > 0)
		{
			Value = *Digit++;
			for (i = __min(BitCount, NUMBITS_USHORT); i > 0; i--)
			{
				if (Value & 1)
				{
					Res *= Base;
					Res.UDiv(n, Quot, Res);
				}
				Base *= Base;
				Base.UDiv(n, Quot, Base);
				Value >>= 1;
			}
			BitCount -= NUMBITS_USHORT;
		}
		return Res;
	}
}

// void XGCD(BigInt a,BigInt b,BigInt& x0,BigInt& y0)
//
// Extended Euclidean Algorithm. Given two integers a and b XGCD()
// returns two integers x0 and y0 such that ax0 - by0 = 1. If they
// don't exist XGCD() returns false.

template <int Size>
bool BigInt<Size>::XGCD (
	BigInt<Size> b, 
	BigInt<Size>& x0, 
	BigInt<Size>& y0)
{
	bool DoNegate;
	BigInt<Size> a;
	BigInt<Size> quot,rem,x,y,x1,y1;

	x0       = 1;
	y1       = 1;
	a        = *this;
	DoNegate = false;
	while (b != 0)
	{
		a.Div(b, quot, rem);
		a        = b;
		b        = rem;
		x        = x1;
		y        = y1;
		x1       = quot*x1 + x0;
		y1       = quot*y1 + y0;
		x0       = x;
		y0       = y;
		DoNegate = ! DoNegate;
	}
	if (DoNegate)
		x0.TwosComplement();
	else
		y0.TwosComplement();
	return a == 1;
}

// bool InvMod(const BigInt& r, const BigInt& p,BigInt& m)
//
// InvMod() returns the multiplicative inverse of r (mod p).
// If the inverse does not exist the function returns false.

template <int Size>
bool InvMod (
	const BigInt<Size>& r,
	const BigInt<Size>& p,
	BigInt<Size>& m)
{
	BigInt<Size> n;

	if (! r.XGCD(p, m, n))
		return false;
	else if (m.IsNeg())
		m += p;	
	return true;
}

template <int Size>
BigInt<Size> InvMod (
	const BigInt<Size>& r,
	const BigInt<Size>& p)
{
	BigInt<Size> x,m,n;

	x = r;
	if (! x.XGCD(p, m, n))
		return 0;
	else if (m.IsNeg())
		m += p;
	return m;
}

template <int Size>
ostream& operator<< (
	ostream& OutStream,
	const BigInt<Size>& Int)
{
	if (Int.IsNeg())
	{
		BigInt<Size> AbsInt = Int;
		AbsInt.TwosComplement();
		OutStream << '-' << AbsInt;
	}
	else {
		long Flags;

		Flags = OutStream.flags();
		if (Flags & ios::hex)
			Int.PrintHexNum(OutStream);
		else if (Flags & ios::oct)
			Int.PrintOctNum(OutStream);
		else
			Int.PrintDecNum(OutStream);
	}
	return OutStream;
}

template <int Size>
istream& operator>> (
	istream& InStream, 
	BigInt<Size>& Int)
{
	char Buffer[IOSTREAM_BUFSZ];

	InStream.getline(Buffer, sizeof(Buffer));
	const streamsize CharCount = InStream.gcount();

	if (CharCount > 0)
	{
		int i;
		char c;
		bool Neg;
		long Radix,Flags,Value;

		i     = 0;
		Int   = 0;
		c     = Buffer[0];
		Flags = InStream.flags();
		if (Flags & ios::oct)
			Radix = 8;
		else if (Flags & ios::hex)
			Radix = 16;
		else
			Radix = 10;
		while (i < CharCount && isspace(c))
			c = Buffer[++i];
		Neg = c == '-';
		if (Neg)
		{
			c = Buffer[++i];
			while (i < CharCount && isspace(c))
				c = Buffer[++i];
		}
		Int   = 0;
		Value = GetValue(c, Radix);
		if (Radix == 10)
		{
			while (i < CharCount && Value != -1)
			{
				Int.Mul(10);
				Int  += Value;
				c     = Buffer[++i];
				Value = GetValue(c, Radix);
			}
		}
		else {
			const int Shift = Radix == 8 ? 3 : 4;

			while (i < CharCount && Value != -1)
			{
				Int.LSh(Shift);
				Int.m_Digits[0] |= Value;
				c     = Buffer[++i];
				Value = GetValue(c, Radix);
			}
		}
		if (Neg)
			Int.TwosComplement();
	}
	return InStream;
}



