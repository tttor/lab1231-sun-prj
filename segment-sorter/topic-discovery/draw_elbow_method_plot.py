#!/usr/bin/python
import sys
import numpy as np
from os import listdir
from os.path import isfile, join
import matplotlib
matplotlib.use('pdf')# see http://stackoverflow.com/questions/19518352/tkinter-tclerror-couldnt-connect-to-display-localhost18-0
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

def plot_elbow_method(meandistortions, out_filepath):
    fig = plt.figure()
    K = [i[0] for i in meandistortions]
    plt.plot(K, [i[1] for i in meandistortions], 'bx-')

    plt.xlabel('k')
    plt.ylabel('Average distortion')
    plt.title('Selecting k with the Elbow Method')

    with PdfPages(out_filepath) as pdf:
        pdf.savefig(fig)

def main(argv):
    elbowmethod_dir = argv[1]
    meandistortion_dir = elbowmethod_dir+'/meandistortion/'
    out_dir = elbowmethod_dir

    dist_filenames = [ f for f in listdir(meandistortion_dir) if isfile(join(meandistortion_dir,f)) ]

    meandistortions = []
    for name in dist_filenames:
        k = int(name.split('.')[1])

        dist_filepath = meandistortion_dir+'/'+name
        dist = np.genfromtxt(dist_filepath).tolist()
        meandistortions.append( (k,dist) )

    meandistortions.sort()
    plot_elbow_method(meandistortions,out_dir+'/elbow_method.pdf')
    
if __name__ == '__main__':
    main(sys.argv)
