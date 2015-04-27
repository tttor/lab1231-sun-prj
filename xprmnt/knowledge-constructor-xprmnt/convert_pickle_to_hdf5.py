'''
Convert the storage of relative_location_knowledge from the pickle format to the hdf5 format
'''
import numpy as np
from pandas import HDFStore,DataFrame,Series

import relative_location_knowledge as kr

def main():
    knowledge_dir = '/home/tor/xprmnt/knowledge-construction'

    # read
    relloc_knowledge = kr.read(knowledge_dir+'/relative-location-knowledge/relloc-pickle')

    # write
    relloc_knowledge_hdf5 = HDFStore(knowledge_dir+'/relative-location-knowledge/relloc-hdf5/relloc.h5')

    for key, local in relloc_knowledge.iteritems():        
        for key2 in local:
            df = DataFrame(local[key2])
            df_id = key+'/'+key2
            print 'writing:', df_id
            relloc_knowledge_hdf5.put(df_id, df)

    print 'writing: obj_class'
    obj_class = Series(relloc_knowledge.keys())
    relloc_knowledge_hdf5.put('obj_class', obj_class)

if __name__ == '__main__':
    main()
