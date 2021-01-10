#ifndef Binario_h
#define Binario_h

#include<iostream>
#include<string>
#include<vector>
#include "Train.h"

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
	void set_time_arrival(int time);
	int get_time_arrival();
	Train* get_train();
	void set_train(Train* treno);
protected:
	Train* treno_fermo = nullptr;
	static const int maxSpeed = 80;
	const int length = 10;
	const bool reverse = false;
	bool fullness = false;
	int timeNewArrival = -1;
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

#endif