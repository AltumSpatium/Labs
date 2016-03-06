%Для данной системы установить тип фазовой картины и изобразить ее
%Задание 2, вариант 17
%Автор:
%Мартынов А.С., БГУИР, ФКСиС, ИиТП, 453504
 
syms C1 C2 s y1(t) y2(t) fy1(t) fy2(t)
s = dsolve('Dx=5*x+y','Dy=12*x+9*y');
y1(t) = simplify(s.x);
y2(t) = simplify(s.y);
disp('y1(t):');
disp(y1(t));
disp('y2(t):');
disp(y2(t));
x=-0.1:0.001:0.1;
hold all;
grid on;
for C1=-3:1:3
    for C2=-3:1:3
        fy1(t) = subs(y1(t),'C1',C1);
        fy1(t) = subs(fy1(t),'C2',C2);
        fy2(t) = subs(y2(t),'C1',C1);
        fy2(t) = subs(fy2(t),'C2',C2);
        plot(fy1(x),fy2(x),'b');
    end;
end;
