#include "Stazione.h"
#include "Binario.h"



//******definizioni******


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
bool Binario_transito::isReverse() const {
	return reverse;
}



Stazione_principale::Stazione_principale(std::string nomeStazione, int distanzaOrigine) : nomeStaz{ nomeStazione }, distanzaOrig{ distanzaOrigine }, bs1{ false, false }, bs2{ false, false },
bs3{ true, false }, bs4{ true, false }, vbs{bs1, bs2, bs3, bs4}  {
}


//Essendo STAZIONE_PRINCIPALE, qualsiasi tipo di treno DEVE SEMPRE FERMARSI (numero >0, se NON DEVE FERMARSI, non si verifica mai):
// == 0 se deve fermarsi ma non essendoci posti DEVE SOSTARE AI BOX,
// <0 se il TRENO DEVE E PUO' FERMARSI in stazione
int Stazione_principale::handle_new_train(bool reverseDirection, Treno* treno) {							
	for (int i = 0; i < vbs.size(); i++) {
		if (vbs[i].isReverse() == reverseDirection && vbs[i].isFull() == false) {					//la direzione coincide con quella del treno? Il binario e' libero per la sosta?
			vbs[i].set_fullness(true);																//allora pongo il binario disponibile pieno
			treno->setRail(i + 1);
			std::cout << "Il treno [nomeTreno] sta arrivando alla stazione "
					  << nomeStaz << " dove occupera' il binario " << i + 1<<std::endl;		// se tolgo la stampa da qua non so che binario sta per essere occupato????????''
			return -1;																				//>0 perche' il treno DEVE E PUO' FERMARSI
		}
	}
	return 0;																						// == 0 il treno deve fermarsi ma non essendoci posti DEVE SOSTARE AI BOX,
}

void Stazione_principale::freePlace(Treno treno){
	int r = treno.getRail();
	switch (r) {
		case 1: bs1.set_fullness(false); break;
		case 2: bs2.set_fullness(false); break;
		case 3: bs3.set_fullness(false); break;
		case 4: bs4.set_fullness(false); break;
	}
}

bool Stazione_principale::can_leave(bool reverseDirection, int time){								//restituisce TRUE se E' PASSATO ABBASTANZA TEMPO DALL'ULTIMA PARTENZA
	if (reverseDirection == true) {																	//caso in cui voglio far partire un treno dal capolinea
		if (timeLastDepartureReverse < 0) {															//caso alla sola prima partenza dalla stazione selezionata
			return true;
		}
		if (time - timeLastDepartureReverse < 4) {
			//se il tempo intercorso fra l'ultima partenza e la richiesta attuale e' minore di 4 minuti,
			//impedisco la partenza per far mantenere ai due treni la distanza di sicurezza di 10 km una volta usciti dalla stazione.
			return false;
		}
		return true;																				//sono passati piu' di 4 minuti, il treno puo' partire
	}
	else {																							//caso in cui voglio far partire un treno dalla stazione di origine verso il capolinea
		if (timeLastDepartureAhead < 0) {															//caso alla sola prima partenza dalla stazione selezionata
			return true;
		}
		if (time - timeLastDepartureAhead < 4) {
			return false;
		}
		return true;
	}
}

//funzione viene chiamata quando si deve aggiornare il minuto in cui e' 
//partito l'ultimo treno (per binari nella stessa direzione)
void Stazione_principale::new_departure(bool reverseDirection, int time){
	if (reverseDirection == true) {							//allora aggiorno timeLastDepartureReverse													
		timeLastDepartureReverse = time;
		return;
	}
	timeLastDepartureAhead = time;
}

bool Stazione_principale::can_arrive_from_box(bool reverseDirection, Treno* treno){
	for (int i = 0; i < vbs.size(); i++) {
		if (vbs[i].isReverse() == reverseDirection && vbs[i].isFull() == false) {					//la direzione coincide con quella del treno? Il binario e' libero per la sosta?
			vbs[i].set_fullness(true);																//allora riempio il binario disponibile
			treno->setRail(i + 1);
			std::cout << "Il treno [nomeTreno] sta arrivando dal posteggio alla stazione "
				<< nomeStaz << " dove occupera' il binario " << i + 1 << std::endl;
			return true;																				
		}
	}
	return false;
}

bool Stazione_principale::can_restart(Treno* treno, int time){
	if (can_leave(treno->get_origin(), time) == false) {
		return false;
	}
	return false;//CAMBIAREEEEEEEEEEEEEEEEEEEEE
	//can_leave ha restituito true. Ora verifico che siano passati almeno 5 minuti da quando il treno si e' fermato
}

void Stazione_principale::new_stopped_train(Treno* treno, int time){
	if (treno->get_origin()) {
		timeNewArrivalReverse = time;
		return;
	}
	timeNewArrivalAhead = time;
	std::cout << "Il treno [nomeTreno] " << "e' giunto alla stazione " << nomeStaz << std::endl;
}


//Essendo STAZIONE_LOCALE, se il treno fornito e' un regionale, si deve fermare, altrimenti (treno veloce o superv) transita soltanto (per la presenza dei binari di transito): 
//numero >0 se NON DEVE FERMARSI, == 0 se deve fermarsi ma non essendoci posti DEVE SOSTARE AI BOX,
// <0 se il TRENO DEVE E PUO' FERMARSI in stazione
int Stazione_locale::handle_new_train(bool reverseDirection, Treno* treno) {
	if (!treno->isRegional()) {
		return 1;																					//il treno e' un veloce o super-veloce quindi TRANSITA SOLTANTO
	}
	for (int i = 0; i < vbs.size(); i++) {
		if (vbs[i].isReverse() == reverseDirection && vbs[i].isFull() == false) {					//la direzione coincide con quella del treno? Il binario e' libero per la sosta?
			vbs[i].set_fullness(true);																//allora pongo il binario disponibile pieno
			treno->setRail(i + 1);
			std::cout << "Il treno [nomeTreno] sta arrivando alla stazione " 
					  << nomeStaz << " dove occupera' il binario " << i + 1<<std::endl;
			return -1;
		}
	}
	return 0;																						// == 0 il treno deve fermarsi ma non essendoci posti DEVE SOSTARE AI BOX,
}

void Stazione_locale::freePlace(Treno treno){
	int r = treno.getRail();
	switch (r) {
		case 1: bs1.set_fullness(false); break;
		case 2: bs2.set_fullness(false); break;
		case 3: bs3.set_fullness(false); break;
		case 4: bs4.set_fullness(false); break;
	}
}


bool Stazione_locale::can_leave(bool reverseDirection, int time) {									//restituisce TRUE se E' PASSATO ABBASTANZA TEMPO DALL'ULTIMA PARTENZA
	if (reverseDirection == true) {																	//caso in cui voglio far partire un treno dal capolinea
		if (timeLastDepartureReverse < 0) {															//caso alla sola prima partenza dalla stazione selezionata
			return true;
		}
		if (time - timeLastDepartureReverse < 4) {
			//se il tempo intercorso fra l'ultima partenza e la richiesta attuale e' minore di 4 minuti,
			//impedisco la partenza per far mantenere ai due treni la distanza di sicurezza di 10 km una volta usciti dalla stazione.
			return false;
		}
		return true;																				//sono passati piu' di 4 minuti, il treno puo' partire
	}
	else {																							//caso in cui voglio far partire un treno dalla stazione di origine verso il capolinea
		if (timeLastDepartureAhead < 0) {															//caso alla sola prima partenza dalla stazione selezionata
			return true;
		}
		if (time - timeLastDepartureAhead < 4) {
			return false;
		}
		return true;
	}
}

void Stazione_locale::new_departure(bool reverseDirection, int time){
	if (reverseDirection == true) {							//allora aggiorno timeLastDepartureReverse													
		timeLastDepartureReverse = time;
		return;
	}
	timeLastDepartureAhead = time;
}
bool Stazione_locale::can_arrive_from_box(bool reverseDirection, Treno* treno) {
	for (int i = 0; i < vbs.size(); i++) {
		if (vbs[i].isReverse() == reverseDirection && vbs[i].isFull() == false) {					//la direzione coincide con quella del treno? Il binario e' libero per la sosta?
			vbs[i].set_fullness(true);																//allora riempio il binario disponibile
			treno->setRail(i + 1);
			std::cout << "Il treno [nomeTreno] e' uscito dai box\n";
			return true;
		}
	}
	return false;
}

bool Stazione_locale::can_restart(Treno* treno, int time){
	if (can_leave(treno->get_origin(), time) == false) {
		return false;
	}
	return false;//CAMBIAREEEEEEEEEEEEEEEEEEEEE
	//can_leave ha restituito true. Ora verifico che siano passati almeno 5 minuti da quando il treno si e' fermato
}

void Stazione_locale::new_stopped_train(Treno* treno, int time){
	if (treno->get_origin()) {
		timeNewArrivalReverse = time;
		return;
	}
	timeNewArrivalAhead = time;
	std::cout << time << " :Il treno [nomeTreno] " << "e' giunto alla stazione " << nomeStaz << std::endl;
}

Stazione_locale::Stazione_locale(std::string nomeStazione, int distanzaOrigine) : nomeStaz{ nomeStazione }, distanzaOrig{ distanzaOrigine }, bs1{ false, false }, bs2{ false, false },
bs3{ true, false }, bs4{ true, false }, bt1{ false }, bt2{ true }, vbs{ bs1, bs2, bs3, bs4 } {
} 














//FUNZIONE DI PROVA:   DA CANCELLARE!!!!!!!!!!!!!!!!!!!!!!!!!!
bool Treno::isRegional()
{
	return true;
}

void Treno::setRail(int railNumber){
	rail = railNumber;
}

int Treno::getRail(){
	return rail;
}

bool Treno::get_origin()
{
	return true;
}

