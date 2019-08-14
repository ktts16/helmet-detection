# root_dir="/home/wu/caffe-ssd"
cur_dir=$(cd $( dirname ${BASH_SOURCE[0]} ) && pwd )
root_dir=$cur_dir/../..
# root_dir=$cur_dir/Documents/github/helmet-detection/hardhat-wearing-detection/SSD-RPA

# echo $cur_dir
# echo $root_dir

redo=1
# data_root_dir="$root_dir/data/Hardhat"
data_root_dir="$HOME/data"
dataset_name="Hardhat"
# mapfile="$data_root_dir/labelmap_hardhat.prototxt"
mapfile="$root_dir/data/$dataset_name/labelmap_hardhat.prototxt"
anno_type="detection"
db="lmdb"
min_dim=0
max_dim=0
width=0
height=0

# echo $mapfile

extra_cmd="--encode-type=jpg --encoded"
if [ $redo ]
then
  extra_cmd="$extra_cmd --redo"
fi
for subset in test trainval
do
  # python $root_dir/scripts/create_annoset.py --anno-type=$anno_type --label-map-file=$mapfile --min-dim=$min_dim --max-dim=$max_dim --resize-width=$width --resize-height=$height --check-label $extra_cmd $data_root_dir $root_dir/data/$dataset_name/$subset.txt $data_root_dir/$db/$dataset_name"_"$subset"_"$db $root_dir/examples/$dataset_name
  python $root_dir/scripts/create_annoset.py --anno-type=$anno_type --label-map-file=$mapfile --min-dim=$min_dim --max-dim=$max_dim --resize-width=$width --resize-height=$height --check-label $extra_cmd $data_root_dir $root_dir/data/$dataset_name/$subset.txt $data_root_dir/$dataset_name/$db/$dataset_name"_"$subset"_"$db examples/$dataset_name
done
