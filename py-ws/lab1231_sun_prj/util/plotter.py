import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

out_dir_path = '/home/tor/sun2/wrt/draft/fig/'

def plot_cm(perf_metrics, clfs):
    n_clf = perf_metrics.shape[0]
    n_sh = perf_metrics.shape[1]
    n_class = np.shape(perf_metrics[0,0]['cm'])[0]
    
    # Arrange the averaged metrics: cm_means
    cm_means = []# the values is averaged over data clones from all class types
    for i in range(n_clf):
        cm_metrics_1D = []# from_one_class from all data clones
        for j in range(n_sh):
            cm_metrics_1D.append(perf_metrics[i,j]['cm'])
        
        cm_mean = np.zeros((n_class, n_class)) # mean over datasets from _one_ classfier type
        for m in range(n_class):
            for n in range(n_class):
                mn_value_list = [cm[m,n] for cm in cm_metrics_1D]
                cm_mean[m,n] = np.mean(mn_value_list)
        cm_means.append(cm_mean)
    
    # Set fig properties
    figs = []
    for i, cm in enumerate(cm_means):
        # Normalize the cm to be in the range of [0.0, 1.0]
        norm_cm = []
        for j in cm:
            n_sample_per_class = sum(j)
            cm_per_class = []
            for k in j:
                cm_per_class.append(float(k)/float(n_sample_per_class))
            norm_cm.append(cm_per_class)
        cm = norm_cm    
        
        #
        fig = plt.figure()
        ax = fig.add_subplot(1, 1, i+1)
        ax.set_aspect(1)
        res = ax.imshow(np.array(cm), cmap=plt.cm.binary, interpolation='nearest')
        res.set_clim(0,1) # set the limits of the color map
        
        for x in xrange(n_class):
            for y in xrange(n_class):
                textcolor = 'black'
                if cm[x][y] > 0.5:
                    textcolor = 'white'
                ax.annotate("%0.2f"%cm[x][y], xy=(y, x),  horizontalalignment='center', verticalalignment='center', color=textcolor)
                
        alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        plt.xticks(range(len(cm)), alphabet[:len(cm)])
        plt.yticks(range(len(cm[0])), alphabet[:len(cm[0])])
        plt.xlabel('PREDICTED')
        plt.ylabel('GROUND TRUTH')
        plt.title(clfs[i])
        figs.append(fig)
    
    # Save
    with PdfPages(out_dir_path + 'cm.pdf') as pdf:
        for fig in figs:
            pdf.savefig(fig) 

def plot_1(perf_metrics, clfs):
    '''Plot accuracy, precision, and recall in a bargraph.
    "mean" and "std" here refer to the one that is over datasets.
    '''
    n_clf = perf_metrics.shape[0]
    n_sh = perf_metrics.shape[1]
    
    # Arrange the metrics
    accuracy_metrics = np.zeros((n_clf, n_sh))
    for i in range(n_clf):
        for j in range(n_sh):
            accuracy_metrics[i,j] = perf_metrics[i,j]['accuracy']
            
    accuracy_means = np.mean(accuracy_metrics, axis=1)
    accuracy_stds = np.std(accuracy_metrics, axis=1)
    
    precision_metrics = np.zeros((n_clf, n_sh))
    for i in range(n_clf):
        for j in range(n_sh):
            precision_metrics[i,j] = perf_metrics[i,j]['precision']
            
    precision_means = np.mean(precision_metrics, axis=1)
    precision_stds = np.std(precision_metrics, axis=1)
    
    recall_metrics = np.zeros((n_clf, n_sh))
    for i in range(n_clf):
        for j in range(n_sh):
            recall_metrics[i,j] = perf_metrics[i,j]['recall']
            
    recall_means = np.mean(recall_metrics, axis=1)
    recall_stds = np.std(recall_metrics, axis=1)

    # Set bar graph properties
    ind = np.arange(n_clf)  # the x locations for the groups
    width = 0.25       # the width of the bars

    fig, ax = plt.subplots()
    rects1 = ax.bar(ind, accuracy_means, width, color='r', yerr=accuracy_stds)
    rects2 = ax.bar(ind+width, precision_means, width, color='y', yerr=precision_stds)
    rects3 = ax.bar(ind+width*2, recall_means, width, color='g', yerr=recall_stds)

    ax.set_ylabel('Scores')
    ax.set_title('Performance comparison among classfiers')
    ax.set_xticks(ind+(width*3/2))
    ax.set_xticklabels(clfs)

    ax.legend( (rects1[0], rects2[0], rects3[0]), ('accuracy', 'precision', 'recall'), loc='upper left' )
    
    # Save
    with PdfPages(out_dir_path + 'accuracy_precision_recall.pdf') as pdf:
        pdf.savefig(fig) 

def plot_2(perf_metrics, clfs):
    '''Plot fbeta_score and hamming_loss in a bargraph.
    "mean" and "std" here refer to the one that is over datasets.
    '''
    n_clf = perf_metrics.shape[0]
    n_sh = perf_metrics.shape[1]
    
    # Arrange the metrics
    fbeta_score_metrics = np.zeros((n_clf, n_sh))
    for i in range(n_clf):
        for j in range(n_sh):
            fbeta_score_metrics[i,j] = perf_metrics[i,j]['fbeta_score']
            
    fbeta_score_means = np.mean(fbeta_score_metrics, axis=1)
    fbeta_score_stds = np.std(fbeta_score_metrics, axis=1)
    
    hamming_loss_metrics = np.zeros((n_clf, n_sh))
    for i in range(n_clf):
        for j in range(n_sh):
            hamming_loss_metrics[i,j] = perf_metrics[i,j]['hamming_loss']
            
    hamming_loss_means = np.mean(hamming_loss_metrics, axis=1)
    hamming_loss_stds = np.std(hamming_loss_metrics, axis=1)
    
    # Set bar graph properties
    ind = np.arange(n_clf)  # the x locations for the groups
    width = 0.25       # the width of the bars

    fig, ax = plt.subplots()
    rects1 = ax.bar(ind, fbeta_score_means, width, color='r', yerr=fbeta_score_stds)
    rects2 = ax.bar(ind+width, hamming_loss_means, width, color='y', yerr=hamming_loss_stds)
    
    ax.set_ylabel('Scores, Loss')
    ax.set_title('Performance comparison among classfiers')
    ax.set_xticks(ind+width)
    ax.set_xticklabels(clfs)

    ax.legend( (rects1[0], rects2[0]), ('fbeta score', 'hamming loss'), loc='upper left' )
    
    # Save
    with PdfPages(out_dir_path + 'fbeta_score_hamming_loss.pdf') as pdf:
        pdf.savefig(fig) 
