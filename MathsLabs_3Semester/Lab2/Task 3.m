syms x f1 U a0 an bn;
n = sym('n','positive');
f1 = - (2/3)*x + 2;
a0 = simplify((2/6)*simplify(int(f1,x,-2,2)));
disp('a0 = ');
disp(a0);
an = simplify((2/6)*simplify(int(f1*cos(n*x*pi/6),x,-2,2)));
disp('an = ');
disp(an);
bn = simplify((2/6)*simplify(int(f1*sin(n*x*pi/6),x,-2,2)));
disp('bn =');
disp(bn);
mas = [3;1000];
hold on
for i = 1:1:2
    U = a0/2 + symsum(an*cos(n*x*pi/6),n,1,mas(i));
    ezplot(U,[-2,2]);
end
for i = 1:1:2
    U = symsum(bn*sin(n*x*pi/6),n,1,mas(i));
    ezplot(U,[-2,2]);
end
