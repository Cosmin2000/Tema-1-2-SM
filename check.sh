#!/bin/bash
#
# Exemplu rulare:
# ./check.sh 1 4   -> ruleaza testele pentru pthreads cu 4 thread-uri
# ./check.sh all 2 -> ruleaza toate testele cu 2 thread-uri
#

INPUT="tests/input"
REF="ref/ref"
CHOICE=(pthreads mpi openMP hybrid)
OUT="out"

# Initializare input
python3 init.py

if [ $# -lt 2 ]; then
    echo "./check.sh  [ 1 - pthreads || 2 - mpi || 3 - openMP || 4 - hybrid || all ]   [number of threads]"
    exit
fi

numberPattern='^[0-9]+$'
if [[ $1 =~ $numberPattern ]]; then

    make ${CHOICE[$1 - 1]}
    
    for i in {1..2}
    do
        if [ $1 -eq 2 ] || [ $1 -eq 4 ];
        then 
            mpirun --oversubscribe -np $2 ${CHOICE[$1 - 1]} $i
        else
            ./${CHOICE[$1 - 1]} $2 $i 
        fi
        diff ${OUT}/${CHOICE[${1} - 1]}/out$i ${REF}$i > /dev/null
        if [[ $? = 0 ]]; then
            echo "Test $i : OK"
        else
            echo "Test $i : FAILED"
        fi
    done



else
    
fi