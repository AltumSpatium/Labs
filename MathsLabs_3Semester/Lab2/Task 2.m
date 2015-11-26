%1)Разложите ф-цию y=f(x) в ряд Фурье на заданном интервале,
%2)Вычислите амплитуды и фазы 3 первых гармоник,
%3)Постройте графики ф-ции и частичных сумм S1, S2, S3
%Задача 2, вариант 19
%Автор:
%Мартынов А.С., БГУИР, ФКСиС, ИиТП, 453504

syms x f1 U a0 an bn;
n = sym('n','positive');
f1 = 1 - x;
a0 = simplify(1/2*(simplify(int(f1,x,-1,1))));
disp('a0 = ');
disp(a0);
an = simplify(1/2*(simplify(int(f1*cos(n*x*pi/2),x,-1,1))));
disp('an = ');
disp(an);
bn = simplify(1/2*(simplify(int(f1*sin(n*x*pi/2),x,-1,1))));
disp('bn = ');
disp(bn);
mas = [1;2;3;1000];
hold on
for i = 1:1:4
    U = a0/2 + symsum((an*cos(n*x*pi/2) + bn*sin(n*x*pi/2)),n,1,mas(i));
    ezplot(U,[-3,1]);
end
