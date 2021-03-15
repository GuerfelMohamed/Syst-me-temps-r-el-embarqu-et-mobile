#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

// définir la durée d'attente pour chaque threads avant de se relancer.
#define DELAY_CONTROLLER 3 
#define	DELAY_TEMPERATURE 2
#define	DELAY_PRESSION 2
#define	DELAY_ECRAN 1
using namespace std;

// Un verro à utiliser pour les sections critiques (utilisé avec les variables memT et memP).
std::mutex  verro;

std::mutex m_chauffage, m_pompe;
std::condition_variable chauffage_on, chauffage_off,pompe_on, pompe_off;

// Des variables booléen pour controler l'etat de le chauffage et la pompe
bool etat_chauffage = false, etat_pompe= false;

//les mémoires temperature et pression de programme.
float memT, memP;

/**
 * Cette fonction controle le fonctionnement de la pompe et le chauffage selon l'algoritheme donné. 
 * @@param les seuils temperature et pression fixés par l'utilisateur.
 * */
void controller(float seuil_T, float seuil_P){
    float T,P;
	while(true){
		verro.lock(); 
		T = memT;
		P = memP;
		verro.unlock(); 
		
		if(T>seuil_T){
            chauffage_off.notify_one(); // signaler l'arret de le chauffage
			if(P>seuil_P){
                pompe_on.notify_one(); 	 //signaler la mise en marche de la pompe
			}
            else{
                pompe_off.notify_one();
			}
		}
        else if(T<seuil_T){
            chauffage_on.notify_one();
            pompe_on.notify_one();
            }
            else{
                chauffage_off.notify_one();
                if(P>seuil_P){
                    pompe_on.notify_one();
                }
                else{
                    pompe_off.notify_one();
                }	
            }
	    std::this_thread::sleep_for(std::chrono::seconds(DELAY_CONTROLLER)); // le thread "controller" sleep pendant une durée "DELAY_CONTROLLER"
	}
}

/**
 * Cette fonction simule les mesures de temperature collectés à partir de capture,
 * si le chauffage est en marche la temperature augmente, et elle diminue sinon. 
 * @@param la temperature quand le programme se lance.
 * */
void temperature(float temperature_initiale){  
    float temperature_capteur = temperature_initiale;	

	while(true) {
		if(etat_chauffage){			    // chauffage en marche
			temperature_capteur += 1.5; // mesures à collecter des capteurs
		}else{							// chauffage en arret
			temperature_capteur -= 0.4;	// mesures à collecter des capteurs			
		}
		verro.lock(); 
		memT = temperature_capteur;
		verro.unlock();	
        std::this_thread::sleep_for(std::chrono::seconds(DELAY_TEMPERATURE)); // le thread "temperature" sleep pendant une durée "DELAY_TEMPERATURE"
	}

}

/**
 * Cette fonction controle le fonctionnement de le chauffage. 
 * si le chauffage est en état d'arret, il attend jusqu'à l'arrivé 
 * d'une signale du "controlleur" pour  changer son état en marche
 * et inversement.
 * */

void chauffage(){
    std::unique_lock<std::mutex> lock(m_chauffage);
	while (true) {
        if(!etat_chauffage){ 	 // chauffage en arret
        chauffage_on.wait(lock); // blockage jusqu'à l'arrivé d'une signale du "controlleur"
		cout<<"chauffage mise en route "<<endl;
        etat_chauffage = true;
        }
        else{ 					// chauffage en marche
        chauffage_off.wait(lock);
		cout<<" chauffage en arrête "<<endl; 
        etat_chauffage = false;  
        }
		}
	}

/**
 * Cette fonction simule les mesures de presssion collectés à partir de capture,
 * si la pompe est en marche le pression augmente, et il diminue sinon. 
 * @@param le pression quand le programme se lance.
 * */
void pression(float pression_initiale){
	float pression_capteur= pression_initiale;
			
	while(true) {
		if(etat_pompe){				 // pompe en marche
			pression_capteur -= 0.1; // mesures à collecter des capteurs
		}
        else{						 // pompe en marche
			pression_capteur += 0.2; // mesures à collecter des capteurs			
		}
		verro.lock();
		memP = pression_capteur;
		verro.unlock();	
		std::this_thread::sleep_for(std::chrono::seconds(DELAY_PRESSION)); // le thread "pression" sleep pendant une durée "DELAY_PRESSION"
	}

}

/**
 * Cette fonction controle le fonctionnement de la pompe. 
 * si la pompe est en état d'arret, il attend jusqu'à l'arrivé 
 * d'une signale du "controlleur" pour  changer son état en marche
 * et inversement.
 * */
void pompe(){
	std::unique_lock<std::mutex> lock(m_pompe);

	while (true) {
		if(!etat_pompe){         // pompe en arret
		    pompe_on.wait(lock); // blockage jusqu'à l'arrivé d'une signale du "controlleur"
			cout<<"pompe mise en route "<<endl;
            etat_pompe = true;
		}
        else{					// pompe en marche
			pompe_off.wait(lock);
			cout<<"pompe en arrêtée" <<endl;
            etat_pompe = false;
		}
	}
}

/**
 * Fonction pour afficher la regulation (changement) de temperature et pression au cours du temps.
 **/
void ecran(){
    float T,P;
    while (true)
    {
        verro.lock();
        T = memT;
        P = memP;
        verro.unlock();
        cout << "Temperateur : " << T << " Pression : " << P <<endl;
        std::this_thread::sleep_for(std::chrono::seconds(DELAY_ECRAN));
    }
}

int main (int argc, char *argv[]) {

    float seuil_T = 25,seuil_P = 0.5; // modifiable par l'utilisateur
    float temperature_capteur = 20, pression_capteur = 1; // mesures à collecter à partir des capteurs

	// les seuils sont fourni par l'utilisateur
	if(argc == 3){
		seuil_T = stof(argv[1]);
		seuil_P = stof(argv[2]);
	}

	
    memT = temperature_capteur; // initialiser la mémoire temperature
    memP = pression_capteur;    // initialiser la mémoire pression.


    thread Threads[6];
    Threads[0]=thread(controller,seuil_T, seuil_P);
    Threads[1]=thread(chauffage);
	Threads[2]=thread(temperature, temperature_capteur);
    Threads[3]=thread(pompe);
	Threads[4]=thread(pression, pression_capteur);
	Threads[5]=thread(ecran); 

	for(int i=0; i<sizeof(Threads); i++) 
		Threads[i].join(); // Le programme attend jusquà tous les threads se termine
       
	return 0;
}