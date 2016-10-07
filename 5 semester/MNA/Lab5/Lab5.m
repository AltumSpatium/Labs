% The grid method for solving the wave equation

syms x a

l = double(0);
r = double(0);
f(x) = 0*x;
a = 14;
T = 2;
M = 70;
N = 45;
tau = double(T/M);
h = double(1.5/N);

% Square
u = double(zeros(N+1, M+1));
ro(x) = sin(x) + cos(x);
q(x) = -a*(sin(x) + cos(x));
diff2ro(x) = diff(ro(x), x, 2); % The second derivative

% Approximate Taylor polynomial
for i = 1:N+1
    ro1 = double(ro(h*(i-1)));
    u(i,1) = ro1;
    u(i, 2) = double(ro1 + tau*q(h*(i-1)) + (0.5*tau^2)*(0 - diff2ro(h*(i-1))));
end

for i = 1:M+1
    u(1, i) = l;
    u(1, M+1) = r;
end

% Difference scheme
for i = 2:N
    for j = 3:M
        u(i, j+1) = double(((tau/h)^2)*(u(i+1,j) + u(i-1,j) - 2*u(i,j)) + 2*u(i,j) - u(i, j-1));
    end
end

% Displaying results
[X,Y] = ndgrid(0:h:1.5, 0:tau:T);
surf(X, Y, u)
