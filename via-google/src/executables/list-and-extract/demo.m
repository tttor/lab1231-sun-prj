function demo()
startup;

fprintf('compiling the code...');
compile;
fprintf('done.\n\n');

% load('VOC2007/car_final');
% model.vis = @() visualizemodel(model, ...
%                   1:2:length(model.rules{model.start}));
% test('000034.jpg', model, -0.3);
% 
% load('INRIA/inriaperson_final');
% model.vis = @() visualizemodel(model, ...
%                   1:2:length(model.rules{model.start}));
% test('000061.jpg', model, -0.3);
% 
% load('VOC2007/person_grammar_final');
% model.class = 'person grammar';
% model.vis = @() visualize_person_grammar_model(model, 6);
% test('000061.jpg', model, -0.6);


addpath('images/');
load('VOC2007/bicycle_final');
model.vis = @() visualizemodel(model, ...
                  1:2:length(model.rules{model.start}));
              
% list = dir('/home/rizkivmaster/FINAL MISSION/CODE FINAL/grabber/bicycle-1411399391/');
% n = size(list);
% for ii = 7:n
%     name = strcat('/home/rizkivmaster/FINAL MISSION/CODE FINAL/grabber/bicycle-1411399391/',list(ii).name);    
%     test(name, model, -0.3);
% end
test('0009.jpg', model, -0.3);
% test('0046.jpg', model, -0.3);
% test('0071.png', model, -0.3);
% test('0092.jpg', model, -0.3);
% test('0103.png', model, -0.3);
% test('0106.JPG', model, -0.3);

function test(imname, model, thresh)
cls = model.class;
fprintf('///// Running demo for %s /////\n\n', cls);

% load and display image
im = imread(imname);
[h w d] = size(im);
if(w>h)
    if(w >1024)
        im = imresize(im,[NaN 1024]);
    end
else
    if(h >1024)
        im = imresize(im,[1024 NaN]);
    end
end
clf;
image(im);
axis equal; 
axis on;
 disp('input image');
 disp('press any key to continue'); 
pause(1);
 disp('continuing...');

% load and display model
model.vis();
 disp([cls ' model visualization']);
 disp('press any key to continue'); 
pause(1);
 disp('continuing...');

% detect objects
[ds, bs] = imgdetect(im, model, thresh);
top = nms(ds, 0.5);
clf;
if model.type == model_types.Grammar
  bs = [ds(:,1:4) bs];
end
reduceboxes(model, bs(top,:))
showboxes(im, reduceboxes(model, bs(top,:)));
 disp('detections');
 disp('press any key to continue'); 
pause(1);
 disp('continuing...');

if model.type == model_types.MixStar
  % get bounding boxes
  bbox = bboxpred_get(model.bboxpred, ds, reduceboxes(model, bs));
  bbox = clipboxes(im, bbox);
  top = nms(bbox, 0.5);
  clf;
  showboxes(im, bbox(top,:));
   disp('bounding boxes');
   disp('press any key to continue'); 
  pause(1);
end

fprintf('\n');
