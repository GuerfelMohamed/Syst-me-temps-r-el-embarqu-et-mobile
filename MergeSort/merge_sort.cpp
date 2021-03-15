#include <iostream>
#include  <fstream>
#include <vector>
#include <algorithm>
#include <thread>         
#include <mutex> 

using namespace std;

#define THREADS 6 // Nombre maximal de threads à utiliser dans le programme
std::mutex mtx;
int thread_counter=0; 

/**
 * Cette fonction fusionne deux tableux T1 et T2 dans un seul tableau T
 * Les elements de T1 et T2 sont inserer dans T par ordre croissant   
 * @@param Deux Pointers sur les tableaux à fusionner.
 * @@return Le resultat de fusion de deux tableaux.
 * */
std::vector<int> merge_table(const vector<int>& T1, const vector<int>& T2){
	
    std::vector<int> T;
    auto item_T1 = T1.begin(), item_T2 = T2.begin();
	while( item_T1 != T1.end() && item_T2 !=T2.end()){
		if(*item_T1 <= *item_T2){
			T.push_back(*item_T1);
			item_T1++;
		}else{
			T.push_back(*item_T2);
			item_T2++;
		}		
	}
    T.insert(T.end(),item_T1,T1.end());
    T.insert(T.end(),item_T2,T2.end());

    return T;
}

/**
 * Une Fonction récursive qui se lance parallèlement (à l'aide de l'utilisation de threads) sur 
 * des moreaux de la tableau T dans la limite de nombre de threads disponible. T est diviser en 
 * deux sous tableaux (T1 et T2), "merge_sort" est appellé récursivement sur ces deux sous tableaux
 * en mode thread si y en a encore disponible, en mode noramle sinon. Une fois la condition d'arret 
 * de récursivité est atteinte et tous les threads se termine, la fonction "merge_table" commence.
 * @@param Pointer sur tableau à trier
 * */
void merge_sort( vector<int>& T){
    
    // Condition d'arret de récursivité
    if(T.size() == 1)
        return;

    // définir un pivot pour le tableau T
    int pivot = T.size()/2;

    // diviser T en deux sous tableaux T1 et T2
    vector<int> T1 = {T.begin(), T.end()-pivot};
    vector<int> T2 = {T.end()-pivot, T.end()};

    // Section critique besion d'être protegée (lecture/écriture de thread_counter), d'ou l'utilisation de verro "mtx"
    mtx.lock();
    if(thread_counter<THREADS){ // Nous disposons encore de threads à utiliser
        std::thread sort_thread(merge_sort, std::ref(T1));
        thread_counter++;
        mtx.unlock();           // fin de section critique
        merge_sort(T2);
        sort_thread.join();     //  merge_sort de T attend jusqu'à tous les threads se termine 
    }
    else{ 
        mtx.unlock();
        merge_sort(T1);
        merge_sort(T2);
    }

    T = merge_table(T1,T2); // fusionner T1 et T2 dans T
}


int main() {
	vector<int> T;
    int aux;

    
    //Lire un tableau à partir du fichier "Table.txt"
    fstream inFile ("Table.txt", ios_base::in);
    while ( inFile >> aux) {
        T.push_back(aux);
    }

    //Afficher le tableau avant le tri
    cout << "Before sort : ";
    for(int i=0;i<T.size();i++)
		cout<<T[i]<<" ";
    cout<<endl; 
    
    // appeler la fonction "merge_sort"
    if(T.size()>1)
        merge_sort(T);
    
    // Afficher le tableau après le tri 
    cout << "After sort : ";
    for(int i=0;i<T.size();i++)
		cout<<T[i]<<" ";
    cout<<endl;

    return 0;
    }
