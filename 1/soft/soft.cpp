#include <string>
#include <iostream>
using namespace std;

// F - CКНФ.
// номер левого разряда (старшего) = 0
// макс - 5 разрядов

string Function(string F, string A)
{
	string res;
	int count_rank = A.length() * 2 - 1;

	for (int i = 0; i < A.length(); i++)
	{
		string B = A;
		
		B.insert(B.begin(), count_rank - i - A.length(), '0');
		B.insert(B.end(), i, '0');

		bool clause = false, neagtion = false;
		for (int j = 0; j < F.length(); j++)
		{
			char ch = F[j];

			if( ch == '*')
			{
				if (clause == 0)
				{
					res.append("0");
					break;
				}
				else
					clause = false;
			}

			if (ch == '!')
			{
				neagtion = true;
			}

			if (isdigit(ch))
			{
				char F_digit_i = B[atoi(&ch)];
				int B_digit_i = atoi(&F_digit_i);

				if (!neagtion && B_digit_i || neagtion && !B_digit_i) 
					clause = true;

				neagtion = false;

				if (j == F.length() - 1 && clause == false)
				{
					res.append("0");
					break;
				}

				continue;
			}
		}

		if (((int)res.length() - 1 - i) < 0)
		{
			res.append("1");
		}
	}

	return res;
}

int main()
{
	setlocale(0, "");
	string F, A;

	cout << "Формат функции: СКНФ; дизъюнкция '+', конъюнкция '*', отрицание '!'." << endl;
	cout << "Номера разрядов начинаются с 0. 0 - старший (левый) разряд." << endl;
	cout << "Максимальное число разрядов - 5." << endl << endl;
	cout << "Введите функцию в формате СКНФ: ";
	cin >> F;
	
	while(true)
	{
		cout << "Введите последовательность А: ";
		cin >> A;
		cout << "F(A): " << Function(F, A) << endl << endl;
	}

    return 0;
}

