/* pmm-secuencial.c
   Producto de dos matrices cuadradas, M y L.
   Para compilar usar (-lrt: real time library)
   gcc  -O2 pmv-secuencial.c -o pmv -lrt
*/

#include <stdlib.h> // biblioteca con funciones atoi(), malloc() y free()
#include <stdio.h> // biblioteca donde se encuentra la función printf()
#include <time.h>  // biblioteca donde se encuentra la función clock_gettime()


int main(int argc, char** argv){

    int i;

    struct timespec cgt1,cgt2; double ncgt; //para tiempo de ejecución

    //Leer argumento de entrada (nº de filas y columnas)
    if (argc<2){
        printf("Error: Falta el número de filas y columnas.\n");
        exit(-1);
    }
    unsigned int f = atoi(argv[1]);// Máximo N =2^32-1=4294967295 (sizeof(unsigned int) = 4 B)

    double **M = (double **)malloc(f*sizeof(double*)) ;
    for( i = 0 ; i < f ; i++) M[i] = (double*)malloc(f * sizeof(double)) ;
    double **L = (double **)malloc(f*sizeof(double*)) ;
    for( i = 0 ; i < f ; i++) L[i] = (double*)malloc(f * sizeof(double)) ;
    // matriz resultado
    double **R = (double **)malloc(f*sizeof(double*)) ;
    for( i = 0 ; i < f ; i++) R[i] = (double*)malloc(f * sizeof(double)) ;

    if ( (L==NULL) ||  (M==NULL) || (R==NULL)){
        printf("Error en la reserva de espacio para los vectores\n");
        exit(-2);
    }

    int j ;

    //Inicializar matrices

    for(int i = 0 ; i < f ; i++){
        for(int j = 0 ; j < f ; j++){
            M[i][j] = 0.5*(i+j);
            L[i][j] = (i+j) ;
            R[i][j] = 0 ;
        }
    }

    clock_gettime(CLOCK_REALTIME,&cgt1);

    // transposición del a matriz L para eliminar fallos de caché.

    for(i = 0 ; i < f ; i++)
        for(j = 0 ; j < f ; j++){
            if(i != j){
                int aux = L[i][j] ;
                L[i][j] = L[j][i] ;
                L[j][i] = aux ;
            }
        }

    int k ;
    //Calcular multiplicación.
    for(i=0; i<f; i++){
        for(j=0 ; j<f ; j++){
            for(k=0 ; k<f ; k++){
                R[i][j] += M[i][k]*L[j][k];
            }
        }
    }
    clock_gettime(CLOCK_REALTIME,&cgt2);
    ncgt+=(double) (cgt2.tv_sec-cgt1.tv_sec)+
        (double) ((cgt2.tv_nsec-cgt1.tv_nsec)/(1.e+9));



    printf("\nR[0][0]=%f ; R[n-1][n-1] = %f", R[0][0], R[f-1][f-1]) ;

    ncgt = ncgt ;

    printf("\nTiempo(seg.): %11.9f\t / filas: %u\t / columnas: %u\t",
           ncgt,f,f) ;

    FILE * fp ;
    fp = fopen("salida","a");
    if(fp==NULL){perror("Error opening file.");}
    fprintf(fp,"\n%d\t%f",f,ncgt);
    fclose(fp);

}
