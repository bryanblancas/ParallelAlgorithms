#!/bin/bash

numbers=(1 2 5 10 20 50 100 150 200 210 220 230 240 249)

for i in ${numbers[*]};
do
	echo "Ejecutando con " $i
	mpirun --hostfile hostfile -np $i ./times
done;