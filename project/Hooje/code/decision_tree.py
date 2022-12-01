
def gini(sequence, sample_weight = [-1]):
    if sample_weight[0] == -1:
        sample_weight = np.ones(len(sequence))/len(sequence)
    values = np.unique(sequence)
    sample_weight = sample_weight.astype(float)
    counts = np.zeros(len(values))
    
    
    #sample_weight.astype(float)
    for i in range(len(sequence)):
        counts[np.where(values == sequence[i])] += sample_weight[i]

    #counts = counts * sample_weight
    #print(counts)
    #print(sample_weight)
    norm_counts =  counts / counts.sum() # calculate norm = calculate p 
    gini_value = 1 - np.sum(norm_counts*norm_counts)
    return gini_value
def entropy(sequence, sample_weight = [-1]):
    if sample_weight[0] == -1:
        sample_weight = np.ones(len(sequence))/len(sequence)
    sample_weight = sample_weight.astype(float)
    values = np.unique(sequence)
    
    counts = np.zeros(len(values))

    for i in range(len(sequence)):
        counts[np.where(values == sequence[i])] += sample_weight[i]

    norm_counts = counts / counts.sum() # calculate norm = calculate p 

    entropy_value =  -np.sum(norm_counts * np.log2(norm_counts))
    return entropy_value


class DecisionTree():
    def __init__(self, criterion='gini', max_depth=10000, max_feature=1000):
        self.max_feature = max_feature
        self.criterion = criterion
        self.max_depth = max_depth
        self.root = None
        self.feature_use = np.zeros(len(train_data[0]))
        #self.sample_weight = None
    def fit(self, xy, sample_weight = [-1]):
        if sample_weight[0] == -1:
            sample_weight = np.ones(len(xy))/len(xy)
        sample_weight = sample_weight.astype(float)

        xy = np.concatenate((xy, sample_weight.reshape(-1,1)), axis = 1)
        xy = xy.tolist()
        self.root = self.get_split(xy)
        self.split(self.root,1)
        #print('fit')
        return None
    def predict(self, test):
        pt = []
        #cnt = 0
        for row in test:
            #print(row)
            prediction = self.predict_class(self.root, row)
            pt.append(prediction)
            #input()
        return pt
    def predict_class(self, node, row):
        if row[node['index']] < node['value']:
            if isinstance(node['left'], dict):
                return self.predict_class(node['left'], row)
            else:
                return node['left']
        else:
            
            if isinstance(node['right'], dict):
                return self.predict_class(node['right'], row)
            else:
                return node['right']

    def split_two(self,index,value,xy): #split to left and right
        left = []
        right = []
        for row in xy:
            if row[index] < value:
                left.append(row)
            else:
                right.append(row)
        return left, right
        
    def get_split(self,xy):
        b_index, b_value, b_score, b_groups = 999, 999, 999, None
        if self.max_feature == 1000 or self.max_feature >len(xy[0])-2:
            self.max_feature = len(xy[0]) - 2
            fl = np.arange(self.max_feature)
        else:
            fl = np.random.choice(len(xy[0]) - 2, self.max_feature, replace = 0)

        for feature_idx in fl:
            for each_thres in xy: # take each row as threshold and get the best threshold
                left, right = self.split_two(feature_idx, each_thres[feature_idx], xy)
                if self.criterion == 'gini':
                    #print(left)
                    if len(left) == 0:
                        left_value = 0
                    else:
                        class_left = np.array(left)[:,-2]
                        left_value = gini(class_left, np.array(left)[:,-1])
                    if len(right) == 0:
                        right_value = 0
                    else:
                        class_right = np.array(right)[:,-2]
                        right_value = gini(class_right, np.array(right)[:,-1])
                    
                    gini_value = left_value * (len(left) / (len(left) + len(right))) + right_value * (len(right) / (len(left) + len(right)))
                    #print(gini_value)
                    if gini_value <= b_score : # get a better split feature
                        #print(f'update gini')
                        b_index, b_value, b_score, b_groups = feature_idx, each_thres[feature_idx], gini_value, (left,right)
                        
                else : 
                    #print(left)
                    if len(left) == 0:
                        left_value = 0
                    else:
                        class_left = np.array(left)[:,-2]
                        left_value = entropy(class_left, np.array(left)[:,-1])
                    if len(right) == 0:
                        right_value = 0
                    else:
                        class_right = np.array(right)[:,-2]
                        right_value = entropy(class_right, np.array(right)[:,-1])
                    
                    entropy_value = left_value * (len(left) / (len(left) + len(right))) + right_value * (len(right) / (len(left) + len(right)))

                    if entropy_value <= b_score : # get a better split feature
  
                        b_index, b_value, b_score, b_groups = feature_idx, each_thres[feature_idx], entropy_value, (left,right)
        self.feature_use[b_index] += 1
        return {'index':b_index, 'value':b_value, 'groups':b_groups}
    
    def split(self,node,depth): # until to leaf, or max_depth
        left, right = node['groups']
        del(node['groups'])        
        if not left or not right: # there is no split
            node['left'] = node['right'] = self.end_to_leaf(left + right)
            return
        
        if depth >= self.max_depth: # check for max depth
            node['left'], node['right'] = self.end_to_leaf(left), self.end_to_leaf(right)
            return
        # process left child
        node['left'] = self.get_split(left)
        self.split(node['left'], depth+1)
        # process right child
        node['right'] = self.get_split(right)
        self.split(node['right'], depth+1)
        
    def end_to_leaf(self, group):
        outcomes = [row[-2] for row in group] # get the class
        class_ = max(set(outcomes), key=outcomes.count) # this leave's class
        #print(outcomes)
        #print(f'node class = {class_}')
        return class_
