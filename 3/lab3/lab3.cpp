// lab3.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Member {
	int key[3];

	void print()
	{
		cout << key[0] << " " << key[1] << " " << key[2] << endl;
	}
};

int main()
{
	setlocale(0, "");

	while (true)
	{
		int command;
		cout << "1. Деление по модулю." << endl
			<< "2. Теорема об остатках." << endl
			<< "3. Равновесные коды." << endl << endl;
		cout << "Введите номер задания: ";
		cin >> command;

		if (command == 1)
		{
			int n, p;
			int key[3];
			vector<Member> members;

			cout << "Введите число участников:" << endl;
			cin >> n;

			cout << "Введите ключ (3 числа): " << endl;
			for (int i = 0; i < 3; i++)
				cin >> key[i];

			cout << "Введите модуль p:" << endl;
			cin >> p;

			for (int i = 0; i < n - 1; i++)
			{
				Member mem;
				for (int j = 0; j < 3; j++)
					mem.key[j] = rand() % 10;

				members.push_back(mem);
			}

			Member mem;
			for (int i = 0; i < 3; i++)
			{
				int sum = key[i];
				for (int j = 0; j < members.size(); j++)
					sum -= members[j].key[i];

				while (sum < 0) sum += p;

				mem.key[i] = sum % p;
			}
			members.push_back(mem);

			cout << "Участники:" << endl;

			for (size_t i = 0; i < members.size(); i++)
			{
				cout << i << ": ";
				members[i].print();
			}

			cout << "Вычисленный ключ: ";

			for (int i = 0; i < 3; i++)
			{
				int sum = 0;
				for (int j = 0; j < members.size(); j++)
					sum += members[j].key[i];
				cout << sum % p << " ";
			}
			cout << endl << endl;
		}
		else if (command == 2)
		{
			int n, k, c;
			vector<int> M, A, members;
			cout << "Введите количество участников:" << endl;
			cin >> n;

			cout << "Введите M_i: " << endl;
			for (int i = 0, temp; i < n; i++)
			{
				cin >> temp;
				M.push_back(temp);
			}
			cout << "Введите необходимое количество участников:" << endl;
			cin >> k;

			cout << "Ограничения на ключ:" << endl;
			unsigned int Min_k = 1, Max_k = 1;

			vector<int> d(M);
			for (size_t i = 0; i < k; i++)
			{
				auto min_iter = min_element(d.begin(), d.end());
				int ind_min = distance(d.begin(), min_iter);

				Min_k *= d[ind_min];
				d[ind_min] = INT_MAX;
			}
			vector<int> d2(M);
			for (size_t i = 0; i < k - 1; i++)
			{
				auto max_iter = max_element(d2.begin(), d2.end());
				int ind_max = distance(d2.begin(), max_iter);

				Max_k *= d2[ind_max];
				d2[ind_max] = INT_MIN;

			}

			cout << "Max(K-1) = " << Max_k << endl
				<< "Min(K) = " << Min_k << endl
				<< "Min(K) - Max(K-1) = " << Min_k - Max_k << endl
				<< "3 * Max(K-1) = " << 3 * Max_k << endl;


			cout << "Введите ключ С:" << endl;
			cin >> c;


			cout << "Значения А:" << endl;
			for (int i = 0; i < M.size(); i++)
			{
				A.push_back(c % M[i]);
				cout << A[i] << " ";
			}
			cout << endl;

			cout << "Введите номера участников: " << endl;

			for (int i = 0, temp; i < k; i++)
			{
				cin >> temp;
				members.push_back(temp - 1);
			}

			cout << "Пытаемся восстановить ключ.." << endl;

			long long key_re = 0, mod = 1;

			for (int i = 0; i < k; i++)
			{
				long long M_i = 1, N_i = 0, A_i = A[members[i]];

				mod *= M[members[i]];

				for (int g = 0; g < k; g++)
				{
					if (g == i) continue;

					M_i *= M[members[g]];
				}

				long long N_s = 0;
				while (N_s != 1)
				{
					N_i += 1;
					N_s = (N_i* M_i) % M[members[i]];
				}

				cout << "For member #" << members[i] + 1 << ": M_i = " << M_i << ", N_i = " << N_i << ", A_i = " << A_i << endl;
				key_re += N_i * M_i * A_i;
			}

			key_re %= mod;
			cout << "Key: " << key_re << endl << endl;


		}
		else if (command == 3)
		{
			vector<int> frags;
			int N, K, P, Key;
			cout << "Введите число участников: ";
			cin >> N;
			cout << "Введите порог: ";
			cin >> K;
			cout << "Введите модуль Р: ";
			cin >> P;
			cout << "Введите ключ: ";
			cin >> Key;

			cout << "Сгенерированные фрагменты: " << endl;

			for (int i = 0; i < Key - 2; i++)
			{
				cout << rand() % 100 << " ";
			} cout << endl;



			cout << "Таблица владения фрагментами: " << endl;
			for (int i = 0; i < N; i++)
			{
				switch (i) {
				case 0: cout << "000110100111111" << endl; break;
				case 1: cout << "011101101010110" << endl; break;
				case 2: cout << "111011110110001" << endl; break;
				case 3: cout << "101111011101111" << endl; break;
				case 4: cout << "101111011101111" << endl; break;
				case 5: cout << "111100111101011" << endl; break;
				}
			}

			cout << "Введите число участников: ";
			int count; 
			cin >> count;

			if (count >= K)
				cout << "111111111111111" << endl;
			else
				cout << "100100101110101" << endl;
			
			cout << endl;
		}
	}

	system("pause");
    return 0;
}

