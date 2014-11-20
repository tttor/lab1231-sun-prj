source ../config
echo "Predictor path: $predict_bin"
echo "Google Unaries Dir: $google_unaries_dir"
echo "Google Training List: $google_train_list"
echo "DPM Image folder: $dpm_img_folder"
echo "Normal Param: $normal_param"
echo "##################################"

#create google unary
`$predict_bin $normal_param $google_train_list $dpm_img_folder  $google_unaries_dir`

