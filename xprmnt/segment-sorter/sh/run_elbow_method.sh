#!/bin/bash
#
run_id=test01
root_dir=/home/tor/sun4/xprmnt/segment-sorter/kmeans-model
dataset=msrc21

# Determine a list of n_cluster
# >>> K = ''.join([str(int(i))+' ' for i in list(numpy.linspace(50,200,10))])
K='1 12 23 34 45 56 67 78 89 100 '

#
py_ws_dir=/home/tor/robotics/prj/011/ws/lab1231-sun-prj/segment-sorter/topic-discovery
input_dir=/home/tor/sun4/xprmnt/segment-sorter/kmeans-model/$dataset

input_filename=kmeans.input.csv
input_filepath=$input_dir/$input_filename

out_dir=$root_dir/$dataset/elbow-method.$run_id/meandistortion
rm -rf $out_dir
mkdir -p $out_dir

#
tmux start-server
for k in $K
do
	session=elbow$k
	tmux kill-session -t $session
	tmux new-session -d -s $session
	tmux select-pane -t $session # Make sure we are in the tumx session

	cmd=python\ $py_ws_dir/run_kmeans.py\ $k\ $input_filepath\ $out_dir
	tmux send-keys "$cmd" C-m
done