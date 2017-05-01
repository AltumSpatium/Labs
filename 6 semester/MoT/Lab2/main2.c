int gcd(int n1, int n2)
{
    int i = 1, a;
    int gcd;

    for (i = 1; i <= n1 && i <= n2 ; i++) {
    	if (n1 % i == 0 && n2 % i == 0) {
    		gcd = i;
    	}
    }

    return gcd;
}
