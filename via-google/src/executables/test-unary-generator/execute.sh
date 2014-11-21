source ../config
echo "Predictor path: $predict_bin"
echo "VOC Testing List: $voc_test_list"
echo "VOC Image folder: $voc_img_folder"
echo "VOC Test Unaries Dir: $voc_test_unary_dir"
echo "Improved Param: $improved_param"
echo "##################################"

#create google unary
`$predict_bin $improved_param $voc_test_list $voc_img_folder  $voc_test_unary_dir`