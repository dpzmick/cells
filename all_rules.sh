#!/bin/bash
make
mkdir all
for rule in {0..255}
do
    make run R=$rule L=10000 T=5000
    make image
    mv output.png all/rule_$rule.png
    echo
    echo
done
make clean
