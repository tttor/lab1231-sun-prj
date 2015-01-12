import os
import networkx as nx
import knowledge_loader
import numpy as np
import matplotlib.pyplot as plt
import voc_obj_intolbl
import voc_obj_lbltoin

indexToNameDict=voc_obj_intolbl.LABELS
nameToIndexDict=voc_obj_lbltoin.LABELS

class KnowledgeVisualizer:
    
    def __init__(self, output_dir):
        self.out = output_dir
        self.class_amount =  len(indexToNameDict)
        
    def visualize_c_knowledge_from(self, knowledge_directory):
        c_knowledge = knowledge_loader.load_cooccurence_knowledge(knowledge_directory)
        shape = (self.class_amount, self.class_amount)
        confussion_arr = np.zeros(shape)
        for i in range (1, self.class_amount + 1):
            for j in range (1, self.class_amount + 1):
                confussion_arr[(i-1)][(j-1)] = round(c_knowledge[indexToNameDict[str(i)]][indexToNameDict[str(j)]], 2) 
        
        '''label for VOC dataset with 20 labels, otherwise comment it

        classlist = ""
        for i in range (1, 11):
            classlist = classlist + indexToNameDict[str(i)]  + ","
        classlist = classlist + "\n"
        for i in range (11, 20):
            classlist = classlist + indexToNameDict[str(i)]  + ","
        classlist = classlist + indexToNameDict[str(20)]
        '''
        
        #plotting
        fig = plt.figure()
        plt.clf()
        ax = fig.add_subplot(111)
        ax.set_aspect(1)
        #ax.set_xlabel("From A to T : " + classlist)
        res = ax.imshow(np.array(confussion_arr), cmap=plt.cm.jet, interpolation='nearest')

        width = len(confussion_arr)
        height = len(confussion_arr[0])

        for x in xrange(width):
            for y in xrange(height):
                ax.annotate(str(confussion_arr[x][y]), xy=(y, x), size = 7, horizontalalignment='center', verticalalignment='center')

        cb = fig.colorbar(res)
        alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        plt.xticks(range(width), alphabet[:width])
        plt.yticks(range(height), alphabet[:height])

        if not os.path.exists(self.out + '/cooccurence_knowledge/'):
            os.makedirs(self.out + '/cooccurence_knowledge/')
            
        plt.savefig(self.out + '/cooccurence_knowledge/cooccurence_knowledge.png')
        plt.close()




    def visualize_s_knowledge_from(self, knowledge_directory):
        '''
        for each class, find the value of each amount on particular spatial position, then plot it with bar
        '''
        print("visualizing spatial layout knowledge . . .")
        s_knowledge = knowledge_loader.load_spatial_knowledge(knowledge_directory)
        for i in range (1, self.class_amount+1):
                classname = indexToNameDict[str(i)]

                top_val = float(s_knowledge[classname]['top'])
                cen_val = float(s_knowledge[classname]['center'])
                btm_val = float(s_knowledge[classname]['bottom'])

                n_groups = 3
                means = (top_val, cen_val, btm_val)
                fig, ax = plt.subplots()
                index = np.arange(n_groups)
                opacity = 0.6

                rects = plt.bar(index, means, width=1.0, alpha = opacity, color = 'b')
                
                plt.xlabel('Position', fontsize = 16)
                plt.ylabel('Area Amount', fontsize = 16)
                plt.ylim(plt.ylim()[0], 1.0)
                plt.title('Spatial layout for class ' + classname, fontsize = 20)
                plt.xticks(index + bar_width, ('top','center','bottom'), fontsize = 16)
                plt.tight_layout()
                
                if not os.path.exists(self.out + '/spatial_knowledge/'):
                    os.makedirs(self.out + '/spatial_knowledge/')

                plt.savefig(self.out + '/spatial_knowledge/' +classname + '.png')
                plt.close()




    def visualize_r_knowledge_from(self, knowledge_directory):
        print("visualizing relative knowledge . . .")
        
        r_knowledge = knowledge_loader.load_relativeposition_knowledge(knowledge_directory)
        rpos_list = ['is_below','is_beside','is_around']
        for i in range (1, self.class_amount+1):
            classname1 = indexToNameDict[str(i)]
            # There are 3 position to be computed
            for pos in range (0, len(rpos_list)):
                position = rpos_list[pos]
                probabilities=[]
                for j in range (1, self.class_amount+1):
                    classname2 = indexToNameDict[str(j)]
                    probabilities.append(r_knowledge[classname1][position][classname2])
                n_groups = 20
                fig, ax = plt.subplots()
                index = np.arange(n_groups)
                bar_width=1
                opacity = 0.6
                rects = plt.bar(index, probabilities, bar_width, alpha=opacity)
                rects[0].set_color('#800000')
                rects[1].set_color('#008000')
                rects[2].set_color('#808000')
                rects[3].set_color('#000080')
                rects[4].set_color('#800080')
                rects[5].set_color('#008080')
                rects[6].set_color('#808080')
                rects[7].set_color('#400080')
                rects[8].set_color('#C00000')
                rects[9].set_color('#408000')
                rects[10].set_color('#C08000')
                rects[11].set_color('#400000')
                rects[12].set_color('#C00080')
                rects[13].set_color('#408080')
                rects[14].set_color('#C08080')
                rects[15].set_color('#004000')
                rects[16].set_color('#804000')
                rects[17].set_color('#00C000')
                rects[18].set_color('#80C000')
                rects[19].set_color('#004080')
                plt.ylabel('Probability', fontsize = 16)
                plt.ylim(plt.ylim()[0], 1.0)
                plt.title('Class ' + classname1  + ' ' + position,fontsize = 20)
                plt.xticks((index-0.5) + bar_width, ('Aer','Bic','Bir','Boa','Bot','Bus','Car','Dog','Cha','Cow','Din','Cat','Hor','Mot','Per','Pot','She','Sof','Tra','Tvm'), fontsize = 13)
                plt.tight_layout()
                
                if not os.path.exists(self.out + '/relative_knowledge/'):
                    os.makedirs(self.out + '/relative_knowledge/')

                plt.savefig(self.out + '/relative_knowledge/' +classname1 + ',' +position +'.png')
                plt.close()



    def visualize_p_knowledge_from(self, knowledge_directory):
        '''
        for each place, find what objects contained on those particular place, plot the contained objects along with its value on graph
        '''
        print("visualizing scene properties knowledge . . .")
        
        p_knowledge = knowledge_loader.load_sceneproperty_knowledge(knowledge_directory)
        placelist = p_knowledge.keys()
        for p in range (0, len(placelist)):
            place = placelist[p]
            containedclass = []
            for cn in range (1, self.class_amount + 1):
                classname = indexToNameDict[str(cn)]
                if (p_knowledge[place][classname]) != 0:
                    containedclass.append(classname)
        
            graph = []
            labels = []
            for i in range (0, len(containedclass)):
                graph.append((place,containedclass[i]))
                string_for_label = str(round((p_knowledge[place][containedclass[i]]), 2))
                labels.append(string_for_label)

            graph_layout='spectral'
            #node_size=2600
            node_size=4800
            node_color='blue'
            node_alpha=0.3
            node_text_size=13
            edge_color='blue'
            edge_alpha=0.3
            edge_tickness=1
            edge_text_pos=0.3
            text_font='sans-serif'
            
            # create networkx graph
            G=nx.DiGraph()
            
            # add edges
            for edge in graph:
                G.add_edge(edge[0], edge[1])
                
            graph_pos=nx.shell_layout(G)
            
            # draw graph
            nx.draw_networkx_nodes(G,graph_pos,node_size=node_size, 
                                   alpha=node_alpha, node_color=node_color)
            nx.draw_networkx_edges(G,graph_pos,width=edge_tickness, length = 20,
                                   alpha=edge_alpha,edge_color=edge_color)
            nx.draw_networkx_labels(G, graph_pos,font_size=node_text_size,
                                    font_family=text_font)
            
            edge_labels = dict(zip(graph, labels))
            nx.draw_networkx_edge_labels(G, graph_pos, edge_labels=edge_labels, label_pos=edge_text_pos, font_size=13)
            
            # show graph
            if not os.path.exists(self.out + '/scene_properties_knowledge/'):
                os.makedirs(self.out + '/scene_properties_knowledge/')

            plt.axis('off')
            plt.title('Scene Properties of ' + place)
            plt.savefig(self.out + '/scene_properties_knowledge/' +place + '.png')
            plt.close()
