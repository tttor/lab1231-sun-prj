source ../config
echo "Trainer path: $train_bin"
echo "VOC training folder: $voc_img_folder"
echo "VOC PNG folder: $voc_png_folder"
echo "VOC train list: $voc_train_list"
echo "Normal Param: $normal_param"
echo "VOC Trainer size: $voc_train_size"
echo "Experiment folder: $current_exp"
echo "Init train list: $init_train_path"
echo "##################################"

`head -n $voc_train_size $voc_train_list > $init_train_path`

#create google 
echo "START TRAINING INITIAL MODEL"
`$train_bin $init_train_path  $voc_img_folder $voc_png_folder $normal_param`