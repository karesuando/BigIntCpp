template<int Size>
RSACryptoSystem<Size>::RSACryptoSystem() :
	Symbols("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789. @$+{[|\\\"]}=?)(/&%#!'^-_<>:,;"),
	Radix(Symbols.length())
{
	InitCharPosition(Symbols);
	PrimeP = HexStringToBigInt<Size>("154594281F272B8915820973631620D6376E6D3F02CA68B4C3C16AF11BEB7113");
	PrimeQ = HexStringToBigInt<Size>("7B7F7CF1842463909414C5AD27FA005DF683A10AC5E7309992494CBE52E90D097");
	RSAModulus = PrimeP * PrimeQ;
	Phi = (PrimeP - 1) * (PrimeQ - 1);
	EncExp = StringToBigInt("Qwerty?");
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
