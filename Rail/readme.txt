readme


Autori dell'assegnamento:
FRANCESCO VISENTIN,	
CHRISTIAN MARCHIORI
SIMONE ROZNOWICZ

Il progetto permette la gestione dei treni in una linea ferroviaria: vengono importati i dati relativi alla timetable
dal file timetables.txt e i quelli relativi alla conformazione della linea ferroviaria dal file line_description.txt.
Le timetables.txt, timetables_2.txt sono associate alla line_description.txt.
Le timetables_1.txt, timetables_3.txt sono associate alla line_description_1.txt

Struttura generale dell'elaborato:
Sono state realizzate tre classi per la gestione dei treni, dei binari e delle stazioni. In aggiunta ad esse abbiamo deciso di creare 
un'ulteriore classe involucro adibita al controllo delle interconnessioni. 
Il main, oltre a leggere i files in input, consiste nell'invocazione del metodo simulate() il quale termina quando tutti i treni sono giunti a destinazione. 
A causa della granularita' del tempo sono stati apportati opportuni acorgimenti per minimizzare le perdite di tempo e rendere la simulazione piu' 
realistica possibile. In particolare, soprattutto per l'elevata velocita' raggiunta dai treni veloci e veloci-super, abbiamo scelto di modificarne
talvolta la posizione in modo non naturale (e' una scelta di ottimizzazione che non sarebbe stata implementata nel caso in cui time fosse stato
un double invece che un int).
Tali modifiche avvengono nelle seguenti situazioni:
• nonappena la posizione del treno supera i 5km che precedono una stazione in cui deve sostare, essa viene riportata a esattamente 5km.
• nonappena la posizione del treno supera la distanza rispetto all'origine di una stazione in cui deve sostare, essa viene riportata esattamente alla
  posizione della stazione.
• nonappena la posizione del treno supera la distanza rispetto all'origine di un posteggio in cui deve sostare, essa viene riportata esattamente alla
  posizione del posteggio.


segue la suddivisione del carico di lavoro:
CHRISTIAN MARCHIORI ---> Train.h,  Train.cpp,  Train_func.cpp
SIMONE ROZNOWICZ    ---> Stazione.h,  Stazione.cpp,  Binario.h,  Binario.cpp
FRANCESCO VISENTIN  ---> Simulation.h,  Simulation.cpp,  main.cpp

Vogliamo sottolineare che moltissime scelte implementative sono state ampiamente discusse e trattate insieme durante tutto lo sviluppo progettuale.
Il compito ha infatti richiesto una stretta collaborazione fra di noi: le singole implementazioni 
si sono rivelate particolarmente legate fra loro, tanto da essere modificate piu' volte per favorire un migliore risultato complessivo.







