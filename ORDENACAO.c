/*
Alunos

Ian Castilho Caldeira Brant  Nº 10133967
Alex Galhardo Nº 10408344
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define BILLION  1000000000L;




/* 
ReferÃªncia Nanoseconds: 
https://stackoverflow.com/questions/13610471/calculating-function-time-in-nanoseconds-in-c-code 
*/

typedef struct {
	char nome[30];
	char regiao[30];
	long int populacao;
	int area;
	float densidade;
} pais;

void ImprimePais(pais p);
void MaxHeapfy(pais *v,int i,int n,int op);
void BuildMaxHeap(pais *v,int n,int op);
double HeapSort (pais *v,int n);
double CountingSort(pais *v, int n);
double QuickSort (pais *v,int p,int r,int op1,int op2, double *mili);
int Partition(pais *v,int p,int r,int pivo,int op);

int main (){

	int i=0, area, count, op, op1, op2;
	double mili=0;

	double tempo;
	long int populacao;

	pais *vet=NULL;

	char nome[30], regiao[30];
	float densidade;

	FILE *arq;

	arq = fopen("countries.txt", "r");

	if (arq==NULL){
		printf ("Erro, nao foi possivel abrir o arquivo!/n");
		exit(0);
	}

	while (!feof(arq)){
		/* abre o arquivo texto e salva os dados em uma struct */
		fscanf(arq,"%s %s %li %d %f", nome, regiao, &populacao, &area, &densidade);

		/* aloca espaÃ§o na struct */
		vet = (pais *)realloc(vet,(i+1)*sizeof(pais));

		strcpy(vet[i].nome, nome);
		strcpy(vet[i].regiao,regiao);
		vet[i].populacao = populacao;
		vet[i].area = area;
		vet[i].densidade = densidade;

		i = i+1;
	} 
	
	
	
	count = i;
	
	

	printf ("Digite o metodo de ordenacao desejado:\n");
	printf ("1: QuickSort (Populacao; Nome ; Densidade Populacional)\n");
	printf ("2: HeapSort (Populacao; Area ; Nome)\n");
	printf ("3: CountingSort (Area)\n");
	scanf ("%d",&op);


	switch (op){
		
		case 1:
			printf ("Escolha a chave de ordenacao:\n");
			printf ("1: Populacao\n");
			printf ("2: Nome\n");
			printf ("3: Densidade Populacional\n");
		
			scanf ("%d", &op1);
		
			printf ("Escolha o metodo de escolha do pivo:\n");
			printf ("1: Primeiro Elemento\n");
			printf ("2: Elemento do Meio\n");
			printf ("3: Ultimo Elemento\n");
		
			scanf ("%d", &op2);
		
			tempo = QuickSort(vet, 0, count - 1, op1, op2, &mili);
			break;

		case 2:
			tempo = HeapSort(vet, count);
			break;
			
		case 3:
			tempo = CountingSort(vet, count);
			break;
	}


	/* imprimir dados ordenados modo console */
	for (i=0;i<count;i++){
		ImprimePais(vet[i]);
	}


	/* salvar dados ordenados modo arquivo */
	if(op==1){

		FILE *ordenadoQuickSort = fopen("ordenadoQuickSort.txt", "w");
		if (arq == NULL){
			exit(0);
		}

		fprintf(ordenadoQuickSort, "Tempo de ordenacao QuickSort: %lf Nanosseconds\n\n", tempo);
		for(i=0; i < count; i++){
			fprintf(ordenadoQuickSort, "Nome Pais: %s\nRegiao: %s\nPopulacao: %li\nArea: %d\nDensidade: %f\n\n", vet[i].nome, vet[i].regiao, vet[i].populacao, vet[i].area, vet[i].densidade);
		}
		printf("Tempo de ordenacao QuickSort: %lf Nanosseconds\n", tempo);
	}
	else if(op == 2){

		FILE *ordenadoHeapSort = fopen("ordenadoHeapSort.txt", "w");
		if (arq == NULL){
			exit(0);
		}

		fprintf(ordenadoHeapSort, "Tempo de ordenacao HeapSort: %lf Nanosseconds\n\n", tempo);
		for(i=0; i < count; i++){
			fprintf(ordenadoHeapSort, "Nome Pais: %s\nRegiao: %s\nPopulacao: %li\nArea: %d\nDensidade: %f\n\n", vet[i].nome, vet[i].regiao, vet[i].populacao, vet[i].area, vet[i].densidade);
		}
		printf("Tempo de ordenacao HeapSort: %lf Nanosseconds\n", tempo);
	}
	else{

		FILE *ordenadoCountingSort = fopen("ordenadoCountingSort.txt", "w");
		if (arq == NULL){
			exit(0);
		}

		fprintf(ordenadoCountingSort, "Tempo de ordenacao CountingSort: %lf Nanosseconds\n\n", tempo);
		for(i=0; i < count; i++){
			fprintf(ordenadoCountingSort, "Nome Pais: %s\nRegiao: %s\nPopulacao: %li\nArea %d\nDensidade: %f\n\n", vet[i].nome, vet[i].regiao, vet[i].populacao, vet[i].area, vet[i].densidade);
		}
		printf("Tempo de ordenacao CountingSort: %lf Nanoseconds\n", tempo);
		printf("Tempo de ordenacao CountingSort: %lf Milliseconds\n", tempo / 1000000);
	}

	fclose(arq);

	return 0;
}

void ImprimePais(pais p){
	printf ("Pais: %s\n", p.nome);
	printf ("Regiao: %s\n", p.regiao);
	printf ("Populacao: %li\n", p.populacao);
	printf ("Area: %d\n",p.area);
	printf ("Densidade Populacional: %.1f\n", p.densidade);
	printf ("\n");
}

double HeapSort (pais *v, int n){
	
	int i,heapsize,op;
	double tempo;
	pais aux;
	
	printf ("Escolha a chave de ordenacao:\n");
	printf ("1: Populacao\n");
	printf ("2: Area\n");
	printf ("3: Nome\n");
	
	scanf ("%d",&op);
	
	heapsize = n;
	
	struct timespec start, stop;
	double accum;

	if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

	BuildMaxHeap(v,n,op);
	
	for (i=n-1;i>0;i--){
		aux = v[i];
		v[i] = v[0];
		v[0] = aux;
		heapsize = heapsize-1;
		MaxHeapfy(v,0,heapsize,op);
	}
	
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

    accum = ( stop.tv_sec - start.tv_sec )+ (double)( stop.tv_nsec - start.tv_nsec )/ (double)BILLION;
    
	return accum;
}

void BuildMaxHeap(pais *v, int n,int op){
	
	int i;
	for (i=(n/2); i>=0 ;i--){
		MaxHeapfy(v, i, n, op);
	}
}

void MaxHeapfy(pais *v, int i, int n, int op){

	int l,r,largest;
	pais aux;
	l = (2*i)+ 1;
	r = (2*i) + 2;
	
	switch (op){
	
	case 1:
	
		if (l<n&&v[l].populacao>v[i].populacao){
			largest = l;
		}
		else {
			largest = i;
		}
	
		if (r<n&&v[r].populacao>v[largest].populacao){
			largest = r;
		}
	
		if (largest!=i){
			aux = v[i];
			v[i] = v[largest];
			v[largest] = aux;
			MaxHeapfy(v,largest,n,op);
		}
		break;
		
	case 2:
	
		if (l<n && v[l].area> v[i].area){
			largest = l;
		} else {
			largest = i;
		}
	
		if (r < n && v[r].area>v[largest].area){
			largest = r;
		}
	
		if (largest!=i){
			aux = v[i];
			v[i] = v[largest];
			v[largest] = aux;
			MaxHeapfy(v,largest,n,op);
		}
		break;
		
	case 3:
	
		if (l < n&& strcmp(v[l].nome, v[i].nome)>0){
			largest = l;
		} else {
			largest = i;
		}
	
		if (r < n && strcmp(v[r].nome, v[largest].nome) >0){
			largest = r;
		}
	
		if (largest!=i){
			aux = v[i];
			v[i] = v[largest];
			v[largest] = aux;
			MaxHeapfy(v,largest,n,op);
		}
		break;
	}

}

double CountingSort(pais *v, int n){
	
	int k,i, j,*aux;
	pais *ord;
	
	struct timespec start, stop;
	double tempo;

	if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

    printf("...esta ordenando...\n");
	
	k = v[0].area;

	for (i=0;i<n;i++){
		if (v[i].area>k){
			k = v[i].area;
		}
	}

	ord = (pais*)calloc(n,sizeof(pais));
	aux = (int*)calloc(k+1,sizeof(int));
	
	for (i=0;i<=k;i++){
		for (j=0;j<n;j++){
			if (v[j].area==i){
				aux[i] = aux[i] + 1;
			}
		}
	}
	

	for (i=1;i<=k;i++){
		aux[i] = aux[i] + aux[i-1];
	}
	
	
	for(i = n-1; i >=0; i--){
           ord[aux[v[i].area]] = v[i];
            aux[v[i].area]--;
        }
        
    for (i=0;i<n;i++){
    	v[i]=ord[i+1];
	}
	
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

    tempo = ( stop.tv_sec - start.tv_sec )+ (double)( stop.tv_nsec - start.tv_nsec )/ (double)BILLION;
	
	return tempo;
}

double QuickSort(pais *v, int p, int r, int op1, int op2, double *mili){
	
	int q,pivo,med[3];
	double tempo;

	struct timespec start, stop;

	if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }
	
	switch (op2){
		
		case 1:
			pivo = p;
			break;
		
		case 2:
			pivo = p + (r-p)/2;
			break;
			
		case 3:
			pivo = r;
			break;
	}
	
	if (p<r){
		q = Partition(v,p,r,pivo,op1);
		QuickSort(v,p,q-1,op1,op2, mili);
		QuickSort(v,q+1,r,op1,op2, mili);
	}

	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

    tempo = ( stop.tv_sec - start.tv_sec )+ (double)( stop.tv_nsec - start.tv_nsec )/ (double)BILLION;
	
	return tempo;
}

int Partition(pais *v,int p,int r,int pivo,int op){
	
	int i,j;
	long int x;
	char y[30];
	pais aux;
	i = p-1;
	
	aux = v[pivo];
	v[pivo] = v[r];
	v[r] = aux;
	
	switch (op){
		
		case 1:
	
			x = v[r].populacao;
	
	
			for (j=p;j<r;j++){
				if (v[j].populacao<=x){
					i = i + 1;
					aux = v[i];
					v[i] = v[j];
					v[j] = aux;
				}
			}
			aux = v[i+1];
			v[i+1] = v[r];
			v[r] = aux;
	
			return i+1;
			
		case 2:
	
			strcpy(y,v[r].nome);
	
	
			for (j=p;j<r;j++){
				if (strcmp(v[j].nome,y)<=0){
					i = i + 1;
					aux = v[i];
					v[i] = v[j];
					v[j] = aux;
				}
			}
			aux = v[i+1];
			v[i+1] = v[r];
			v[r] = aux;
	
			return i+1;
			
		case 3:
	
			x = (v[r].densidade)*10;
	
	
			for (j=p;j<r;j++){
				if ((v[j].densidade)*10<=x){
					i = i + 1;
					aux = v[i];
					v[i] = v[j];
					v[j] = aux;
				}
			}
			aux = v[i+1];
			v[i+1] = v[r];
			v[r] = aux;
	
			return i+1;
	}
}
