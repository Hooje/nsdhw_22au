/* _np.cpp */
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <iostream>
#include <stddef.h>
#include <vector>
#include <algorithm>
#include <set>

#include "my_func.h"
//#include "mycuda.h"

namespace py = pybind11;
using std::cout;
using std::endl;

//int x = 0;
//int my_cuda(int , int);

DecisionTree dt(0,0,0);
void dt_pred(py::array_t<double>& input, py::array_t<double>& target)
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
    int dataArraySize = rows * columns;
    vector<double> dataVec(ptr, ptr + dataArraySize);
    vector<double> targetVec(tptr, tptr + rows);
    vector<double> predVec;
    predVec = dt.predict(dataVec, rows, columns);
    cout<<"predict value : ";
    print_vec(predVec);
    cout<<"true value : ";
    print_vec(targetVec);
    double acc;
    acc = get_accuracy(predVec, targetVec);
    cout<<"accuracy = "<<acc<<endl;
    //cout<<x<<endl;
    //cout<<"test "<<dt.max_depth<<endl;
    //cout<<"test "<<dt.class_n<<endl;

}

void arrays_2d(py::array_t<double>& input, py::array_t<double>& target)
{
    //my_cuda();//3,4);
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
    DecisionTree dt2(100, columns, class_n);
    dt = dt2;
    dt.fit(dataVec);
    //cout<<"test "<<dt.max_depth<<endl;
    //cout<<"test "<<dt.class_n<<endl;
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
    //x = 100;
    return ;
}



PYBIND11_MODULE(_np, m) {
    m.doc() = "numpy example";
    m.def("arrays_2d", &arrays_2d, "arrays_2d");
    m.def("dt_pred", &dt_pred, "dt_pred");
    //m.def("my_cuda", &my_cuda, "my_cuda");
}