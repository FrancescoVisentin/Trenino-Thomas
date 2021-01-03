#ifndef Stazione_h
#define Stazione_h

#include<iostream>
#include<string>
#include<vector>
//DI PROVA DA CANCELLARE!!!!!!!!!!!!!!!!!!!!!
//-		PROBLEMA: IL NUMERO CHE INDICA IL BINARIO OCCUPATO DAL TRENO E' UN NUMERO VALIDO QUANDO IL TRENO 
//		NON E' FERMO NE' SI TROVA NEI 20 KM PRIMA DELLA STAZIONE!!!!!!!!!!!!
//-     SERVE UNA FUNZIONE GET_NOME_TRENO()
//-		SERVE UNA FUNZIONE GET_RAIL()   CHE MI RESTITUISCE IL BINARIO OCCUPATO DAL TRENO 
//-		SERVE UNA FUNZIONE SET_RAIL()   (VEDI SOTTO QUELLE FITTIZIE)

//-serve un comando per liberare un posto (devo liberarne uno specifico 
//(allora in questo caso dovrei salvare in treno il binario in cui si trova)
//cosi passandomi treno nella funzione      freePlace (Treno treno)     libero il binario corrispondente)
//-sostituire treno con la classe reale (e usare la funzione reale isRegional())
//
class Treno {
public:
	bool isRegional();
	void setRail(int railNumber);				//setta una variabile di treno che indica il binario attual mente occupato dal treno: la variabile e' settata a -1 se il treno non e' fermo in stazione o nei 20 km che la precedono  
	int getRail();
private:
	int rail = -1;
												//quando il treno non e' ancora partito assumo non si trovi in stazione
};
class Treno_regonale : public Treno {

};




class Binario {
public:
	Binario() {};
protected:
	virtual bool isReverse() const = 0;
};


class Binario_standard : public Binario {
public:
	Binario_standard() {};
	Binario_standard(const bool rev, bool full) : reverse{ rev }, fullness{ full } {
	}
	int speed() const;											//restituisce la velocita' limite del binario
	bool isFull();
	bool isReverse() const;
	void set_fullness(bool changeFullness);
protected:
	static const int maxSpeed = 80;
	const int length = 10;
	const bool reverse = false;
	bool fullness = false;
};


class Binario_transito : public Binario {
public:
	Binario_transito() {};
	Binario_transito(const bool rev) : reverse{ rev } {
	}
	bool isReverse() const;
protected:
	const bool reverse = false;
};





class Stazione {
public:
	virtual int handle_new_train(bool reverseDirection, Treno& treno) = 0;
	virtual void freePlace(Treno treno) = 0;
protected:
	Stazione() {};
};


class Stazione_principale : public Stazione {
public:
	Stazione_principale(std::string nomeStazione, int distanzaOrigine);
	int handle_new_train(bool reverseDirection, Treno& treno);		   //restituisce puntatore a Binario_standard in cui si puo' far sostare il treno. In caso non ci siano binari disponibili, restituisce nullptr
	void freePlace(Treno treno);
	bool can_leave(bool reverseDirection, int time);
protected:
	std::vector<Binario_standard> vbs = { };
	std::string nomeStaz = "";
	int distanzaOrig = 0;
	int timeLastDepartureReverse = -1;
	int timeLastDepartureAhead = -1;
	Binario_standard bs1;
	Binario_standard bs2;
	Binario_standard bs3;
	Binario_standard bs4;
};


class Stazione_locale : public Stazione {
public:
	Stazione_locale(std::string nomeStazione, int distanzaOrigine);
	int handle_new_train(bool reverseDirection, Treno& treno);
	void freePlace(Treno treno);
	bool can_leave(bool reverseDirection, int time);
protected:
	std::vector<Binario_standard> vbs = { };
	std::string nomeStaz = "";
	int distanzaOrig = 0;
	int timeLastDepartureReverse = -1;
	int timeLastDepartureAhead = -1;
	Binario_standard bs1;
	Binario_standard bs2;
	Binario_standard bs3;
	Binario_standard bs4;
	Binario_transito bt1;
	Binario_transito bt2;
};

#endif //endif of: Train_h 
