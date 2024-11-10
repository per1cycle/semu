echo "Cloning submodule..."
git submodule update --init --recursive 

# echo "Compiling test binary"
# cd riscv-tests
# ./configure

# cd ..
echo "Building project..."
mkdir build
sudo apt update 
sudo apt install cmake gcc g++
cmake ..
make -j${nproc}

./app/semu 