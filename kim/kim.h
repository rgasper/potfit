/****************************************************************
*
* KIM-potfit
* 
* force_kim.h
*
* header file for calculating force via KIM
****************************************************************/ 

#ifndef KIM_H
#define KIM_H

#include "KIM_API_C.h"
#include "KIM_API_status.h"

#ifndef DIM
#define DIM 3
#endif



/* Define neighborlist structure */
typedef struct
{
   int iteratorId;
   int* NNeighbors;
   int* neighborList;
   double* RijList;
	 int* BeginIdx;   /* The position of first neighbor of each atom in 
	 										the neighbor list */
} NeighObjectType;

typedef struct
{
  char** name;
 	double** value;   /* the pointer to parameters */
	double** nestedvalue; /*nest all values into one long variable, would be equal  
	                        to value if all optimizalbe has rank 0 */
	int* rank;
	int** shape;
	int Nparam;				/* number of optimizable parameters */
} OptParamType;

OptParamType* OptParamAllConfig;


/* function prototypes */

/* functions defined in `kim.c' */
void InitKIM();

void FreeKIM();

int InitObject();

int get_OptimizableParamInfo(void* pkim, OptParamType* OptParam);

int nest_OptimizableParamValue(void* pkim, OptParamType* OptParam, int inlcude_cutoff);

int PublishCutoff(void* pkim, double cutoff);

int CreateKIMObj(void* pkim, int Natoms, int Nspecies, int start);

int get_neigh(void* kimmdl, int *mode, int *request, int* part,
              int* numnei, int** nei1part, double** Rij);					

int CalcForce(void* pkim, double** energy, double** force, double** virial,
							int useforce, int usestress);

int get_OptimizableParamSize();


/* functions in force_[interaction]_kim.c */
int PublishParam(void* pkim, OptParamType* FreeParam, double* PotTable);

#ifdef PAIR
/* used only for check purpose (could be deleted ) */
int AnalyticForce(double epsilon, double sigma, double cutoff,
									double r, double* phi_val, double* phi_grad); 
#endif /* PAIR */

#ifdef EAM
int CreateModel();
int MakeModel();
int CreateParamFileEAM();
#endif /*EAM*/

#endif /* KIM_H */