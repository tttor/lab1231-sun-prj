import networkx as nx # representasi graf
import pygraphviz as pgv # visualisasi graf, butuh pygraphviz or pydot untuk nx.to_agraph()
import cPickle

scene_prop_filepath = '/home/tor/xprmnt/knowledge-construction/scene-property-knowledge/scene_property.pickle'
out_dir = '/home/tor/Dropbox/robotics/prj/011/wrt/tor/kc-benchmarker/s2is-usings2is/fig/scene-prop'
edge_val_th = 0.1

G = nx.DiGraph()
G.graph['rankdir'] = 'LR'
G.graph['dpi'] = 120

relloc = None
with open(scene_prop_filepath, 'rb') as input_file:
	relloc = cPickle.load(input_file)

for key,val in relloc.iteritems():
	G.add_node(key, color='red', style='filled', fillcolor='pink')

	total = 0.0
	for key2,val2 in val.iteritems():
		total = total + val2
	
	for key2,val2 in val.iteritems():
		label_val = round( (float(val2)/total),2 )
		if label_val > edge_val_th:
			G.add_node(key2, color='blue', style='filled', fillcolor='grey')
			G.add_edge(key,key2,label=label_val) # edge antara X ke Y
		
	A = nx.to_agraph(G)
	A.layout(prog='dot')
	namefile= out_dir+'/'+'gambar_'+key+'.svg'
	A.draw(namefile)	
	G.clear()
