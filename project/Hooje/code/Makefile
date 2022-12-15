CC = g++
INCLUDE = `python3 -m pybind11 --includes`
POSFIX = `python3-config --extension-suffix`
CONFIG = `python3-config --includes`
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC


.PHONY: all, np, clean

all: clean np test

np : _np.cpp 
	$(CC)  $(CFLAGS) $(INCLUDE) _np.cpp -o _np$(POSFIX) $(CONFIG)

test:
	python3 py_file.py 

clean:
	rm -f *.so 
	rm -rf __pycache* .pytest*