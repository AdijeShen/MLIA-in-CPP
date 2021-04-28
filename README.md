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

