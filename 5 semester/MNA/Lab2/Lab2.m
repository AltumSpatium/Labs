% The solution of the boundary value problem by the difference approximations

syms x a b l r Y

a = sin(19);
b = cos(19);
l = -1;
r = 1;
n = 9;
h = (r-l)/n;

interval = l:h:r; % Divide cut into 9 parts
yVect = sym('y%d', [1 n+1]); % Creating a symbolic vector yVect = [y1 y2 ... y9 y10]

% Expressing y'' from:
% y''(Xk) = (y_k+1 - 2*y_k + y_k-1) / h^2 , y_k = y(Xk)
% and substitute in system
equations(1) = subs(a*(yVect(2) - 2*yVect(1))/h^2 + (1 + b*x^2)*yVect(1) + 1, x, interval(1));
for k=2:n-2
	equations(k) = subs(a*(yVect(k+1) - 2*yVect(k) + yVect(k-1))/h^2 + (1 + b*x^2)*yVect(k) + 1, x, interval(k));
end
equations(n-1) = subs(a*(-2*yVect(n-1) + yVect(n-2))/h^2 + (1 + b*x^2)*yVect(n-1) + 1, x, interval(n-1));
% Now we've got a system of (n+1) equations, a decision of which will be an approximate solution of the boundary value problem

% Solving the system, each equation equals to zero
Y = solve(equations == [0 0 0 0 0 0 0 0]);

% Getting results
y(1) = 0;
y(2) = Y.y1;
y(3) = Y.y2;
y(4) = Y.y3;
y(5) = Y.y4;
y(6) = Y.y5;
y(7) = Y.y6;
y(8) = Y.y7;
y(9) = Y.y8;
y(10) = 0;

% Displaying results
disp(y);
plot(interval, y);
