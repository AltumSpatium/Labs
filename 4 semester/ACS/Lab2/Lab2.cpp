#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iomanip>
#include <math.h>

using namespace std;

double input()
{
	double input;
	int c;
	int count = 0;
	int flagpoint = 0;
	char line[307];

	do
	{
		while (((c = getch()) != 13))
		{
			if (isdigit(c) || (c == '-' && !count) ||
				(c == '.' && !flagpoint && count > 0 && line[0] != '-') ||
				(c == '.' && !flagpoint && count > 1) || (c == 8 && count))
			{
				if (c == 8)
				{
					putch('\b');
					putch(' ');
					putch('\b');
					if (line[--count] == '.')
						flagpoint = 0;
				}
				else if (count < 305)
				{
					if (c == '.')
						flagpoint = 1;
					line[count++] = (char)c;
					putch(c);
				}
			}
		}
	} while (!count || line[count - 1] == '.' || (line[count - 2] == '-' && line[count - 1] == '0'));

	line[count] = 0;
	input = atof(line);
	cout << endl;
	return input;
}

char exitapp()
{
	int c;
	int count = 0;
	char sign;

	do
	{
		while (((c = getch()) != 13))
		{
			if (c == 'y' || c == 'n' || c == 8)
			{
				if (c == 8)
				{
					putch('\b');
					putch(' ');
					putch('\b');
					count--;
				}
				else if (count < 1)
				{
					count++;
					sign = (char)c;
					putch(c);
				}
			}
		}
	} while (!count);
	cout << endl;
	return sign;
}

double calcY(double x)
{
	double y;
	double c4 = 4, c8 = 8;

	__asm // Calculating Y(x)
	{
			fldpi; st(0) = Pi
			fldpi; st(0) = Pi, st(1) = Pi
			fmul; st(0) = Pi ^ 2
			fdiv c8; st(0) = (Pi ^ 2) / 8

			fld x; st(0) = x, st(1) = (Pi ^ 2) / 8
			fabs; st(0) = | x | , st(1) = (Pi ^ 2) / 8

			fldpi; st(0) = Pi, st(1) = | x | , st(2) = (Pi ^ 2) / 8
			fdiv c4; st(0) = Pi / 4, st(1) = | x | , st(2) = (Pi ^ 2) / 8

			fmulp st(1), st(0); st(0) = Pi * x / 4, st(1) = (Pi ^ 2) / 8

			fsub; st(0) = (Pi ^ 2) / 8 - Pi * x / 4

			fstp y; st(0) = 0, y = (Pi ^ 2) / 8 - Pi * x / 4
	}

	return y;
}

double calcS(double x, int k)
{
	double s;
	double c2 = 2;

	__asm // Calculating S(x)
	{
			fld1; st(0) = 1

			fild k; st(0) = k, st(1) = 1

			fmul c2; st(0) = 2k, st(1) = 1

			fxch; st(0) = 1, st(1) = 2k
			fsub; st(0) = 2k - 1

			fld x; st(0) = x, st(1) = 2k - 1

			fmulp st(1), st(0); st(0) = (2k - 1)x
			fcos; st(0) = cos[(2k - 1)x]

			fld1; st(0) = 1, st(1) = cos[(2k - 1)x]
			fild k; st(0) = k, st(1) = 1, st(2) = cos[(2k - 1)x]
			fmul c2; st(0) = 2k, st(1) = 1, st(2) = cos[(2k - 1)x]
			fsubrp st(1), st(0); st(0) = 2k - 1, st(1) = cos[(2k - 1)x]

			fst s; st(0) = 2k - 1, st(1) = cos[(2k - 1)x], s = 2k - 1

			fmul s; st(0) = (2k - 1) ^ 2, st(1) = cos[(2k - 1)x]

			fdiv; st(0) = cos[(2k - 1)x] / (2k - 1) ^ 2, st(1) = cos[(2k - 1)x]

			fstp s; st(0) = cos[(2k - 1)x], s = cos[(2k - 1)x] / (2k - 1) ^ 2

			fstp st(0); st(0) = 0
	}
	return s;
}

double calcE(double y, double s)
{
	double e;
	__asm
	{
			fld y; st(0) = y
			fld s; st(0) = s, st(1) = y
			fsub; st(0) = y - s
			fabs; st(0) = | y - s |
			fstp e; st(0) = 0, e = | y - s |
	}
	return e;
}

void computation(double a, double b, double h, double eps)
{
	double c2 = 2, c4 = 4, c8 = 8;
	double e = 0;
	double x = 0;
	double s = 0, y = 0;
	int n = 0, k = 1;

	cout << "Enter eps: " << endl;
	eps = input();

	cout << endl << "x changes from a = " << a << " to b = " << b << " with step h = " << h << ", eps = " << eps << endl << endl;
	cout << "Result table: " << endl << endl;
	cout << "\t" << setw(10) << left << "x"
		<< setw(10) << left << "n"
		<< setw(16) << left << "Y(x)"
		<< setw(16) << left << "S(x)"
		<< endl << endl;

	x = a;
	if (a < b)
	{
		while (x <= b)
		{
			y = calcY(x);
			k = 1;
			do
			{
				s += calcS(x, k);
				e = calcE(y, s);
				n = ++k;
			} while (e > eps);

			cout << "\t" << setw(10) << left << x
				<< setw(10) << left << n
				<< setw(16) << left << y
				<< setw(16) << left << s
				<< endl;

			s = 0;
			x += h;
		}
	}
	else if (a > b)
	{
		while (x >= b)
		{
			y = calcY(x);
			k = 1;
			do
			{
				s += calcS(x, k);
				e = calcE(y, s);
				n = ++k;
			} while (e > eps);

			cout << "\t" << setw(10) << left << x
				<< setw(10) << left << n
				<< setw(16) << left << y
				<< setw(16) << left << s
				<< endl;

			s = 0;
			x += h;
		}
	}
}

int main()
{
	double a = 0.1, b = 0.8, h = 0.1, eps = 0.001;
	bool exit = false;
	char exitsign = 'n';

	while (!exit)
	{
		cout << "Enter a: " << endl;
		a = input();
		cout << "Enter b: " << endl;
		b = input();
		if (a != b)
		{
			if (a > -3.2 && a < 3.2 && b < 3.2 && b > -3.2)
			{
				cout << "Enter h: " << endl;
				h = input();
				if ((a < b && h > 0) || (a > b && h < 0))
				{
					computation(a, b, h, eps);
				}
				else
				{
					cout << "Incorrect step! Try again..." << endl;
				}
			}
			else
			{
				cout << "Series diverges! Try again..." << endl;
			}
		}
		else
		{
			cout << "'a' can't equals 'b' ! Try again..." << endl;
		}

		cout << endl << "Exit? (y/n)" << endl;
		exitsign = exitapp();
		switch (exitsign)
		{
		case 'y':
			exit = true;
			break;
		default:
			break;
		}
		exitsign = '\0';
	}
	return 0;
}