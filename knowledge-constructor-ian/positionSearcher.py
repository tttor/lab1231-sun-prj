class PositionSearcher:
    def compute_rect_area(ob):
        obj_xmin = int(ob[1]) 
        obj_ymin = int(ob[2]) 
        obj_xmax = int(ob[3]) 
        obj_ymax = int(ob[4]) 
        return (obj_xmax - obj_xmin) * (obj_ymax - obj_ymin)
    
    def isInside(ob1, ob2):
        obj1_xmin = int(ob1[1]) 
        obj1_ymin = int(ob1[2]) 
        obj1_xmax = int(ob1[3]) 
        obj1_ymax = int(ob1[4]) 
        obj2_xmin = int(ob2[1]) 
        obj2_ymin = int(ob2[2])
        obj2_xmax = int(ob2[3])
        obj2_ymax = int(ob2[4])
        
        if obj1_xmin > obj2_xmin and obj1_xmax < obj2_xmax and obj1_ymin > obj2_ymin and obj1_ymax < obj2_ymax:
            return True
    
    def isBelow(ob1, ob2, th):
        obj1_xmin = int(ob1[1]) 
        obj1_ymin = int(ob1[2]) 
        obj1_xmax = int(ob1[3]) 
        obj1_ymax = int(ob1[4]) 
        obj2_xmin = int(ob2[1]) 
        obj2_ymin = int(ob2[2])
        obj2_xmax = int(ob2[3])
        obj2_ymax = int(ob2[4])
        
        if obj2_xmin > obj1_xmin and obj2_xmax < obj1_xmax and obj1_ymax > obj2_ymax and obj2_ymin < obj1_ymin < obj2_ymax:
            difference_area = (obj2_ymax - obj1_ymin) * (obj2_xmax - obj2_xmin)
            obj2_area = PositionSearcher.compute_rect_area(ob2)
            #threshold
            if obj2_area != 0:
                if difference_area / obj2_area <= th/100:
                    #print(ob1[0] + " dibawah " + ob2[0] + " , beririsan sedikit, " + ob2[0] + " lebih kecil dari " + ob1[0])
                    return True
        
        elif obj2_xmin < obj1_xmin and obj2_xmax > obj1_xmax and obj1_ymax > obj2_ymax and obj2_ymin < obj1_ymin < obj2_ymax:
            difference_area = (obj2_ymax - obj1_ymin) * (obj1_xmax - obj1_xmin)
            obj_area = min(PositionSearcher.compute_rect_area(ob1) , PositionSearcher.compute_rect_area(ob2))
            #threshold
            if obj_area != 0:
                if difference_area / obj_area <= th/100:
                    #print(ob1[0] + " dibawah " + ob2[0] + " , beririsan sedikit, " + ob1[0] + " lebih kecil dari " + ob2[0])
                    return True
        
        elif obj2_xmin < obj1_xmax < obj2_xmax and obj2_ymin < obj1_ymin < obj2_ymax :
            difference_area = (obj1_xmax - obj2_xmin) * (obj2_ymax - obj1_ymin)
            obj_area = min(PositionSearcher.compute_rect_area(ob1) , PositionSearcher.compute_rect_area(ob2))
            #threshold
            if obj_area != 0:
                if difference_area /obj_area <= th/100:
                    #print(ob2[0] + " ada di atas kanan " + ob1[0])
                    return True
                
        elif obj2_xmin < obj1_xmin < obj2_xmax and obj2_ymin < obj1_ymin < obj2_ymax :
            difference_area = (obj2_xmax - obj1_xmin) * (obj2_ymax - obj1_ymin)
            obj_area = min(PositionSearcher.compute_rect_area(ob1) , PositionSearcher.compute_rect_area(ob2))
            #threshold
            if obj_area != 0:
                if difference_area /obj_area <= th/100:
                    #print(ob2[0] + " ada di atas kiri " + ob1[0])
                    return True
            
        #tidak bersentuhan
        elif obj1_ymin > obj2_ymax:
            #print(ob1[0] + " ada di bawah " + ob2[0] + " ,tidak bersentuhan")
            return True
                
    def isBeside(ob1, ob2):
        obj1_xmin = int(ob1[1]) 
        obj1_ymin = int(ob1[2]) 
        obj1_xmax = int(ob1[3]) 
        obj1_ymax = int(ob1[4]) 
        obj2_xmin = int(ob2[1]) 
        obj2_ymin = int(ob2[2])
        obj2_xmax = int(ob2[3])
        obj2_ymax = int(ob2[4])
        
        if (obj1_xmin > obj2_xmax or obj1_xmax < obj2_xmin) and ((obj1_ymin < obj2_ymin and obj1_ymax < obj2_ymax) or (obj1_ymin > obj2_ymin and obj1_ymax > obj2_ymax) or (obj1_ymin < obj2_ymin and obj1_ymax > obj2_ymax) or (obj1_ymin > obj2_ymin and obj1_ymax < obj2_ymax)):
            return True
        
    def isAround(ob1, ob2, th):
        obj1_xmin = int(ob1[1]) 
        obj1_ymin = int(ob1[2]) 
        obj1_xmax = int(ob1[3]) 
        obj1_ymax = int(ob1[4]) 
        obj2_xmin = int(ob2[1]) 
        obj2_ymin = int(ob2[2])
        obj2_xmax = int(ob2[3])
        obj2_ymax = int(ob2[4])

        #intersect around top
        if obj1_xmin > obj2_xmin and obj1_xmax < obj2_xmax and obj1_ymin < obj2_ymin and obj2_ymin < obj1_ymax < obj2_ymax:
            difference_area = (obj1_ymax - obj2_ymin) * (obj1_xmax - obj1_xmin)
            obj1_area = PositionSearcher.compute_rect_area(ob1)
            #threshold
            if difference_area / obj1_area > th/100:
                #print("intersect diatas")
                return True
        
        #intersect around bottom
        elif obj1_xmin > obj2_xmin and obj1_xmax < obj2_xmax and obj1_ymax > obj2_ymax and obj2_ymin < obj1_ymin < obj2_ymax:
            difference_area = (obj2_ymax - obj1_ymin) * (obj1_xmax - obj1_xmin)
            obj1_area = PositionSearcher.compute_rect_area(ob1)
            #threshold
            if difference_area / obj1_area > th/100:
                #print("intersect dibawah")
                return True
                
        #intersect around left
        elif obj1_ymin > obj2_ymin and obj1_ymax < obj2_ymax and obj1_xmin < obj2_xmin and obj2_xmin < obj1_xmax < obj2_xmax:
            difference_area = (obj1_xmax - obj2_xmin) * (obj1_ymax - obj1_ymin)
            obj1_area = PositionSearcher.compute_rect_area(ob1)
            #threshold
            if difference_area / obj1_area > th/100:
                #print("intersect dikiri")
                return True
                
        #intersect around right
        elif obj1_ymin > obj2_ymin and obj1_ymax < obj2_ymax and obj1_xmax > obj2_xmax and obj2_xmin < obj1_xmin < obj2_xmax:
            difference_area = (obj2_xmax - obj1_xmin) * (obj1_ymax - obj1_ymin)
            obj1_area = PositionSearcher.compute_rect_area(ob1)
            #threshold
            if difference_area / obj1_area > th/100:
                #print("intersect dikanan")
                return True
                
        #intersect bigger top
        elif obj1_xmin < obj2_xmin and obj1_xmax > obj2_xmax and obj1_ymin < obj2_ymin and obj2_ymin < obj1_ymax < obj2_ymax:
            difference_area = (obj1_ymax - obj2_ymin) * (obj2_xmax - obj2_xmin)
            obj2_area = PositionSearcher.compute_rect_area(ob2)
            #threshold
            #print("siap-siap diatas gede")
            if difference_area / obj2_area > th/100:
                return True
            
        #intersect bigger bottom
        elif obj1_xmin < obj2_xmin and obj1_xmax > obj2_xmax and obj1_ymax > obj2_ymax and obj2_ymin < obj1_ymin < obj2_ymax:
            difference_area = (obj2_ymax - obj1_ymin) * (obj2_xmax - obj2_xmin)
            obj2_area = PositionSearcher.compute_rect_area(ob2)
            #threshold
            #print("siap-siap dibawah gede")
            if difference_area / obj2_area > th/100:
                return True
                
        #intersect bigger left
        elif obj1_ymin < obj2_ymin and obj1_ymax > obj2_ymax and obj1_xmin < obj2_xmin and obj2_xmin < obj1_xmax < obj2_xmax:
            difference_area = (obj1_xmax - obj2_xmin) * (obj2_ymax - obj2_ymin)
            obj2_area = PositionSearcher.compute_rect_area(ob2)
            #threshold
            #print("siap-siap dikiri gede")
            if difference_area / obj2_area > th/100:
                return True
        
        #intersect bigger right
        elif obj1_ymin < obj2_ymin and obj1_ymax > obj2_ymax and obj1_xmax > obj2_xmax and obj2_xmin < obj1_xmin < obj2_xmax:
            difference_area = (obj2_xmax - obj1_xmin) * (obj2_ymax - obj2_ymin)
            obj2_area = PositionSearcher.compute_rect_area(ob2)
            #threshold
            #print("siap-siap dikanan gede")
            if difference_area / obj2_area > th/100:
                return True
                
        #intersect around topleft
        elif obj2_xmin < obj1_xmax < obj2_xmax and obj2_ymin < obj1_ymax < obj2_ymax :
            difference_area = (obj1_xmax - obj2_xmin) * (obj1_ymax - obj2_ymin)
            obj_area = min(PositionSearcher.compute_rect_area(ob1) , PositionSearcher.compute_rect_area(ob2))
            #threshold
            if difference_area /obj_area > th/100:
                #print("pojok kiri atas")
                return True
                
        #intersect around topright
        elif obj2_xmin < obj1_xmin < obj2_xmax and obj2_ymin < obj1_ymax < obj2_ymax :
            difference_area = (obj2_xmax - obj1_xmin) * (obj1_ymax - obj2_ymin)
            obj_area = min(PositionSearcher.compute_rect_area(ob1) , PositionSearcher.compute_rect_area(ob2))
            #threshold
            if difference_area /obj_area > th/100:
                #print("pojok kanan atas")
                return True
                
        #around bottomleft
        elif obj2_xmin < obj1_xmax < obj2_xmax and obj2_ymin < obj1_ymin < obj2_ymax :
            difference_area = (obj1_xmax - obj2_xmin) * (obj2_ymax - obj1_ymin)
            obj_area = min(PositionSearcher.compute_rect_area(ob1) , PositionSearcher.compute_rect_area(ob2))
            #threshold
            if difference_area /obj_area > th/100:
                #print("pojok kiri bawah")
                return True
                
        #around bottomright
        elif obj2_xmin < obj1_xmax < obj2_xmax and obj2_ymin < obj1_ymin < obj2_ymax :
            difference_area = (obj2_xmax - obj1_xmin) * (obj2_ymax - obj1_ymin)
            obj_area = min(PositionSearcher.compute_rect_area(ob1) , PositionSearcher.compute_rect_area(ob2))
            #threshold
            if difference_area /obj_area > th/100:
                #print("pojok kanan bawah")
                return True
                
        #crossing horizontally
        elif obj1_xmin < obj2_xmin and obj1_xmax > obj2_xmax and obj1_ymin > obj2_ymin and obj1_ymax < obj2_ymax:
            difference_area = (obj2_xmax - obj2_xmin) * (obj1_ymax - obj1_ymin)
            obj_area = min(PositionSearcher.compute_rect_area(ob1) , PositionSearcher.compute_rect_area(ob2))
            #threshold
            if difference_area /obj_area > th/100:
                #print("nyamping miring")
                return True
                
        #crossing vertically
        elif obj1_xmin > obj2_xmin and obj1_xmax < obj2_xmax and obj1_ymin < obj2_ymin and obj1_ymax > obj2_ymax:
            difference_area = (obj1_xmax - obj1_xmin) * (obj2_ymax - obj2_ymin)
            obj_area = min(PositionSearcher.compute_rect_area(ob1) , PositionSearcher.compute_rect_area(ob2))
            #threshold
            if difference_area /obj_area > th/100:
                #print("nyamping tegak")
                return True
            
