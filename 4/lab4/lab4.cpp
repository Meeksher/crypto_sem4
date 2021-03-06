// RSA.cpp : RSA.
//

#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>
using namespace std;

/////////////////////////////////////////////////////////////////////
//Алгоритм "решето Сундарама". Выбирает все простые числа
//до заданного (случайно сгенерированного).
int sundaram(int n)
{
	int *a = new int[n], i, j, k;
	memset(a, 0, sizeof(int) * n);
	for (i = 1; 3 * i + 1 < n; i++)
	{
		for (j = 1; (k = i + j + 2 * i*j) < n && j <= i; j++)
			a[k] = 1;
	}
	//Выбирает из списка простых чисел ближайшее к заданному.
	for (i = n - 1; i >= 1; i--)
		if (a[i] == 0)
		{
			return (2 * i + 1);
			break;
		}
	delete[] a;
}
/////////////////////////////////////////////////////////////////////
//Алгоритм Евклида. Алгоритм для нахождения наибольшего
//общего делителя двух целых чисел. Используется для проверки
//чисел на взаимопростоту.
int gcd(int a, int b)
{
	int c;
	while (b)
	{
		c = a % b;
		a = b;
		b = c;
	}
	return abs(a);
}
/////////////////////////////////////////////////////////////////////

struct Member {
	unsigned int p, q, f, t = 0;
	unsigned int d, n;

	Member(unsigned int P, unsigned int Q, unsigned int D)
	{
		p = P;
		q = Q;
		d = D;
		n = p * q;
		f = (p - 1) * (q - 1); 
		
		unsigned int e_simple = 0;
		while (e_simple != 1) e_simple = (++t * d) % f;
	}
};

void send_msg(Member bank, Member cur_mem)
{
	const int MAX = 1024;
	char *Text = new char[MAX];
	unsigned int *CryptoText = new unsigned int[MAX];
	unsigned int *Tdecrypt = new unsigned int[MAX];

	cout << "Введите сообщение: " << endl;
	cin >> Text;


	for (int j = 0; j < strlen(Text); j++)
	{
		unsigned int ASCIIcode = (static_cast<int>(Text[j]));
		unsigned int c = ASCIIcode;


		for (int i = 1; i < cur_mem.t; i++)
		{
			c *= ASCIIcode;
			c %= cur_mem.n;
		}

		cout << "Crypt 1: " << ASCIIcode << "^" << cur_mem.t << " mod " << cur_mem.n << " = " << c << endl;
		unsigned int c_old = c;
		for (int i = 1; i < bank.d; i++)
		{
			c *= c_old;
			c %= bank.n;
		}
		cout << "Crypt 2: " << c_old << "^" << bank.d << " mod " << bank.n << " = " << c << endl;

		CryptoText[j] = c;
	}

	for (int j = 0; j < strlen(Text); j++)
	{
		unsigned int l = (static_cast<int>(CryptoText[j])), ascii = l;


		for (int i = 1; i < bank.t; i++)
		{
			l *= ascii;
			l %= bank.n;
		}

		cout << "Decrypt 1: " << ascii << "^" << bank.t << " mod " << bank.n << " = " << l << endl;
		unsigned int old_l = l;
		for (int i = 1; i < cur_mem.d; i++)
		{
			l *= old_l;
			l %= cur_mem.n;
		}

		cout << "Decrypt 2: " << old_l << "^" << cur_mem.d << " mod " << cur_mem.n << " = " << l << endl;
		Tdecrypt[j] = l;
	}

	cout << endl << setw(5) << "Text" << setw(6) << "ASCII"
		<< setw(20) << "CryptoText/Block#" << setw(14)
		<< "ASCIIdecrypt" << setw(14) << "Text decrypt" << endl
		<< "------------------------------------------------------------" << endl;

	for (int j = 0; j < strlen(Text); j++)
	{
		cout << setw(5) << Text[j] << setw(6) << static_cast<int>(Text[j]) << setw(20)
			<< CryptoText[j] << setw(14) << Tdecrypt[j] << setw(14) << static_cast<char>(Tdecrypt[j]) << endl;
	}

	delete[] Text;
	delete[] CryptoText;
	delete[] Tdecrypt;

}

int main()
{
	setlocale(0, "");

	while (true)
	{
		int command;
		cout << "1. Общий ключ для двух пользователей." << endl
			<< "2. Передача шифрованных сообщений." << endl
			<< "3. Собщения от клиентов банку." << endl << endl;
		cout << "Введите номер задания: ";
		cin >> command;

		if (command == 1)
		{
			unsigned int p, g, k1, k2;
			cout << "Введите простое число p и примитивный корень g: " << endl;
			cin >> p >> g;
			cout << "Введите ключи k1 и k2: " << endl;
			cin >> k1 >> k2;
			int msg1 = g, msg2 = g;
			//g^k1 mod p = msg1
			for (int i = 1; i < k1; i++)
			{
				msg1 *= g;
				msg1 %= p;
			}

			//g^k2 mod p = msg1
			for (int i = 1; i < k2; i++)
			{
				msg2 *= g;
				msg2 %= p;
			}

			cout << "Сгенерированное сообщение клиента 1: " << msg1 << endl;
			cout << "Сгенерированное сообщение клиента 2: " << msg2 << endl;
			cout << "Выработка общего ключа.." << endl;

			int sh_key1 = msg2, sh_key2 = msg1;

			for (int i = 1; i < k1; i++)
			{
				sh_key1 *= msg2;
				sh_key1 %= p;
			}

			for (int i = 1; i < k2; i++)
			{
				sh_key2 *= msg1;
				sh_key2 %= p;
			}

			cout << "Общий ключ, выработанный первым пользователем: " << sh_key1 << endl;
			cout << "Общий ключ, выработанный вторым пользователем: " << sh_key2 << endl;

			cout << endl;

		}
		else if (command == 2)
		{
			unsigned int p, q, d, e = 0;

			cout << "Введите числа p и q:" << endl;
			cin >> p >> q;


			unsigned int n = p * q;

			//Генерация числа d и проверка его на взаимопростоту
			//с числом ((p-1)*(q-1)).
			
			unsigned int d_simple = 0;
			while (d_simple != 1)
			{
				d = rand() % 100;
				d_simple = gcd(d, ((p - 1)*(q - 1)));
			}
			d = 7; // !!!!!
			cout << "Выработанное число d: " << d << endl;

			//Определение числа e, для которого является истинным
			//соотношение (e*d)%((p-1)*(q-1)) = 1.
			unsigned int e_simple = 0;
			while (e_simple != 1)
			{
				e += 1;
				e_simple = (e*d) % ((p - 1)*(q - 1));
			}
			cout << "Выработанное число e: " << e << endl;

			const int MAX = 1024;
			char *Text = new char[MAX];

			cout << "Введите шифруемый текст:" << endl;
			cin >> Text;

			//Массив для хранения шифротекста.
			unsigned int *CryptoText = new unsigned int[MAX];
			unsigned int *Tdecrypt = new unsigned int[MAX];
			//Получение из введённых данных десятичного кода ASCII и
			//дальнейшее его преобразование по формуле c_i = (m_i^e)%n.

			cout << endl << setw(5) << "Text" << setw(6) << "ASCII"
				<< setw(20) << "CryptoText/Block#" << setw(14)
				<< "ASCIIdecrypt" << setw(14) << "Text decrypt" << endl
				<< "------------------------------------------------------------" << endl;
			for (int j = 0; j < strlen(Text); j++)
			{
				unsigned int ASCIIcode = (static_cast<int>(Text[j]));
				unsigned int c = ASCIIcode;
				cout << "Crypt: " << ASCIIcode << "^" << e << " mod " << n << endl;

				for (int i = 1; i < e; i++)
				{
					c *= ASCIIcode;
					c %= n;
				}

				CryptoText[j] = c;
			}

			//Расшифровка полученного кода по формуле mi = (ci^d)%n
			//и перевод его в десятичный код ASCII.
			for (int j = 0; j < strlen(Text); j++)
			{
				unsigned int ASCIIcode = (static_cast<int>(CryptoText[j]));
				unsigned int l = ASCIIcode;
				for (int i = 1; i < d; i++)
				{
					l *= ASCIIcode;
					l %= n;
				}

				Tdecrypt[j] = l;
			}

			for (int j = 0; j < strlen(Text); j++)
			{
				cout << setw(5) << Text[j] << setw(6) << static_cast<int>(Text[j]) << setw(20)
					<< CryptoText[j] << setw(14) << Tdecrypt[j] << setw(14) << static_cast<char>(Tdecrypt[j]) << endl;
			}

			delete[] Text;
			delete[] CryptoText;
			delete[] Tdecrypt;

			cout << endl;
		}
		else if (command == 3)
		{
			int count;
			cout << "Введите количество участников: " << endl;
			cin >> count;

			vector<Member> members;

			for (size_t i = 0; i < count; i++)
			{
				unsigned int p, q, d;
				cout << "Введите числа p, q, d " << i + 1 << "-го участника:" << endl;
				cin >> p >> q >> d;

				Member mem(p, q, d);
				members.push_back(mem);
			}

			int client_n;
			cout << "Выберите номер клиента-отправителя: " << endl;
			cin >> client_n;

			Member bank = members[0], cur_mem = members[client_n];
			send_msg(bank, cur_mem);

			int n1, n2;
			cout << "Игра в имитацию. Введите n1 и n2, номера истинного отправителя и ложного соответственно:" << endl;
			cin >> n1 >> n2;
			Member memtrue = members[n1], memfalse = members[n2];

			Member sender(memtrue.p, memtrue.q, memfalse.d);
			sender.n = memfalse.n;

			//на этом месте повисает, т.к. неверное подбирает ключ t - он огромный.
			send_msg(bank, sender);


			cout << endl;
		}
	}

	system("pause");
	return 0;
}

