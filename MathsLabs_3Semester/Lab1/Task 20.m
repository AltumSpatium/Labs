%��������� �������� � ��������� �� 0,001
%������ 20, ������� 19
%�����:
%�������� �.�., �����, �����, ����, 453504

syms f fun x
f(x)=(1 - exp(-x/2)) / (x);
fun(x)=taylor(f(x),x,'ExpansionPoint',0);
disp('���:')
disp(fun(x));
disp('��������:');
disp(int(fun(x),x,0,0.4))