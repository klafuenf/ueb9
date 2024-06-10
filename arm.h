//arm.h
#include "ueb9_header.h"
using namespace std;
#include <iostream>
#ifndef _arm_H
#define _arm_H

class arm
{
private:
public:
	arm();  // Konstruktor
	~arm(); // Destructor

	void getgeo(const char* );
	void calc(double , double* , double* );
	void show();
	void set_line_color(int);
	int anz_p;
	int stiftfarbe = 1;
	
	double Arm[50][2];  // max 50 Punkte mit je ein x- und ein y-Wert
	double Armn[50][2];  //dieser Buchstabe wird transformiert aus arm
	double l; // effektive Laenge des Arms
};
#endif
