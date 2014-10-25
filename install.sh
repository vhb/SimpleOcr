wget "http://bitbucket.org/eigen/eigen/get/3.1.4.tar.gz"
tar -zxf 3.1.4.tar.gz
rm 3.1.4.tar.gz
cd eigen-eigen-36bf2ceaf8f5
mkdir build
cd build
cmake ..
sudo make install

git clone https://github.com/Itseez/opencv.git
cd opencv
mkdir build
cd build
cmake ..
make -j8
