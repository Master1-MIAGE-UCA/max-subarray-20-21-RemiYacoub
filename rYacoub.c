#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

struct tablo {
  int * tab;
  int size;
};

void printArray(struct tablo * tmp) {
  printf("---- Array of size %i ---- \n", tmp->size);
  int size = tmp->size;
  int i;
  for (i = 0; i < size; ++i) {
    printf("%i ", tmp->tab[i]);
  }
  printf("\n");
}

struct tablo * allocateTablo(int size) {
  struct tablo * tmp = malloc(sizeof(struct tablo));
  tmp->size = size;
  tmp->tab = malloc(size*sizeof(int));
  return tmp;
}

void montee(struct tablo * source, struct tablo *destination) {
  
  int half = (int) destination->size/2;
  int i;
  for ( i=1 ; i<half ; i++ ) {
      destination->tab[i] = 0;
  }
 for( i=0 ; i <source->size ; i++ ){
    destination->tab[half]=source->tab[i];
    half++;
  }
  for (int l = log2(source->size)-1; l>=0 ; l-- ){
    #pragma omp parallel
    {
      int up_bo = pow( 2, l+1 );
      for (int j = pow(2,l)+1; j<up_bo-1 ; j++){
          destination->tab[j] = destination->tab[2*j] + destination->tab[2*(j+1)] ;
      }
    }
  }
}

void descente(struct tablo * source, struct tablo * destination) {
  
  destination->tab[0]=0;
  for( int l=0 ; l< log2(destination->size) ; l++){
    #pragma omp parallel
    {
      int n = pow(2,(l+1))-1;
      for( int j = pow(2,l) ; j <= n ; j++){
        if( j%2 == 0 ) {
          destination->tab[j] = destination->tab[(j/2) ];
        } else {
          destination->tab[j] = source->tab[j-1] + destination->tab[((j-1)/2)];
        }
      }
    }
  }
}

void final(struct tablo * source, struct tablo *destination) {
  
  #pragma omp parallel
  {
    int m = log2(destination->size)-1 ;
    int j ;
    int up_bo = pow(2, m+1) ; 
    for( j=pow(2, m) ; j < up_bo-1 ; j++ ){
          destination->tab[j] = destination->tab[j] + source->tab[j] ;
    }
  }
}

void generateArray(struct tablo * s, int siz , char** args) {
  //construction d'un tableau pour tester
  s->size = siz; printf("Q : ");
  s->tab=malloc(s->size * sizeof(int));
  for (int i = 0 ; i < s->size - 1; i++ ){
    s->tab[i]= atoi( args[i+1] );
    printf("%d ",s->tab[i]);
  }
  printf(" \n " );
}

void generateArrayc(struct tablo * s, int siz) {
  //construction d'un tableau pour tester
  s->size = siz; printf("Q : ");
  s->tab=malloc(s->size * sizeof(int));
  for (int i = 0 ; i < s->size - 1; i++ ){
    s->tab[i]= 0;
    printf("%d ", s->tab[i]);
  }
  printf(" \n " );
}


void getPsum(struct tablo * s, struct tablo * ads, int siz ) {

  int acc =0;
  #pragma omp parallel 
  {
      for (int i = 0 ; i <= siz-1; ++i ){
        ads->tab[i] = acc+ (s->tab[i]); 
        acc = ads->tab[i];
      }
  }

  printf(" \n " );
}

void getSsum(struct tablo * s, struct tablo * ads, int siz ) {

  int acc = 0;
  #pragma omp parallel for
  for (int i = siz-1 ; i >=0 ; --i ){
    ads->tab[i] = acc+ (s->tab[i]); 
    acc = ads->tab[i];
  }
  printf(" \n " );
}


 #define max(a,b) \
       ({ typeof (a) _a = (a); \
           typeof (b) _b = (b); \
         _a > _b ? _a : _b; })

void getSmax(struct tablo * s, struct tablo * ds, int siz ) {

  int acc = 0;
  #pragma omp parallel for
  for (int i = 0 ; i <= siz-1; i++ ){
    ds->tab[i] = max(acc, (s->tab[i])); 
    acc = ds->tab[i];
  }
  printf(" \n " );
}
void getPmax(struct tablo * s, struct tablo * ds, int siz ) {

  int acc = 0;
  #pragma omp parallel for
  for (int i = siz-1; i >= 0; i-- ){
    ds->tab[i] = max(acc, (s->tab[i]));
    acc = ds->tab[i];
  }
  printf(" \n " );  
}
/*
void getmm(struct tablo * Ms, struct tablo * PMAX,  struct tablo * SMAX, struct tablo * PSUM, struct tablo * SSUM, int siz ) {
  #pragma omp parallel for
  for  (int i =1 ; i <= n ; i++ ){
      Ms->tab[i] = PMAX->tab[i] - SSUM->tab[i] + Q->tab[i]
      Mp->tab[i] = SMAX->tab[i] - PSUM->tab[i] + Q->tab[i]
      M->tab[i] = Ms->tab[i] + Mp->tab[i] - Q->tab[i]
  }
  printf(" \n " );  
}
*/
void get_seq(struct tablo * s, struct tablo * ds, int siz ) {
  //construction d'un tableau pour tester
  ds->size = siz; printf("Result : ");
  //printf("Result : ");
  ds->tab=malloc(ds->size * sizeof(int));
  int acc = 0;
  #pragma omp parallel for
  for (int i = 0 ; i < ds->size - 1; i++ ){
    //ds->tab[i]= atoi( args[i+1] );
  }
  printf(" \n " );  
}

int main(int argc, char**  argv) {


  struct tablo q;
  generateArray( &q, argc , argv );
  

  struct tablo psum;
    generateArrayc( &psum, argc);

  getPsum(&q ,&psum, argc );
  printArray(&psum);
  struct tablo ssum;
    generateArrayc( &ssum, argc);

  getSsum(&q ,&ssum, argc );
 //printArray(&ssum);
  struct tablo smax;
    generateArrayc( &smax, argc);

  getSmax(&q ,&smax, argc );
  
  printArray(&smax);
  struct tablo pmax;
    generateArrayc( &pmax, argc);


  getPmax(&q ,&pmax, argc );
 //printArray(&pmax);

  struct tablo mm;
      generateArrayc( &mm, argc);

  //getmm(&q ,&mm, argc );
 //printArray(&mm);

  struct tablo seq;
      generateArrayc( &seq, argc);

  get_seq(&q , &seq, argc );
 //printArray(&seq);

/*
  struct tablo * a = malloc(sizeof(struct tablo));
  a = allocateTablo(1+ q.size*2 );

  montee(&q, a);
  printArray(a);

  struct tablo * b = malloc(sizeof(struct tablo));
  b = allocateTablo(1+ (q.size*2 ));

  descente(a, b);
  printArray(b);
	
  final(a,b);
  printArray(b);
  */

}