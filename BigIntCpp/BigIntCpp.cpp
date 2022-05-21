// BigIntCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define BUFFER_SIZE 50
#include <iostream>
#include "RSACryptoSystem.h"

int main()
{
	string CipherTxt;
	RSACryptoSystem<BUFFER_SIZE> RSA;

	if (RSA.Encrypt("Once that report is published, a Parliamentary Committee will then be able", CipherTxt))
	{
		string PlainText;
		
		if (RSA.Decrypt(CipherTxt, PlainText))
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
