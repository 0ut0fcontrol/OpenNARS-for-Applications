export OMP_NUM_THREADS=4
export NARS=$(dirname $(readlink -f $BASH_SOURCE))
export PATH=$PATH:$NARS