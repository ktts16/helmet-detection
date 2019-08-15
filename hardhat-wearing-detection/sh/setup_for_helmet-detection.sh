# set CAFFE_ROOT (environment variable)
export CAFFE_ROOT=$HOME/github/helmet-detection/hardhat-wearing-detection/SSD-RPA

# append path to PYTHONPATH (environment variable)
export PYTHONPATH=$PYTHONPATH:$CAFFE_ROOT/python

# install required Python packages
cd $CAFFE_ROOT/python 
for req in $(cat requirements.txt); do pip install $req; done
