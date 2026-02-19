#!/bin/bash
#SBATCH --nodes=1
#SBATCH -p class
#SBATCH --exclusive
#SBATCH --ntasks=1
#SBATCH --time=01:01:00
#SBATCH --output=sbatch/SLURM-sqrt-C32_skylake_2026-02-18_21-12-04.log
#SBATCH --mem=16G
#SBATCH --cpus-per-task=32
#SBATCH --job-name=sqrt-C32
#SBATCH --distribution=block:block
#SBATCH --constraint=skylake
./prog4_sqrt/sqrt -t 32 2> logs/sqrt-C32.log
