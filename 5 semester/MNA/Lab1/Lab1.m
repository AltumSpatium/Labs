% Solution of boundary value problems by collocation, Galerkin and LSMs

syms y n x f1 f2 i fi k toSolve ;

aArray = sym('a%d',[1 9]);
a = -1;
b = 1;
fi(n) = x^n * (1 - x);
y = 0;
n = 10;

% Collocation method
for i=1:n-1
    y =  y + aArray(i) * fi(i);
end

psi(x) = sin(15) * diff(y,'x',2) + (1 + cos(15)*x*x)* y + 1; % Discrepancy
xEs = (-1 + 2/n):2/n:(1 - 2/n); % Points of collocation

for i=1:length(xEs)
    toSolve(i) = psi(xEs(i)); % Substitution
end

array = solve(toSolve == zeros(1,length(xEs))); % Solution

a(1) = double(array.a1);
a(2) = double(array.a2);
a(3) = double(array.a3);
a(4) = double(array.a4);
a(5) = double(array.a5);
a(6) = double(array.a6);
a(7) = double(array.a7);
a(8) = double(array.a8);
a(9) = double(array.a9);

disp('Метод коллокаций');
disp(a)
hold on;
ResultY(x) = subs(y,aArray,a);
%plot(xEs,ResultY(xEs))


% Integral LSM
a = -1;
b = 1;

for i=1:length(xEs)
    I(i) = 2 * int ( psi(x) * diff(psi(x),aArray(i)),'x',a,b); % Minimize the integral of the square of the residuals
end

array = solve(I == 0); % The system of normal equations

a(1) = double(array.a1);
a(2) = double(array.a2);
a(3) = double(array.a3);
a(4) = double(array.a4);
a(5) = double(array.a5);
a(6) = double(array.a6);
a(7) = double(array.a7);
a(8) = double(array.a8);
a(9) = double(array.a9);

disp('Интегральный МНК');
disp(a);

ResultY(x) = subs(y,aArray,a);
plot(xEs,ResultY(xEs), 'red');


% Discrete LSM
a = -1;
b = 1;
points = 100; % Choosing points
axis = a:(b-a)/points:b;
    S(1) = 0;
   
for i=1:length(axis);
    S = S + psi(axis(i))^2; % Finding the sum of squares of the residuals
end

for i=1:length(xEs)
    Ds(i) = diff(S,aArray(i)); % Finding solutions system
end

array = solve(Ds == 0); % Solving

a(1) = double(array.a1);
a(2) = double(array.a2);
a(3) = double(array.a3);
a(4) = double(array.a4);
a(5) = double(array.a5);
a(6) = double(array.a6);
a(7) = double(array.a7);
a(8) = double(array.a8);
a(9) = double(array.a9);

disp('Дискретный МНК');
disp(a);
ResultY(x) = subs(y,aArray,a);
%plot(xEs,ResultY(xEs), 'green');


% Galerkin method
a = -1;
b = 1;

for i=1:length(xEs)
    I(i) = int (psi(x)*fi(i),x,a,b); % Building a system of equations of integrals by the discrepancy
end

array = solve(I == 0); % Solving

a(1) = double(array.a1);
a(2) = double(array.a2);
a(3) = double(array.a3);
a(4) = double(array.a4);
a(5) = double(array.a5);
a(6) = double(array.a6);
a(7) = double(array.a7);
a(8) = double(array.a8);
a(9) = double(array.a9);

disp('Метод Галеркина');
disp(a);

ResultY(x) = subs(y,aArray,a);
%plot(xEs,ResultY(xEs), 'yellow');