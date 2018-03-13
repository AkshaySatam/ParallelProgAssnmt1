#!/bin/bash
#SBATCH -n 1 # Number of cores
#SBATCH -N 1 # Ensure that all cores are on one machine
#SBATCH -t 0-00:05 # Runtime in D-HH:MM
#SBATCH -p development # Partition to submit to
#SBATCH --mem=1 # Memory pool for all cores (see also --mem-per-cpu)
#SBATCH -o op.out # File to which STDOUT will be written
#SBATCH -e question1.err # File to which STDERR will be written
./run.out

