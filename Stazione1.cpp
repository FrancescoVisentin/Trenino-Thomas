#include<iostream>
#include<string>
#include<vector>
#include "Stazione1.h"
class Treno {

};
class Treno_regonale : Treno {

};
/*
- funzion isToStop(Treno treno): va bene che metto piu' metodi in cui cambia solo il parametro pasato e a seconda di quello restituisco t o f
  Esempio isToStop(Treno_regionale treno) oppure isToStop(Treno_veloce treno) oppure isToStop(Treno_superv treno)?? E invece per le stazioni principali mi faccio dare solo treno perche' rispondo sempre T qualsiasi sia il tipo di treno
- ma serve la funzione vecBT()???? che mi restituisce il vettore di binari di transito della stazione????
- nella funzione 	Binario_standard handle_new_train(bool isReverse, Treno treno);   mi faccio passare un treno ma in realta' forse non mi serve,
  a meno che non sia utile che cerchi in qualche modo di registrare nelle informazioni del treno il binario in cui si trova (ma non credo)
  Se pero' questa cosa non serve, basta sapere che a ogni chaimata della funzione viene automaticamente occupato un posto o ,
  in caso non sia possibile, viene riservato un posto per i box


  potrei far ritornare a handle_new_train(...) tipo numero <0 se non deve fermarsi, ==0 se deve fermarsi ma non essendoci posti deve sostare ai box, >0 se il tipo di treno considerato deve solo transitare
  (IN QUESTO CASO NEL COSTRUTTORE MI SERVE ANCHE CHE VENGA PASSATO IL TIPO DI TRENO PERO')

  METTI ORDINE E NEL MAIN FAI INCLUDE.H
*/



//PROVVISORIAMENTE HO DICHIARAZIONI E DEFINIZIONI NEL CPP
class Binario {
public:
	
protected:
	Binario() {};								  //va qui o in public come gli altri
	virtual int speed() const = 0;                //restituisce la velocita' limite del binario
	virtual bool isFull() = 0;
	virtual bool isReverse() = 0;
	virtual void set_fullness(bool changeFullness) = 0;

};


class Binario_standard : Binario {
public:
	Binario_standard() {};
	Binario_standard(const bool rev, bool full) : reverse{ rev }, fullness{ full } {
	}
	int speed() const;							 //restituisce la velocita' limite del binario
	bool isFull();
	bool isReverse();
	void set_fullness(bool changeFullness);
protected:
	
	static const int maxSpeed = 80;
	const int length = 10;
	bool reverse = false;
	bool fullness = false;
};



class Binario_transito : Binario {
public:
	Binario_transito() {};
	Binario_transito(const bool rev) : reverse{ rev } {
	}
	bool isReverse();
	int speed() const { return false; }			//SONO FITTIZI: NON SO SE VA BENE
	bool isFull() {return false; }				//SONO FITTIZI: NON SO SE VA BENE
	void set_fullness(bool changeFullness) {};  //SONO FITTIZI: NON SO SE VA BENE
protected:
	
	bool reverse = false;
	bool fullness = false;
};










class Stazione {
public:
	
	//virtual std::vector<Binario_transito> vecBT() = 0;	
	virtual std::vector<Binario_standard> vecBS() = 0;
	virtual Binario_standard handle_new_train(bool isReverse, Treno treno) = 0;
	//virtual bool check_stop(bool isReverse, Treno treno) = 0;
protected:
	Stazione() {};
};




class Stazione_principale : Stazione {
public:
	Stazione_principale(std::string nomeStazione, int distanzaOrigine) : nomeStaz{nomeStazione}, distanzaOrig {distanzaOrigine}, bs1 { false, false }, bs2{ false, false },
		bs3{ true, false }, bs4{ true, false } {
	}
	                                                     //restituisce VETTORE DI 2 BINARI DI TRANSITO
	std::vector<Binario_standard> vecBS();                  //restituisce VETTORE DI 4 BINARI STANDARD
	Binario_standard handle_new_train(bool isReverse, Treno treno);  //restituisce puntatore a Binario_standard in cui si puo' far sostare il treno. In caso non ci siano binari disponibili, restituisce nullptr
	//bool check_stop(bool isReverse, Treno treno);        //restituisce true se il treno deve fermarsi ai box, false se non deve fermarsi
	bool isToStop(Treno treno);										//restituisce TRUE se il treno e' tenuto a fermarsi in questa stazione (stazione principale)

protected:
	std::string nomeStaz = "";
	int distanzaOrig = 0;
	Binario_standard bs1;
	Binario_standard bs2;
	Binario_standard bs3;
	Binario_standard bs4;


};

class Stazione_locale : Stazione {
public:
	Stazione_locale(std::string nomeStazione, int distanzaOrigine) : nomeStaz{ nomeStazione }, distanzaOrig{ distanzaOrigine }, bs1{ false, false }, bs2{ false, false },
		bs3{ true, false }, bs4{ true, false }, bt1{ false }, bt2{ true } {
	}
	std::vector<Binario_transito> vecBT();
	std::vector<Binario_standard> vecBS();                   //restituisce VETTORE DI 4 BINARI STANDARD
protected:
	std::string nomeStaz = "";
	int distanzaOrig = 0;
	Binario_standard bs1;
	Binario_standard bs2;
	Binario_standard bs3;
	Binario_standard bs4;
	Binario_transito bt1;
	Binario_transito bt2;
};
















//******definizioni*******
int Binario_standard::speed() const{
    return maxSpeed;
}

bool Binario_standard::isFull(){
    return fullness;
}
bool Binario_standard::isReverse(){
    return reverse;
}

void Binario_standard::set_fullness(bool changeFullness){
    fullness = changeFullness;
}

std::vector<Binario_transito> Stazione_locale::vecBT(){
	return std::vector<Binario_transito> {bt1, bt2};
}

std::vector<Binario_standard> Stazione_principale::vecBS(){
	return std::vector<Binario_standard> {bs1, bs2, bs3, bs4};
}

Binario_standard  Stazione_principale::handle_new_train(bool direction, Treno treno) {       //il parametro isReverse mi serve per capire in che direzione va il treno e capire quindi a quali binari serve chiedere la disponibilita'
//richiamo il vettore di binari della stazione e per ogni binario uso metodo isFull()
//se sono tutti full, pongo   isStop=true;   
	std::vector<Binario_standard> vBinStand = vecBS();                                          //prendo il mio vettore di binari standard riferito alla stazione considerata 
	for (int i = 0; i < vBinStand.size(); i++) {
		if (vBinStand[i].isReverse() == direction && vBinStand[i].isFull() == false) {    //la direzione coincide con quella del treno? Il binario e' libero per la sosta?
			vBinStand[i].set_fullness(true);                                               //allora pongo il binario disponibile pieno
			return vBinStand[i];                                                            //ritorno il binario scelto per la sosta del treno in arrivo
		}
	}
	//non ho trovato binari standard liberi: devo mandare il treno ai box
	return Binario_standard(false, false);//NO  DEVE RESTITUIRMI QUALCOSA DI VUOTO
}

bool Stazione_principale::isToStop(Treno treno){
	return true;								//nelle stazioni principali si fermano sempre regionali, veloci, super-veloci
}

bool Binario_transito::isReverse(){
	return reverse;
}
