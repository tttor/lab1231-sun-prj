source ..config
echo "Evaluator: $evaluator_path"
echo "GT CSV: $voc_gt_csv"
echo "Test CSV: $voc_test_csv"
`python $evaluator_path $voc_gt_csv $voc_test_csv $voc_eval_list`