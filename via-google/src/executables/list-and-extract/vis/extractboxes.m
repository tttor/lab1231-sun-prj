function window = extractboxes(im, boxes, out)
% Draw bounding boxes on top of an image.
%   showboxes(im, boxes, out)
%
%   If out is given, a pdf of the image is generated (requires export_fig).

if ~isempty(boxes)
    x1 = boxes(:,1);
    y1 = boxes(:,2);
    x2 = boxes(:,3);
    y2 = boxes(:,4);
    line([x1 x1 x2 x2 x1]', [y1 y2 y2 y1 y1]', 'color', c, 'linewidth', cwidth, 'linestyle', s);
end

% save to pdf
if print
  % requires export_fig from http://www.mathworks.com/matlabcentral/fileexchange/23629-exportfig
  export_fig([out]);
end
