#include <iostream>

#include <vector>
#include <string>
#include <map>

// Реализация через вектор байтовых интов с соответствующими операциями
class BigInt
{
private:
	std::vector<char> v;
	bool isNegative;

	
	void init(const std::string n)
	{
		isNegative = 0; int i = 0;
		if (n[0] == '+' || n[0] == '-')
		{
			isNegative = (n[0] == '-') ? 1 : 0;
			i = 1;
		}
		v.reserve(n.length() - i);
		for (int j = n.length() - i - 1; j >= 0; --j)
		{
			v[j] = n[j];
		}
	}

	void normalize(BigInt& val)
	{
		size_t i = val.v.size();
		while (i != 0 && v[i] == 0)
			--i;

		if (i != 0)
		{
			val.v.erase(v.begin() + i, v.end() - 1);
		}
	}

public:
	//Constructors
	BigInt() : isNegative(false) {}
	template <typename T>
	BigInt(int n) { init(std::to_string(n)); normalize(*this); }
	BigInt(long n) { init(std::to_string(n)); normalize(*this); }
	BigInt(long long n) { init(std::to_string(n)); normalize(*this); }

	BigInt(const std::string val)
	{
		int i = 0; isNegative = 0;
		if (val[i] == '+' || val[i] == '-')
		{
			isNegative = (val[i] == '+') ? (0) : (1);
			++i;
		}

		v.reserve(val.length());
		for (int j = 0; j < val.length(); ++ j)
		{
			v[j] = val[j + i];
		}
	}
	BigInt(const BigInt& other): v(other.v), isNegative(other.isNegative) {}

	BigInt& operator=(const BigInt& other)
	{
		if (this != &other)
		{
			v = other.v;
			isNegative = other.isNegative;
		}
		return *this;
	}

	BigInt& operator+=(const BigInt& other)
	{
		while (v.size() < other.v.size())
		{
			v.push_back(0);
		}

		if ((!isNegative && !other.isNegative) || (isNegative && other.isNegative))
		{
			int carry = 0;
			for (int i = 0; i < other.v.size(); ++i)
			{
				carry += v[i] + other.v[i];
				v[i] = carry % 10;
				carry /= 10;
			}

			for (int i = other.v.size(); carry && i < v.size(); ++i)
			{
				carry += v[i];
				v[i] = carry % 10;
				carry /= 10;
			}

			if (carry)
			{
				v.push_back(carry);
			}
		}
		else
		{
			(isNegative) ? (*this -= other) : (*this = other - (*this));
		}

		normalize(*this);

		return *this;
	}
	BigInt& operator-=(const BigInt& other)
	{
		if (isNegative != other.isNegative)
		{
			*this += other;
			return *this;
		}
		else if (isNegative && other.isNegative)
		{
			*this = other - *this;
		}
		else
		{
			int borrow = 0, dig;
			for (int i = 0; i < v.size(); ++ i)
			{
				dig = v[i] - borrow;
				if (i < other.v.size())
				{
					dig -= other.v[i];
				}

				if (dig < 0)
				{
					dig += 10;
					borrow = 1;
				}
				else
				{
					borrow = 0;
				}

				v[i] = dig;
			}
			
			//If other.v.size() > v.size()
			for (int i = v.size(); i < other.v.size(); ++i)
			{
				dig = other.v[i] - borrow;

				if (dig < 0)
				{
					borrow = 1;
					dig += 10;
				}
				v.push_back(other.v[i]);
			}
		}
		
		normalize(*this);
		return *this;
	}

	friend BigInt operator-(const BigInt&, const BigInt&);
	friend BigInt operator+(const BigInt&, const BigInt&);

	//Suffix operators
	friend BigInt operator"" _bi(const char*);

	//Stream operators
	friend std::ostream& operator<<(std::ostream&, const BigInt&);
};

BigInt operator+(const BigInt& first, const BigInt& second)
{
	BigInt res = first;
	res += second;
	return res;
}
BigInt operator-(const BigInt& first, const BigInt& second)
{
	BigInt res = first;
	res -= second;
	return res;
}

BigInt operator"" _bi(const char* val)
{
	return BigInt(val);
}
std::ostream& operator<<(std::ostream& out, const BigInt& bi)
{
	if (bi.isNegative)out << '-';

	for (int i = bi.v.size() - 1; i >= 0; --i)
	{
		out << static_cast<int>(bi.v[i]);
	}
	return out;
}

int main()
{
	BigInt first = 1000000000111111111111111111111111_bi;
	//std::cout << first;
}