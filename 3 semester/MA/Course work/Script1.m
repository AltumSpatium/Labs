% Нахождение области сходимости

syms n x i F L S absS dots

F(x,n) = 3^n*x^n*(3*x/n);
disp('Область сходимости ряда:');
L(x) = limit(abs(F(x,n+1)/F(x,n)),n,inf);
disp(L(x));
disp(solve(L(x) < 1, x, 'Real', true));
dots = solve(L(x) == 1, x);
for i = 1:1:2
    S = symsum(subs(F(x,n), x, dots(i)), n, 1, inf);
    absS = symsum(abs(subs(F(x,n), x, dots(i))), n, 1, inf);
    disp('Ряд в точке');
    disp(dots(i));
    if (S ~= inf) && (S ~= -inf) && (isnan(S) ~= 1)
        if (absS ~= inf) && (absS ~= -inf) && (isnan(absS) ~= 1)
            disp('Сходится абсолютно');
        end
        disp('Сходится условно');
        else
            disp('Расходится');
    end
end   