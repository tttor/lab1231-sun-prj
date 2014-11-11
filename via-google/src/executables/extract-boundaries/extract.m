function extract()
startup
file = fopen('compilation');
C = textscan(file,'%d %s %s %s %s %s','Delimiter',',');
ids = C{1};
classnames = C{2};
modeldirs = C{3};
srcdirs = C{4};
tgtimgdirs = C{5};
tgtxmldirs = C{6};

for ll = 1:size(ids,1)
    load(char(modeldirs{ll}));
    class = char(classnames{ll});
    id = ids(ll);
    fullsrc = char(srcdirs{ll});
    tgtimgdir = char(tgtimgdirs{ll});
    tgtxmldir = char(tgtxmldirs{ll});
    
    dirExists = exist(tgtimgdir, 'dir');
    if(~dirExists)
        fprintf('making directory %s',tgtimgdir);
        mkdir(tgtimgdir);
    end
    
    
    dirExists = exist(tgtxmldir, 'dir');
    if(~dirExists)
        fprintf('making directory %s',tgtxmldir);
        mkdir(tgtxmldir);
    end
                
    [pathstr,filename,ext] = fileparts(fullsrc);
    needtowrite = false;
    
    %prepar
    docNode = com.mathworks.xml.XMLUtils.createDocument('annotation');
    annotation = docNode.getDocumentElement;
    
    
    fldir = docNode.createElement('filedir');
    fldir.appendChild(docNode.createTextNode(fullsrc));
    annotation.appendChild(fldir);
    
    %test(name, model, 0.7);
    try
    image = imread(fullsrc);
    im = image;
    [h w d] = size(image);
    scale = 1;
    %normalizing windows
    if(w>h)
        if(w >1024)
            im = imresize(image,[NaN 1024]);
            scale = w/1024;
        end
    else
        if(h >1024)
            im = imresize(image,[1024 NaN]);
            scale = h/1024;
        end
    end
    
    thresh = model.thresh;
    fprintf(['Processing class ' class ' on ' fullsrc '\n']);
    
    % detect objects
    [ds, bs] = imgdetect(im, model, thresh);
    top = nms(ds, 0.5);
    clf;
    if model.type == model_types.Grammar
        bs = [ds(:,1:4) bs];
    end
    
    bbox = bboxpred_get(model.bboxpred, ds, reduceboxes(model, bs));
    bbox = clipboxes(im, bbox);
    top = nms(bbox, 0.5);
    boxes = bbox(top,:);

    revived = ((boxes-1) .* scale)+1;
    if ~isempty(boxes)
 
        %write file
        x1 = revived(:,1);
        y1 = revived(:,2);
        x2 = revived(:,3);
        y2 = revived(:,4);
        
        
  
        
        boxcount=size(x1,1);
        if(boxcount>0)
            xmax = max(x1(1,1),x2(1,1));
            xmin = min(x1(1,1),x2(1,1));
            ymax = max(y1(1,1),y2(1,1));
            ymin = min(y1(1,1),y2(1,1));
            if (xmin >= 0 && xmin <= w-1 && xmax >= 0 && xmax <= w-1 && ymin >= 0 && ymin <= h-1 && ymax >= 0 && ymax <= h-1)
                xmin = max(xmin,0);
                xmax = min(xmax,w-1);
                ymin = max(ymin,0);
                ymax = min(ymax,h-1);
                obj = docNode.createElement('object');
                objid = docNode.createElement('id');
                objid.appendChild(docNode.createTextNode(num2str(id)));
                obj.appendChild(objid);
                objname = docNode.createElement('name');
                objname.appendChild(docNode.createTextNode(class));
                obj.appendChild(objname);
                objbnd = docNode.createElement('bndbox');
                objxmin = docNode.createElement('xmin');
                objxmin.appendChild(docNode.createTextNode(num2str(xmin)));
                objxmax = docNode.createElement('xmax');
                objxmax.appendChild(docNode.createTextNode(num2str(xmax)));
                objymin = docNode.createElement('ymin');
                objymin.appendChild(docNode.createTextNode(num2str(ymin)));
                objymax = docNode.createElement('ymax');
                objymax.appendChild(docNode.createTextNode(num2str(ymax)));
                objbnd.appendChild(objxmin);
                objbnd.appendChild(objxmax);
                objbnd.appendChild(objymin);
                objbnd.appendChild(objymax);
                obj.appendChild(objbnd);
                annotation.appendChild(obj);
                croppedImage = image(round(ymin+1):round(ymax+1),round(xmin+1):round(xmax+1),:);
                imwrite(croppedImage,[tgtimgdir '/' filename '.jpg'],'jpg');
                needtowrite = true;
            end
        end
    end
    if(needtowrite)
        xmlwrite([tgtxmldir '/' filename '.xml'],docNode);
    end
    catch
    fprintf('Skipping a file \n');
    end
end
