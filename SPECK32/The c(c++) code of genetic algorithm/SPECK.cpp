#include "SPECK.h"

int generation=0;
double sumfitness=0.0;
int bestweight=1280;
int refitness;

struct individualga population[POPSIZE];
struct individualga newpopulation[POPSIZE];
struct finddifference *head,*p;
double pfitness[POPSIZE];
int ffitness[POPSIZE];

individual individual0;
individual backup;
individual test;


clock_t start, finish;
long long totaltime;
int day, hour, minite, second;


int qflag=0;
int q1flag=0;
FILE *fpdata = NULL;
FILE *fpout = NULL;


int main()
{	
	int i,j,k;
	start=clock();

	srand((int)time(0));

	head=p=(struct finddifference *)malloc(sizeof (struct finddifference));
	head->next=p->next=NULL;
	
	GenerateInitPopulation();

	CalculateFitnessValue();

	PenaltylikeFunction();

	while(generation<MAXGENERATION)
	{
		generation++;		

		SelectionOperation();
		
		CrossoverOperation();
		
		MutationOperation();
		
		CalculateFitnessValue();

		PenaltylikeFunction();

	}

		CalculateFitnessValue();

		//output the results

		if (!qflag)
		{
			fpout = fopen("resultout.txt", "w");
			qflag = 1;
		}
		else
		{
			fpout = fopen("resultout.txt", "a");
		}
        while(p->next!=NULL)
		{
			p=p->next;
			fprintf(fpout, "indifference \n");
			for (k = 0; k < BLOCKLENGTH; k++)
			{
				if (k % 8 != 7)
					fprintf(fpout, "%d", p->headdiff[k]);
				else
					fprintf(fpout, "%d ", p->headdiff[k]);
			}
			fprintf(fpout, "\n");
			fprintf(fpout, "outdifference \n");
			for (k = 0; k < BLOCKLENGTH; k++)
			{
				if (k % 8 != 7)
					fprintf(fpout, "%d", p->taildiff[k]);
				else
					fprintf(fpout, "%d ", p->taildiff[k]);
			}
			fprintf(fpout, "\n");
			fprintf(fpout, "first generation %d, fitness %d, count number %d, time: %d day,%d hour,%d minite, %d second \n",p->firstgeneration,p->findfitness,p->findnum,p->findday,p->findhour,p->findminite,p->findsecond);
		}

		p=head;

		fclose(fpout);
		
	finish=clock();
	totaltime=(long long)(finish-start)/CLOCKS_PER_SEC;
	day=totaltime/86400;
	hour=(totaltime%86400)/3600;
	minite=(totaltime%86400)%3600/60;
	second=(totaltime%86400)%3600%60;
	printf("time is %d day %d hour %d minite %d second",day,hour,minite,second);


        return 0;
}


