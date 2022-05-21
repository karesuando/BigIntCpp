template<int Size>
RSACryptoSystem<Size>::RSACryptoSystem(
	const string& MessageChars,
	const string& PrimePHex,
	const string& PrimeQHex,
	const string& EncExpStr) :
	Symbols(MessageChars),
	Radix(Symbols.length()),
	PrimeP(HexStringToBigInt<Size>(PrimePHex.c_str())),
    PrimeQ(HexStringToBigInt<Size>(PrimeQHex.c_str())),
    RSAModulus(PrimeP * PrimeQ),
    Phi((PrimeP - 1) * (PrimeQ - 1))
{
	InitCharPosition(Symbols);
	EncExp = StringToBigInt(EncExpStr);
	DecExp = InvMod(EncExp, Phi);
}

template<int Size>
RSACryptoSystem<Size>::RSACryptoSystem(
	const string& MsgChars,
	const BigInt<Size>& PrimeP_,
	const BigInt<Size>& PrimeQ_,
	const BigInt<Size>& EncExp_) :
	Symbols(MsgChars),
	Radix(Symbols.length()),
	PrimeP(PrimeP_),
	PrimeQ(PrimeQ_),
	RSAModulus(PrimeP_* PrimeQ_),
	Phi((PrimeP_ - 1)* (PrimeQ_ - 1))
{
	InitCharPosition(Symbols);
	EncExp = EncExp_;
	DecExp = InvMod(EncExp, Phi);
}

template<int Size>
void RSACryptoSystem<Size>::InitCharPosition(const string& Symbols)
{
	memset((void *)CharPosition, 0, sizeof(CharPosition));
	for (int i = 0; i < Radix; i++)
	{
		int ch = Symbols[i];
		CharPosition[ch] = i;
	}
}

template <int Size>
string RSACryptoSystem<Size>::BigIntToString(BigInt<Size> Int)
{
	string String = "";
	unsigned short i;
	BigInt<Size> Quot, Rem;
	const BigInt<Size> Base = Radix;

	do {
		Int.UDiv(Base, Quot, Rem);
		Int = Quot;
		i = Rem.GetUShort();
		String = Symbols[i] + String;
	} while (Int > 0);
	return String;
}

template <int Size>
bool RSACryptoSystem<Size>::Encrypt(
	const string& PlainText,
	string& CipherText)
{
	BigInt<Size> PlainTextValue;

	PlainTextValue = StringToBigInt(PlainText);
	if (PlainTextValue >= RSAModulus)
		return false;
	else {
		BigInt<Size> CipherTextValue;

		CipherTextValue = PlainTextValue.PowerMod(EncExp, RSAModulus);
		CipherText = BigIntToString(CipherTextValue);
		return true;
	}
}

template <int Size>
BigInt<Size> RSACryptoSystem<Size>::StringToBigInt(const string& String)
{
	BigInt<Size> Value;

	for (const char* str = String.c_str(); *str != '\0'; str++)
		Value = Radix * Value + CharPosition[*str];
	return Value;
}

template <int Size>
bool RSACryptoSystem<Size>::Decrypt(
	const string& CipherText,
	string& PlainText)
{
	BigInt<Size> CipherTextValue;

	CipherTextValue = StringToBigInt(CipherText);
	if (CipherTextValue >= RSAModulus)
		return false;
	else {
		BigInt<Size> PlainTextValue;

		PlainTextValue = CipherTextValue.PowerMod(DecExp, RSAModulus);
		PlainText = BigIntToString(PlainTextValue);
		return true;
	}
}
