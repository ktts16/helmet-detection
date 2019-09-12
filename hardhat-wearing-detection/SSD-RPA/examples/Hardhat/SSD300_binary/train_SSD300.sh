#!/bin/sh
./build/tools/caffe train -solver="examples/Hardhat/SSD300_binary/solver.prototxt" \
-weights="models/VGGNet/VGG_ILSVRC_16_layers_fc_reduced.caffemodel" \
--gpu 0 2>&1 | tee examples/Hardhat/SSD300_binary/SSD300_Hardhat.log
# 2>&1 | tee examples/Hardhat/SSD300_binary/SSD300_Hardhat.log

\
