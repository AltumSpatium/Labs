% The grid method solutions of the heat equation

syms x t x0 x1 t0 t1 h s p q;

x0 = 0;
x1 = 1;
t0 = 0;
t1 = 1;
N = 20;
M = 20;
h = (x1-x0)/N;
tau = (t1-t0)/M;

% Setting sufficiently smooth functions
f(x,t) = sin(14)*t + cos(14)*x + 1;
s(x) = 0.5*x^2;
p(t) = 0.5*t^2;
q(t) = 0.5+0.5*t^2 + t;

% Grid
y = zeros(N+1,M+1);

% Initial condition
for i=2:N
    y(i,1) = s(h*(i-1));
end

% Boundary conditions
for i = 1:M+1
    y(1,i) = p((i-1)*tau);
    y(M+1,i) = q(tau*(i-1));
end

% The difference equation for y(i,j)
k = tau/(h^2);
for i = 2:N
    for j = 2:M+1
        y(i,j) = k*y(i-1,j-1) + (1-2*k)*y(i,j-1) + k*y(i+1,j-1) + tau*f((i-1)*h,(j-1)*tau);
    end
end

% Displaying results
mesh(0:tau:1,0:h:1,y);
figure;
plot3(0:tau:1, 0:h:1,y);
disp(y);
