#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <future>
#include <string>

#define NB_threads 6
#define NB_colonnes 50

using namespace std;

// Liste des couleurs dans lesquelles la couleur du texte peut être modifiée.
string Rouge = "\033[01;32m";
string Vert_clair = "\033[01;33m";
string Jaune =  "\033[01;34m";
string Bleu_clair =  "\033[01;35m";
string Magenta_clair =  "\033[01;36m";
string Blanc =  "\033[01;37m";
string couleurs[6] = {Rouge, Vert_clair, Jaune, Bleu_clair, Magenta_clair, Blanc};

// Mutex utilisé pour organiser le flux des threads
mutex m1;
mutex m2;

// bools et entiers utilisés par l'arbitre
bool finished = false;
int counter = 0;
int first = 0;

/**
  * @desc: Déplacer le curseur sur une ligne et une colonne spécifiques
  * @param: int lig - ligne où le curseur doit être déplacé
  * @param: int col - colonne où le curseur doit être déplacé
  * @return: null
*/
void moveto(int lig, int col){
    printf("\033[%d;%df",lig, col);
}

/**
  * @desc: Efface l'écran
  * @return: null
*/
void erase_scr()
{
    printf("\033[2J");
}

/**
  * @desc: Rend le curseur visible à l'écran.
  * @return: null
*/
void set_curseur_visible()
{
    printf("\x1B[?25h");
}

/**
  * @desc: Rend le curseur invisible à l'écran.
  * @return: null
*/
void set_curseur_invisible()
{
    printf("\x1B[?25l");
}

/**
  * @desc: Déplace un participant de la gauche de l'écran à la position NB_colonnes.
  * @param: int ma_ligne - le numéro du participant.
  * @return: null
*/
void courrir(int ma_ligne);

/**
  * @desc: Efface les caractères dans une ligne
  * @return: null
*/
void erase_line(){
	printf("\033[1K");
}

/**
  * @desc: imprime le numéro du participant en première position
  * @return: null
*/
void print_position(){
    // un thread qui imprime le participant actuel à la première place 
    do{
        moveto(21, 25);
        cout << couleurs[first-1]<< first;
        usleep(50000);
    }
    while(!finished); //Tant qu'aucun gagnant n'a été déclaré 
}

int main()
{
    int i ;
    thread tab_id1[NB_threads];
    thread arbitre;
    int tab_num_ligne_a_ecran[NB_threads]; // les valeurs envoyées aux threads
    srand(time(NULL));

    erase_scr(); // On efface l'écran
    set_curseur_invisible();
    moveto(20,1);
    cout << "Créations des threads \n";
    cout << "La première place est : ";
    usleep(1000000); // Attendre 1 seconde pour que le texte soit affiché avant de lancer les threads.

    for (i=0; i< NB_threads; i++)
    {
        tab_num_ligne_a_ecran[i]=i+1;
        tab_id1[i]=thread(courrir, tab_num_ligne_a_ecran[i]);
    }
    arbitre = thread(print_position);

    arbitre.join();
    //cout<< "ATTENTIONS : Sans Join, Main termine et les threads s'arrètent\n" ;
    for (i=0; i< NB_threads; i++)
    {
        tab_id1[i].join();
    }    

    moveto(NB_threads+1, 1);
    cout << Blanc;
    cout << "\n\n\n\n Fin de main\n";
    set_curseur_visible();
    return 0;
}

void courrir(int ma_ligne)
{
    //cout << "Thread << ma ligne << lancé \n" ;
    int i;
    //char mon_signe[3] = {(char)(ma_ligne+((int)'A')-1),'>',0}; // donnerra de 'A'.. 'Z'
    char mon_signe[3] = {'>', '<', '>'};
    for (i=0; i<NB_colonnes; i++)
    {
        //Lorsque la course est terminée, le participant s'arrête de bouger.
        if (finished)
            return;
   
        moveto(ma_ligne, i);
        erase_line();
        moveto(ma_ligne, i);

        // Blocage du mutex pour la déclaration du première place pour éviter la modification simultanée.
        m2.lock();
        if (i > counter){
            counter = i;
            first = ma_ligne;
        }
        m2.unlock();

        // Afficher les caractères à la nouvelle place 
        cout << couleurs[ma_ligne - 1] ;
        puts(mon_signe);
        usleep(2*50000*(rand()%3+1));
        // OU std::this thread::sleep for(std::chrono::nanoseconds(50000*(rand()%3+1)));
    }
    
    /** Blocage du mutex qui gère le gagnant de la course
      * La première personne à atteindre cette section aura un accès exclusif à celle-ci
      * Les autres seront arrêtées par le bool finished
    */
    if (m1.try_lock()){
        finished = true;

        // Déclaration du gagnant de la course
        moveto(21, 1);
        set_curseur_visible();
        cout << couleurs[ma_ligne - 1];
        cout << "Joueur " << ma_ligne << " a gagné la course" << endl;
    }

}

