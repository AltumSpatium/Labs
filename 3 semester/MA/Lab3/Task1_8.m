%��� (y'= x+2y) ������� ������� ���������
%������������ ������, ���������� ����� �(3;0)
%������� 1, ������ 8, ������� 17
%�����:
%�������� �.�., �����, �����, ����, 453504

hold on;
for k = -5:1:5
    for x = -3:0.4:3
        y = (k-x)/2;
        x1=(x-0.2):0.2:(x+0.2);
        y1=k*x1+y-k*x;
        plot(x1,y1,'r');
    end
end
axis([-3 4 -3 2]);
x1 = -2.5 : 0.1 : 6;
y1 = 1/4*((-2)*x + 7 * exp(x1.*2 - 6) - 1);
plot(x1,y1,'b');