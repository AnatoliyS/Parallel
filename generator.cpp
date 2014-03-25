#include <stdio.h>
#include <stdlib.h>

int n;
int maxw;
int w;
char* filename;

int main(int argc, char** argv){
	filename = argv[1];
	n = atoi(argv[2]);
	maxw = atoi(argv[3]);

	FILE* f = freopen(filename, "w", stdout);
	
	printf("%d\n", n);
	for(int i = 1; i < n; i++)
		for(int j = i+1; j <= n; j++){
			w = int(double(rand())/double(RAND_MAX)*maxw);
			printf("%d %d %d\n", i, j, w);
		}


	return 0;
}