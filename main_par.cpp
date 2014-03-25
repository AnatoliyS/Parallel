#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <memory.h>
#include <mpi.h>
#include <vector>
using namespace std;

#define MASTER 0

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

long long fact[30];
vector<int> getPermutation(int n, int k) {
    if(k > fact[n]) k = fact[n];
    if(k == 0) k = 1;

    vector<int> s;
    int i;        
    vector<int> index(n,0);

    int kk = k - 1;
    for (i=n-1; i>=0; i--) {
        while (kk >= fact[i]) { //index[i] * i! 
            index[i]++;
            kk -= fact[i];
        }
    }

    vector<int> arr;
    for (i=0; i<n; i++) arr.push_back(i+1);
    for (i=n-1; i>=0; i--) {
        s.push_back(arr[index[i]]);
        arr.erase(arr.begin() + index[i]);
    }
    return s;
}


int main(int argc, char** argv){
	double start, end;
	MPI_Status status;
	long long curAns,
		globalAns,
		recievedAns = 10;
	int	taskid,	        /* task ID - also used as seed number */
		numtasks,       /* number of tasks */
		rc;             /* return code */

	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS) {
	    printf ("Error starting MPI program. Terminating.\n");
	    MPI_Abort(MPI_COMM_WORLD, rc);
	}
 
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
 	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

 	MPI_Barrier(MPI_COMM_WORLD); 
	start = MPI_Wtime();
 	
 	//
 	// Every process reads data
 	//
	printf ("MPI task %d has started...\n", taskid);

	fact[0] = 1;
	for(int i = 1; i < 20; i++){
		fact[i] = fact[i-1]*((long long)i);
	}

	filename = argv[1];
	FILE* f = freopen(filename, "r", stdin);

	scanf("%d", &n);
	printf("Worker %d read n = %d\n", taskid, n);

	memset(g, 0, sizeof(g));
	while(scanf("%d %d %d", &a, &b, &w) == 3){
		g[a][b] = w;
		g[b][a] = w;
	}
	printf("Worker %d readed graph.\n", taskid);
	
	//
 	// Master partition allpermutation between processes
 	//
 	long long permutPerWorker = 0;
 	long long numOfPermutations = fact[n];

 	if (taskid == MASTER) { 		
 		int numWorkers = numtasks;
 		permutPerWorker = numOfPermutations / numWorkers + 1;
 		printf("Master Broadcast tasks, permutPerWorker = %lld\n", permutPerWorker);
 	}

 	MPI_Bcast(&permutPerWorker, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
 		
 
 	printf("Task %d, numOfPermutation = %lld of %lld\n", taskid, taskid*permutPerWorker, numOfPermutations);

 	//
 	// Workers do job
 	//

	vector<int> v = getPermutation(n, taskid*permutPerWorker);
 	
 	for(int i = 0; i < n; i++)
		p[i] = v[i];

	printf("Task %d will work with: \n", taskid);
	output(p);

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


	}while(std::next_permutation(&p[0],&p[n]) && step < permutPerWorker);

	printf("Task %d Cur = %lld\n", taskid, ans);


	// Everybody send to Master their results
	rc = MPI_Reduce(&ans, &recievedAns, 1, MPI_LONG_LONG, MPI_MIN, MASTER, MPI_COMM_WORLD);
	if (rc != MPI_SUCCESS)
    	printf("%d: failure on mpc_reduce\n", taskid);

	
	if(taskid == MASTER)
		printf("Min circle: %lld\n", recievedAns);
	//output(opt);


	MPI_Barrier(MPI_COMM_WORLD); 
	end = MPI_Wtime();

	MPI_Finalize();

	if (taskid == 0) { 
	    printf("Runtime = %f\n", end-start);
	}

	return 0;
}