%����������� ���������� ��������������� ���� �� ������� [0,1]. 
%��� ����� n ���������� �������� ����������� ����� ���� �� ����������� 0,1 
%��� ������ � �������������� [0,1]
%������ 15, ������� 19
%�����:
%�������� �.�., �����, �����, ����, 453504

syms n x eps k Uk S Sn fun; 
eps = 0.1; 
fun(x) = ((-1)^n) * (x^n) /(4*n-7); 
for x = 0:1 
    k = 0;
    Uk = 1;
    while (abs(Uk) > eps)
        k = k + 1;
        Uk = (x^k) /(4*k-7);
        disp(k);
    end
end
disp('n: ');
disp(k); 
x = 0:0.05:1;
S = symsum(fun(x),n,1,Inf);
Sn = symsum(fun(x),n,1,k); 
hold on; 
plot(x, Sn, 'p');
plot(x, S, 'b');
plot(x, S + eps, 'g');
plot(x, S - eps, 'g');