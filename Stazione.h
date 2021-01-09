#ifndef Stazione_h
#define Stazione_h
#include "Binario.h"
#include "Train.h"

#include<iostream>
#include<string>
#include<vector>

//-serve un comando per liberare un posto (devo liberarne uno specifico 
//(allora in questo caso dovrei salvare in treno il binario in cui si trova)
//cosi passandomi treno nella funzione      freePlace (Treno treno)     libero il binario corrispondente)
//-sostituire treno con la classe reale (e usare la funzione reale isRegional())
//



class Stazione {
public:
	virtual int handle_new_train(bool reverseDirection, Train* treno) = 0;
	virtual void freePlace(Train* treno) = 0;
	virtual bool can_leave(bool reverseDirection, int time) = 0;
	virtual void new_departure(bool reverseDirection, int time) = 0;
	virtual bool can_arrive_from_box(bool reverse, Train* treno) = 0;
	virtual bool can_restart(Train* treno, int time) = 0;
	virtual void new_stopped_train(Train*, int time) = 0;
protected:
	
	Stazione() {};
	std::string nomeStaz = "";
	int distanzaOrig = 0;
	int timeLastDepartureReverse = -1;
	int timeLastDepartureAhead = -1;
	
};


class Stazione_principale : public Stazione {
public:
	Stazione_principale(std::string nomeStazione, int distanzaOrigine);
	int handle_new_train(bool reverseDirection, Train* treno);		   //restituisce puntatore a Binario_standard in cui si puo' far sostare il treno. In caso non ci siano binari disponibili, restituisce nullptr
	void freePlace(Train* treno);
	bool can_leave(bool reverseDirection, int time);
	void new_departure(bool reverseDirection, int time);
	bool can_arrive_from_box(bool reverse, Train* treno);
	bool can_restart(Train* treno, int time);
	void new_stopped_train(Train*, int time);
private:
	Binario_standard* from_int_to_rail(Train* treno);
	Binario_standard* get_other_rail(int rail);
	std::vector<Binario_standard> vbs = {};
	
	std::vector<Train*> vtt = {};
	Binario_standard bs1;
	Binario_standard bs2;
	Binario_standard bs3;
	Binario_standard bs4;
};


class Stazione_locale : public Stazione {
public:
	Stazione_locale(std::string nomeStazione, int distanzaOrigine);
	int handle_new_train(bool reverseDirection, Train* treno);
	void freePlace(Train* treno);
	bool can_leave(bool reverseDirection, int time);
	void new_departure(bool reverseDirection, int time);
	bool can_arrive_from_box(bool reverse, Train* treno);
	bool can_restart(Train* treno, int time);
	void new_stopped_train(Train*, int time);
private:
	Binario_standard* from_int_to_rail(Train* treno);
	Binario_standard* get_other_rail(int rail);
	std::vector<Binario_standard> vbs = {};
	
	std::vector<Train*> vtt = {};
	Binario_standard bs1;
	Binario_standard bs2;
	Binario_standard bs3;
	Binario_standard bs4;
	Binario_transito bt1;
	Binario_transito bt2;
};

#endif //endif of: Train_h 
