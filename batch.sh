#!/bin/bash
#SBATCH --nodes=1
#SBATCH --exclusive
#SBATCH --ntasks=1
#SBATCH --time=00:09:00
#SBATCH --output=sbatch/SLURM-mandelbrot-C2_2026-02-16_23-38-52.log
#SBATCH --mem=16G
#SBATCH --cpus-per-task=2
#SBATCH --job-name=mandelbrot-C2
#SBATCH --distribution=block:block
#SBATCH --constraint=skylake
./prog1_mandelbrot_threads/mandelbrot -t 2 -i 1 2> logs/mandelbrot-C2.log
