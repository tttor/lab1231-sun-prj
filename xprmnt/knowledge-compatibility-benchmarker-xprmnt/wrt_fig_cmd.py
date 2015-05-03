#!/usr/bin/python

# list_filepath = '/home/tor/Dropbox/robotics/prj/011/wrt/tor/kc-benchmarker/s2is-usings2is/fig/regression/qual-result/oneobjgt.list'
list_filepath = '/home/tor/Dropbox/robotics/prj/011/wrt/tor/kc-benchmarker/s2is-usings2is/fig/regression/qual-result/twoobjgt.list'
# list_filepath = '/home/tor/Dropbox/robotics/prj/011/wrt/tor/kc-benchmarker/s2is-usings2is/fig/regression/qual-result/threeobjgt.list'

#read list
img_filepath_list = []
with open(list_filepath) as f:
    img_filepath_list = f.readlines()
img_filepath_list = [x.strip('\n') for x in img_filepath_list]
print len(img_filepath_list)

#read the data
for img_filepath in img_filepath_list:
    data_filepath =  img_filepath[0:-15]+'.data'
    print data_filepath

    data = []
    with open(data_filepath) as f:
        data = f.readlines()
    data = [x.strip('\n') for x in data]

    ca = round(float(data[0]),3)
    score = round(float(data[1]),3) 
    d = round(float(data[2]),3)

    #write the data in latex format
    cmd = '\\subfigure[score = '+str(score)+', averaged-CA = '+str(ca)+', $\Delta = '+str(d)+'$] {\includegraphics[scale=.45]{'+img_filepath+'}}'
    out_filepath = list_filepath+'.texcmd'
    
    with open(out_filepath,'a') as f:
        f.write(cmd)
        f.write('\n')

