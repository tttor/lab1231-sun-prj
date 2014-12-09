import cv2
import numpy
import sys
from msrc import color_class_map

def make_list_from_img_file(img_path):
  output = ''
  print 'img_path=', img_path
  img = cv2.imread(img_path)
  r,c,d = img.shape
  for rr in range(r):
    for cc in range(c):       
      px = (img[rr,cc,2], img[rr,cc,1], img[rr,cc,0])
      label_of_px = color_class_map[px]
      output += str(label_of_px)
      output += ',' if cc+1 < c else '\n'
  return output

def main():
  print len(sys.argv)
  assert len(sys.argv)==3, 'INSUFFICIENT NUMBER OF ARGUMENTS'

  target_listpath = sys.argv[1]
  gt_dir = sys.argv[2]
  # output_dir = sys.argv[3]

  with open(target_listpath) as f:
    target_list = f.readlines()
  target_list = [x.strip('\n') for x in target_list]

  data = [ {'gt_img': gt_dir+'/'+target+'.bmp', 'gt_csv': gt_dir+'/'+target+'.csv'} for target in target_list ]

  for data_in in data:
    # print (data_in["gt_img"])
    with open(data_in["gt_csv"], 'w') as the_file:        
        str_buf = make_list_from_img_file(data_in["gt_img"])
        the_file.write(str_buf)

if __name__ == '__main__':
    main()