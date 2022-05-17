// BigIntCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BigInt.h"
#define BUFFER_SIZE 50

BigInt<BUFFER_SIZE> StringToBigInt(const char String[]);
int* CreateCharPosArray(const char Symbols[]);

const char Symbols[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789. @$+{[|\\\"]}=?)(/&%#!'^-_<>:,;";
const long Radix = strlen(Symbols);   // Talbas
const int* CharPos = CreateCharPosArray(Symbols);
const BigInt<BUFFER_SIZE> PrimeP = HexStringToBigInt<BUFFER_SIZE>("154594281F272B8915820973631620D6376E6D3F02CA68B4C3C16AF11BEB7113");
const BigInt<BUFFER_SIZE> PrimeQ = HexStringToBigInt<BUFFER_SIZE>("7B7F7CF1842463909414C5AD27FA005DF683A10AC5E7309992494CBE52E90D097");
const BigInt<BUFFER_SIZE> RSAModulus = PrimeP * PrimeQ;
const BigInt<BUFFER_SIZE> Phi = (PrimeP - 1) * (PrimeQ - 1);
const BigInt<BUFFER_SIZE> EncExp = StringToBigInt("Qwerty?");
const BigInt<BUFFER_SIZE> DecExp = InvMod(EncExp, Phi);

int* CreateCharPosArray(
	const char Symbols[])
{
	long i;
	int ch;
	const long Radix = strlen(Symbols);
	static int Position[256] = { 0 };

	for (i = 0; i < Radix; i++)
	{
		ch = Symbols[i];
		Position[ch] = i;
	}
	return Position;
}

template <int Size>
string BigIntToString(BigInt<Size> Int)
{
	string String = "";
	unsigned short i;
	BigInt<BUFFER_SIZE> Quot, Rem;
	const BigInt<BUFFER_SIZE> Base = Radix;

	do {
		Int.UDiv(Base, Quot, Rem);
		Int = Quot;
		i = Rem.GetUShort();
		String = Symbols[i] + String;
	} while (Int > 0);
	return String;
}

bool Encrypt(
	const char PlainText[],
	string& CipherText)
{
	BigInt<BUFFER_SIZE> PlainTextValue;

	PlainTextValue = StringToBigInt(PlainText);
	if (PlainTextValue >= RSAModulus)
		return false;
	else {
		BigInt<BUFFER_SIZE> CipherTextValue;

		CipherTextValue = PlainTextValue.PowerMod(EncExp, RSAModulus);
		CipherText = BigIntToString(CipherTextValue);
		return true;
	}
}


BigInt<BUFFER_SIZE> StringToBigInt(const char String[])
{
	unsigned char c;
	BigInt<BUFFER_SIZE> Value;

	for (c = *String++; c != '\0'; c = *String++)
		Value = Radix * Value + CharPos[c];
	return Value;
}

bool Decrypt(
	const string& CipherText,
	string& PlainText)
{
	BigInt<BUFFER_SIZE> CipherTextValue;
	
	CipherTextValue = StringToBigInt(CipherText.c_str());
	if (CipherTextValue >= RSAModulus)
		return false;
	else {
		BigInt<BUFFER_SIZE> PlainTextValue;

		PlainTextValue = CipherTextValue.PowerMod(DecExp, RSAModulus);
		PlainText = BigIntToString(PlainTextValue);
		return true;
	}
}

int main()
{
	string CipherTxt;

	if (Encrypt("Why the hell doesn't this work?", CipherTxt))
	{
		string PlainText;
	
		if (Decrypt(CipherTxt, PlainText))
			cout << PlainText << endl;
	}
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
