#include "SPECK.h"


void LeftShift(int shiftarr[],int n,int m)
{
	int i;
	for(i=0;i<n;i++)
	{
		if(i+m<n)
			shiftarr[i]=shiftarr[i+m];
		else
			shiftarr[i]=0;
	}
}


void RightShift(int shiftarr[],int n,int m)
{
	int i;
	for(i=n-1;i>=0;i--)
	{
		if(i<m)
			shiftarr[i]=0;
		else
			shiftarr[i]=shiftarr[i-m];
	}
}


void RotateLeftShift(bool shiftarr[],int n,int m)
{
	int i;
	bool *p;
	if(NULL==(p=(bool *)malloc(sizeof(bool)*n)))
	{
		printf("error");
		exit(1);
	}

	for(i=0;i<n;i++)
	{
		*(p+i)=shiftarr[(i+m)%n];
	}
	for(i=0;i<n;i++)
	{
		shiftarr[i]=*(p+i);
	}

	free(p);
	p=NULL;
}


void RotateRightShift(bool shiftarr[],int n,int m)
{
	int i;
	bool *p;
	if(NULL==(p=(bool *)malloc(sizeof(bool)*n)))
	{
		printf("error");
		exit(1);
	}

	for(i=0;i<n;i++)
	{
		*(p+i)=shiftarr[((i-m)+n)%n];
	}
	for(i=0;i<n;i++)
	{
		shiftarr[i]=*(p+i);
	}

	free(p);
	p=NULL;
}


void GenerateInitPopulation(void)
{
	int i,j,k;

	for(i=0;i<POPSIZE;i++)
	{
		for(k=0;k<BLOCKLENGTH;k++)
		{
			population[i].state[k]=((rand() / (double)(RAND_MAX)) < 0.125)? 1:0;
		}
		
	}

}

void CalculateFitnessValue(void)
{
	int i,j,k;
	int sum,flag,result;
	bool a[BLOCKLENGTH/2],b[BLOCKLENGTH/2],c[BLOCKLENGTH/2];


	for (i = 0; i < POPSIZE; i++)
	{
		// generate the initial differential characteristic
		
		for (k = 0; k < BLOCKLENGTH / 2; k++)
		{
			individual0.path[0][k] = population[i].state[k];
			individual0.path[0][HALFBLOCKLENGTH + k] = population[i].state[BLOCKLENGTH / 2 + k];
			a[k] = population[i].state[k];
			b[k] = population[i].state[BLOCKLENGTH / 2 + k];
		}


		sum = 0;
		for (k = 1; k < LENGTH; k++)
		{
			RotateRightShift(a, HALFBLOCKLENGTH, RIGHTROTATESHIFT);
			c[HALFBLOCKLENGTH - 1] = a[HALFBLOCKLENGTH - 1] ^ b[HALFBLOCKLENGTH - 1];
			for (j = HALFBLOCKLENGTH - 2; j >= 0; j--)
			{
				if (a[j + 1] == b[j + 1] && a[j + 1] == c[j + 1])
				{
					c[j] = a[j] ^ b[j] ^ b[j + 1];
				}
				else
				{
					c[j] = 0;
				}
			}
			for (j = 1; j < HALFBLOCKLENGTH; j++)
			{
				if (!(a[j] == b[j] && a[j] == c[j]))
				{
					sum++;
				}
			}


			RotateLeftShift(b, HALFBLOCKLENGTH, LEFTROTATESHIFT);

			for (j = 0; j < HALFBLOCKLENGTH; j++)
			{
				a[j] = c[j];
				b[j] = b[j] ^ c[j];
				individual0.path[k][j] = a[j];
				individual0.path[k][HALFBLOCKLENGTH + j] = b[j];
			}
		}
		individual0.fitness = sum;
		population[i].fitness = sum;

		FirstRound(i);
	}

	sumfitness=0.0;
	for(i=0;i<POPSIZE;i++)
	{
		sumfitness=sumfitness+population[i].fitness;
	}

}



void SelectionOperation(void)
{
	
	int i,j,index;
	
	double p;
	double fsumfitness;

	for(i=0;i<POPSIZE;i++)
	{
		ffitness[i]= HIGHTWEIGHT-population[i].fitness;
	}
	
	
	fsumfitness=0.0;
	for(i=0;i<POPSIZE;i++)
	{

		fsumfitness=fsumfitness+ffitness[i];
		
	}

	for(i=0;i<POPSIZE;i++)
	{
		pfitness[i]= ffitness[i]/fsumfitness;
	}

	for(i=1;i<POPSIZE;i++)
	{
		pfitness[i]= pfitness[i-1]+pfitness[i];
	}



	for(i=0;i<POPSIZE;i++)
	{

		p=rand() / (double)(RAND_MAX);

		index=0;

		if(p>pfitness[POPSIZE-1])
		{
			index=POPSIZE-1;

		}
		else
		{
			while(p>pfitness[index])
			{
				index++;
			}
		}
	
		
		newpopulation[i]=population[index];
	
	}



	for(i=0;i<POPSIZE;i++)
	{
		population[i]=newpopulation[i];
	}

	
}


void CrossoverOperation(void)
{
	int i,j,k,parent,point[2],temp;
	static  int index[POPSIZE];
	double p;

	for(i=0;i<POPSIZE;i++)
	{
		index[i]=i;
	}

	for(i=0;i<POPSIZE;i++)
	{
		parent=rand()%(POPSIZE-i);
		temp=index[i];
		index[i]=index[i+parent];
		index[i+parent]=temp;
	}


	for(i=0;i<(POPSIZE-1);i+=2)
	{
		p=rand() / (double)(RAND_MAX);

		if(p<Pc)
		{
			point[0]=rand() % BLOCKLENGTH;
			point[1]=rand() % BLOCKLENGTH;
			

			if(point[0]>point[1])
			{
				temp=point[0];
				point[0]=point[1];
				point[1]=temp;
			}
			
			
			for(j=point[0];j<point[1];j++)
			{
				temp=population[index[i]].state[j];
				population[index[i]].state[j]=population[index[i+1]].state[j];
				population[index[i+1]].state[j]=temp;
			}
			
		}
		
	}

}


void MutationOperation(void)
{
	int i,j,k;
	double p;

	for(i=0;i<POPSIZE;i++)
	{
		p=rand() / (double)(RAND_MAX);
		if(p<Pm)
		{
			j=rand() % BLOCKLENGTH;
			
			population[i].state[j]=(population[i].state[j]==0)?  1:0;
		}
	}


}


void PenaltylikeFunction(void)
{
	int i,j,k;
	int sum,flag,result;
	bool a[BLOCKLENGTH/2],b[BLOCKLENGTH/2],c[BLOCKLENGTH/2];

	for(i=0;i<POPSIZE;i++)
	{
		if (population[i].fitness == 0)//a type of inferior individuals
		{
			for(k=0;k<BLOCKLENGTH;k++)
			{
				population[i].state[k]=((rand() / (double)(RAND_MAX)) < 0.125)? 1:0;
			}

			for (k = 0; k < BLOCKLENGTH / 2; k++)
			{
				individual0.path[0][k] = population[i].state[k];
				individual0.path[0][HALFBLOCKLENGTH + k] = population[i].state[BLOCKLENGTH / 2 + k];
				a[k] = population[i].state[k];
				b[k] = population[i].state[BLOCKLENGTH / 2 + k];
			}

			sum = 0;
			for (k = 1; k < LENGTH; k++)
			{
				RotateRightShift(a, HALFBLOCKLENGTH, RIGHTROTATESHIFT);
				c[HALFBLOCKLENGTH - 1] = a[HALFBLOCKLENGTH - 1] ^ b[HALFBLOCKLENGTH - 1];
				for (j = HALFBLOCKLENGTH - 2; j >= 0; j--)
				{
					if (a[j + 1] == b[j + 1] && a[j + 1] == c[j + 1])
					{
						c[j] = a[j] ^ b[j] ^ b[j + 1];
					}
					else
					{
						c[j] = 0;
					}
				}
				for (j = 1; j < HALFBLOCKLENGTH; j++)
				{
					if (!(a[j] == b[j] && a[j] == c[j]))
					{
						sum++;
					}
				}


				RotateLeftShift(b, HALFBLOCKLENGTH, LEFTROTATESHIFT);

				for (j = 0; j < HALFBLOCKLENGTH; j++)
				{
					a[j] = c[j];
					b[j] = b[j] ^ c[j];
					individual0.path[k][j] = a[j];
					individual0.path[k][HALFBLOCKLENGTH + j] = b[j];
				}
			}
			individual0.fitness = sum;
			population[i].fitness = sum;

			

			FirstRound(i);


		}
		if (population[i].fitness > 110)//another type of inferior individuals
		{
			result=0;
			for (j = 0; j < BLOCKLENGTH; j++)
			{
				if (population[i].state[j] == 1)
				{
					result++;
				}
			}

			if(result>15)
			{




			for (j = 0; j < BLOCKLENGTH; j++)
			{
				if (population[i].state[j] == 1)
				{
					population[i].state[j] = ((rand() / (double)(RAND_MAX)) < 0.5) ? 1 : 0;
				}
			}

			

			for (k = 0; k < BLOCKLENGTH / 2; k++)
			{
				individual0.path[0][k] = population[i].state[k];
				individual0.path[0][HALFBLOCKLENGTH + k] = population[i].state[BLOCKLENGTH / 2 + k];
				a[k] = population[i].state[k];
				b[k] = population[i].state[BLOCKLENGTH / 2 + k];
			}

			sum = 0;
			for (k = 1; k < LENGTH; k++)
			{
				RotateRightShift(a, HALFBLOCKLENGTH, RIGHTROTATESHIFT);
				c[HALFBLOCKLENGTH - 1] = a[HALFBLOCKLENGTH - 1] ^ b[HALFBLOCKLENGTH - 1];
				for (j = HALFBLOCKLENGTH - 2; j >= 0; j--)
				{
					if (a[j + 1] == b[j + 1] && a[j + 1] == c[j + 1])
					{
						c[j] = a[j] ^ b[j] ^ b[j + 1];
					}
					else
					{
						c[j] = 0;
					}
				}
				for (j = 1; j < HALFBLOCKLENGTH; j++)
				{
					if (!(a[j] == b[j] && a[j] == c[j]))
					{
						sum++;
					}
				}


				RotateLeftShift(b, HALFBLOCKLENGTH, LEFTROTATESHIFT);

				for (j = 0; j < HALFBLOCKLENGTH; j++)
				{
					a[j] = c[j];
					b[j] = b[j] ^ c[j];
					individual0.path[k][j] = a[j];
					individual0.path[k][HALFBLOCKLENGTH + j] = b[j];
				}
			}
			individual0.fitness = sum;
			population[i].fitness = sum;


			FirstRound(i);

		}
		}
	}
}


void FirstRound(int indinum)
{
	int i, j, k, m,sum;
	bool a[HALFBLOCKLENGTH], b[HALFBLOCKLENGTH], c[HALFBLOCKLENGTH], d[HALFBLOCKLENGTH];

	sum=0;

	//traverse all the first change bits

	for (i = 1; i<LENGTH-2 && sum<MINIWEIGHT; i++)
	{
		for (k = 0; k<HALFBLOCKLENGTH; k++)
		{
			a[k] = individual0.path[i - 1][k];
		}
		RotateRightShift(a, HALFBLOCKLENGTH, RIGHTROTATESHIFT);


		for (j = HALFBLOCKLENGTH - 1; j>0; j--)
		{

			if (!(a[j] == individual0.path[i - 1][HALFBLOCKLENGTH + j] && a[j] == individual0.path[i][j]))
				
			{
				SetOnePoint(i - 1, j - 1, individual0,indinum,sum);
			}
		}

		for (j = 1; j<HALFBLOCKLENGTH; j++)
		{

			if (!(a[j] == individual0.path[i - 1][HALFBLOCKLENGTH + j] && a[j] == individual0.path[i][j]))
				
			{
				sum++;
			}
		}
	}
}



void SetOnePoint(int row, int col, individual PreRound, int indinum,int fsum)
{
	int i, j, k;
	int sum, flag, result;
	bool a[BLOCKLENGTH / 2], b[BLOCKLENGTH / 2], c[BLOCKLENGTH / 2];
	individual PresentRound;
	int ppround;

	PresentRound = PreRound;



	// add one change bit

		for (k = 0; k < BLOCKLENGTH / 2; k++)
		{
			a[k] = PresentRound.path[row][k];
			b[k] = PresentRound.path[row][BLOCKLENGTH / 2 + k];
		}
		RotateRightShift(a, BLOCKLENGTH / 2, RIGHTROTATESHIFT);
		for (k = BLOCKLENGTH / 2 - 1; k > col; k--)
		{
			c[k] = PresentRound.path[row + 1][k];
		}
		c[k] = 1;
		ppround = 1;
		

		for (k = col - 1; k >= 0; k--)
		{
			if (a[k + 1] == b[k + 1] && a[k + 1] == c[k + 1])
			{
				c[k] = a[k] ^ b[k] ^ b[k + 1];

			}
			else
			{

				c[k] = 0;

			}
		}

		RotateLeftShift(b, BLOCKLENGTH / 2, LEFTROTATESHIFT);

		for (k = 0; k < BLOCKLENGTH / 2; k++)
		{
			a[k] = c[k];
			b[k] = b[k] ^ c[k];
			PresentRound.path[row + 1][k] = a[k];
			PresentRound.path[row + 1][BLOCKLENGTH / 2 + k] = b[k];
		}

		for (j = row + 1; j < LENGTH - 1; j++)
		{
			RotateRightShift(a, BLOCKLENGTH / 2, RIGHTROTATESHIFT);

			c[BLOCKLENGTH / 2 - 1] = a[BLOCKLENGTH / 2 - 1] ^ b[BLOCKLENGTH / 2 - 1];

			for (k = BLOCKLENGTH / 2 - 2; k >= 0; k--)
			{
				if (a[k + 1] == b[k + 1] && a[k + 1] == c[k + 1])
				{
					c[k] = a[k] ^ b[k] ^ b[k + 1];

				}
				else
				{

					c[k] = 0;

				}
			}

			RotateLeftShift(b, BLOCKLENGTH / 2, LEFTROTATESHIFT);

			for (k = 0; k < BLOCKLENGTH / 2; k++)
			{
				a[k] = c[k];
				b[k] = b[k] ^ c[k];
				PresentRound.path[j + 1][k] = a[k];
				PresentRound.path[j + 1][BLOCKLENGTH / 2 + k] = b[k];
			}

		}

		//calculate the weight of the new differential characteristic
		sum = fsum;
		for (k = 0; k < BLOCKLENGTH / 2; k++)
		{
			a[k] = PresentRound.path[row][k];
		}

		for (i = row; i < LENGTH - 1; i++)
		{
			RotateRightShift(a, BLOCKLENGTH / 2, RIGHTROTATESHIFT);
			for (k = 1; k < BLOCKLENGTH / 2; k++)
			{
				if (!(a[k] == PresentRound.path[i][BLOCKLENGTH / 2 + k] && a[k] == PresentRound.path[i + 1][k]))
				{
					sum++;
				}
			}
			for (k = 0; k < BLOCKLENGTH / 2; k++)
			{
				a[k] = PresentRound.path[i + 1][k];
			}
		}
		PresentRound.fitness = sum;
		if (population[indinum].fitness > sum)
		{
			population[indinum].fitness = sum;
		}

		
		if (PresentRound.fitness <= MINIWEIGHT)//if the weight meets the condition, output this differential characteristic
		{
			while(p->next!=NULL)
			{
				p=p->next;
				for (i = 0; i < BLOCKLENGTH; i++)
				{
					if(!(PresentRound.path[0][i]==p->headdiff[i]&&PresentRound.path[LENGTH-1][i]==p->taildiff[i]))
					{
						break;
					}
				}
				if(i==BLOCKLENGTH)
				{
					p->findnum=p->findnum+1;
					p=head;
					break;
				}
			}			

			if(p->next==NULL)
			{
				p->next=(struct finddifference *)malloc(sizeof (struct finddifference));
				p=p->next;
				for (i = 0; i < BLOCKLENGTH; i++)
				{
					p->headdiff[i]=PresentRound.path[0][i];
					p->taildiff[i]=PresentRound.path[LENGTH-1][i];
				}
				p->firstgeneration=generation;
				p->findfitness=PresentRound.fitness;
				p->findnum=1;
				p->next=NULL;
				

			
				if (!qflag)
				{
					fpout = fopen("resultout.txt", "w");
					qflag = 1;
				}
				else
				{
					fpout = fopen("resultout.txt", "a");
				}
				fprintf(fpout, "find one better road, fitness is %d \n", PresentRound.fitness);
				printf("find one better road, fitness is %d \n", PresentRound.fitness);
				for (i = 0; i < LENGTH; i++)
				{
					fprintf(fpout, "round index %d \n", i);
					for (k = 0; k < BLOCKLENGTH / 2; k++)
					{
						if (k % 8 != 7)
							fprintf(fpout, "%d", PresentRound.path[i][k]);
						else
							fprintf(fpout, "%d ", PresentRound.path[i][k]);
					}
					fprintf(fpout, "\n");
					for (k = 0; k < BLOCKLENGTH / 2; k++)
					{
						if (k % 8 != 7)
							fprintf(fpout, "%d", PresentRound.path[i][HALFBLOCKLENGTH + k]);
						else
							fprintf(fpout, "%d ", PresentRound.path[i][HALFBLOCKLENGTH + k]);
					}
					fprintf(fpout, "\n");
				}

				finish=clock();
				totaltime=(long long)(finish-start)/CLOCKS_PER_SEC;
				day=totaltime/86400;
				hour=(totaltime%86400)/3600;
				minite=(totaltime%86400)%3600/60;
				second=(totaltime%86400)%3600%60;
				fprintf(fpout,"time is %d day %d hour %d minite %d second \n",day,hour,minite,second);
				p->findday=day;
				p->findhour=hour;
				p->findminite=minite;
				p->findsecond=second;
				for (i = 0; i < ENDFITNESS; i++)
				{
					p->fitnum[i]=0;
				}
				p=head;
				fclose(fpout);
				start=clock();

			}

			

		}

		//depth-first search for the next change bit
		if (PresentRound.fitness <= FIRSTINDIWEIGHT)
		{
			sum=fsum;
		for (k = 0; k < BLOCKLENGTH / 2; k++)
		{
			a[k] = PresentRound.path[row][k];
		}
		RotateRightShift(a, HALFBLOCKLENGTH, RIGHTROTATESHIFT);
		for (j = col; j > 0; j--)
		{

			if (!(a[j] == PresentRound.path[row][HALFBLOCKLENGTH + j] && a[j] == PresentRound.path[row + 1][j]))
				
			{
				
					NextOneRound(row, j - 1, PresentRound, ppround, indinum,sum);

			}
		}
		for (j = 1; j < HALFBLOCKLENGTH; j++)
		{

			if (!(a[j] == PresentRound.path[row][HALFBLOCKLENGTH + j] && a[j] == PresentRound.path[row + 1][j]))
				
			{

				sum++;

			}
		}

		for (i = row + 1; i < LENGTH - 1 &&sum<MINIWEIGHT; i++)
		{
			for (k = 0; k < HALFBLOCKLENGTH; k++)
			{
				a[k] = PresentRound.path[i][k];
			}
			RotateRightShift(a, HALFBLOCKLENGTH, RIGHTROTATESHIFT);

			for (j = HALFBLOCKLENGTH - 1; j > 0; j--)
			{

				if (!(a[j] == PresentRound.path[i][HALFBLOCKLENGTH + j] && a[j] == PresentRound.path[i + 1][j]))
					
				{
					
						NextOneRound(i, j - 1, PresentRound, ppround, indinum,sum);

				}
			}
			for (j = 1; j < HALFBLOCKLENGTH; j++)
			{
				if (!(a[j] == PresentRound.path[i][HALFBLOCKLENGTH + j] && a[j] == PresentRound.path[i + 1][j]))
					
				{
					sum++;

				}	
			}
		}
		}

	
}



void NextOneRound(int row, int col, individual PreRound, int pround,int indinum,int fsum)
{
	int i, j, k;
	int sum, flag, result, num;
	bool a[BLOCKLENGTH / 2], b[BLOCKLENGTH / 2], c[BLOCKLENGTH / 2];
	individual PresentRound;
	int ppround;
	
	ppround = pround;
	

	PresentRound = PreRound;




	
	// add one change bit
		for (k = 0; k < BLOCKLENGTH / 2; k++)
		{
			a[k] = PresentRound.path[row][k];
			b[k] = PresentRound.path[row][BLOCKLENGTH / 2 + k];
		}
		RotateRightShift(a, BLOCKLENGTH / 2, RIGHTROTATESHIFT);
		for (k = BLOCKLENGTH / 2 - 1; k > col; k--)
		{
			c[k] = PresentRound.path[row + 1][k];
		}
		c[k] = 1;
		ppround++;
		
		for (k = col - 1; k >= 0; k--)
		{
			if (a[k + 1] == b[k + 1] && a[k + 1] == c[k + 1])
			{
				c[k] = a[k] ^ b[k] ^ b[k + 1];

			}
			else
			{

				c[k] = 0;

			}
		}

		RotateLeftShift(b, BLOCKLENGTH / 2, LEFTROTATESHIFT);

		for (k = 0; k < BLOCKLENGTH / 2; k++)
		{
			a[k] = c[k];
			b[k] = b[k] ^ c[k];
			PresentRound.path[row + 1][k] = a[k];
			PresentRound.path[row + 1][BLOCKLENGTH / 2 + k] = b[k];
		}

		for (j = row + 1; j < LENGTH - 1; j++)
		{
			RotateRightShift(a, BLOCKLENGTH / 2, RIGHTROTATESHIFT);

			c[BLOCKLENGTH / 2 - 1] = a[BLOCKLENGTH / 2 - 1] ^ b[BLOCKLENGTH / 2 - 1];

			for (k = BLOCKLENGTH / 2 - 2; k >= 0; k--)
			{
				if (a[k + 1] == b[k + 1] && a[k + 1] == c[k + 1])
				{
					c[k] = a[k] ^ b[k] ^ b[k + 1];

				}
				else
				{

					c[k] = 0;

				}
			}

			RotateLeftShift(b, BLOCKLENGTH / 2, LEFTROTATESHIFT);

			for (k = 0; k < BLOCKLENGTH / 2; k++)
			{
				a[k] = c[k];
				b[k] = b[k] ^ c[k];
				PresentRound.path[j + 1][k] = a[k];
				PresentRound.path[j + 1][BLOCKLENGTH / 2 + k] = b[k];
			}

		}

		//calculate the weight of the new differential characteristic
		sum = fsum;
		for (k = 0; k < BLOCKLENGTH / 2; k++)
		{
			a[k] = PresentRound.path[row][k];
		}

		for (i = row; i < LENGTH - 1; i++)
		{
			RotateRightShift(a, BLOCKLENGTH / 2, RIGHTROTATESHIFT);
			for (k = 1; k < BLOCKLENGTH / 2; k++)
			{
				if (!(a[k] == PresentRound.path[i][BLOCKLENGTH / 2 + k] && a[k] == PresentRound.path[i + 1][k]))
				{
					sum++;
				}
			}
			for (k = 0; k < BLOCKLENGTH / 2; k++)
			{
				a[k] = PresentRound.path[i + 1][k];
			}
		}
		PresentRound.fitness = sum;
		if (population[indinum].fitness > sum)
		{
			population[indinum].fitness = sum;
		}

		

		if (PresentRound.fitness <= MINIWEIGHT)//if the weight meets the condition, output this differential characteristic
		{
			while(p->next!=NULL)
			{
				p=p->next;
				for (i = 0; i < BLOCKLENGTH; i++)
				{
					if(!(PresentRound.path[0][i]==p->headdiff[i]&&PresentRound.path[LENGTH-1][i]==p->taildiff[i]))
					{
						break;
					}
				}
				if(i==BLOCKLENGTH)
				{
					p->findnum=p->findnum+1;
					p=head;
					break;
				}
			}

			

			if(p->next==NULL)
			{
				p->next=(struct finddifference *)malloc(sizeof (struct finddifference));
				p=p->next;
				for (i = 0; i < BLOCKLENGTH; i++)
				{
					p->headdiff[i]=PresentRound.path[0][i];
					p->taildiff[i]=PresentRound.path[LENGTH-1][i];
				}
				p->firstgeneration=generation;
				p->findfitness=PresentRound.fitness;
				p->findnum=1;
				p->next=NULL;
				

			
				if (!qflag)
				{
					fpout = fopen("resultout.txt", "w");
					qflag = 1;
				}
				else
				{
					fpout = fopen("resultout.txt", "a");
				}
				fprintf(fpout, "find one better road, fitness is %d \n", PresentRound.fitness);
				printf("find one better road, fitness is %d \n", PresentRound.fitness);
				for (i = 0; i < LENGTH; i++)
				{
					fprintf(fpout, "round index %d \n", i);
					for (k = 0; k < BLOCKLENGTH / 2; k++)
					{
						if (k % 8 != 7)
							fprintf(fpout, "%d", PresentRound.path[i][k]);
						else
							fprintf(fpout, "%d ", PresentRound.path[i][k]);
					}
					fprintf(fpout, "\n");
					for (k = 0; k < BLOCKLENGTH / 2; k++)
					{
						if (k % 8 != 7)
							fprintf(fpout, "%d", PresentRound.path[i][HALFBLOCKLENGTH + k]);
						else
							fprintf(fpout, "%d ", PresentRound.path[i][HALFBLOCKLENGTH + k]);
					}
					fprintf(fpout, "\n");
				}


				finish=clock();
				totaltime=(long long)(finish-start)/CLOCKS_PER_SEC;
				day=totaltime/86400;
				hour=(totaltime%86400)/3600;
				minite=(totaltime%86400)%3600/60;
				second=(totaltime%86400)%3600%60;
				fprintf(fpout,"time is %d day %d hour %d minite %d second \n",day,hour,minite,second);
				p->findday=day;
				p->findhour=hour;
				p->findminite=minite;
				p->findsecond=second;
				for (i = 0; i < ENDFITNESS; i++)
				{
					p->fitnum[i]=0;
				}
				p=head;

				fclose(fpout);
				start=clock();

			}
			
		}


		//depth-first search for the next change bit
		if ((row <= 6 && PresentRound.fitness <= INDIWEIGHT&&(PresentRound.fitness-PreRound.fitness<= HIGHWEIGHT))||(row > 6 && PresentRound.fitness <= (INDIWEIGHT-15)&&(PresentRound.fitness-PreRound.fitness<= 0)))
		//the conditions that the next change bit should satisfy
		{
			
			if(ppround<= POINTROUND)
			{
				sum=fsum;

			for (k = 0; k < BLOCKLENGTH / 2; k++)
			{
				a[k] = PresentRound.path[row][k];
			}
			RotateRightShift(a, HALFBLOCKLENGTH, RIGHTROTATESHIFT);
			for (j = col; j > 0; j--)
			{

				if (!(a[j] == PresentRound.path[row][HALFBLOCKLENGTH + j] && a[j] == PresentRound.path[row + 1][j]))
					
				{
					
						NextOneRound(row, j - 1, PresentRound, ppround, indinum,sum);
					
				}
			}

			for (j = 1; j < HALFBLOCKLENGTH; j++)
			{
				if (!(a[j] == PresentRound.path[row][HALFBLOCKLENGTH + j] && a[j] == PresentRound.path[row + 1][j]))
				{
					sum++;
				}
			}

			for (i = row + 1; i < LENGTH - 1 && sum<MINIWEIGHT; i++)
			{
				for (k = 0; k < HALFBLOCKLENGTH; k++)
				{
					a[k] = PresentRound.path[i][k];
				}
				RotateRightShift(a, HALFBLOCKLENGTH, RIGHTROTATESHIFT);

				for (j = HALFBLOCKLENGTH - 1; j > 0; j--)
				{

					if (!(a[j] == PresentRound.path[i][HALFBLOCKLENGTH + j] && a[j] == PresentRound.path[i + 1][j]))
						
					{
						
						NextOneRound(i, j - 1, PresentRound, ppround, indinum,sum);
						
					}
				}
				for (j = 1; j < HALFBLOCKLENGTH; j++)
				{

					if (!(a[j] == PresentRound.path[i][HALFBLOCKLENGTH + j] && a[j] == PresentRound.path[i + 1][j]))
						
					{
						sum++;
					}
				}

			}

		}
		}
		
	
}

