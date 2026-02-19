#!/bin/bash
#SBATCH --nodes=1
#SBATCH -p class
#SBATCH --exclusive
#SBATCH --ntasks=1
#SBATCH --time=01:01:00
#SBATCH --output=sbatch/SLURM-plot.py-C1_skylake_2026-02-18_22-47-34.log
#SBATCH --mem=16G
#SBATCH --cpus-per-task=1
#SBATCH --job-name=plot.py-C1
#SBATCH --distribution=block:block
#SBATCH --constraint=skylake
./prog6_kmeans/plot.py -t 1 2> logs/plot.py-C1.log
