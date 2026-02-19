#!/bin/bash
#SBATCH --nodes=1
#SBATCH -p class
#SBATCH --exclusive
#SBATCH --ntasks=1
#SBATCH --time=01:01:00
#SBATCH --output=sbatch/SLURM-kmeans-C10_skylake_2026-02-19_00-58-59.log
#SBATCH --mem=16G
#SBATCH --cpus-per-task=10
#SBATCH --job-name=kmeans-C10
#SBATCH --distribution=block:block
#SBATCH --constraint=skylake
./prog6_kmeans/kmeans 2> logs/kmeans-C10.log
