cd print
clang++ -o print-base print.cpp
cd ..
cd print-f
clang++ -o print print.cpp
cd ..
cd print-k
clang++ -o print-w print.cpp
cd ..
cd len
g++ -std=c++11 -O2 -o len len.cpp
cp -r len ~/../usr/bin
cd ..
