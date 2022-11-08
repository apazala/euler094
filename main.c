#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


typedef struct acoeff {
	int a0;
	int aperiod[1000];
	int period;
}acoeff;

int contfracperiod(int d, acoeff* ac) {
	int a0, a, b, c, b0, c0, i = 0;
	a0 = sqrt(d);

	c = d - a0*a0;
	if (!c) return 0;

	ac->a0 = a0;
	c0 = c;
	b = b0 = a0;
	int a02 = a0 << 1;
	do {
		a = (a0 + b) / c;
		b = a*c - b;
		c = (d - b*b) / c;
		ac->aperiod[i++] = a;
	} while (a != a02);
	ac->period = i;

	return i;
}

int arka(const acoeff* ac, int k) {
	if (k <= ac->period) return (k == 0) ? ac->a0 : ac->aperiod[k - 1];

	return ac->aperiod[(k - 1) % ac->period];
}


int main()
{
	acoeff ac;

	/*int n;
	scanf("%d", &n);*/

	int period, ak;
	int dans = 0;
	int d = 3;
	period = contfracperiod(d, &ac);

	uint64_t h1 = 0;
	uint64_t h = 1;
	uint64_t h2;

	uint64_t q1 = 1;
	uint64_t q = 0;
	uint64_t q2;

	int ktarget = ((period & 1) ? 2 * period - 1 : period - 1);

	for (int k = 0; k <= ktarget; k++) {
		h2 = h1; h1 = h; q2 = q1; q1 = q;
		ak = arka(&ac, k);
		h = h1*ak + h2;
		q = q1*ak + q2;
	}

	//Minimal Pell solution x = h, y = q;
	const int64_t x1 = h; const int64_t y1 = q; const int64_t dy1 = d*y1;

	//reading and answering
	int t;
	scanf("%d", &t);

	uint64_t n, a3, perim;
	while (t--) {
		//reset
		h = x1; q = y1;

		scanf("%" SCNu64, &n);

		int64_t ans = -2;
		//Iteration for next solutions
		a3 = 2 * x1 - 1;
		while(a3 <= n+4)
		{
			//area3 = q*(h - 2); <- check both factor to avoid overflow
			if (a3 % 3 == 0 && (q % 3 == 0 || (h - 2)%3 == 0)) {
				perim = a3 - 1;
				if (perim > n) break;
				ans += perim;

			}

			a3 += 2; 
			//area3 = q*(h + 2); <- check both factor to avoid overflow
			if (a3 % 3 == 0 && (q % 3 == 0 || (h + 2) % 3 == 0)) {
				perim = a3 + 1;
				if (perim > n) break;
				ans += perim;
			}

			//next iteration
			h1 = h; q1 = q;
			h = x1*h1 + dy1*q1;
			q = y1*h1 + x1*q1;

			//next a3
			a3 = 2 * h - 1;
		}

		printf("%" PRId64 "\n", ans);
	}



	return 0;
}
