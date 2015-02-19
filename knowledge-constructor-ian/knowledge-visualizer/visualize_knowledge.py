from knowledgeVisualizer import KnowledgeVisualizer

knowledge_dir = '/home/ian-djakman/Documents/data/output/knowledge-compatibility-benchmarker/knowledge'
output_dir = '/home/ian-djakman/Documents/data/output/knowledge-compatibility-benchmarker/knowledge/visualization'

c_knowledge_file = knowledge_dir + '/cooccurence_knowledge.xml'
s_knowledge_file = knowledge_dir + '/spatial_knowledge.xml'
r_knowledge_file = knowledge_dir + '/relativeposition_knowledge.xml'
p_knowledge_file = knowledge_dir + '/scene_properties_knowledge.xml'

def main(): 
    kv = KnowledgeVisualizer(output_dir)
    #kv.visualize_c_knowledge_from(c_knowledge_file)
    #kv.visualize_s_knowledge_from(s_knowledge_file)
    kv.visualize_r_knowledge_from(r_knowledge_file)
    #kv.visualize_p_knowledge_from(p_knowledge_file)

if __name__ == '__main__':
    main()
