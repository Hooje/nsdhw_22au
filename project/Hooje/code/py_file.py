import _np
from _np import arrays_2d
import numpy as np
from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split


def main():
    tmp = np.array([[0,1],[2,3]])
    #arrays_2d(tmp)
    iris = load_iris()
    ori_data = iris.data 
    ori_target = iris.target
    X_train, X_test, y_train, y_test = train_test_split(
        ori_data, ori_target, test_size=0.3, random_state=42)
    #print(ori_data)
    arrays_2d(ori_data, ori_target)

if __name__=="__main__":
    main()
