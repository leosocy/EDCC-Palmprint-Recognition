# 说明

## Config

用于对EDCC算法的参数进行配置。共有四个参数：

- `imageSize`:作为输入的掌纹ROI图像尺寸
- `gaborKernelSize`:EDCC使用的2DGabor小波核尺寸
- `gaborDirections`:EDCC使用的2DGabor方向个数
- `laplaceKernelSize`:EDCC使用的拉普拉斯图像增强算子核尺寸

`Tips`:您可以通过更改这些参数，调试以获得最高的识别准确率。

## TrainingSet

用于描述训练集的ID与掌纹实例的对应关系。需要遵守如下规则：

- 一个ID可以对应n个掌纹实例，但是掌纹图像路径不能重复。
- 两个不同ID中不能有相同的掌纹图像路径。

## Features

EDCC算法提取的掌纹特征。该文件在调用`GetTrainingSetFeatures`API后生成。

**请勿手动修改！**