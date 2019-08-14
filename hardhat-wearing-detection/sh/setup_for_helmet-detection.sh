# set CAFFE_ROOT (environment variable)
export CAFFE_ROOT=$HOME/github/helmet-detection/hardhat-wearing-detection/SSD-RPA

# append path to PYTHONPATH (environment variable)
export PYTHONPATH=$PYTHONPATH:$CAFFE_ROOT/python

# install required Python packages

sudo apt-get install python-matplotlib python-numpy python-pil python-scipy
sudo apt-get install build-essential cython
sudo apt-get install python-skimage
pip install protobuf
