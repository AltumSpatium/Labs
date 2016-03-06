% Равномерная сходимость на отрезке [0,1]. Найти номер, при котором
% абсолютная величина остатка не превосходит 0,1

syms n x eps k Fk S Sn F;

eps = 0.1; 
F(x) = ((-1)^n) * (x^n) /(4*n-7); 
for x = 0:1 
    k = 0;
    Fk = 1;
    while (abs(Fk) > eps)
        k = k + 1;
        Fk = (x^k) /(4*k-7);
        disp('k:');
        disp(k);
    end
end
disp('Номер n равен: ');
disp(k); 
x = 0:0.05:1;
S = symsum(F(x),n,1,Inf);
Sn = symsum(F(x),n,1,k); 
hold on; 
plot(x, Sn, 'p');
plot(x, S, 'b');
plot(x, S + eps, 'g');
plot(x, S - eps, 'g');