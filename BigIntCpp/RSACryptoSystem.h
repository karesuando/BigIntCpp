#pragma once
#include "BigInt.h"

template<int Size>
class RSACryptoSystem
{
public:
	bool Encrypt(const string& PlainText, string& CipherText);
	bool Decrypt(const string& CipherText, string& PlainText);
    RSACryptoSystem();

private:
	const string Symbols;
	const long Radix;   // Talbas
	BigInt<Size> PrimeP;
	BigInt<Size> PrimeQ;
	BigInt<Size> RSAModulus;
	BigInt<Size> Phi;
	BigInt<Size> EncExp;
	BigInt<Size> DecExp;
    int CharPosition[256];

private:
	void InitCharPosition(const string&);
	string BigIntToString(BigInt<Size>);
	BigInt<Size> StringToBigInt(const string&);
};

#include "RSACryptoSystem.cpp"