source ../config
google_unaries_dir="/home/rizkivmaster/ws/results/14736/VGS_unary"
normal_param="/home/rizkivmaster/ws/results/14736/init_param"
google_train_list="/home/rizkivmaster/ws/datasets/smallVOC2010/split/vocgooglesvmTrain.txt"
google_test_list="/home/rizkivmaster/ws/datasets/smallVOC2010/split/Test.txt"
dpm_img_folder="/home/rizkivmaster/ws/datasets/smallVOC2010/JPEGImages"
#create google unary
#`$predict_bin $normal_param $google_train_list $dpm_img_folder  $google_unaries_dir`
`$predict_bin $normal_param $google_test_list $dpm_img_folder  $google_unaries_dir`
