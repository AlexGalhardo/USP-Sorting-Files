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
Referencia:
https://marciobueno.com/arquivos/ensino/ed2/ED2_11_Pesquisa.pdf
*/

typedef struct {
	char uf[2];
	int uf_cod;
	int mun_cod;
	char nome[50];
	int populacao;

}municipio;

typedef struct no {
	struct no *pai,*esq,*dir;
	municipio info;
}folha;

typedef struct {
	folha *raiz;
}arvore;

void ImprimeMunicipio(municipio p);
double BuscaSequencial(municipio *v,int n,int op);
void BuscaBinaria (municipio *v,int n,int op,char nome[50],int mun_cod,int populacao,int p, int r);
void BuscaBST(folha *raiz,int mun_cod);
void MaxHeapfy(municipio *v,int i,int n,int op);
void BuildMaxHeap(municipio *v,int n,int op);
void HeapSort (municipio *v,int n,int op);
void Insere (arvore *a,folha *raiz,municipio x);

int BuscaInterpolacao(municipio *v, int mun_cod, int count);


int main(){

	int i=0, populacao,uf_cod, mun_cod, count, op, op1, op2;
	double tempo;
	municipio *vet=NULL;
	char nome[50],uf[2];
	arvore *a;

	struct timespec start, stop;

	FILE *arq;

	a = (arvore*)malloc(sizeof(arvore));
	a->raiz = NULL;

	arq = fopen("municipios.txt","r");
	if (arq==NULL){
	    printf ("Erro, nao foi possivel abrir o arquivo/n");
	    exit(0);
	}

	while (!feof(arq)){

		fscanf (arq,"%s %d %d %s %d",uf,&uf_cod,&mun_cod,nome,&populacao);

		vet = (municipio *)realloc(vet,(i+1)*sizeof(municipio));

		strcpy(vet[i].nome,nome);
		strcpy(vet[i].uf,uf);
		vet[i].populacao = populacao;
		vet[i].uf_cod = uf_cod;
		vet[i].mun_cod = mun_cod;

		i = i+1;
	} 

	count = i;

	printf ("Digite o metodo de busca desejado:\n");
	printf ("1: Busca Sequencial (Nome; Codigo do Municipio; Populacao)\n");
	printf ("2: Busca Binaria (Nome; Codigo do Municipio; Populacao)\n");
	printf ("3: Busca por Interpolação (Nome; Codigo do Municipio; Populacao)\n");
	printf ("4: Busca por Arvore Binaria de Busca (Codigo do Municipio)\n");

	scanf ("%d",&op);

	switch (op){
		
		case 1:
			printf ("Escolha a chave de ordenacao:\n");
			printf ("1: Nome\n");
			printf ("2: Codigo do Municipio\n");
			printf ("3: Populacao\n");
		
			scanf ("%d",&op1);
		
			tempo = BuscaSequencial(vet,count,op1);
		
			break;
		
		case 2:
			
			printf ("Escolha a chave de ordenacao:\n");
			printf ("1: Nome\n");
			printf ("2: Codigo do Municipio\n");
			printf ("3: Populacao\n");
		
			scanf ("%d",&op1);
			
			HeapSort(vet,count,op1);
			
			switch (op1){
				
				case 1:
					printf ("Digite o nome do municipio que deseja buscar, substituindo espaços por _:\n");
					scanf ("%s", nome);
					break;
				case 2:
					printf ("Digite o codigo do municipio que deseja buscar:\n");
					scanf ("%d", &mun_cod);
					break;
				case 3:
					printf ("Digite a populacao municipio que deseja buscar:\n");
					scanf ("%d", &populacao);
					break;
			}
			
			//tempoInicial = clock();
			if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
		      perror( "clock gettime" );
		      return EXIT_FAILURE;
		    }	
		
			BuscaBinaria(vet,count,op1,nome,mun_cod,populacao,0,count-1);
			
			if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
		      perror( "clock gettime" );
		      return EXIT_FAILURE;
		    }
		    tempo = ( stop.tv_sec - start.tv_sec )+ (double)( stop.tv_nsec - start.tv_nsec )/ (double)BILLION;
			
			break;
			
		case 3:

			printf ("Digite o codigo do municipio que deseja buscar:\n");
			scanf ("%d", &mun_cod);

			if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
		      perror( "clock gettime" );
		      return EXIT_FAILURE;
		    }	

		    BuscaInterpolacao(vet, mun_cod, count);

			if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
		      perror( "clock gettime" );
		      return EXIT_FAILURE;
		    }
			
			tempo = ( stop.tv_sec - start.tv_sec )+ (double)( stop.tv_nsec - start.tv_nsec )/ (double)BILLION;

			break;
			
		case 4:
			
			printf ("Digite o codigo do municipio que deseja buscar:\n");
			scanf ("%d",&mun_cod);
			for (i=0;i<count;i++){
				Insere(a,a->raiz,vet[i]);
			}

			if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
		      perror( "clock gettime" );
		      return EXIT_FAILURE;
		    }

			BuscaBST(a->raiz,mun_cod);

			if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
		      perror( "clock gettime" );
		      return EXIT_FAILURE;
		    }

		    tempo = ( stop.tv_sec - start.tv_sec )+ (double)( stop.tv_nsec - start.tv_nsec )/ (double)BILLION;
			break;
	}

	printf ("Tempo da Busca: %lf Nanoseconds\n", tempo);

	fclose(arq);

	return 0;
}

void ImprimeMunicipio(municipio p){
	
	printf ("\n\n");
	printf ("Municipio: %s\n",p.nome);
	printf ("Estado: %s\n",p.uf);
	printf ("Codigo do Municipio: %.5d\n",p.mun_cod);
	printf ("Codigo do Estado: %d\n",p.uf_cod);
	printf ("Populacao: %d\n",p.populacao);
	printf ("\n\n");
}


int BuscaInterpolacao(municipio *v, int chave, int n) {

	int ini = 0;
	int meio;
	int fim = n - 1;
	while (ini <= fim) {
		meio = ini + ((fim-ini)*(chave-v[ini].mun_cod)) / (v[fim].mun_cod-v[ini].mun_cod);

		if (chave < v[meio].mun_cod) {
			fim = meio - 1;
		} else if (chave > v[meio].mun_cod) {
			ini = meio + 1;
		} else {
			ImprimeMunicipio(v[meio]);
			return meio;
		}
	}
	return -1; // Índice Impossível
}

void HeapSort (municipio *v,int n,int op){
	
	int i,heapsize;
	municipio aux;
	
	heapsize = n;	

	BuildMaxHeap(v,n,op);
	
	for (i=n-1;i>0;i--){
		aux = v[i];
		v[i] = v[0];
		v[0] = aux;
		heapsize = heapsize-1;
		MaxHeapfy(v,0,heapsize,op);
	}
	
}

void BuildMaxHeap(municipio *v,int n,int op){
	
	
	int i;
	
	for (i=(n/2);i>=0;i--){
		MaxHeapfy(v,i,n,op);
	}
}

void MaxHeapfy(municipio *v,int i,int n,int op){

	
	int l,r,largest;
	municipio aux;
	l = (2*i)+ 1;
	r = (2*i) + 2;
	
	switch (op){
	
	case 1:
	
		if (l<n&&strcmp(v[l].nome,v[i].nome)>0){
			largest = l;
		} else {
			largest = i;
		}
	
		if (r<n&&strcmp(v[r].nome,v[largest].nome)>0){
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
	
		if (l<n&&v[l].mun_cod>v[i].mun_cod){
			largest = l;
		} else {
			largest = i;
		}
	
		if (r<n&&v[r].mun_cod>v[largest].mun_cod){
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
	
		if (l<n&&v[l].populacao>v[i].populacao){
			largest = l;
		} else {
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
	}

}

void Insere (arvore *a,folha *raiz,municipio x){
	
	folha *p;
	p = (folha*)malloc(sizeof(folha));
	p->info = x;
	p->pai = NULL;
	p->esq = NULL;
	p->dir = NULL;
	
	if (a->raiz==NULL){
		a->raiz = p;
		return;
	}
	
	if (x.mun_cod<raiz->info.mun_cod){
		if (raiz->esq==NULL){
		raiz->esq = p;
		raiz->esq->pai = raiz;
		return;
		} else {
			Insere(a,raiz->esq,x);
		}
	}
	
	if (x.mun_cod>=raiz->info.mun_cod){
		if (raiz->dir==NULL){
		raiz->dir = p;
		raiz->dir->pai = raiz;
		return;
		} else {
			Insere(a,raiz->dir,x);
		}
	}	
}

double BuscaSequencial(municipio *v,int n,int op){
	
	int i,mun_cod,populacao;
	char nome[50];
	double tempo;

	struct timespec start, stop;
	
	switch (op){
		
		case 1:
			
			printf ("Digite o nome do municipio que deseja buscar, substituindo espaços por _:\n\n");
			scanf ("%s",nome);
			
			if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
		      perror( "clock gettime" );
		      return EXIT_FAILURE;
		    }
			
			for (i=0;i<n;i++){
				
				if (strcmp(v[i].nome,nome)==0){
					ImprimeMunicipio(v[i]);
					if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
				      perror( "clock gettime" );
				      return EXIT_FAILURE;
				    }
					tempo = ( stop.tv_sec - start.tv_sec )+ (double)( stop.tv_nsec - start.tv_nsec )/ (double)BILLION;
					return tempo;
				}
				
			}
			
			printf ("\n\nO municipio buscado nao foi encontrado.\n\n");
			break;
			
		case 2:
			
			printf ("Digite o codigo do municipio que deseja buscar:\n");
			scanf ("%d",&mun_cod);
			
			if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
		      perror( "clock gettime" );
		      return EXIT_FAILURE;
		    }
			
			for (i=0;i<n;i++){
				
				if (v[i].mun_cod==mun_cod){
					ImprimeMunicipio(v[i]);
					if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
				      perror( "clock gettime" );
				      return EXIT_FAILURE;
				    }
					tempo = ( stop.tv_sec - start.tv_sec )+ (double)( stop.tv_nsec - start.tv_nsec )/ (double)BILLION;
					return tempo;
				}
				
			}
			
			printf ("\n\nO municipio buscado nao foi encontrado.\n\n");
			break;
			
		case 3:
			
			printf ("Digite a populacao municipio que deseja buscar:\n");
			scanf ("%d",&populacao);
			
			if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
		      perror( "clock gettime" );
		      return EXIT_FAILURE;
		    }
			
			for (i=0;i<n;i++){
				
				if (v[i].populacao==populacao){
					ImprimeMunicipio(v[i]);
					if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
				      perror( "clock gettime" );
				      return EXIT_FAILURE;
				    }
					tempo = ( stop.tv_sec - start.tv_sec )+ (double)( stop.tv_nsec - start.tv_nsec )/ (double)BILLION;
					return tempo;
				}
				
			}
			
			printf ("\n\nO municipio buscado nao foi encontrado.\n\n");
			break;
		
		
		
	}
}

void BuscaBinaria(municipio *v,int n,int op,char nome[50],int mun_cod,int populacao,int p,int r){
	
	int i;

	
	
	switch (op){
		
		case 1:
			
			i = p + (r-p)/2;	
			
			if (strcmp(nome,v[i].nome)==0){
				ImprimeMunicipio(v[i]);
				return;
			}
			
			if (p==r){
				printf ("\n\nO municipio buscado nao foi encontrado.\n\n");
				return;
			}
			
			if (strcmp(nome,v[i].nome)<0){
				BuscaBinaria(v,n,op,nome,mun_cod,populacao,p,i-1);
			}
			
			if (strcmp(nome,v[i].nome)>0){
				BuscaBinaria(v,n,op,nome,mun_cod,populacao,i+1,r);
			}
			
			break;

			
			break;
			
		case 2:
					
			i = p + (r-p)/2;	
			
			if (mun_cod==v[i].mun_cod){
				ImprimeMunicipio(v[i]);
				return;
			}
			
			if (p==r){
				printf ("\n\nO municipio buscado nao foi encontrado.\n\n");
				return;
			}
			
			if (mun_cod<v[i].mun_cod){
				BuscaBinaria(v,n,op,nome,mun_cod,populacao,p,i-1);
			}
			
			if (mun_cod>v[i].mun_cod){
				BuscaBinaria(v,n,op,nome,mun_cod,populacao,i+1,r);
			}
			
			break;
			
		case 3:
			
			i = p + (r-p)/2;	
			
			if (populacao==v[i].populacao){
				ImprimeMunicipio(v[i]);
				return;
			}
			
			if (p==r){
				printf ("\n\nO municipio buscado nao foi encontrado.\n\n");
				return;
			}
			
			if (populacao<v[i].populacao){
				BuscaBinaria(v,n,op,nome,mun_cod,populacao,p,i-1);
			}
			
			if (populacao>v[i].populacao){
				BuscaBinaria(v,n,op,nome,mun_cod,populacao,i+1,r);
			}
			break;
		
		
		
	}
}

void BuscaBST(folha *raiz,int mun_cod){
	
	if (raiz==NULL){
		printf ("\n\nO municipio buscado nao foi encontrado.\n\n");
		return;
	}

	if (raiz->info.mun_cod == mun_cod){
		ImprimeMunicipio(raiz->info);
		return;
	}

	if (mun_cod<raiz->info.mun_cod){
		BuscaBST(raiz->esq,mun_cod);
	} 
	
	if (mun_cod>raiz->info.mun_cod){
		BuscaBST(raiz->dir,mun_cod);
	} 
	
	return;
}

