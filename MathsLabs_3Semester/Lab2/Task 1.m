syms x f1 f2 U a0 an bn n;
n = sym('n','positive');
f1 = -(pi/4) - x/2;
f2 = pi/4;
a0 = simplify(1/pi*(simplify(int(f1,x,-pi,0)) + simplify(int(f2,x,0,pi))));
disp('a0=');
disp(a0);
an = simplify(1/pi*(simplify(int(f1*cos(n*x),x,-pi,0)) + simplify(int(f2*cos(n*x),x,0,pi))));
disp('an=');
disp(an);
bn = simplify(1/pi*(simplify(int(f1*sin(n*x),x,-pi,0)) + simplify(int(f2*sin(n*x),x,0,pi))));
disp('bn=');
disp(bn);
mas = [0;1;2;1000];
hold on
for i = 1:1:4
    U = a0/2 + symsum((an*cos(n*x) + bn*sin(n*x)),n,1,mas(i));
    ezplot(U,[-pi,pi]);
end
