export OMP_NUM_THREADS=4
export NARS=$(pwd)/$(dirname "$0")
export PATH=$PATH:$NARS
echo "add $NARS to PATH"
