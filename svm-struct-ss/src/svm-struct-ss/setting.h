#ifndef SVM_STRUCT_SS_SETTING_H
#define SVM_STRUCT_SS_SETTING_H

#define DEBUG_ON

/* default precision for solving the optimization problem */
# define DEFAULT_EPS         0.1

/* default loss rescaling method: 1=slack_rescaling, 2=margin_rescaling */
# define DEFAULT_RESCALING   2

/* default loss function: */
# define DEFAULT_LOSS_FCT    0

/* default optimization algorithm to use: 
@svm_struct_learn.h
#define  NSLACK_ALG               0
#define  NSLACK_SHRINK_ALG        1
#define  ONESLACK_PRIMAL_ALG      2
#define  ONESLACK_DUAL_ALG        3
#define  ONESLACK_DUAL_CACHE_ALG  4 */
# define DEFAULT_ALG_TYPE    3

/* store Psi(x,y) (for ALG_TYPE 1) instead of recomputing it every time: */
# define USE_FYCACHE         1

/* decide whether to evaluate sum before storing vectors in constraint
   cache: 
   0 = NO, 
   1 = YES (best, if sparse vectors and long vector lists), 
   2 = YES (best, if short vector lists),
   3 = YES (best, if dense vectors and long vector lists) */
# define COMPACT_CACHED_VECTORS 1
   
/* minimum absolute value below which values in sparse vectors are
   rounded to zero. Values are stored in the FVAL type defined in svm_common.h 
   RECOMMENDATION: assuming you use FVAL=float, use 
     10E-15 if COMPACT_CACHED_VECTORS is 1 
     10E-10 if COMPACT_CACHED_VECTORS is 2 or 3 
*/
# define COMPACT_ROUNDING_THRESH 10E-15

#endif