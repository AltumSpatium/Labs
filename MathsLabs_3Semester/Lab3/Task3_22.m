%����� �������� �� ��������� �����������
%������� 3, ������ 22, ������� 17
%�����:
%�������� �.�., �����, �����, ����, 453504

syms p F(p) f(t) t
F(p) = p/((p^2 + 1)*(p^2 - 2));
f(t) = ilaplace(F(p));
f(t) = simplify(f(t));
disp(f(t));