import matplotlib.pyplot as plt
import matplotlib.patches as patches

def write_prob_map_collection():
    knowledge_dir = '/home/tor/xprmnt/knowledge-construction/relative-location-knowledge/prob-map-dirichlet-off.voc.20150315.123826'
    out_dir = '/home/tor/Dropbox/robotics/prj/011/wrt/tor/kc-benchmarker/s2is-usings2is/fig/knowledge/relloc'
    ext = '.pdf.png'

    # obj_class_list = ['chair', 'dog', 'person', 'sofa']
    obj_class_list = ['chair', 'dog', 'person', 'sofa', 'bus', 'car', 'cow']

    # 20-class: sheep causes err
    # obj_class_list = ['aeroplane','bicycle','bird','boat','bottle','bus','car','cat','chair','cow','diningtable','dog','horse','motorbike','person','pottedplant','sofa','train','tvmonitor']

    # 21-class
    # obj_class_list = ['aeroplane','bicycle','bird','boat','bottle','bus','car','cat','chair','cow','diningtable','dog','horse','motorbike','person','pottedplant','sheep','sofa','train','tvmonitor']

    n_obj_class = len(obj_class_list)
    f, axes = plt.subplots(n_obj_class, n_obj_class, sharex='col', sharey='row')
    for i, row_axes in enumerate(axes):
        cprime = obj_class_list[i]
        print 'cprime=', cprime
        for j, ax in enumerate(row_axes):
            c = obj_class_list[j]
            image_filepath = knowledge_dir+'/'+cprime+'/'+c+'_given_'+cprime+ext
            image = plt.imread(image_filepath)
            w,h = image.shape

            im = ax.imshow(image, cmap='binary_r')
            # patch = patches.Circle((h/2, w/2), radius=170, transform=ax.transData)
            # im.set_clip_path(patch)
            ax.axis('off')
            if i==0:
                ax.set_title(c)
            if j==0:
                ax.text(-100, 200, cprime, rotation='vertical', verticalalignment='center', horizontalalignment='center')
    
    # plt.show()
    plt.savefig(out_dir+'/relloc.png',bbox_inches = 'tight')
    plt.close()

if __name__ == '__main__':
    write_prob_map_collection()
