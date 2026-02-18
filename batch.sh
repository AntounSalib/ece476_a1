#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH -t 1:01:00
#SBATCH --output=sbatch/SLURM-mandelbrot-C32_2026-02-18_18-55-02.log
#SBATCH --mem=16G
#SBATCH --cpus-per-task=32
#SBATCH --job-name=mandelbrot-C32
#SBATCH --distribution=block:block
#SBATCH --constraint=skylake
./prog1_mandelbrot_threads/mandelbrot -t 32 -i 1 -v 1 2> logs/mandelbrot-C32.log
