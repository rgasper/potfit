/****************************************************************
 *
 * potfit.h: potfit header file
 *
 ****************************************************************
 *
 * Copyright 2002-2014
 *	Institute for Theoretical and Applied Physics
 *	University of Stuttgart, D-70550 Stuttgart, Germany
 *	http://potfit.sourceforge.net/
 *
 ****************************************************************
 *
 *   This file is part of potfit.
 *
 *   potfit is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   potfit is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with potfit; if not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************/

#ifndef POTFIT_H
#define POTFIT_H

#define POTFIT_VERSION "potfit-git"

#ifdef __INTEL_COMPILER

/* remark #981: operands are evaluated in unspecified order */
#pragma warning(disable:981)

/* remark #1572: floating-point equality and inequality comparisons are unreliable */
#pragma warning(disable:1572)

#endif /* __INTEL_COMPILER */

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef MPI
#include <mpi.h>
#endif /* MPI */

#include "random.h"

/* general flag for threebody potentials (MEAM, Tersoff, SW, ...) */
#if defined MEAM || defined STIWEB || defined TERSOFF
#define THREEBODY
#endif /* MEAM || TERSOFF || STIWEB */

/* define EAM if TBEAM is defined */
#if defined TBEAM && !defined EAM
#define EAM
#endif /* TBEAM && !EAM */

#ifdef APOT
#define APOT_STEPS 500		/* number of sampling points for analytic pot */
#define APOT_PUNISH 10e6	/* general value for apot punishments */
#endif /* APOT */

#if defined EAM || defined ADP || defined MEAM
#define DUMMY_WEIGHT 100.0
#endif /* EAM || ADP || MEAM */

#define FORCE_EPS 0.1

/****************************************************************
 *
 *  SLOTS: number of different distance tables used in the force calculations
 *
 *  In potfit all potentials are calculated via spline interpolations of pre-
 *  calculated potential tables. To speed up the spline calculation,
 *  the exact position of a neighbor distance has to be known with respect to the
 *  tabulated values. For each potential function in a force routine there should
 *  be a different slot with the corresponding potential table information.
 *
 *  SLOTS = 1 for the following interactions:
 *  	PAIR, COULOMB, DIPOLE, TERSOFF
 *  	0 ... pair distance
 *
 *  EAM: 	SLOTS = 2
 *  	0 ... pair distance
 *  	1 ... transfer function
 *
 *  STIWEB: 	SLOTS = 2
 *  	0 ... pair distance
 *  	1 ... exponential functions
 *
 *  TBEAM: 	SLOTS = 3
 *  	0 ... pair distance
 *  	1 ... transfer function
 *  	2 ... 2nd transfer function
 *
 *  MEAM: 	SLOTS = 3
 *  	0 ... pair distance
 *  	1 ... transfer function
 *  	2 ... f(r_ij)
 *
 *  ADP: 	SLOTS = 4
 *  	0 ... pair distance
 *  	1 ... transfer function
 *  	2 ... dipole term
 *  	3 ... quadrupole term
 *
 ****************************************************************/

#define SLOTS 1

#if defined EAM || defined STIWEB
#undef SLOTS
#define SLOTS 2
#endif /* EAM || STIWEB */

#if defined TBEAM || defined MEAM
#undef SLOTS
#define SLOTS 3
#endif /* TBEAM || MEAM */

#if defined ADP
#undef SLOTS
#define SLOTS 4
#endif /* ADP */

#define MAX(a,b)   ((a) > (b) ? (a) : (b))
#define MIN(a,b)   ((a) < (b) ? (a) : (b))
#define SPROD(a,b) (((a).x * (b).x) + ((a).y * (b).y) + ((a).z * (b).z))
#define SWAP(A,B,C) (C)=(A);(A)=(B);(B)=(C);

/****************************************************************
 *
 *  include type definitions after all preprocessor flags are properly set
 *
 ****************************************************************/

#include "types.h"

/****************************************************************
 *
 *  global variables
 *
 ****************************************************************/

/* MAIN is defined only once in the main module */
#ifdef MAIN
#define EXTERN			/* define variables in main */
#define INIT(data) = data	/* initialize data only in main */
#else
#define EXTERN extern		/* declare them extern otherwise */
#define INIT(data)		/* skip initialization otherwise */
#endif /* MAIN */

/* system variables */
EXTERN int myid INIT(0);	/* Who am I? (0 if serial) */
EXTERN int num_cpus INIT(1);	/* How many cpus are there */
#ifdef MPI
EXTERN MPI_Datatype MPI_ATOM;
EXTERN MPI_Datatype MPI_NEIGH;
#ifdef THREEBODY
EXTERN MPI_Datatype MPI_ANGL;
#endif
EXTERN MPI_Datatype MPI_STENS;
EXTERN MPI_Datatype MPI_VECTOR;
#endif /* MPI */

/* general settings (from parameter file) */
EXTERN char config[255] INIT("\0");	/* file with atom configuration */
EXTERN char distfile[255] INIT("\0");	/* file for distributions */
EXTERN char endpot[255] INIT("\0");	/* file for end potential */
EXTERN char flagfile[255] INIT("\0");	/* break if file exists */
EXTERN char imdpot[255] INIT("\0");	/* file for IMD potential */
EXTERN char maxchfile[255] INIT("\0");	/* file with maximal changes */
EXTERN char output_prefix[255] INIT("\0");	/* prefix for all output files */
EXTERN char output_lammps[255] INIT("\0");	/* lammps output files */
EXTERN char plotfile[255] INIT("\0");	/* file for plotting */
EXTERN char plotpointfile[255] INIT("\0");	/* write points for plotting */
EXTERN char startpot[255] INIT("\0");	/* file with start potential */
EXTERN char tempfile[255] INIT("\0");	/* backup potential file */
EXTERN int imdpotsteps INIT(1000);	/* resolution of IMD potential */
EXTERN int ntypes INIT(-1);	/* number of atom types */
EXTERN int opt INIT(0);		/* optimization flag */
EXTERN int seed INIT(4);	/* seed for RNG */
EXTERN int usemaxch INIT(0);	/* use maximal changes file */
EXTERN int write_output_files INIT(0);
EXTERN int write_lammps_files INIT(0);
EXTERN int write_pair INIT(0);
EXTERN int writeimd INIT(0);
EXTERN int write_lammps INIT(0);	/* write output also in LAMMPS format */
#ifdef EVO
EXTERN double evo_threshold INIT(1.e-6);
#else /* EVO */
EXTERN char anneal_temp[20] INIT("\0");
#endif /* EVO */
EXTERN double eweight INIT(-1.0);
EXTERN double sweight INIT(-1.0);
EXTERN double extend INIT(2.0);	/* how far should one extend imd pot */
#ifdef APOT
EXTERN int compnodes INIT(0);	/* how many additional composition nodes */
EXTERN int enable_cp INIT(0);	/* switch chemical potential on/off */
EXTERN double apot_punish_value INIT(0.0);
EXTERN double plotmin INIT(0.0);	/* minimum for plotfile */
#endif /* APOT */

/* configurations */
EXTERN atom_t *atoms;		/* atoms array */
EXTERN atom_t *conf_atoms;	/* Atoms in configuration */
EXTERN char **elements;		/* element names from vasp2force */
EXTERN int **na_type;		/* number of atoms per type */
EXTERN int *cnfstart;		/* Nr. of first atom in config */
EXTERN int *conf_uf;
#ifdef STRESS
EXTERN int *conf_us;
#endif /* STRESS */
EXTERN int *inconf;		/* Nr. of atoms in each config */
EXTERN int *useforce;		/* Should we use forces */
#ifdef STRESS
EXTERN int *usestress;		/* Should we use stresses */
#endif /* STRESS */
EXTERN int have_elements INIT(0);	/* do we have the elements ? */
EXTERN int natoms INIT(0);	/* number of atoms */
EXTERN int nconf INIT(0);	/* number of configurations */
#ifdef CONTRIB
EXTERN int have_contrib_box INIT(0);	/* do we have a box of contrib. atoms? */
EXTERN int n_spheres INIT(0);	/* number of spheres of contrib. atoms */
EXTERN double *r_spheres;	/* radii of the spheres of contrib. atoms */
#endif /* CONTRIB */
EXTERN double global_cell_scale INIT(1.0);	/* global scaling parameter */
EXTERN double *coheng;		/* Cohesive energy for each config */
EXTERN double *conf_vol;
EXTERN double *conf_weight;	/* weight of configuration */
EXTERN double *force_0;		/* the forces we aim at */
EXTERN double *rcut;
EXTERN double *rmin;
EXTERN double *volume;		/* volume of cell */
EXTERN double rcutmin INIT(999.9);	/* minimum of all cutoff values */
EXTERN double rcutmax INIT(0.0);	/* maximum of all cutoff values */
#ifdef STRESS
EXTERN sym_tens *conf_stress;
EXTERN sym_tens *stress;	/* Stresses in each config */
#endif /* STRESS */
EXTERN vector box_x, box_y, box_z;
#ifdef CONTRIB
EXTERN vector cbox_o;		/* origin of box of contrib. atoms */
EXTERN vector cbox_a, cbox_b, cbox_c;	/* box vectors for box of contrib. atoms */
EXTERN vector *sphere_centers;	/* centers of the spheres of contrib. atoms */
#endif /* CONTRIB */
EXTERN vector tbox_x, tbox_y, tbox_z;

/* potential variables */
EXTERN int *gradient;		/* Gradient of potential fns.  */
EXTERN int *invar_pot;
EXTERN int format INIT(-1);	/* format of potential table */
EXTERN int have_grad INIT(0);	/* Is gradient specified?  */
EXTERN int have_invar INIT(0);	/* Are invariant pots specified?  */
#ifdef APOT
EXTERN int *smooth_pot;
EXTERN int cp_start INIT(0);	/* cp in opt_pot.table */
EXTERN int global_idx INIT(0);	/* index for global parameters in opt_pot table */
EXTERN int global_pot INIT(0);	/* number of "potential" for global parameters */
EXTERN int have_globals INIT(0);	/* do we have global parameters? */
EXTERN double *calc_list;	/* list of current potential in the calc table */
EXTERN double *compnodelist;	/* list of the composition nodes */
#endif /* APOT */

/* potential tables */
EXTERN pot_table_t opt_pot;	/* potential in the internal representation used for minimisation */
EXTERN pot_table_t calc_pot;	/* the potential table used for force calculations */
#ifdef APOT
EXTERN apot_table_t apot_table;	/* potential in analytic form */
EXTERN int n_functions INIT(0);	/* number of analytic function prototypes */
EXTERN function_table_t function_table;	/* table with all functions */
#endif /* APOT */

/* optimization variables */
EXTERN int fcalls INIT(0);
EXTERN int mdim INIT(0);
EXTERN int ndim INIT(0);
EXTERN int ndimtot INIT(0);
EXTERN int paircol INIT(0);	/* How manc columns for pair potential */
EXTERN double d_eps INIT(1e-6);

/* general variables */
EXTERN int firstatom INIT(0);
EXTERN int firstconf INIT(0);
EXTERN int myatoms INIT(0);
EXTERN int myconf INIT(0);

/* pointers for force-vector */
EXTERN int energy_p INIT(0);	/* pointer to energies */
#ifdef STRESS
EXTERN int stress_p INIT(0);	/* pointer to stresses */
#endif /* STRESS */
#if defined EAM || defined ADP || defined MEAM
EXTERN int dummy_p INIT(0);	/* pointer to dummy constraints */
EXTERN int limit_p INIT(0);	/* pointer to limiting constraints */
#endif /* EAM || ADP || MEAM */
#ifdef APOT
EXTERN int punish_par_p INIT(0);	/* pointer to parameter punishment contraints */
EXTERN int punish_pot_p INIT(0);	/* pointer to potential punishment constraints */
#endif /* APOT */

/* memory management */
EXTERN char **pointer_names;
EXTERN int num_pointers INIT(0);
EXTERN void **all_pointers;
EXTERN double *u_address;

/* variables needed for atom distribution with mpi */
#ifdef MPI
EXTERN int *atom_dist;
EXTERN int *atom_len;
EXTERN int *conf_dist;
EXTERN int *conf_len;
#endif /* MPI */

/* misc. stuff - has to belong somewhere */
EXTERN int *idx;
EXTERN int init_done INIT(0);
EXTERN int plot INIT(0);	/* plot output flag */
#if defined EAM || defined ADP || defined MEAM
EXTERN double *lambda;		/* embedding energy slope... */
#endif
EXTERN double *maxchange;	/* Maximal permissible change */
EXTERN dsfmt_t dsfmt;		/* random number generator */
EXTERN char *component[6];	/* componentes of vectors and tensors */

/* variables needed for electrostatic options */
#ifdef COULOMB
EXTERN double dp_eps INIT(14.40);	/* this is e^2/(4*pi*epsilon_0) in eV A */
EXTERN double dp_cut INIT(10.0);	/* cutoff-radius for long-range interactions */
#endif /* COULOMB */
#ifdef DIPOLE
EXTERN double dp_tol INIT(1.0e-7);	/* dipole iteration precision */
EXTERN double dp_mix INIT(0.2);	/* mixing parameter (other than that one in IMD) */
#endif /* DIPOLE */

/****************************************************************
 *
 *  global function pointers
 *
 ****************************************************************/

EXTERN double calc_forces(double *, double *, int);
EXTERN double (*splint) (pot_table_t *, double *, int, double);
EXTERN double (*splint_grad) (pot_table_t *, double *, int, double);
EXTERN double (*splint_comb) (pot_table_t *, double *, int, double, double *);
#ifdef APOT
EXTERN void (*write_pot_table) (apot_table_t *, char *);
#else
EXTERN void (*write_pot_table) (pot_table_t *, char *);
#endif /* APOT */

/****************************************************************
 *
 *  function prototypes
 *
 ****************************************************************/

/* general functions [potfit.c] */
void  error(int, char *, ...);
void  warning(char *, ...);

/* error reports [errors.c] */
void  write_errors(double *, double);

/* reading parameter file [param.c] */
int   getparam(char *, void *, param_t, int, int);
void  check_parameters_complete(char *);
void  read_parameters(int, char **);
void  read_paramfile(FILE *);

/* general force functions [forces.c] */
void  set_forces();
void  init_forces();
void  set_force_vector_pointers();

/* force routines for different potential models [force_xxx.c] */
#ifdef PAIR
EXTERN const char interaction_name[5] INIT("PAIR");
#elif defined EAM && !defined COULOMB
#ifndef TBEAM
EXTERN const char interaction_name[4] INIT("EAM");
#else
EXTERN const char interaction_name[6] INIT("TBEAM");
#endif /* TBEAM */
#elif defined ADP
EXTERN const char interaction_name[4] INIT("ADP");
#elif defined COULOMB && !defined EAM
EXTERN const char interaction_name[7] INIT("ELSTAT");
#elif defined COULOMB && defined EAM
EXTERN const char interaction_name[11] INIT("EAM_ELSTAT");
#elif defined MEAM
EXTERN const char interaction_name[5] INIT("MEAM");
#elif defined STIWEB
EXTERN const char interaction_name[7] INIT("STIWEB");
void  update_stiweb_pointers(double *);
#elif defined TERSOFF
#ifdef TERSOFFMOD
EXTERN const char interaction_name[11] INIT("TERSOFFMOD");
#else
EXTERN const char interaction_name[8] INIT("TERSOFF");
#endif /* TERSOFFMOD */
void  update_tersoff_pointers(double *);
#endif /* interaction type */

/* rescaling functions for EAM [rescale.c] */
#if !defined APOT && ( defined EAM || defined MEAM )
double rescale(pot_table_t *, double, int);
void  embed_shift(pot_table_t *);
#endif /* !APOT && (EAM || MEAM) */

/* MPI parallelization [mpi_utils.c] */
#ifdef MPI
void  init_mpi(int, char **);
void  shutdown_mpi(void);
void  broadcast_params(void);
void  broadcast_neighbors(void);
void  broadcast_angles(void);
void  potsync(void);
#endif /* MPI */

#endif /* POTFIT_H */
