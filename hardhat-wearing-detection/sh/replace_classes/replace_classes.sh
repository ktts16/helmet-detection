# > SELECT search_dir
#search_dir=$HOME"/data/GDUT-HWD/test_sed/test_sed"
search_dir=$HOME"/data/GDUT-HWD/Annotations"
for entry in "$search_dir"/*
do
  sed -r -i.txt -f replace.sed $entry
done
