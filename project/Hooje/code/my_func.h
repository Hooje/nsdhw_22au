#include <iostream>
#include <stddef.h>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;


double get_accuracy(vector<double> v1, vector<double> v2)
{
    double ans;
    double cnt = 0;
    for(int i = 0; i < int(v1.size()); i++)
    {
        if(v1[i] == v2[i])
        {
            cnt+=1;
        }
    }
    ans = cnt / double(v1.size());
    return ans;
}
template <class T>
void print_vec(vector<T> dataVec)
{
    for(int i = 0; i < int(dataVec.size()); i++)
    {
        cout<<dataVec[i]<<' ';
    }
    cout<<endl;
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
    double target = 0; // if leaf target = its class
    int idx = -1; // use idx feature to split
    double value = -1; // if sth's idx feature > value  go to right, <= go to left
    node *left = NULL;
    node *right = NULL;
    two_vec* group = NULL;
};



void fake_gpu_function(vector<double> xy, int columns, int rows, vector<double>& score_vec, int i, int f_idx);

void simd_function(vector<double> xy, int columns, int rows, int f_idx, int& b_index, double& b_value, double& b_score, two_vec** p_b_groups);

class DecisionTree
{
public:
    int max_depth = 100, columns = 0, class_n = 0;
    node* root = NULL;
    DecisionTree(int max_depth, int columns, int class_n);

    void fit(vector<double> xy);
    vector<double> predict(vector<double> test, int rows, int columns);
    node* predict_class(node* t_node, vector<double> row);
    two_vec* split_two(int idx, double value, vector<double> xy);
    void split(node* t_node, int depth);
    double end_to_leaf(vector<double> subxy);
    node* get_split(vector<double> xy);
};

DecisionTree::DecisionTree(int max_depth, int columns, int class_n)
{   
    this->max_depth = max_depth;
    this->columns = columns;
    this->class_n = class_n;
    //cout<<"max_depth = "<<max_depth<<endl;
}
void DecisionTree::fit(vector<double> xy)
{
    //cout<<"fit"<<endl;
    //cout<<"into get_split"<<endl;
    this->root = this->get_split(xy);
    //cout<<"finish get_split"<<endl;
    this->split(this->root, 1);
    //cout<<"finish split"<<endl;
}
vector<double> DecisionTree::predict(vector<double> test, int rows, int columns)
{
    vector<double> pt;
    vector<double> tmp;
    double tmp_pt;
    node* class_node;
    for(int i = 0; i < rows; i++)
    {
        tmp = copy_vector(test, i*columns, (i+1)*columns); //不包含 last_idx
        
        class_node = this->predict_class(this->root, tmp);
        tmp_pt = class_node->target;
        pt.push_back(tmp_pt);
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
    //cout<<"split two"<<endl;
    vector<double> left, right, row;
    int rows = xy.size() / this->columns;
    //cout<<rows<<endl;
    
    for(int i = 0; i < rows; i++)
    {
        //cout<<"split two "<<i<<"th loop"<<endl;
        row = copy_vector(xy, i*columns, (i+1)*columns); //不包含 last_idx
        if(row[idx] < value)
        {
            left = vec_concat(left, row);
        }
        else
        {
            right = vec_concat(right, row);
        }
    }
    //cout<<"leave loop"<<endl;
    two_vec* group = new two_vec();

    group->v1 = left;
    group->v2 = right;
    //cout<<"wait return"<<endl;
    return group;
}
void DecisionTree::split(node* t_node, int depth)
{
    //cout<<"into split"<<endl;
    two_vec* tmp;
    tmp = t_node->group;
    //cout<<tmp->v1.size()<<endl;
    
    //cout<<"half split"<<endl;
    if(tmp->v1.size() == 0)
    {
        t_node->leaf = 1;
        t_node->target = this->end_to_leaf(tmp->v2);
        return;
    }
    else if(tmp->v2.size() == 0)
    {
        t_node->leaf = 1;
        t_node->target = this->end_to_leaf(tmp->v1);
        return;
    }
    //cout<<"half split"<<endl;
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
node* DecisionTree::get_split(vector<double> xy)
{
    //cout<<"get split"<<endl;
    int rows = xy.size() / this->columns;
    int columns = this->columns;
    two_vec* b_groups = new two_vec();
    two_vec* tmp_groups;
    int b_index = 0;
    double b_value = 999, b_score = 999, left_value, right_value;
    vector<double> eachrow;
    int use_simd = 1;
    for(int f_idx = 0; f_idx < this->columns; f_idx ++)
    {   
        
        if(use_simd == 1)
        {
            simd_function(xy, columns, rows, f_idx, b_index, b_value, b_score, &b_groups);
            continue;
        }
        //cout<<"b score = "<<b_score<<endl;
        //cout<<"test simd_function"<<endl;
        //cout<<b_groups->v1.size()<<endl;
        
        for(int i = 0; i < rows; i++)
        {
            
            
            eachrow = copy_vector(xy, i*columns, (i+1)*columns); //不包含 last_idx
            tmp_groups = split_two(f_idx, eachrow[f_idx], xy);

            if(tmp_groups->v1.size()==0)
            {
                left_value = 0;
            }
            else
            {   
                vector<double> class_left;
                int tmp_rows = tmp_groups->v1.size() / columns;
                for(int c_idx = 0; c_idx < tmp_rows; c_idx++)
                {
                    double tmp_c = tmp_groups->v1[c_idx*columns + columns -1];
                    class_left.push_back(tmp_c);
                }
                left_value = gini(class_left);
            }
            if(tmp_groups->v2.size()==0)
            {
                right_value = 0;
            }
            else
            {
                vector<double> class_right;
                int tmp_rows = tmp_groups->v2.size() / columns;
                for(int c_idx = 0; c_idx < tmp_rows; c_idx++)
                {
                    double tmp_c = tmp_groups->v2[c_idx*columns + columns -1];
                    class_right.push_back(tmp_c);
                }
                right_value = gini(class_right);
            }
            double lenleft = double(tmp_groups->v1.size());
            double lenright = double(tmp_groups->v2.size());
            double lenall = lenleft + lenright;
            double gini_value = left_value * (lenleft/lenall) + right_value * (lenright/lenall);
            if(gini_value < b_score)
            {   
                
                b_index = f_idx;
                b_value = eachrow[f_idx];
                b_score = gini_value;
                b_groups = tmp_groups;
            }       
            

            
        }
               
    }
    
    //cout<<b_index<<" "<<b_score<<" "<<b_value<<endl;;
    node* tmp_node = new node();
    tmp_node->idx = b_index;
    
    tmp_node->group = b_groups;
    
    tmp_node->value = b_value;
    //cout<<"finish get_split"<<endl;
    //cout<<b_groups->v1.size()<<endl;
    //cout<<"test get_split"<<endl;
    return tmp_node;
}   

double DecisionTree::end_to_leaf(vector<double> subxy)
{
    //cout<<"end to leaf"<<endl;
    int rows = subxy.size() / this->columns;
    vector<double> class_tmp(this->class_n, 0);
    for(int i = 0; i < rows; i++)
    {
        double tmp_c = subxy[i*columns + columns -1];
        class_tmp[tmp_c] += 1;
    }
    int tmp = 0;
    double tmp_value = 0;
    for(int i = 0; i < this->class_n; i++)
    {
        if(class_tmp[i]>tmp_value)
        {
            tmp = i;
        }
    }
    return double(tmp);
}

two_vec* split_two(int idx, double value, vector<double> xy, int columns)
{
    //cout<<"split two"<<endl;
    vector<double> left, right, row;
    int rows = xy.size() / columns;
    //cout<<rows<<endl;
    
    for(int i = 0; i < rows; i++)
    {
        //cout<<"split two "<<i<<"th loop"<<endl;
        row = copy_vector(xy, i*columns, (i+1)*columns); //不包含 last_idx
        if(row[idx] < value)
        {
            left = vec_concat(left, row);
        }
        else
        {
            right = vec_concat(right, row);
        }
    }
    //cout<<"leave loop"<<endl;
    two_vec* group = new two_vec();

    group->v1 = left;
    group->v2 = right;
    //cout<<"wait return"<<endl;
    return group;
}

void fake_gpu_function(vector<double> xy, int columns, int rows, vector<double>& score_vec, int i, int f_idx)
{
    vector<double> eachrow;
    two_vec* tmp_groups;
    eachrow = copy_vector(xy, i*columns, (i+1)*columns); //不包含 last_idx
    tmp_groups = split_two(f_idx, eachrow[f_idx], xy, columns);
    double left_value, right_value;
    if(tmp_groups->v1.size()==0)
    {
        left_value = 0;
    }
    else
    {   
        vector<double> class_left;
        int tmp_rows = tmp_groups->v1.size() / columns;
        for(int c_idx = 0; c_idx < tmp_rows; c_idx++)
        {
            double tmp_c = tmp_groups->v1[c_idx*columns + columns -1];
            class_left.push_back(tmp_c);
        }
        left_value = gini(class_left);
    }
    if(tmp_groups->v2.size()==0)
    {
        right_value = 0;
    }
    else
    {
        vector<double> class_right;
        int tmp_rows = tmp_groups->v2.size() / columns;
        for(int c_idx = 0; c_idx < tmp_rows; c_idx++)
        {
            double tmp_c = tmp_groups->v2[c_idx*columns + columns -1];
            class_right.push_back(tmp_c);
        }
        right_value = gini(class_right);
    }
    double lenleft = double(tmp_groups->v1.size());
    double lenright = double(tmp_groups->v2.size());
    double lenall = lenleft + lenright;
    double gini_value = left_value * (lenleft/lenall) + right_value * (lenright/lenall);
    score_vec[i] = gini_value;            
    //cout<<gini_value<<" ";
}
void simd_function(vector<double> xy, int columns, int rows, int f_idx, int& b_index, double& b_value, double& b_score, two_vec** p_b_groups)
{
    //cout<<"start function"<<endl;
    vector<double> score_vec(rows, 0);
    int thread_dim = 10;
    int block_dim = rows / thread_dim + 1;
    //cout<<rows<<endl; //105
    //cout<<block_dim<<endl; //11

    for(int i = 0; i < block_dim; i +=1 )
    {
        //cout<<i<<endl;
        for(int j = 0; j < thread_dim; j += 1)
        {
            
            int idx = thread_dim * i + j;
            if(idx >= rows)
            {
                break;
            }
            fake_gpu_function(xy, columns, rows, score_vec, idx, f_idx);
        }
    }
    //cout<<"finish part1"<<endl;
    //找最小的 idx，然後改 b
    //int min_idx = 0;
    //int min_value = 999;
    //cout<<endl;
    //print_vec(score_vec);
    //int x;
    //cin>>x;
    int tmprow = 0;
    int flag = 0;
    for(int i = 0; i <rows; i+=1)
    {
        if(score_vec[i] < b_score)
        {
            flag = 1;
            tmprow = i;
            b_index = f_idx;
            b_score = score_vec[i];
        }   
    }
    //cout<<"finish part2"<<endl;
    //cout<<b_index<<endl;
    if(!flag)
        return;
    vector<double> thisrow;
    thisrow = copy_vector(xy, tmprow*columns, (tmprow+1)*columns); //不包含 last_idx
    b_value = thisrow[f_idx];
    //cout<<b_index<<endl;
    two_vec* tmp_groups;
    tmp_groups = split_two(f_idx, thisrow[f_idx], xy, columns);
    *p_b_groups = tmp_groups;
    //cout<<"test bgroups"<<endl;
    //cout<<tmp_groups->v1.size()<<endl;
    //cout<<b_index<<endl;
    //cout<<"finish part2"<<endl;
    //cout<<"end function"<<endl;

}