//Roznowicz Simone matricola 1216375

#include "Binario.h"

//restituisce la velocita' limite del binario
int Binario_standard::speed() const {
	return maxSpeed;
}

//binariio occupato o libero
bool Binario_standard::isFull() {
	return fullness;
}

//direzione del binario da origize a capolinea oppure viceversa (reverse)
bool Binario_standard::isReverse() const {
	return reverse;
}

//riempie il binario
void Binario_standard::set_fullness(bool changeFullness) {
	fullness = changeFullness;
}

//imposta l'orarrio d'arrivo
void Binario_standard::set_time_arrival(int time){
	timeNewArrival = time;
}

//restituisce l'ultimo tempo di arrivo
int Binario_standard::get_time_arrival(){
	return timeNewArrival;
}

//restituisce il treno momentaneamente associato al binario in questione
Train* Binario_standard::get_train(){
	return treno_fermo;
}

//associa momentaneamente un treno ad uno specifico binario
void Binario_standard::set_train(Train* treno){
	treno_fermo = treno;
}

//direzione del binario da origize a capolinea oppure viceversa (reverse)
bool Binario_transito::isReverse() const {
	return reverse;
}
