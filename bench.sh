#!/bin/bash
make
for run in {1..10}
do
    echo "Running trial $run"
    gtime -f"%e" -o"b_log" --append make run R=30 L=10000 T=1000
done
make clean

cat b_log | awk 'NR == 1 { max=$1; min=$1; sum=0 }
   { if ($1>max) max=$1; if ($1<min) min=$1; sum+=$1;}
   END {printf "Min: %d\tMax: %d\tAverage: %f\n", min, max, sum/NR}'

rm b_log
