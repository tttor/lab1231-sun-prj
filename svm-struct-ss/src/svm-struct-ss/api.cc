/***********************************************************************/
/*                                                                     */
/*   svm_struct_api.c                                                  */
/*                                                                     */
/*   Definition of API for attaching implementing SVM learning of      */
/*   structures (e.g. parsing, multi-label classification, HMM)        */ 
/*                                                                     */
/*   Author: Thorsten Joachims                                         */
/*   Date: 03.07.04                                                    */
/*                                                                     */
/*   Copyright (c) 2004  Thorsten Joachims - All rights reserved       */
/*                                                                     */
/*   This software is available for non-commercial use only. It must   */
/*   not be modified and distributed without prior permission of the   */
/*   author. The author is not responsible for implications from the   */
/*   use of this software.                                             */
/*                                                                     */
/***********************************************************************/

#include <stdio.h>
#include <string.h>
#include "svm_struct/svm_struct_common.h"

#include "api.h"
#include "util.h"
#include "joint_feature_extractor.h"
#include "io.h"
#include "find_most_violated_constraint.h"
#include "loss_function.h"

#include <string>

// MODIFIED:
SAMPLE read_struct_examples(char *file, STRUCT_LEARN_PARM *sparm)
{
  /* Reads struct examples and returns them in sample. The number of
     examples must be written into sample.n */
  SAMPLE   sample;  /* sample */
  EXAMPLE  *examples;
  long     n;       /* number of examples */

  // n=100; /* replace by appropriate number of examples */
  // examples=(EXAMPLE *)my_malloc(sizeof(EXAMPLE)*n);

  /* fill in your code here */
  std::vector<std::string> list;
  list = svm_struct_ss::io::read_list(std::string(file));

  n = list.size();

  examples = (EXAMPLE *)my_malloc(sizeof(EXAMPLE)*n);
  svm_struct_ss::util::set_examples(list,examples);

  //
  sample.n = n;
  sample.examples = examples;

  // debug_var("sample.n", sample.n);
  // debug_var("sample.examples[0].x.id",sample.examples[0].x.id);
  // debug_var("sample.examples[0].y.height",sample.examples[0].y.height);
  // debug_var("sample.examples[0].y.label[0]",sample.examples[0].y.flatten_label[0]);
  // assert(false);

  return(sample);
}

// MODIFIED:
void        init_struct_model(SAMPLE sample, STRUCTMODEL *sm, 
			      STRUCT_LEARN_PARM *sparm, LEARN_PARM *lparm, 
			      KERNEL_PARM *kparm)
{
  /* Initialize structmodel sm. The weight vector w does not need to be
     initialized, but you need to provide the maximum size of the
     feature space in sizePsi. This is the maximum number of different
     weights that can be learned. Later, the weight vector w will
     contain the learned weights for the model. */
  debug_in_msg("init_struct_model");

  sm->sizePsi = svm_struct_ss::util::get_n_feature();
  debug_var("sm->sizePsi",sm->sizePsi);

  debug_out_msg("init_struct_model");
}

// MODIFIED:
CONSTSET    init_struct_constraints(SAMPLE sample, STRUCTMODEL *sm, 
            STRUCT_LEARN_PARM *sparm)
{
  /* Initializes the optimization problem. Typically, you do not need
     to change this function, since you want to start with an empty
     set of constraints. However, if for example you have constraints
     that certain weights need to be positive, you might put that in
     here. The constraints are represented as lhs[i]*w >= rhs[i]. lhs
     is an array of feature vectors, rhs is an array of doubles. m is
     the number of constraints. The function returns the initial
     set of constraints. */
  debug_in_msg("init_struct_constraints");

  CONSTSET c;
  long     sizePsi=sm->sizePsi;
  long     i;
  WORD     words[2];

  if(START_WITH_EMPTY_SET_OF_CONSTRAINTS) { /* normal case: start with empty set of constraints */
    debug_msg("normal case: start with empty set of constraints");
    c.lhs=NULL;
    c.rhs=NULL;
    c.m=0;
  }
  else { /* add constraints so that all learned weights are
            positive. WARNING: Currently, they are positive only up to
            precision epsilon set by -e. */
    debug_msg("add constraints so that all learned weights are positive");       
    c.lhs=my_malloc(sizeof(DOC *)*sizePsi);
    c.rhs=my_malloc(sizeof(double)*sizePsi);
    for(i=0; i<sizePsi; i++) {
      words[0].wnum=i+1;
      words[0].weight=1.0;
      words[1].wnum=0;
      /* the following slackid is a hack. we will run into problems,
         if we have move than 1000000 slack sets (ie examples) */
      c.lhs[i]=create_example(i,0,1000000+i,1,create_svector(words,"",1.0));
      c.rhs[i]=0.0;
    }
    c.m = sizePsi;// TODO @tttor: why? is this how to dd constraints so that all learned weights are positive?
  }
  debug_var("c.m",c.m);
  debug_in_msg("init_struct_constraints");
  return(c);
}

// MODIFIED:
LABEL       find_most_violated_constraint_marginrescaling(PATTERN x, LABEL y, 
						     STRUCTMODEL *sm, 
						     STRUCT_LEARN_PARM *sparm)
{
  /* Finds the label ybar for pattern x that that is responsible for
     the most violated constraint for the margin rescaling
     formulation. For linear slack variables, this is that label ybar
     that maximizes

            argmax_{ybar} loss(y,ybar)+psi(x,ybar)

     Note that ybar may be equal to y (i.e. the max is 0), which is
     different from the algorithms described in
     [Tschantaridis/05]. Note that this argmax has to take into
     account the scoring function in sm, especially the weights sm.w,
     as well as the loss function, and whether linear or quadratic
     slacks are used. The weights in sm.w correspond to the features
     defined by psi() and range from index 1 to index
     sm->sizePsi. Most simple is the case of the zero/one loss
     function. For the zero/one loss, this function should return the
     highest scoring label ybar (which may be equal to the correct
     label y), or the second highest scoring label ybar, if
     Psi(x,ybar)>Psi(x,y)-1. If the function cannot find a label, it
     shall return an empty label as recognized by the function
     empty_label(y). */
  debug_in_msg("find_most_violated_constraint_marginrescaling");
  LABEL ybar;

  /* insert your code for computing the label ybar here */
  STRUCTMODEL model = *sm;
  ybar = svm_struct_ss::find_most_violated_constraint::margin_rescaling(x, y, model);

  debug_out_msg("find_most_violated_constraint_marginrescaling");
  return(ybar);
}

// MODIFIED
SVECTOR     *psi(PATTERN x, LABEL y, STRUCTMODEL *sm,
		 STRUCT_LEARN_PARM *sparm)
{
  /* Returns a feature vector describing the match between pattern x
     and label y. The feature vector is returned as a list of
     SVECTOR's. Each SVECTOR is in a sparse representation of pairs
     <featurenumber:featurevalue>, where the last pair has
     featurenumber 0 as a terminator. Featurenumbers start with 1 and
     end with sizePsi. Featuresnumbers that are not specified default
     to value 0. As mentioned before, psi() actually returns a list of
     SVECTOR's. Each SVECTOR has a field 'factor' and 'next'. 'next'
     specifies the next element in the list, terminated by a NULL
     pointer. The list can be though of as a linear combination of
     vectors, where each vector is weighted by its 'factor'. This
     linear combination of feature vectors is multiplied with the
     learned (kernelized) weight vector to score label y for pattern
     x. Without kernels, there will be one weight in sm.w for each
     feature. Note that psi has to match
     find_most_violated_constraint_???(x, y, sm) and vice versa. In
     particular, find_most_violated_constraint_???(x, y, sm) finds
     that ybar!=y that maximizes psi(x,ybar,sm)*sm.w (where * is the
     inner vector product) and the appropriate function of the
     loss + margin/slack rescaling method. See that paper for details. */
  /* insert code for computing the feature vector for x and y here */
  // TODO @tttor: we may have a list of 3 feature vector: unary, pairwise_horizontal, pairwise_vertical
  // Currently, they are merged in one feature vector, so this psi() returns a list of one big feature vector
  debug_in_msg("psi");

  SVECTOR* fvec;
  fvec = (SVECTOR *) my_malloc(sizeof(SVECTOR));// fvec points to a list of _one_ feature-vector
  fvec->next = NULL;// we have only this single feature vector
  fvec->factor = 1.0;
  fvec->kernel_id = 0;
  fvec->userdefined = NULL;
  
  const size_t n_word = sm->sizePsi+1;// plus one for a termination flag, where wnum=0
  fvec->words = (WORD *) my_malloc( sizeof(WORD)*n_word );

  svm_struct_ss::joint_feature_extractor::extract_feature(x, y, n_word, fvec);

  // set the termination sign
  fvec->words[n_word-1].wnum = 0;

  debug_out_msg("psi");
  return(fvec);
}

// MODIFIED:
double      loss(LABEL y, LABEL ybar, STRUCT_LEARN_PARM *sparm)
{
  /* loss for correct label y and predicted label ybar. The loss for
     y==ybar has to be zero. sparm->loss_function is set with the -l option. */
  debug_in_msg("loss");

  double loss = 0.0;
  if(sparm->loss_function == 0) { /* type 0 loss: 0/1 loss */
                                  /* return 0, if y==ybar. return 1 else */
    loss = svm_struct_ss::loss_function::zero_one_loss(y,ybar);
  }
  else {
    /* Put your code for different loss functions here. But then
       find_most_violated_constraint_???(x, y, sm) has to return the
       highest scoring label with the largest loss. */
  }
  
  debug_out_msg("loss");
  return loss;
}

// MODIFIED:
LABEL       classify_struct_example(PATTERN x, STRUCTMODEL *sm, 
            STRUCT_LEARN_PARM *sparm)
{
  /* Finds the label yhat for pattern x that scores the highest
     according to the linear evaluation function in sm, especially the
     weights sm.w. The returned label is taken as the prediction of sm
     for the pattern x. The weights correspond to the features defined
     by psi() and range from index 1 to index sm->sizePsi. If the
     function cannot find a label, it shall return an empty label as
     recognized by the function empty_label(y). */
  debug_in_msg("classify_struct_example");
  LABEL y;

  /* insert your code for computing the predicted label y here */
  assert(false && "NOT IMPLEMENTED YET");

  debug_out_msg("classify_struct_example");
  return(y);
}

// MODIFIED:
LABEL       find_most_violated_constraint_slackrescaling(PATTERN x, LABEL y, 
                 STRUCTMODEL *sm, 
                 STRUCT_LEARN_PARM *sparm)
{
  /* Finds the label ybar for pattern x that that is responsible for
     the most violated constraint for the slack rescaling
     formulation. For linear slack variables, this is that label ybar
     that maximizes

            argmax_{ybar} loss(y,ybar)*(1-psi(x,y)+psi(x,ybar)) 

     Note that ybar may be equal to y (i.e. the max is 0), which is
     different from the algorithms described in
     [Tschantaridis/05]. Note that this argmax has to take into
     account the scoring function in sm, especially the weights sm.w,
     as well as the loss function, and whether linear or quadratic
     slacks are used. The weights in sm.w correspond to the features
     defined by psi() and range from index 1 to index
     sm->sizePsi. Most simple is the case of the zero/one loss
     function. For the zero/one loss, this function should return the
     highest scoring label ybar (which may be equal to the correct
     label y), or the second highest scoring label ybar, if
     Psi(x,ybar)>Psi(x,y)-1. If the function cannot find a label, it
     shall return an empty label as recognized by the function
     empty_label(y). */
  debug_in_msg("find_most_violated_constraint_slackrescaling");
  LABEL ybar;

  /* insert your code for computing the label ybar here */
  assert(false && "NOT IMPLEMENTED YET: find_most_violated_constraint_slackrescaling");

  debug_out_msg("find_most_violated_constraint_slackrescaling");
  return(ybar);
}

// MODIFIED:
int         empty_label(LABEL y)
{
  /* Returns true, if y is an empty label. An empty label might be
     returned by find_most_violated_constraint_???(x, y, sm) if there
     is no incorrect label that can be found for x, or if it is unable
     to label x at all */
  if (y.size==0) return 1;
  else return 0;
}

// MODIFIED:
int         finalize_iteration(double ceps, int cached_constraint,
             SAMPLE sample, STRUCTMODEL *sm,
             CONSTSET cset, double *alpha, 
             STRUCT_LEARN_PARM *sparm)
{
  /* This function is called just before the end of each cutting plane iteration. 

  ceps is the amount by which the most violated constraint found in the current iteration was violated. 
  cached_constraint is true if the added constraint was constructed from the cache. 
  
  If the return value is FALSE, then the algorithm is allowed to terminate. 
  If it is TRUE, the algorithm will keep iterating even if the desired precision sparm->epsilon is already reached. */
  using namespace std;
  // assert(false && "NOT IMPLEMENTED YET: finalize_iteration");
  return(0);
}

// MODIFIED:
void        print_struct_learning_stats(SAMPLE sample, STRUCTMODEL *sm,
          CONSTSET cset, double *alpha, 
          STRUCT_LEARN_PARM *sparm)
{
  /* This function is called after training and allows final touches to
     the model sm. But primarly it allows computing and printing any
     kind of statistic (e.g. training error) you might want. */
}

void        print_struct_testing_stats(SAMPLE sample, STRUCTMODEL *sm,
               STRUCT_LEARN_PARM *sparm, 
               STRUCT_TEST_STATS *teststats)
{
  /* This function is called after making all test predictions in
     svm_struct_classify and allows computing and printing any kind of
     evaluation (e.g. precision/recall) you might want. You can use
     the function eval_prediction to accumulate the necessary
     statistics for each prediction. */
}

void        eval_prediction(long exnum, EXAMPLE ex, LABEL ypred, 
          STRUCTMODEL *sm, STRUCT_LEARN_PARM *sparm, 
          STRUCT_TEST_STATS *teststats)
{
  /* This function allows you to accumlate statistic for how well the
     predicition matches the labeled example. It is called from
     svm_struct_classify. See also the function
     print_struct_testing_stats. */
  if(exnum == 0) { /* this is the first time the function is
          called. So initialize the teststats */
  }
}

// MODIFIED:
void        write_struct_model(char *file, STRUCTMODEL *sm, 
			       STRUCT_LEARN_PARM *sparm)
{
  /* Writes structural model sm to file file. */
}

// MODIFIED:
STRUCTMODEL read_struct_model(char *file, STRUCT_LEARN_PARM *sparm)
{
  /* Reads structural model sm from file file. This function is used
     only in the prediction module, not in the learning module. */
}

// MODIFIED:
void        write_label(FILE *fp, LABEL y)
{
  /* Writes label y to file handle fp. */
} 

// MODIFIED:
void        free_pattern(PATTERN x) {
  /* Frees the memory of x. */
}

// MODIFIED:
void        free_label(LABEL y) {
  /* Frees the memory of y. */
}

// MODIFIED:
void        free_struct_model(STRUCTMODEL sm) 
{
  /* Frees the memory of model. */
  if(sm.w) free(sm.w); /* this is free'd in free_model */
  if(sm.svm_model) free_model(sm.svm_model,1);

  /* add free calls for user defined data here */
  if(sm.learning_param) free(sm.learning_param);
}

// YET EMPTY DEFINITIONS _or_ NOT YET ELABORATED //////////////////////////////
void        svm_struct_learn_api_init(int argc, char* argv[])
{
  debug_in_msg("svm_struct_learn_api_init");
  /* Called in learning part before anything else is done to allow
     any initializations that might be necessary. */
  debug_out_msg("svm_struct_learn_api_init");
}

void        svm_struct_learn_api_exit()
{
  /* Called in learning part at the very end to allow any clean-up
     that might be necessary. */
}

void        svm_struct_classify_api_init(int argc, char* argv[])
{
  /* Called in prediction part before anything else is done to allow
     any initializations that might be necessary. */
}

void        svm_struct_classify_api_exit()
{
  /* Called in prediction part at the very end to allow any clean-up
     that might be necessary. */
}

void        free_struct_sample(SAMPLE s)
{
  /* Frees the memory of sample s. */
  int i;
  for(i=0;i<s.n;i++) { 
    free_pattern(s.examples[i].x);
    free_label(s.examples[i].y);
  }
  free(s.examples);
}

void        print_struct_help()
{
  /* Prints a help text that is appended to the common help text of
     svm_struct_learn. */
  printf("         --* string  -> custom parameters that can be adapted for struct\n");
  printf("                        learning. The * can be replaced by any character\n");
  printf("                        and there can be multiple options starting with --.\n");
}

void         parse_struct_parameters(STRUCT_LEARN_PARM *sparm)
{
  /* Parses the command line parameters that start with -- */
  int i;

  for(i=0;(i<sparm->custom_argc) && ((sparm->custom_argv[i])[0] == '-');i++) {
    switch ((sparm->custom_argv[i])[2]) 
      { 
      case 'a': i++; /* strcpy(learn_parm->alphafile,argv[i]); */ break;
      case 'e': i++; /* sparm->epsilon=atof(sparm->custom_argv[i]); */ break;
      case 'k': i++; /* sparm->newconstretrain=atol(sparm->custom_argv[i]); */ break;
      default: printf("\nUnrecognized option %s!\n\n",sparm->custom_argv[i]);
         exit(0);
      }
  }
}

void        print_struct_help_classify()
{
  /* Prints a help text that is appended to the common help text of
     svm_struct_classify. */
  printf("         --* string -> custom parameters that can be adapted for struct\n");
  printf("                       learning. The * can be replaced by any character\n");
  printf("                       and there can be multiple options starting with --.\n");
}

void         parse_struct_parameters_classify(STRUCT_LEARN_PARM *sparm)
{
  /* Parses the command line parameters that start with -- for the
     classification module */
  int i;

  for(i=0;(i<sparm->custom_argc) && ((sparm->custom_argv[i])[0] == '-');i++) {
    switch ((sparm->custom_argv[i])[2]) 
      { 
      /* case 'x': i++; strcpy(xvalue,sparm->custom_argv[i]); break; */
      default: printf("\nUnrecognized option %s!\n\n",sparm->custom_argv[i]);
         exit(0);
      }
  }
}
