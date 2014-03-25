#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <memory.h>
#include <vector>
#include <time.h>
using namespace std;

char* filename;
int n;
int a, b, w;
int g[100][100];
int p[100];
int opt[100];
const long long inf = 1000LL*1000LL*1000LL*1000LL;

void output(int* p){
	printf("Permutation:");
	for(int i = 0; i < n; i++){
		printf("%d ", p[i]);
	}
	printf("\n");
}


int main(int argc, char** argv){
	time_t start = clock();

	filename = argv[1];
	FILE* f = freopen(filename, "r", stdin);

	scanf("%d", &n);
	memset(g, 0, sizeof(g));
	while(scanf("%d %d %d", &a, &b, &w) == 3){
		g[a][b] = w;
		g[b][a] = w;
	}
 	
 	for(int i = 0; i < n; i++)
		p[i] = i+1;

	long long ans = inf;
	long long cur = 0;
	int step = 1;
	do{
		//printf("Step %d\n", step);
		
		cur = 0;
		for(int i = 1; i < n; i++){
			cur += g[p[i]][p[i-1]];
		}
		cur += g[p[n-1]][p[0]];

		//output(p);
		//printf("Cur = %lld\n", cur);

		if(cur < ans){
			ans = cur;
			memcpy(opt, p, sizeof(p));
		}

		step++;


	}while(std::next_permutation(&p[0],&p[n]));
	
	printf("Min circle: %lld\n", ans);
	output(opt);

	time_t finish = clock();
	printf("Time: %f\n", ((double)(finish-start))/CLOCKS_PER_SEC);

	return 0;
}