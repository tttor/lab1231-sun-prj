source ../config
echo "Predictor path: $predict_bin"
echo "VOC Testing List: $voc_test_list"
echo "VOC Image folder: $voc_img_folder"
echo "VOC Test Unaries Dir: $google_test_unary_path"
echo "Improved Param: $improved_param"
echo "##################################"

#create google unary
echo "START TO GENERATE UNARIES FOR TEST IMAGES"
`$predict_bin $improved_param $voc_test_list $voc_img_folder  $google_test_unary_path`