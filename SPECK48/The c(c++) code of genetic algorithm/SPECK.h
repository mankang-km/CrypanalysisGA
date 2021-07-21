#ifndef _SPECK_
#define _SPECK_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define POPSIZE 1500
#define LENGTH 13
#define RIGHTROTATESHIFT 8
#define LEFTROTATESHIFT 3
#define HIGHTWEIGHT 576

#define BESTWEIGHT 40
#define MINIWEIGHT 49
#define FIRSTINDIWEIGHT 90
#define INDIWEIGHT 90
#define HIGHWEIGHT 20
#define POINTROUND 12
#define ENDFITNESS 61

#define MAXGENERATION 40000
#define BLOCKLENGTH 48
#define HALFBLOCKLENGTH 24
#define Pc 0.8
#define Pm 0.08


 extern int generation;
 extern double sumfitness;
 extern int bestweight;
 extern int refitness;

 struct individualga
{
	bool state[BLOCKLENGTH];
	int fitness;
};

 struct individual
 {
	 bool path[LENGTH][BLOCKLENGTH];
	 int fitness;
 };
 struct finddifference
 {
	 bool headdiff[BLOCKLENGTH];
	 bool taildiff[BLOCKLENGTH];

	 int firstgeneration;
	 int findfitness;
	 int findnum;

	 int findday;
	 int findhour;
	 int findminite;
	 int findsecond;

	 int fitnum[ENDFITNESS];
	struct finddifference *next;
 };

extern struct individualga population[POPSIZE];
extern struct individualga newpopulation[POPSIZE];
extern struct finddifference *head,*p;

extern double pfitness[POPSIZE];
extern int ffitness[POPSIZE];

extern individual individual0;
extern individual backup;
extern individual test;



extern bool singleindi[BLOCKLENGTH];
extern clock_t start, finish;
extern long long totaltime;
extern int day, hour, minite, second;


extern int qflag;
extern int q1flag;
extern FILE *fpdata;
extern FILE *fpout;


void LeftShift(int shiftarr[],int n,int m);


void RightShift(int shiftarr[],int n,int m);


void RotateLeftShift(bool shiftarr[],int n,int m);


void RotateRightShift(bool shiftarr[],int n,int m);


void GenerateInitPopulation(void);


void CalculateFitnessValue(void);


void FirstRound(int indinum);


void SetOnePoint(int row, int col, individual PreRound,int indinum,int fsum);


void NextOneRound(int row, int col, individual PreRound, int pround, int indinum,int fsum);


void SelectionOperation(void);


void CrossoverOperation(void);


void MutationOperation(void);


void PenaltylikeFunction(void);


#endif
