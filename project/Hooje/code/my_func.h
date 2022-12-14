#include <iostream>
#include <stddef.h>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

template <class T>
void print_vec(vector<T> dataVec)
{
    for(int i = 0; i < int(dataVec.size()); i++)
    {
        cout<<dataVec[i]<<' ';
    }
}

template <class T>
vector<T> copy_vector(vector<T> ori_vector, int first_idx,int last_idx) //不包含 last_idx
{
    typename vector<T>::const_iterator first = ori_vector.begin() + first_idx;
    typename vector<T>::const_iterator last = ori_vector.begin() + last_idx;
    vector<T> newVec(first, last);
    return newVec;
}

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

vector<double> vec_concat(vector<double> vector1, vector<double>vector2)
{
    vector1.insert( vector1.end(), vector2.begin(), vector2.end() );
    return vector1;
}
struct two_vec
{
    vector<double> v1;
    vector<double> v2;
};
struct node
{
    int leaf = 0; // if leaf then 1
    int target = 0; // if leaf target = its class
    int idx = -1; // use idx feature to split
    double value = -1; // if sth's idx feature > value  go to right, <= go to left
    node *left = NULL;
    node *right = NULL;
    two_vec* group = NULL:
};



class DecisionTree
{
public:
    int max_depth = 100, columns = 0, class_n = 0;
    node* root = NULL;
    DecisionTree(int max_depth);

    void fit(vector<double> xy);
    vector<int> predict(vector<double> test, int rows, int columns);
    node* predict_class(node* t_node, vector<double> row);
    two_vec* split_two(int idx, double value, vector<double> xy);
    void split(node* t_node, int depth);
    int end_to_leaf(vector<double> subxy);
    void get_split();
};

DecisionTree::DecisionTree(int max_depth, int columns, int class_n)
{   
    this->max_depth = max_depth;
    this->columns = columns;
    this->class_n = class_n;
    cout<<"max_depth = "<<max_depth<<endl;
}
void DecisionTree::fit(vector<double> xy)
{
    this->root = this->get_split(xy);
    this->split(this->root, 1);
}
vector<int> DecisionTree::predict(vector<double> test, int rows, int columns)
{
    vector<int> pt;
    vector<double> tmp;
    int tmp_pt;
    for(int i = 0; i < rows; i++)
    {
        tmp = copy_vector(test, i*columns, (i+1)*columns); //不包含 last_idx
        tmp_pt = this->predict_class(this->root, tmp)
        pt.push_back(tmp_pt)
    }
    return pt;
}
node* DecisionTree::predict_class(node* t_node, vector<double> row)
{
    if(t_node->leaf == 1) // if this is leaf, then return this
    {
        return t_node;
    }
    if(row[t_node->idx] < t_node->value) //left
    {
        return this->predict_class(t_node->left, row);
    }
    else //right
    {
        return this->predict_class(t_node->right, row);
    }
}
two_vec* DecisionTree::split_two(int idx, double value, vector<double> xy)
{
    vector<double> left, right, row;
    for(int i = 0; i < rows; i++)
    {
        row = copy_vector(test, i*columns, (i+1)*columns); //不包含 last_idx
        if(row[idx] < value)
        {
            left = vec_concat(left, row);
        }
        else
        {
            right = vec_concat(right, row);
        }
    }
    two_vec* group;
    group->v1 = left;
    group->v2 = right;
    return group;
}
void DecisionTree::split(node* t_node, int depth)
{
    two_vec* tmp;
    tmp = t_node->group;
    if(tmp->v1 == NULL)
    {
        t_node->leaf = 1;
        t_node->target = this->end_to_leaf(tmp->v2);
        return;
    }
    else if(tmp->v2 == NULL)
    {
        t_node->leaf = 1;
        t_node->target = this->end_to_leaf(tmp->v1);
        return;
    }
    if(depth >= this->max_depth)
    {
        t_node->leaf = 1;
        vector<double> tmp_vec;
        tmp_vec = vec_concat(tmp->v1, tmp->v2);
        t_node->target = this->end_to_leaf(tmp_vec);
    }
    t_node->left = this->get_split(tmp->v1);
    this->split(t_node->left, depth+1);
    t_node->right = this->get_split(tmp->v2);
    this->split(t_node->right, depth+1);
    

    
    return;
}   
void DecisionTree::get_split(vector<double> xy)
{
    
    return;
}   

int DecisionTree::end_to_leaf(vector<double> subxy)
{
    int rows = subxy.size() / this->columns;
    vector<int> class_tmp(this->class_n, 0);
    for(int i = 0; i < rows; i++)
    {
        int tmp_c = subxy[i*columns + columns -1];
        class_tmp[tmp_c] += 1;
    }
    int tmp = 0, tmp_value = 0;
    for(int i = 0; i < this->class_n; i++)
    {
        if(class_tmp[i]>tmp_value)
        {
            tmp = i;
        }
    }
    return tmp;
}

