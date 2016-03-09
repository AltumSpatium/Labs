#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

double input()
{
	double input;
	int c;
	int count = 0;
	int flagpoint = 0;
	char line[20];

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
				else if (count < 18)
				{
					if (c == '.')
						flagpoint = 1;
					line[count++] = (char)c;
					putch(c);
				}
			}
		}
	} while (!count || line[count - 1] == '.' || (line[0] == '-' && line[count - 1] == '0'));

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

int main()
{
	double c2 = 2, c4 = 4;
	double a, b, c;
	double x, x1, x2;
	double discrim = 0, sqrtdiscrim = 0;
	bool exit = false;
	char exitsign = 'n';

	while (!exit)
	{
		cout << "Enter a: " << endl;
		a = input();
		cout << "Enter b: " << endl;
		b = input();
		cout << "Enter c: " << endl;
		c = input();

		if (a != 0)
		{
			cout << endl << "Your equation: ";
			if (b >= 0)
				if (c >= 0)
					cout << a << "*x^2 + " << b << "*x + " << c << " = 0" << endl;
				else
					cout << a << "*x^2 + " << b << "*x + (" << c << ") = 0" << endl;
			else
				if (c >= 0)
					cout << a << "*x^2 + (" << b << ")*x + " << c << " = 0" << endl;
				else
					cout << a << "*x^2 + (" << b << ")*x + (" << c << ") = 0" << endl;

			__asm
			{
				fld b; Loading coefficient b into st(0)
				fmul b; Squaring b

				fld c4; 4
				fmul a; 4 * a
				fmul c; 4 * a * c

				fsubp st(1), st(0); b ^ 2 - 4 * a*c

				fstp discrim; Loading result into variable
			}
			cout << endl << "Discriminant: " << endl << "D = b^2 - 4*a*c = (" << b << ")^2 - 4*(" << a << ")*(" << c << ") = " << discrim << endl << endl;
			if (discrim > 0)
			{
				__asm
				{
					fld discrim
					fsqrt; Finding the square root of the discriminant
					fst sqrtdiscrim; Saving it to variable

					fld b; Loading coefficient b into st(0)
					fchs; Changing sign of coefficient b

					fadd sqrtdiscrim; Adding square root of the discriminant

					fld a; Loading coefficient a into st(0)
					fmul c2; Multiplying by 2

					fdivp st(1), st(0); (-b + sqrt(disc)) / (2 * a)

					fstp x1; Saving first root of equation to variable

					fld b; Loading coefficient b into st(0)
					fchs; Changing sign of coefficient b
					fsub sqrtdiscrim; Subtract square root of the discriminant

					fld a; a
					fmul c2; 2 * a

					fdivp st(1), st(0); (-b - sqrt(disc)) / (2 * a)

					fstp x2; Saving second root of equation to variable
				}
				cout << "x1 = (-b + sqrt(D)) / (2*a) = " << x1 << ", x2 = (-b - sqrt(D)) / (2*a) = " << x2 << endl;
			}
			else if (discrim == 0)
			{
				__asm
				{
					fld b; Loading coefficient b into st(0)
					fchs; Changing sign of coefficient b
					fld a; a
					fmul c2; 2 * a

					fdivp st(1), st(0); (-b / (2 * a))

					fstp x; Saving root of equation to variable
				}
				cout << "x = -b / (2*a) = " << x << endl;
			}
			else if (discrim < 0)
			{
				cout << "The equation has no roots!" << endl;
			}
		}
		else
		{
			if (b != 0)
			{
				cout << endl << "Your equation: ";
				if (b >= 0)
					if (c >= 0)
					cout << b << "*x + " << c << " = 0" << endl;
				else
					cout << b << "*x + (" << c << ") = 0" << endl;
				else
				if (c >= 0)
					cout << b << "*x + " << c << " = 0" << endl;
				else
					cout << b << "*x + (" << c << ") = 0" << endl;
				__asm
				{
					fld c; Loading coefficient c into st(0)
					fchs; Changing sign of coefficient c
					fld b; Loading coefficient b into st(0)

					fdivp st(1), st(0); -c / b

					fstp x; Saving root of equation to variable
				}
				cout << endl << "x = -c / b = " << x << endl;
			}
			else
			{
				cout << endl << "Equation " << b << " * x + " << c << " = 0 can't be solved!" << endl;
			}
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