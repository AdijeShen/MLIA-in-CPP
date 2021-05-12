This is an C++ implementation of book ["Machine Learning in Action"](https://www.manning.com/books/machine-learning-in-action?query=Machine%20Learning%20in%20Action). with cmake so it can be compiled cross platform.

The reason I choose C++ other than lies in that I want to enhance machine learning with homomorphic encryption.

Down below are my learning note of machine learning.

# classification

## kNN

k-Nearest Neighbors classification algorithm

k-近邻算法

### 优缺点

优点：精度高，对异常值不敏感，对数据无任何假设

缺点：计算成本高，需要大量内存

作用于：数值，标称值(nominal values)

### kNN的一般方法

1. 收集：任何方法。

2. 准备：需要计算距离，所以需要数字。一个结构化的数据格式是最好的。
3. 分析：任何方法都可以。
4. 训练：kNN算法不需要训练
5. 测试：计算错误率。
6. 使用：这个应用程序需要输入一些数据并输出结构化的数值。接下来，应用程序在这个输入数据上运行kNN算法，并确定输入数据应该属于哪个类别。然后，该应用程序对计算出的类采取一些行动。

### 伪代码

输入为一条数据inX，以及一个数据集dataset，输出为推测inX的分类（label/class）

For every point in our dataset:
	calculate the distance between inX and the current point
	sort the distances in increasing order
	take k items with lowest distances to inX
	find the majority class among these items
	return the majority class as our prediction for the class of inX

对于数据集中的每一个点：
	计算inX和当前点之间的距离
	按照递增的顺序对距离进行排序
	取出与inX距离最小的k个条目
	在k个条目中找出数量最多的分类
	将数量最多的分类作为我们对inX类别的预测返回。

