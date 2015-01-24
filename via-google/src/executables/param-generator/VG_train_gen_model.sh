source ../config
echo "Trainer path: $train_bin"

init_train_path="/home/rizkivmaster/ws/datasets/smallVOC2010/split/vocgooglesvmTrain.txt"
voc_img_folder="/home/rizkivmaster/ws/datasets/smallVOC2010/JPEGImages"
voc_png_folder="/home/rizkivmaster/ws/datasets/smallVOC2010/SegmentationClass"
normal_param="/home/rizkivmaster/ws/results/VG/model"
test_list="/home/rizkivmaster/ws/datasets/smallVOC2010/split/Test.txt"
test_unaries="/home/rizkivmaster/ws/results/VG/unaries"

#create google 
#echo "START TRAINING INITIAL MODEL"
#`$train_bin $init_train_path  $voc_img_folder $voc_png_folder $normal_param`
`$predict_bin $normal_param $test_list $voc_img_folder  $test_unaries`
