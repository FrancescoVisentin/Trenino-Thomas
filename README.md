readme


Autori dell'assegnamento:
FRANCESCO VISENTIN,	
CHRISTIAN MARCHIORI
SIMONE ROZNOWICZ

Il progetto permette la gestione dei treni in una linea ferroviaria: vengono importati i dati relativi alla timetable
dal file timetables.txt e i quelli relativi alla conformazione della linea ferroviaria dal file line_description.txt.

Struttura generale dell'elaborato:
Sono state realizzate tre classi per la gestione dei treni, dei binari e delle stazioni. In aggiunta ad esse abbiamo deciso di creare 
un'ulteriore classe involucro adibita al controllo delle interconnessioni. 
Il main, oltre a leggere i files in input, consiste nell'invocazione del metodo simulate() il quale termina quando tutti i treni sono giunti a destinazione. 


Segue la suddivisione del carico di lavoro:
CHRISTIAN MARCHIORI ---> Train.h,  Train.cpp,  Train_func.cpp
SIMONE ROZNOWICZ    ---> Stazione.h,  Stazione.cpp,  Binario.h,  Binario.cpp
FRANCESCO VISENTIN  ---> Simulation.h,  Simulation.cpp,  main.cpp

Vogliamo sottolineare che moltissime scelte implementative sono state ampiamente discusse e trattate insieme durante tutto lo sviluppo progettuale.
Il compito ha infatti richiesto una stretta collaborazione fra di noi: le singole implementazioni 
si sono rivelate perticolarmente legate fra loro, tanto da essere modificate piu' volte per favorire un migliore risultato complessivo.
