#!/bin/bash

#SBATCH --job-name=gol
#SBATCH --partition=cosc3500
#SBATCH --account=cosc3500
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --time=0-00:01 # time (D-HH:MM)

time ./gol