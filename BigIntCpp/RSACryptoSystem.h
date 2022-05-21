#pragma once
#include "BigInt.h"

template<int Size>
class RSACryptoSystem
{
public:
	bool Encrypt(const string& PlainText, string& CipherText);
	bool Decrypt(const string& CipherText, string& PlainText);
    RSACryptoSystem (
		const string& MessageChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789. @$+{[|\\\"]}=?)(/&%#!'^-_<>:,;",
		const string& PrimePHex = "154594281F272B8915820973631620D6376E6D3F02CA68B4C3C16AF11BEB7113",
		const string& PrimeQHex = "7B7F7CF1842463909414C5AD27FA005DF683A10AC5E7309992494CBE52E90D097",
		const string& EncExpStr = "Qwerty?");
	RSACryptoSystem(
		const string& MessageChars,
		const BigInt<Size>& PrimePHex,
		const BigInt<Size>& PrimeQHex,
		const BigInt<Size>& EncExpStr);

private:
	const string Symbols;
	const long Radix;   // Talbas
	const BigInt<Size> PrimeP;
	const BigInt<Size> PrimeQ;
	const BigInt<Size> RSAModulus;
	const BigInt<Size> Phi;
	BigInt<Size> EncExp;
	BigInt<Size> DecExp;
    int CharPosition[256];

private:
	void InitCharPosition(const string&);
	string BigIntToString(BigInt<Size>);
	BigInt<Size> StringToBigInt(const string&);
};

#include "RSACryptoSystem.cpp"