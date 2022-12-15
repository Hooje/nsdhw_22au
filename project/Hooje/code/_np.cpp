/* _np.cpp */
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <iostream>
#include <stddef.h>
#include <vector>
#include <algorithm>
#include <set>

#include "my_func.h"
namespace py = pybind11;
using std::cout;
using std::endl;


int arrays_2d(py::array_t<double>& input, py::array_t<double>& target)
{
    py::buffer_info buf  = input.request();
    py::buffer_info tbuf  = target.request();
    if (buf.ndim != 2)
    {
        throw std::runtime_error("numpy.ndarray dims must be 2!");
    }
    double* ptr = (double*)buf.ptr;
    double* tptr = (double*)tbuf.ptr; 
    int rows = buf.shape[0];
    int columns = buf.shape[1];
    
    //cout<<rows<<columns<<endl;

    int dataArraySize = rows * columns;

    vector<double> dataVec(ptr, ptr + dataArraySize);
    //一維表示二維
    vector<double> targetVec(tptr, tptr + rows);
    //vector<double> tmp = copy_vector(dataVec,1,3);
    //print_vec(dataVec);
    //print_vec(tmp);
    //dataVec.insert(ptr, ptr + dataArraySize);
    //targetVec.insert(tptr, tptr + columns);

    set<double> targetset = convertToSet(targetVec);
    
    int class_n = targetset.size();
    //cout<<class_n<<endl;
    //printSet(targetset);
    DecisionTree dt(100, columns, class_n);
    dt.fit(dataVec);
    //-------------------------------------------
    /*
    for (int i = 0; i < buf.shape[0]; i++)
    {
        for (int j = 0; j < buf.shape[1]; j++)
        {
            double a = ptr[i*buf.shape[1] + j];
            cout<<a<<' ';
        }
        cout<<endl;
    }

    */

    //--------------------------------------------
    
    return 0;
}



PYBIND11_MODULE(_np, m) {
    m.doc() = "numpy example";
    m.def("arrays_2d", &arrays_2d, "arrays_2d");
}