# Systme temps réel, embarqué et mobile

1. **CalculePi :**
   - Il existe 2 version pour le calcule de Pi : En série et en parallèle
   - Commande d'execution : `g++ Pi_Parallel.cpp `
   - Commande d'execution : `g++ Pi_Parallel.cpp -lpthread`
   - En compilant et en exécutant l'un des deux fichiers, la valeur approximative de PI sera imprimée

      `Valeur approchee de Pi: 3.141529...`
2. **CourseChevaux :**
    - Simulation de course
    -  Commande d'execution : `g++ CourseChevaux.cpp -lpthread`
    - Exemple d'affichage :
```              ><>
         ><>
        ><>
      ><>
         ><>
       ><>
Créations des threads
La première place est : 2
```
3. **MergeSort :**
  - Commande d'execution : `g++ -pthread merge_sort.cpp`
  - Exemple d'affichage
     `Before sort : 10 5 100 30 22 44 99 55 1 3 6 0 16 41 14 17 10 20 50 56 36`
     
     `After sort : 0 1 3 5 6 10 10 14 16 17 20 22 30 36 41 44 50 55 56 99 100`
4. **RegulateurTemperaturePression :**
  - Commande d'execution : 
    
    `g++ -pthread Regulateur.cpp`
    
    With parameters :`g++ -pthread Regulateur.cpp Temperature_arg Pression_arg`
  - Exemple d'affichage 
 
    `Temperateur : 19.6 Pression : 1.2`
 
    `Temperateur : 19.6 Pression : 1.2`
  
    `Temperateur : 19.2 Pression : 1.4`
  
    `chauffage mise en route pompe mise en route`
    `Temperateur : 19.2 Pression : 1.4`
  
    `Temperateur : 20.7 Pression : 1.3`
  
    `Temperateur : 20.7 Pression : 1.3`
