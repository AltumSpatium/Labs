%������ ������ ����
%������� 3, ������ 24, ������� 17
%�����:
%�������� �.�., �����, �����, ����, 453504

syms y(t) t;
y(t) = dsolve('2*D2y + 5*Dy =29*cos(t)','Dy(0) = 0', 'y(0) = -1');
y(t) = simplify(y(t));
disp(y(t));
t = -2*pi:pi/20:2*pi;
plot(t, y(t));