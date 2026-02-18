#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH -t 1:01:00
#SBATCH --output=sbatch/SLURM-mandelbrot_ispc-C4_2026-02-17_22-56-41.log
#SBATCH --mem=16G
#SBATCH --cpus-per-task=4
#SBATCH --job-name=mandelbrot_ispc-C4
#SBATCH --distribution=block:block
#SBATCH --constraint=skylake
./prog3_mandelbrot_ispc/mandelbrot_ispc -t 2> logs/mandelbrot_ispc-C4.log
