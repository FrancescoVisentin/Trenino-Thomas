#include "Binario.h"


int Binario_standard::speed() const {
	return maxSpeed;
}

bool Binario_standard::isFull() {
	return fullness;
}
bool Binario_standard::isReverse() const {
	return reverse;
}

void Binario_standard::set_fullness(bool changeFullness) {
	fullness = changeFullness;
}

void Binario_standard::set_time_arrival(int time){
	timeNewArrival = time;
}

int Binario_standard::get_time_arrival(){
	return timeNewArrival;
}

Train* Binario_standard::get_train(){
	return treno_fermo;
}

void Binario_standard::set_train(Train* treno){
	treno_fermo = treno;
}

bool Binario_transito::isReverse() const {
	return reverse;
}
