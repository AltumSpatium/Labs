%����� ������� ��, ��������������� ��������� ��������
%������� 3, ������ 23, ������� 17
%�����:
%�������� �.�., �����, �����, ����, 453504

syms f_op(t) t;
hold on;
f_op(t) = dsolve('D2y - 2*Dy + y = exp(t)/(1+(t^2))', 'y(0)=0','Dy(0)=0');
f_op(t) = simplify(f_op(t));
disp(f_op(t));
f(t) = (-exp(t))*(1/2*log((t^2)+1) - (t*atan(t)));
t = -5:0.5:5;
plot(t,f_op(t),'b',t,f(t),'kx');