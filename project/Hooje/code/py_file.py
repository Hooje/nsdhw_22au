import _np
from _np import arrays_2d
import numpy as np
from sklearn.datasets import load_iris


def main():
    tmp = np.array([[0,1],[2,3]])
    #arrays_2d(tmp)
    iris = load_iris()
    ori_data = iris.data 
    ori_target = iris.target
    
    arrays_2d(ori_data, ori_target)

if __name__=="__main__":
    main()
