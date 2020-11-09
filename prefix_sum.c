#include <stdio.h>
#include <math.h>
#include <stdbool.h>

void display(int x, int y, int tree[x][y]){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			if(tree[i][j]!=0) printf("%i\t", tree[i][j]);
			else printf("_\t");
		}
		printf("\n");
	}
}
int main(void)
{
	int a[8] = {3,1,4,1,5,9,2,6};
	
	//variables based on A
	int sizea = sizeof(a)/sizeof(a[0]);
	int layers = (int)(log(sizea)/log(2) + 1.5);
	int tree[layers][sizea];
	for(int i = 0; i<layers; i++){			//make the tree all 0s
		for(int j = 0; j<sizea; j++){
			tree[i][j] = 0;
		}
	}
	for(int i = 0; i<sizea; i++){				//fill out the bottom/last layer
		tree[layers-1][i] = a[i];
	}
	
	
	int pairsum = 0;

	//printf("sizea:%i layers:%i\n", sizea, layers);
	
	//printf("jtest: %i\n", (int)(pow(2,3)));

	
	for (int i = layers-1; i>=0; i--){ //go through layers
		//xmt
		
   	#pragma omp parallel for private(j)
		//printf("layers: %i\n", i);
		for (int j = 0; j<(int)(pow(2,i)); j+=2){	//goes through every other index in the row
			//printf("j: %i\n", j);
			
			pairsum = tree[i][j] + tree[i][j+1];
			
			//printf("pairsum: %i\n", pairsum);
			
			if(i!=0) tree[i-1][j/2] = pairsum;
		}
	}
	display(layers, sizea, tree);
	printf("\n\n\ntotalsum: %i\n\n\n", pairsum);
	
	
	////prefix tree
	int prefixtree[layers][sizea];
	for(int i = 0; i<layers; i++){			//make the prefixtree all 0s
		for(int j = 0; j<sizea; j++){
			prefixtree[i][j] = 0;
		}
	}
	
	//fill out the prefixtree
	prefixtree[0][0] = tree[0][0];
	for (int a = 0; a<layers; a++){
		//xmt
		#pragma omp parallel for private(b)
		for (int b = 0; b<(int)(pow(2,a)); b++){	//goes through the indices in the current row
			if (b==0) prefixtree[a+1][b] = tree[a+1][b];							//first left child
			else prefixtree[a+1][2*b] = prefixtree[a][b-1] + tree[a+1][2*b];	//other left children
			
			printf("cousin: %i, corr: %i\n", prefixtree[a][b-1], tree[a+1][b]);
						
			prefixtree[a+1][2*b+1] = prefixtree[a][b];								//right child
			
			printf("a: %i, b: %i\n", a, b);
			display(layers, sizea, prefixtree);
			printf("\n");
			/*printf("a: %i\n",a);
			printf("b: %i\n",b);
			printf("parent: %i\n",prefixtree[a][b]);
			printf("right child: %i\n\n",prefixtree[a+1][b+1]);
		*/}
	}
	
	printf("\n\n\nFINAL TREE:\n");
	display(layers, sizea, prefixtree);
	
}
