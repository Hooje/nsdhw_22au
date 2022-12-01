#include <iostream>
#include <stddef.h>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

template <class T>
set<T> convertToSet(vector<T> v)
{
    set<T> s;
    for (T x : v) {
        s.insert(x);
    }
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
    //int vec_len = vec.size();
    set<double> vec_set_ori;//(vec, vec + vec.size());
    vec_set_ori = convertToSet(vec);
    vector<double> vec_cnt(vec.size(),0), vec_set(vec_set_ori.begin(), vec_set_ori.end());
    //int x;
    for(int i = 0; i < int(vec.size()); i++)
    {
        for(int j = 0; j < int(vec_set.size()); j++)
        {
            if(vec[i] == vec_set[j])
            {
                vec_cnt[j] += double(1)/vec.size();
            }
        }
    }
    for(int i = 0; i < int(vec_cnt.size()); i++)
    {
        vec_cnt[i] = vec_cnt[i] * vec_cnt[i];
    }
    double gini_value = 1;
    for(int i = 0; i < int(vec_cnt.size()); i++)
    {
        gini_value -= vec_cnt[i];
    }
    return gini_value;
}



struct node
{
    int idx = -1; // use idx feature to split
    double value = -1; // if sth's idx feature > value  go to right, <= go to left
    node *left = NULL;
    node *right = NULL;
};

class DecisionTree
{
public:
    int max_depth = 1000;
    node* root = NULL;
    DecisionTree(int max_depth);

    void fit();
    void predict();
    void predict_class();
    void split_two();
    void split();
    void end_to_leaf();
};

DecisionTree::DecisionTree(int max_depth)
{   
    max_depth = max_depth;
    cout<<"max_depth = "<<max_depth<<endl;
}
DecisionTree::fit()
{

}
DecisionTree::predict()
{

}
DecisionTree::predict_class()
{

}
DecisionTree::split_two()
{

}
DecisionTree::split()
{

}

DecisionTree::end_to_leaf()
{

}

