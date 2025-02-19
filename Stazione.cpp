//Roznowicz Simone matricola 1216375

#include "Stazione.h"

std::string Stazione::get_name() {
	return nomeStaz;
}
Stazione_principale::Stazione_principale(std::string nomeStazione, int distanzaOrigine)
{
	Binario_standard bs1{ false, false };
	Binario_standard bs2{ true, false };

	
	nomeStaz = nomeStazione ;
	distanzaOrig = distanzaOrigine;
	//riempio un vettore di binari standard che usero' in altre funzioni
	vbs.push_back(bs1);
	vbs.push_back(bs1);
	vbs.push_back(bs2);
	vbs.push_back(bs2);
}


//Essendo STAZIONE_PRINCIPALE, qualsiasi tipo di treno DEVE SEMPRE FERMARSI (numero >0, se NON DEVE FERMARSI, non si verifica mai):
// == 0 se deve fermarsi ma non essendoci posti DEVE SOSTARE AI BOX,
// <0 se il TRENO DEVE E PUO' FERMARSI in stazione
int Stazione_principale::handle_new_train(bool reverseDirection, Train* treno, int time) {
	for(int i = 0; i < vbs.size(); i++)
	{
		if(vbs[i].get_train() != nullptr && vbs[i].get_train()->get_train_number() == treno->get_train_number())
			return -1;
	}

	for (int i = 0; i < vbs.size(); i++) {
		if (vbs[i].isReverse() == reverseDirection && vbs[i].isFull() == false) {					//la direzione coincide con quella del treno? Il binario e' libero per la sosta?
			vbs[i].set_fullness(true);
			vbs[i].set_train(treno);																//allora pongo il binario disponibile pieno
			treno->set_current_rail(i + 1);
			std::cout << time << ": Il treno numero " << treno->get_train_number() << " e' a 20 km dalla prossima stazione "
				<< nomeStaz << " dove occupera' il binario " << i + 1<<std::endl;
			return -1;																				//>0 perche' il treno DEVE E PUO' FERMARSI
		}
	}
	return 0;																						// == 0 il treno deve fermarsi ma non essendoci posti DEVE SOSTARE AI BOX,
}

//libera un posto in stazione (libera un binario)
void Stazione_principale::freePlace(Train* treno){
	from_int_to_rail(treno)->set_fullness(false);
}

//metodo che controlla siano passati almeno 4 minuti dalla partenza dell'ultimo treno nella stessa direzione
//Infatti per la granularita' del tempo 4 minuti e' il tempo minimo affinche' due treni appena usciti dalla stazione
//mantengano la distanza di sicurezza di 10km
bool Stazione_principale::can_leave(bool reverseDirection, int time) const{							//restituisce TRUE se E' PASSATO ABBASTANZA TEMPO DALL'ULTIMA PARTENZA
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

//metodo viene chiamata quando si deve aggiornare il minuto in cui e' 
//partito l'ultimo treno (per binari nella stessa direzione)
void Stazione_principale::new_departure(bool reverseDirection, int time){
	if (reverseDirection == true) {							//allora aggiorno timeLastDepartureReverse													
		timeLastDepartureReverse = time;
		return;
	}
	timeLastDepartureAhead = time;
}

//metodo che viene chiamato interrogando la stazione se sia possibile liberare un treno dai box riservandogli
//un binario in stazione
bool Stazione_principale::can_arrive_from_box(bool reverseDirection, Train* treno){
	for (int i = 0; i < vbs.size(); i++) {
		if (vbs[i].isReverse() == reverseDirection && vbs[i].isFull() == false) {					//la direzione coincide con quella del treno? Il binario e' libero per la sosta?
			vbs[i].set_fullness(true);																//allora riempio il binario disponibile
			treno->set_current_rail(i + 1);
			std::cout << "     Il treno " << treno->get_train_number() << " sta arrivando dal posteggio alla stazione "
					  << nomeStaz << " dove occupera' il binario " << i + 1 << std::endl;
			return true;																				
		}
	}
	return false;
}

//restituisce true se un determinato treno puo' ripartire dalla stzione
bool Stazione_principale::can_restart(Train* treno, int time){
	//controllo che sia passato abbastanza tempo dall'ultima partenza (cosi' che due treni non partano troppo ravvicinati)
	if (can_leave(treno->get_origin(), time) == false) {
		return false;
	}

	//controllo che siano passati 5 minuti dall'arrivo del treno
	if (time - from_int_to_rail(treno)->get_time_arrival() < 5) {
		return false;
	}

	//il treno è all'ultima stazione e son passati 5 minuti
	if(treno->eol()  <= treno->get_position()) {
		from_int_to_rail(treno)->set_train(nullptr);
		freePlace(treno);
		return true;
	}
	
	//controllo la priorita' fra i due possibili treni in partenza dalla stazione
	if (treno->get_train_type() == 3) {
		from_int_to_rail(treno)->set_train(nullptr);
		freePlace(treno);
		return true;
	}
	//controllo la priorita' fra due treni che sono fermi: do la PRECEDENZA AI TRENI PIU' VELOCI
	Train* other_train = get_other_rail(treno->get_current_rail())->get_train();
	if (other_train == nullptr) {
		from_int_to_rail(treno)->set_train(nullptr);
		freePlace(treno);
		return true;
	}
	if (treno->get_train_type() == 2 && (other_train->get_train_type() == 2 || other_train->get_train_type() == 1)) {
		from_int_to_rail(treno)->set_train(nullptr);
		freePlace(treno);
		return true;
	}
	if (treno->get_train_type() == 1 && other_train->get_train_type() == 1) {
		from_int_to_rail(treno)->set_train(nullptr);
		freePlace(treno);
		return true;
	}
	return false;
}

//metodo chiamato all'arrivo di un nuovo treno in stazione: aggiorna l'orario d'arrivo
void Stazione_principale::new_stopped_train(Train* treno, int time) {
	from_int_to_rail(treno)->set_time_arrival(time);
	std::cout << "     Il treno " << treno->get_train_number() << " e' giunto alla stazione " << nomeStaz
		<< " e sosta al binario " << treno->get_current_rail() << std::endl;
}

//aggiornametno non necessario per le stazioni principali. 
void Stazione_principale::update_transit()
{
	return;
}

//metodo di supporto che, a partire dalnumero di un binario, 
//restituisce il binario corrispondente
Binario_standard* Stazione_principale::from_int_to_rail(Train* treno) {
	Binario_standard* ptr = nullptr;
	switch (treno->get_current_rail()) {
	case 1: ptr = &vbs[0]; break;
	case 2: ptr = &vbs[1]; break;
	case 3: ptr = &vbs[2]; break;
	case 4: ptr = &vbs[3]; break;
	}
	return ptr;
}

//restituisce il binario nella stessa direzione di quello in qui si trova il treno
Binario_standard* Stazione_principale::get_other_rail(int rail) {
	Binario_standard* ptr = nullptr;
	switch (rail) {
	case 1: ptr = &vbs[1]; break;
	case 2: ptr = &vbs[0]; break;
	case 3: ptr = &vbs[3]; break;
	case 4: ptr = &vbs[2]; break;
	}
	return ptr;
}




Stazione_locale::Stazione_locale(std::string nomeStazione, int distanzaOrigine)
{
	Binario_standard bs1{ false, false };
	Binario_standard bs2{ true, false };

	nomeStaz = nomeStazione;
	distanzaOrig = distanzaOrigine;

	//riempio un vettore di binari standard che usero' in altre funzioni
	vbs.push_back(bs1);
	vbs.push_back(bs1);
	vbs.push_back(bs2);
	vbs.push_back(bs2);

}

//Essendo STAZIONE_LOCALE, se il treno fornito e' un regionale, si deve fermare, altrimenti (treno veloce o superv) transita soltanto (per la presenza dei binari di transito): 
//numero >0 se NON DEVE FERMARSI, == 0 se deve fermarsi ma non essendoci posti DEVE SOSTARE AI BOX,
// <0 se il TRENO DEVE E PUO' FERMARSI in stazione
int Stazione_locale::handle_new_train(bool reverseDirection, Train* treno, int time) {
	if (!treno->isRegional()) {
		vtt.push_back(treno);																		//registro un treno di transito
		return 1;																					//il treno e' un veloce o super-veloce quindi TRANSITA SOLTANTO
	}

	for(int i = 0; i < vbs.size(); i++)
	{
		if(vbs[i].get_train() != nullptr && vbs[i].get_train()->get_train_number() == treno->get_train_number())
			return -1;
	}

	for (int i = 0; i < vbs.size(); i++) {
		if (vbs[i].isReverse() == reverseDirection && vbs[i].isFull() == false) {					//la direzione coincide con quella del treno? Il binario e' libero per la sosta?
			vbs[i].set_fullness(true);
			vbs[i].set_train(treno);																//allora pongo il binario disponibile pieno
			treno->set_current_rail(i + 1);
			std::cout << time << ": Il treno numero " << treno->get_train_number() << " e' a 20 km dalla prossima stazione "
				<< nomeStaz << " dove occupera' il binario " << i + 1<<std::endl;
			return -1;
		}
	}
	return 0;																						// == 0 il treno deve fermarsi ma non essendoci posti DEVE SOSTARE AI BOX,
}

//libera un posto in stazione (libera un binario)
void Stazione_locale::freePlace(Train* treno){
	from_int_to_rail(treno)->set_fullness(false);
}


//metodo che controlla siano passati almeno 4 minuti dalla partenza dell'ultimo treno nella stessa direzione
//Infatti per la granularita' del tempo 4 minuti e' il tempo minimo affinche' due treni appena usciti dalla stazione
//mantengano la distanza di sicurezza di 10km
bool Stazione_locale::can_leave(bool reverseDirection, int time) const{								//restituisce TRUE se E' PASSATO ABBASTANZA TEMPO DALL'ULTIMA PARTENZA
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
			return true;
		if (timeLastDepartureAhead < 0) {															//caso alla sola prima partenza dalla stazione selezionata
		}
		if (time - timeLastDepartureAhead < 4) {
			return false;
		}
		return true;
	}
}

//metodo viene chiamata quando si deve aggiornare il minuto in cui e' 
//partito l'ultimo treno (per binari nella stessa direzione)
void Stazione_locale::new_departure(bool reverseDirection, int time){
	if (reverseDirection == true) {							//allora aggiorno timeLastDepartureReverse													
		timeLastDepartureReverse = time;
		return;
	}
	timeLastDepartureAhead = time;
}

//metodo che viene chiamato interrogando la stazione se sia possibile liberare un treno dai box riservandogli
//un binario in stazione
bool Stazione_locale::can_arrive_from_box(bool reverseDirection, Train* treno) {
	for (int i = 0; i < vbs.size(); i++) {
		if (vbs[i].isReverse() == reverseDirection && vbs[i].isFull() == false) {					//la direzione coincide con quella del treno? Il binario e' libero per la sosta?
			vbs[i].set_fullness(true);																//allora riempio il binario disponibile
			treno->set_current_rail(i + 1);
			std::cout << "     Il treno " << treno->get_train_number() << " sta arrivando dal posteggio alla stazione "
					  << nomeStaz << " dove occupera' il binario " << i + 1 << std::endl;
			return true;
		}
	}
	return false;
}

//restituisce true se un determinato treno puo' ripartire dalla stzione
bool Stazione_locale::can_restart(Train* treno, int time){
	
	//controllo che sia passato abbastanza tempo dall'ultima partenza (cosi' che due treni non partano troppo ravvicinati)
	if (can_leave(treno->get_origin(), time) == false) {
		return false;
	}
	//controllo la presenza di treni in transito da lasciar passare
	for (int i = 0; i < vtt.size(); i++) {
		if (treno->get_origin() == vtt[i]->get_origin()) {		//treno in transito che va nella stessa direzione del treno che deve partire
			return false;										//c'e' un treno in transito da far passare
		}
	}
	//controllo che siano passati 5 minuti dall'arrivo del treno

	if (time - from_int_to_rail(treno)->get_time_arrival() < 5) {
		return false;
	}

	//il treno è all'ultima stazione e son passati 5 minuti
	if(treno->eol()  <= treno->get_position()) {
		from_int_to_rail(treno)->set_train(nullptr);
		freePlace(treno);
		return true;
	}

	//controllo la priorita' fra i due possibili treni in partenza dalla stazione
	if (treno->get_train_type() == 3) {
		from_int_to_rail(treno)->set_train(nullptr);
		freePlace(treno);
		return true;
	}
	//controllo la priorita' fra due treni che sono fermi: do la PRECEDENZA AI TRENI PIU' VELOCI
	Train* other_train = get_other_rail(treno->get_current_rail())->get_train();
	if (other_train == nullptr) {
		from_int_to_rail(treno)->set_train(nullptr);
		freePlace(treno);
		return true;
	}
	if (treno->get_train_type() == 2 && (other_train->get_train_type() == 2 || other_train->get_train_type() == 1)) {
		from_int_to_rail(treno)->set_train(nullptr);
		freePlace(treno);
		return true;
	}
	if (treno->get_train_type() == 1 && other_train->get_train_type() == 1) {
		from_int_to_rail(treno)->set_train(nullptr);
		freePlace(treno);
		return true;
	}
	return false;
}

//metodo di supporto che, a partire dalnumero di un binario, 
//restituisce il binario corrispondente
Binario_standard* Stazione_locale::from_int_to_rail(Train* treno){
	Binario_standard* ptr = nullptr;
	switch (treno->get_current_rail()) {
	case 1: ptr = &vbs[0]; break;
	case 2: ptr = &vbs[1]; break;
	case 3: ptr = &vbs[2]; break;
	case 4: ptr = &vbs[3]; break;
	}
	return ptr;
}

//restituisce il binario nella stessa direzione di quello in qui si trova il treno
Binario_standard* Stazione_locale::get_other_rail(int rail){
	Binario_standard* ptr = nullptr;
	switch (rail) {
	case 1: ptr = &vbs[1]; break;
	case 2: ptr = &vbs[0]; break;
	case 3: ptr = &vbs[3]; break;
	case 4: ptr = &vbs[2]; break;
	}
	return ptr;
}

//metodo chiamato all'arrivo di un nuovo treno in stazione: aggiorna l'orario d'arrivo
void Stazione_locale::new_stopped_train(Train* treno, int time) {
	from_int_to_rail(treno)->set_time_arrival(time);
	std::cout << "     Il treno " << treno->get_train_number() << " e' giunto alla stazione " << nomeStaz
		<< " e sosta al binario " << treno->get_current_rail() << std::endl;
}

//in una stazione locale si puo' verificare il transito di un treno: e' opportuano gestire 
//il vettore di treni in trnsito eliminando da esso gli elementi che hanno superato i 5 km oltre una
//stazione o sono giunti alla destinazione finale
void Stazione_locale::update_transit()
{
	for (int i = 0; i < vtt.size(); i++) {

		if(vtt[i]->get_position() >= vtt[i]->eol()){
			vtt.erase(vtt.begin() + i);
			i--;
		}

		else if (!vtt[i]->get_origin() && vtt[i]->get_position() - distanzaOrig > 5) {						//treno va da origine a capolinea 
			vtt.erase(vtt.begin() + i);
			i--;
		}
		else if (vtt[i]->get_origin() && (vtt[i]->eol() - vtt[i]->get_position() < distanzaOrig - 5)) {		//treno va da capolinea a origine (reverse)
			vtt.erase(vtt.begin() + i);
			i--;
		}
	}
}

