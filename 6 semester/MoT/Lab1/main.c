typedef struct Point {
	int x;
	int y;
} Point;

int IsPointInsidepolygon(Point* p, int Number, int x, int y) {
	int i1, i2, n, N, S, S1, S2, S3, flag;
	N = Number;
	for (n = 0; n < N; n++) {
		flag = 0;
		i1 = n < N - 1 ? n + 1 : 0;
		while (flag == 0) {
			i2 = i1 + 1;
			if (i2 >= N)
				i2 = 0;
			if (i2 == (n < N - 1 ? n + 1 : 0))
				break;
			S = abs(p[i1].x * (p[i2].y - p[n].y) + p[i2].x * (p[n].y - p[i1].y) + p[n].x  * (p[i1].y - p[i2].y));
			S1 = abs(p[i1].x * (p[i2].y - y) + p[i2].x * (y - p[i1].y) + x * (p[i1].y - p[i2].y));
			S2 = abs(p[n].x * (p[i2].y - y) + p[i2].x * (y - p[n].y) + x * (p[n].y - p[i2].y));
			S3 = abs(p[i1].x * (p[n].y - y) + p[n].x * (y - p[i1].y) + x * (p[i1].y - p[n].y));
			if (S == S1 + S2 + S3) {
				flag = 1;
				break;
			}
			i1 = i1 + 1;
			if (i1 >= N)
				i1 = 0;
		}
		if (flag == 0)
			break;
	}
	return flag;
}

int main() {
	Point p1 = { 0, 0 };
	Point p2 = { 2, 0 };
	Point p3 = { 2, 2 };
	Point p4 = { 0, 2 };
	Point p[4] = { p1, p2, p3, p4 };

	int result = IsPointInsidepolygon(p, 4, 1, 1);
	if (result == 1) {
		printf("Inside\n");
	} else {
		printf("Outside\n");
	}
	return 0;
}
