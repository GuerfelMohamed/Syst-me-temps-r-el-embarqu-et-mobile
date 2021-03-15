#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

/**
 * @desc: Estime la valeur de Pi par l'experience de DartBoard 
 * @return: Renvoie la valeur estimée de Pi pour une iteration.
*/
double DartBoard(){
    double darts = 5000;
    double circle_count = 0.0f;

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


int main()
{
    srand( (unsigned)time( NULL ) );

    // continent les valeurs estimées cumulées de PI afin de calculer la moyenne et la valeur finale de PI.
    double valuePI = 0.0f;
    for(int i = 0; i < 500000; i++){
        valuePI += DartBoard();
        cout << "iterations: " << i+1 << ", valeur de PI: " << valuePI/(i+1) << endl;
    }

    return 0;
}