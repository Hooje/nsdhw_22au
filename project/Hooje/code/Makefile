CC = g++
INCLUDE = `python3 -m pybind11 --includes`
POSFIX = `python3-config --extension-suffix`
CONFIG = `python3-config --includes`
#CFLAGS = -O3 -shared -std=c++11 -fPIC -Wall  
CFLAGS = -Wall -shared -std=c++11 -fPIC -L. -lcutest 


.PHONY: all, cuda, np, clean

all: clean cuda np test

np : _np.cpp 
	$(CC)  $(CFLAGS) $(INCLUDE) _np.cpp -o _np$(POSFIX) $(CONFIG)

test:
	python3 py_file.py 

cuda: cuda_test.cu
	nvcc --shared -Xcompiler -fPIC cuda_test.cu -o libcutest.so
clean:
	rm -f *.so 
	rm -rf __pycache* .pytest*