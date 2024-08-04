#!/usr/bin/env bash

set -Eeuo pipefail

systems=("spruce" "livegraph" "terrace" "sortledton")

datasetsOfLcc=("notredame-10000" "wiki-1000" "caida-10000" "stackoverflow-1000")

datasetsOfBc=("notredame-10000" "wiki-1000" "caida-10000" "stackoverflow-1000")

# tasks=("lcc" "bc")

repeatTime="5"

for sys in "${systems[@]}"; do
    for dataset in "${datasetsOfLcc[@]}"; do
        # for task in "${tasks[@]}"; do
            # test lcc
            echo "Current tes: ./bin/test-$sys lcc $repeatTime ./bin/$dataset.txt >> test\_$sys\_$dataset\_lcc.txt"
            ./bin/test-$sys lcc $repeatTime ./bin/$dataset.txt >> "test_${sys}_${dataset}_lcc.txt"

            echo "Current tes: ./bin/test-$sys bc $repeatTime ./bin/$dataset.txt >> test\_$sys\_$dataset\_bc.txt"
            ./bin/test-$sys bc $repeatTime ./bin/$dataset.txt >> "test_${sys}_${dataset}_bc.txt"

        # done
    done
done

echo "Finished!"