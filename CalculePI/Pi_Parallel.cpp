#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread>

// Augmenter NUM_ITERATIONS pour des estimations plus précises 
#define NUM_ITERATIONS 100000
#define NUM_THREADS 5

using namespace std;

// contient la somme cumulative globale des Pi estimés .
double globalValuePI = 0.0f;

/**
 * @desc: Estime la valeur de Pi par l'experience de DartBoard .
 * @return: Renvoie la valeur estimée de Pi pour une iteration.
*/
double DartBoard(){
    double darts = 5000;
    double circle_count = 0;

    // Simuler le lancement d'une flèche sur un plateau et récupérer ses x et y
    for(int i = 0; i < darts; i++){
        double xcoordinate, ycoordinate;
        xcoordinate = (double) rand()/RAND_MAX;
        ycoordinate = (double) rand()/RAND_MAX;

        // Obtenir le nombre de flèches qui sont tombées dans le cercle de rayon 1
        if ((xcoordinate * xcoordinate) + (ycoordinate * ycoordinate) <= 1){
            circle_count++;
        }
    }

    // La valeur estimer de PI
    double PI = (4.0f*circle_count)/darts;

    return PI;
}

/**
 * @desc: Utiliser des expériences de dartboard pour une fraction du nombre total de simulations
 * @param: int iterations - Nombre d'itérations à partir du nombre total d'itérations que le thread va exécuter.
 * @return: Renvoie la valeur estimée de Pi par tout les simulations.
*/
void CalculatePI(int iterations){
    // contient la somme cumulative local pour chaque thread des Pi estimés 
    double localValuePI = 0.0f;
    for(int i = 0; i < iterations; i++){
        localValuePI += DartBoard();
    }
    globalValuePI += localValuePI;
}

int main()
{
    srand( (unsigned)time( NULL ) );

    /**
     * Lance NUM_THREADS threads, chacun calculera la somme cumulée de l'IP estimé
     * pour sa partie des simulations totales.
    */ 
    thread Ids[NUM_THREADS];
    for(int k = 0; k < NUM_THREADS; k++){
        Ids[k] = thread(CalculatePI, NUM_ITERATIONS/NUM_THREADS);
    }
    for(int k = 0; k < NUM_THREADS; k++) Ids[k].join();

    cout<< "Valeur approchee de Pi: " << globalValuePI/(double)NUM_ITERATIONS << endl;

    return 0;
}