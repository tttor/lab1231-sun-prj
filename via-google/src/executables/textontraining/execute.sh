source ../config
echo "Trainer path: $train_bin"
echo "Target param folder: $normal_param"
`$train_bin $normal_param`
`$predict_bin $normal_param $google_unaries_dir`

