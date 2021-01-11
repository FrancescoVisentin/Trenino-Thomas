//Roznowicz Simone matricola 1216375

#ifndef Stazione_h
#define Stazione_h
#include "Binario.h"
#include "Train.h"

#include<iostream>
#include<string>
#include<vector>


class Stazione {
public:
	static const int T_VELOCITY = 80; //massima velocità di transito in una stazione.

	virtual int handle_new_train(bool reverseDirection, Train* treno, int time) = 0;
	virtual void freePlace(Train* treno) = 0;
	virtual bool can_leave(bool reverseDirection, int time) const = 0;
	virtual void new_departure(bool reverseDirection, int time) = 0;
	virtual bool can_arrive_from_box(bool reverse, Train* treno) = 0;
	virtual bool can_restart(Train* treno, int time) = 0;
	virtual void new_stopped_train(Train*, int time) = 0;
	virtual void update_transit() = 0;
	std::string get_name();
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
	int handle_new_train(bool reverseDirection, Train* treno, int time);		   
	void freePlace(Train* treno);
	bool can_leave(bool reverseDirection, int time) const;
	void new_departure(bool reverseDirection, int time);
	bool can_arrive_from_box(bool reverse, Train* treno);
	bool can_restart(Train* treno, int time);
	void new_stopped_train(Train*, int time);
	void update_transit();
private:
	Binario_standard* from_int_to_rail(Train* treno);
	Binario_standard* get_other_rail(int rail);
	std::vector<Binario_standard> vbs = {};
	
};


class Stazione_locale : public Stazione {
public:
	Stazione_locale(std::string nomeStazione, int distanzaOrigine);
	int handle_new_train(bool reverseDirection, Train* treno, int time);
	void freePlace(Train* treno);
	bool can_leave(bool reverseDirection, int time) const;
	void new_departure(bool reverseDirection, int time);
	bool can_arrive_from_box(bool reverse, Train* treno);
	bool can_restart(Train* treno, int time);
	void new_stopped_train(Train*, int time);
	void update_transit();
private:
	Binario_standard* from_int_to_rail(Train* treno);
	Binario_standard* get_other_rail(int rail);
	std::vector<Binario_standard> vbs = {};
	
	std::vector<Train*> vtt = {};
};

#endif //endif of: Train_h 
