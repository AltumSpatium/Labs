syms x P0 P1 P2 P3 P4 C0 C1 C2 C3 C4 f;
P0(x) = 1;
P1(x) = x;
P2(x) = (1/2)*(3*x^2 - 1);
P3(x) = (1/2)*(5*x^3 - 3*x);
P4(x) = (1/8)*(35*x^4 - 30*x^2 + 3);
f(x) = x^4 + x + 1;
C0 = (int(P0(x)*f(x),x,-1,1))/(int(P0(x)*P0(x),x,-1,1));
disp('C0 = ');
disp(C0);
C1 = (int(P1(x)*f(x),x,-1,1))/(int(P1(x)*P1(x),x,-1,1));
disp('C1 = ');
disp(C1);
C2= (int(P2(x)*f(x),x,-1,1))/(int(P2(x)*P2(x),x,-1,1));
disp('C2 = ');
disp(C2);
C3 = (int(P3(x)*f(x),x,-1,1))/(int(P3(x)*P3(x),x,-1,1));
disp('C3 = ');
disp(C3);
C4 = (int(P4(x)*f(x),x,-1,1))/(int(P4(x)*P4(x),x,-1,1));
disp('C4 = ');
disp(C4);