#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

template <class T>
set<T> convertToSet(vector<T> v)
{
    // Declaring the set
    set<T> s;
 
    // Traverse the Vector
    for (T x : v) {
 
        // Insert each element
        // into the Set
        s.insert(x);
    }
 
    // Return the resultant Set
    return s;
}

template <class T>
void printSet(set<T> s)
{
 
    cout << "Set: ";
    for (T x : s) {
        cout << x << " ";
    }
    cout << endl;
}
double gini(vector<double> vec)
{
    
    set<double> vec_set_ori;//(vec, vec + vec.size());
    vec_set_ori = convertToSet(vec);
    vector<double> vec_cnt(vec.size(),0), vec_set(vec_set_ori.begin(), vec_set_ori.end());
    int x;
    for(int i = 0; i < vec.size(); i++)
    {
        for(int j = 0; j < vec_set.size(); j++)
        {
            if(vec[i] == vec_set[j])
            {
                vec_cnt[j] += double(1)/vec.size();
            }
        }
    }
    for(int i = 0; i < vec_cnt.size(); i++)
    {
        vec_cnt[i] = vec_cnt[i] * vec_cnt[i];
    }
    double gini_value = 1;
    for(int i = 0; i < vec_cnt.size(); i++)
    {
        gini_value -= vec_cnt[i];
    }
    return gini_value;
}

int main()
{
    int b[10]{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    //double A[] = {0.3, 0.7, 0.8};
    double* A = new double[100];
    //A = {0.3, 0.7, 0.8};
    for(int i = 0; i < 100; i +=1)
        A[i] = 0;//(double) (i % 10);
    
    vector<double> dataVec;
    int dataArraySize = 100;
    //cout<<dataArraySize<<endl;
    dataVec.insert(dataVec.end(), &A[0], &A[dataArraySize]);
    //targetVec.insert(targetVec.end(), &targetArray[0], &targetArray[dataArraySize]);
    //cout<<dataVec[99]<<endl;

    //cout<<A[0]<<endl;
    double gini_value = gini(dataVec);
    cout<<gini_value<<endl;
    return 0;
}