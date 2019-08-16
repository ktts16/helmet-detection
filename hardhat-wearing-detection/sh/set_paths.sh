# set CAFFE_ROOT (environment variable)
export CAFFE_ROOT=$HOME/github/helmet-detection/hardhat-wearing-detection/SSD-RPA
echo 'CAFFE_ROOT:'
echo $CAFFE_ROOT

# append path to PYTHONPATH (environment variable)
export PYTHONPATH=$PYTHONPATH:$CAFFE_ROOT/python
# export PYTHONPATH=$CAFFE_ROOT/python
echo 'PYTHONPATH:'
echo $PYTHONPATH