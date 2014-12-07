dataset_name = 'voc'
class_list = get_class_of(dataset_name)
training_image_list = get_training_image_list(dataset_name)

prob_map = None

################### ALTERNATIVE 1
for image in training_image_list:
  segment_list = segment(image,good_param)
  for each_class in class_list:
      given_class = each_class
      segment_list_for_given_class = get_segment_list_for(given_class)

      for each_segment in segment_list_for_given_class:
          centroid = get_centroid(each_segment)

          for pixel in image:
              offset = get_offset(centroid, pixel)
              diriclet_offset = apply_diriclet(offset, alpha = 5)
              norm_offset = normalize_by_width_height(diriclet_offset)
              pixel_class = get_pixel_class(pixel)

              weight = get_size(each_segment)
              prob_map[given_class][pixel_class][offset.x][offset.y] = weight * norm_offset
              
################### ALTERNATIVE 2
for image in training_image_list:
  segment_list = segment(image,good_param)
  for segment in segment_list:
    centroid = get_centroid(segment)
    given_class = get_class(centroid)
    weight = get_size(segment)

    for each_class in class_list:
      pixel_list = get_pixel_of(each_class)
      for pixel in pixel_list:
        offset = get_offset(centroid, pixel)
        diriclet_offset = apply_diriclet(offset, alpha = 5)
        norm_offset = normalize_by_width_height(diriclet_offset)

        prob_map[given_class][each_class][offset.x][offset.y] = weight * norm_offset

# 
prob_map = normalized(prob_map)# such that the prob of a given class is summed up 1.0
blurred_prob_map = apply_gaussian_filter(prob_map, variance= 0.10)


#Make consideration about segment_list_for_given_class 

      
