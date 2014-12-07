/************************************************************/

/*   svm_struct_api.c                                                  */
/*                                                                     */
/*   Definition of API for attaching implementing SVM learning of      */
/*   structures (e.g. parsing, multi-label classification, HMM)        */
/*                                                                     */
/*   Author: Thorsten Joachims                                         */
/*   Date: 03.07.04                                                    */
/*                                                                     */
/*   Copyright (c) 2004  Thorsten Joachims - All rights reserved       */
/*                                                            (char*)         */
/*   This software is available for non-commercial use only. I(char*)t must   */
/*   not be modified and distributed without prior permission of the   */
/*   author. The author is not responsible for implications from the   */
/*   use of this software.                                             */
/*                                                                     */
/***********************************************************************/

#include <stdio.h>
#include <string.h>
#include "svm_struct/svm_struct_common.h"
#include "svm_struct_api.h"
#include <fstream>
#include <iostream>



#include "ssvm_ss_dataset_constraint.h"
using namespace std;

inline size_t var_idx(size_t x, size_t y, size_t width)
{
    return  x + y * width;
}

void        svm_struct_learn_api_init(int argc, char *argv[])
{
    /* Called in learning part before anything else is done to allow
       any initializations that might be necessary. */
}

void        svm_struct_learn_api_exit()
{
    /* Called in learning part at the very end to allow any clean-up
       that might be necessary. */
}

void        svm_struct_classify_api_init(int argc, char *argv[])
{
    /* Called in prediction part before anything else is done to allow
       any initializations that might be necessary. */
}

void        svm_struct_classify_api_exit()
{
    /* Called in prediction part at the very end to allow any clean-up
       that might be necessary. */
}

//read list of file id
SAMPLE      read_struct_examples(char *file, STRUCT_LEARN_PARM *sparm)
{
    /* Reads struct examples and returns them in sample. The number of
       examples must be written into sample.n */
    SAMPLE   sample;  /* sample */
    EXAMPLE  *examples;
    long     n_sample;       /* number of examples */


    string tmp;
    ifstream counter(file);
    n_sample = 0;
    _("OK");
    if (counter.is_open())
        while (getline(counter, tmp)) ++n_sample;


    _("OK");

    __("READ", n_sample);

    //n_sample=ssvm_ss::dataset::n_example; /* replace by appropriate number of examples */
    examples = (EXAMPLE *) malloc(sizeof(EXAMPLE) * n_sample);
    //EXAMPLE  examples[n_sample];

    string id;
    int exampleIndex = 0;
    ifstream reader(file);
    __("PART ", 4);
    if (reader.is_open())
    {
        while (getline(reader, id))
        {
            string unary_path = ssvm_ss::dataset::unary_directory + "/" + id + ".unary";
            string png_path = ssvm_ss::dataset::png_directory + "/" + id + ".png";
            string image_path = ssvm_ss::dataset::jpg_directory + "/" + id + ".jpg";

            QImage png_matrix;
            png_matrix.load(png_path.c_str(), "png");

            examples[exampleIndex].x.height = png_matrix.height();
            examples[exampleIndex].x.width = png_matrix.width();
            examples[exampleIndex].y.height = png_matrix.height();
            examples[exampleIndex].y.width = png_matrix.width();
            __("PART ", 6);

            strcpy(examples[exampleIndex].x.image_path, image_path.c_str());
            strcpy(examples[exampleIndex].x.unary_path, unary_path.c_str());
            __(examples[exampleIndex].x.image_path, 7);
            examples[exampleIndex].y.png_matrix = png_matrix;
            examples[exampleIndex].y.n_label = ssvm_ss::image_constraint::n_label;
            examples[exampleIndex].x.bypass = png_matrix;
            exampleIndex++;

            __("PART ", 5);
        }
    }
    else
    {
        throw invalid_argument("file not found");
    }

    sample.n = n_sample;
    sample.examples = examples;
    if(struct_verbosity>=0)
    printf(" (%d examples) ",sample.n);
    __("END ", 1);
    return (sample);
}
void set_unary_weights(STRUCTMODEL *sm, PATTERN x, double *weights)
{
    size_t windowheight = ssvm_ss::image_constraint::height;
    size_t windowwidth = ssvm_ss::image_constraint::width;
    size_t windowmiddlex = windowwidth / 2;
    size_t windowmiddley = windowheight / 2;
    size_t imagemiddlex = x.width / 2;
    size_t imagemiddley = x.height / 2;
    size_t windowoffsetx = windowmiddlex - imagemiddlex;
    size_t windowoffsety = windowmiddley - imagemiddley;
    size_t windowboundaryx = windowoffsetx + x.width;
    size_t windowboundaryy = windowoffsety + x.height;
    for (int xx = 0; xx < x.width; xx++)
    {
        for (int yy = 0; yy < x.height; yy++)
        {

            weights[xx + (yy * x.width)] = sm->w[(windowoffsetx + xx) + (windowoffsety + yy) * windowwidth];
        }
    }
}

void set_pair_weights(STRUCTMODEL *sm, PATTERN x, double *weights)
{
    size_t windowheight = ssvm_ss::image_constraint::height;
    size_t windowwidth = ssvm_ss::image_constraint::width;
    size_t windowmiddlex = windowwidth / 2;
    size_t windowmiddley = windowheight / 2;
    size_t imagemiddlex = x.width / 2;
    size_t imagemiddley = x.height / 2;
    size_t windowoffsetx = windowmiddlex - imagemiddlex;
    size_t windowoffsety = windowmiddley - imagemiddley;
    size_t windowboundaryx = windowoffsetx + x.width;
    size_t windowboundaryy = windowoffsety + x.height;
    size_t windowoffset = windowheight * windowwidth;
    for (int xx = 0; xx < x.width - 1; xx++)
        for (int yy = 0; yy < x.height - 1; yy++)
            weights[xx + (yy * (x.width - 1))] = sm->w[windowoffset + (windowoffsetx + xx) + (windowoffsety + yy) * windowwidth];
}

void set_color(QImage png_matrix)
{

    QString colorfile = "VOC2010.ct";
    QVector<QRgb> colorTable;
    QFile file(colorfile);
    if (!file.open(QFile::ReadOnly))
        qFatal( "Failed to load '%s'", qPrintable( colorfile ) );
    QDataStream s(&file);
    s >> colorTable;
    file.close();
    png_matrix.setColorTable(colorTable);
}

void infer(PATTERN x, LABEL &y, STRUCTMODEL *sm)
{
    //set target png
    // QImage output_png;


    // y.png_matrix = QImage(x.width,x.height,QImage::Format_Indexed8);
    // QString colorfile = "VOC2010.ct";
    // QVector<QRgb> colorTable;
    // QFile file(colorfile);
    // if (!file.open(QFile::ReadOnly))
    //   qFatal( "Failed to load '%s'", qPrintable( colorfile ) );
    // QDataStream s(&file);
    // s >> colorTable;
    // file.close();
    // y.png_matrix.setColorTable(colorTable);

    // y.height = x.height;
    // y.width = x.width;
    // y.n_label = ssvm_ss::image_constraint::n_label;
    // printf("start infer\n");



    // double * unary_weights = (double*) malloc(sizeof(double)*x.width*x.height);
    // double * pair_weights = (double*) malloc(sizeof(double)*(x.width-1)*(x.height-1));
    // set_unary_weights(sm,x,unary_weights);
    // set_pair_weights(sm,x,pair_weights);

    // ProbImage unary_matrix;
    // unary_matrix.load(x.unary_path);

    // cv::Mat image_matrix;
    // image_matrix = cv::imread(x.image_path, CV_LOAD_IMAGE_COLOR);

    // size_t n_label = y.n_label;
    // printf("label terdeteksi : %d\n",n_label);
    // printf("image_size %d %d\n",image_matrix.cols,image_matrix.rows);

    // lab1231_sun_prj::shotton::annotate(n_label,image_matrix,unary_matrix,unary_weights,pair_weights,y.png_matrix);


    y.png_matrix = x.bypass;


    //set up inferer

}

void        init_struct_model(SAMPLE sample, STRUCTMODEL *sm,
                              STRUCT_LEARN_PARM *sparm, LEARN_PARM *lparm,
                              KERNEL_PARM *kparm)
{
    /* Initialize structmodel sm. The weight vector w does not need to be
       initialized, but you need to provide the maximum size of the
       feature space in sizePsi. This is the maximum number of different
       weights that can be learned. Later, the weight vector w will
       contain the learned weights for the model. */
    sm->sizePsi = ssvm_ss::image_constraint::feature_size; /* replace by appropriate number of features */
}
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

    CONSTSET c;
    long     sizePsi = sm->sizePsi;
    long     i;
    WORD     words[2];
    _("init constraints");

    if (1)  /* normal case: start with empty set of constraints */
    {
        c.lhs = NULL;
        c.rhs = NULL;
        c.m = 0;
    }
    else
    {
        /* add constraints so that all learned weights are
                  positive. WARNING: Currently, they are positive only up to
                  precision epsilon set by -e. */
        c.lhs = (DOC **)my_malloc(sizeof(DOC *) * sizePsi);
        c.rhs = (double *)my_malloc(sizeof(double) * sizePsi);
        for (i = 0; i < sizePsi; i++)
        {
            words[0].wnum = i + 1;
            words[0].weight = 1.0;
            words[1].wnum = 0;
            /* the following slackid is a hack. we will run into problems,
               if we have move than 1000000 slack sets (ie examples) */
            c.lhs[i] = create_example(i, 0, 1000000 + i, 1, create_svector(words, "", 1.0));
            c.rhs[i] = 0.0;
        }
    }
    _("end constraints");
    return (c);
}

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

    /* insert your code for computing the predicted label y here */
    LABEL y;
    infer(x, y, sm);

    return (y);
}





LABEL       find_most_violated_constraint_slackrescaling(PATTERN x, LABEL y,
        STRUCTMODEL *sm,
        STRUCT_LEARN_PARM *sparm)
{
    _("start init slack");
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


    /* insert your code for computing the label ybar here */
    LABEL ybar;
    _("end slack");
    return (ybar);
}

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
    printf("Finding most violated constraint");
    LABEL ybar = classify_struct_example(x, sm, sparm);

    /* insert your code for computing the label ybar here */

    return (ybar);
}

int         empty_label(LABEL y)
{
    /* Returns true, if y is an empty label. An empty label might be
       returned by find_most_violated_constraint_???(x, y, sm) if there
       is no incorrect label that can be found for x, or if it is unable
       to label x at all */
    return (0);
}

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
    _("start init psi\n");
    SVECTOR *fvec = (SVECTOR *) my_malloc(sizeof(SVECTOR));
    fvec->words = (WORD *) my_malloc(sizeof(WORD) * (sm->sizePsi));
    fvec->next = NULL;
    fvec->factor = 1.0;
    fvec->kernel_id = 0;
    // __("PSI",1);
    printf("ukuran sizePsi: %d\n",sm->sizePsi);
    for (size_t indexPsi = 0 ; indexPsi < sm->sizePsi; indexPsi++)
    {
        fvec->words[indexPsi].wnum = indexPsi + 1;
        fvec->words[indexPsi].weight = 0.0;
    }
    fvec->words[sm->sizePsi-1].wnum = 0;
    // fvec->words[sm->sizePsi-1].wnum = 0;
    // __("PSI SO SO",((words+1))->wnum);
    size_t windowheight = ssvm_ss::image_constraint::height;
    size_t windowwidth = ssvm_ss::image_constraint::width;
    size_t windowmiddlex = windowwidth / 2;
    size_t windowmiddley = windowheight / 2;
    size_t imagemiddlex = x.width / 2;
    size_t imagemiddley = x.height / 2;
    size_t windowoffsetx = windowmiddlex - imagemiddlex;
    size_t windowoffsety = windowmiddley - imagemiddley;
    size_t windowboundaryx = windowoffsetx + x.width;
    size_t windowboundaryy = windowoffsety + x.height;
    size_t windowoffset = windowheight * windowwidth;
    //assert(sizeof(words)/sizeof(WORD) == sm->sizePsi );
    // assert(x.height==y.png_matrix.height() && x.width==y.png_matrix.width());
    // __("PSI",3);
    ProbImage unary_matrix;
    unary_matrix.load(x.unary_path);

    cv::Mat image_matrix;
    image_matrix = cv::imread(x.image_path, CV_LOAD_IMAGE_COLOR);

    _(x.image_path);

    for (size_t xx = 0; xx < x.width; xx++)
        for (size_t yy = 0; yy < x.height; yy++)
        {
            assert((windowoffsetx + xx) + (windowoffsety + yy)*windowwidth < sm->sizePsi);
            fvec->words[(windowoffsetx + xx) + (windowoffsety + yy)*windowwidth].weight = unary_matrix(xx, yy, y.png_matrix.pixelIndex(xx, yy));
        }

    // __("PSI",4);
    assert(sm->sizePsi - windowoffset > 0);
    // __("size spsi",sm->sizePsi - windowoffset);
    double *pair_psi = (double *)malloc(sizeof(double) * (sm->sizePsi - windowoffset));
    // __("PSI",7);



    lab1231_sun_prj::shotton::get_2nd_order_psi(image_matrix, unary_matrix, y.png_matrix, pair_psi);
    // __("PSI",8);
    size_t type1psioffset = (x.width - 1) * (x.height - 1);


    for (size_t xx = 0; xx < x.width - 1; xx++)
        for (size_t yy = 0; yy < x.height - 1; yy++)
        {
            fvec->words[windowoffset + (windowoffsetx + xx) + (windowoffsety + yy)*windowwidth].weight = pair_psi[xx + (yy * (x.width - 1))];
            fvec->words[windowoffset + type1psioffset + (windowoffsetx + xx) + (windowoffsety + yy)*windowwidth].weight = pair_psi[type1psioffset + xx + (yy * (x.width - 1))];
        }
    __("PSI", 6);

    //padding

    



    /* insert code for computing the feature vector for x and y here */
    _("end init psi");
    return (fvec);
}

double      loss(LABEL y, LABEL ybar, STRUCT_LEARN_PARM *sparm)
{
    /* loss for correct label y and predicted label ybar. The loss for
       y==ybar has to be zero. sparm->loss_function is set with the -l option. */
    if (sparm->loss_function == 0)  /* type 0 loss: 0/1 loss */
    {
        /* return 0, if y==ybar. return 1 else */
        // LABEL y;
        y.png_matrix.save("temp_output", "png", 0);
        ybar.png_matrix.save("temp_output_bar", "png", 0);
        double sum = 0.0;
        for (int xx = 0; xx < y.width; xx++)
            for (int yy = 0; yy < y.height; yy++)
            {
                sum += (y.png_matrix.pixelIndex(xx, yy) != ybar.png_matrix.pixelIndex(xx, yy)) ? 1.0 : 0.0;
            }
        printf("loss : %f\n", sum);
        return sum;
    }
    else
    {
        /* Put your code for different loss functions here. But then
           find_most_violated_constraint_???(x, y, sm) has to return the
           highest scoring label with the largest loss. */
        printf("Unkown loss function\n");
        exit(1);
    }
}

int         finalize_iteration(double ceps, int cached_constraint,
                               SAMPLE sample, STRUCTMODEL *sm,
                               CONSTSET cset, double *alpha,
                               STRUCT_LEARN_PARM *sparm)
{
    /* This function is called just before the end of each cutting plane iteration. ceps is the amount by which the most violated constraint found in the current iteration was violated. cached_constraint is true if the added constraint was constructed from the cache. If the return value is FALSE, then the algorithm is allowed to terminate. If it is TRUE, the algorithm will keep iterating even if the desired precision sparm->epsilon is already reached. */
    return (0);
}

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
    if (exnum == 0)
    {
        /* this is the first time the function is
                called. So initialize the teststats */
    }
}

void        write_struct_model(char *file, STRUCTMODEL *sm,
                               STRUCT_LEARN_PARM *sparm)
{
    /* Writes structural model sm to file file. */
}

STRUCTMODEL read_struct_model(char *file, STRUCT_LEARN_PARM *sparm)
{
    /* Reads structural model sm from file file. This function is used
       only in the prediction module, not in the learning module. */
}

void        write_label(FILE *fp, LABEL y)
{
    /* Writes label y to file handle fp. */
}

void        free_pattern(PATTERN x)
{
    /* Frees the memory of x. */
}

void        free_label(LABEL y)
{
    /* Frees the memory of y. */
}

void        free_struct_model(STRUCTMODEL sm)
{
    /* Frees the memory of model. */
    /* if(sm.w) free(sm.w); */ /* this is free'd in free_model */
    if (sm.svm_model) free_model(sm.svm_model, 1);
    /* add free calls for user defined data here */
}

void        free_struct_sample(SAMPLE s)
{
    /* Frees the memory of sample s. */
    int i;
    for (i = 0; i < s.n; i++)
    {
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

    for (i = 0; (i < sparm->custom_argc) && ((sparm->custom_argv[i])[0] == '-'); i++)
    {
        switch ((sparm->custom_argv[i])[2])
        {
        case 'a': i++; /* strcpy(learn_parm->alphafile,argv[i]); */ break;
        case 'e': i++; /* sparm->epsilon=atof(sparm->custom_argv[i]); */ break;
        case 'k': i++; /* sparm->newconstretrain=atol(sparm->custom_argv[i]); */ break;
        default: printf("\nUnrecognized option %s!\n\n", sparm->custom_argv[i]);
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

    for (i = 0; (i < sparm->custom_argc) && ((sparm->custom_argv[i])[0] == '-'); i++)
    {
        switch ((sparm->custom_argv[i])[2])
        {
        /* case 'x': i++; strcpy(xvalue,sparm->custom_argv[i]); break; */
        default: printf("\nUnrecognized option %s!\n\n", sparm->custom_argv[i]);
            exit(0);
        }
    }
}

