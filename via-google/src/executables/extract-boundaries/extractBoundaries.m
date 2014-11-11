function extractBoundaries(modelsdirs,srcdirs,tgtdir)
startup
size(modelsdirs,1)
for ll = 1:size(modelsdirs,1)
    load(modelsdirs{ll})
    srcdir = srcdirs{ll};
    listjpg = dir([srcdir '/*.jpg']);
    listpng = dir([srcdir '/*.png']);
    list = [listjpg;listpng];
    n = size(list,1);
    %list file by model
    for ii = 1:n
        
        needtowrite = false;
        
        %prepar
        docNode = com.mathworks.xml.XMLUtils.createDocument('annotation');
        annotation = docNode.getDocumentElement;
        
        %preparing constants
        filename = list(ii).name;
        scale = 1;
        
        fullsrc = strcat(srcdir,'/',filename)
        flnm = docNode.createElement('filename');
        flnm.appendChild(docNode.createTextNode(filename));
        annotation.appendChild(flnm);
        
        fldir = docNode.createElement('filedir');
        fldir.appendChild(docNode.createTextNode(fullsrc));
        annotation.appendChild(fldir);
        
        %test(name, model, 0.7);
        %try
            image = imread(fullsrc);
            im = image;
            [h w d] = size(image);
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
            
            %for jj = 1:m
            %    model = models(jj);
                cls = model.class;
                thresh = model.thresh;
                fprintf(['Processing class ' cls ' on ' fullsrc '\n']);
                
                dirExists = exist([tgtdir cls], 'dir');
                if(~dirExists)
                    cd(tgtdir);
                    mkdir(cls);
                    cd('..');
                end
                
                
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
                
                %make the box out
                %     %boxes = reduceboxes(model, bs(top,:));
                %     x1(del) = [];
                %     x2(del) = [];
                %     y1(del) = [];
                %     y2(del) = [];
                revived = ((boxes-1) .* scale)+1;
                if ~isempty(boxes)
                    %show image
                    %         figure;
                    %         hold on;
                    %         imshow(im);
                    %         pause(1);
                    %         line([x1 x1 x2 x2 x1]', [y1 y2 y2 y1 y1]', 'color', 'b', 'linewidth', 2);
                    
                    %write file
                    x1 = revived(:,1);
                    y1 = revived(:,2);
                    x2 = revived(:,3);
                    y2 = revived(:,4);
                    
                    
                    %             del = find(((x1 == 0) .* (x2 == 0) .* (y1 == 0) .* (y2 == 0)) == 1);
                    %             x1(del) = [];
                    %             x2(del) = [];
                    %             y1(del) = [];
                    %             y2(del) = [];
                    
                    boxcount=size(x1,1);
                    strs = strtok(filename,'.');
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
                            objname = docNode.createElement('name');
                            objname.appendChild(docNode.createTextNode(cls));
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
                            croppedImage = image((ymin+1):(ymax+1),(xmin+1):(xmax+1),:);
                            imwrite(croppedImage,[tgtdir '/' cls '/' strs '.jpg'],'jpg');
                            needtowrite = true;
                        end
                    end
                end
        %catch
        %    fprintf('skipping %s \n',fullsrc);
        %end
        if(needtowrite)
            xmlwrite([tgtdir cls '/' strs '.xml'],docNode);
        end
    end
end

end