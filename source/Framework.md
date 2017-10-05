# 代码架构

### IO

- 加载EDCC算法的配置文件`loadConfig`( 配置文件为JSON格式，需要JsonCpp支持 )
- 加载掌纹图像类别-路径列表`loadPalmprintGroup`
- 加载掌纹训练集特征库`loadPalmprintFeatureData`
- 保存掌纹训练集特征库`savePalmprintFeatureData`


### Check

- 检查列表合法性`checkList`
- 检查配置文件合法性`checkConfig`


### Core

- 提取特征前准备工作`prepare`
- 对一副掌纹图像进行编码获取特征`getEDCC`
- 计算两幅掌纹图像相似度得分`getMatchingScore`
- 点-点匹配得分`p2pMatch`
- 区域-区域匹配得分`a2aMatch`

### Match

- 根据最高匹配得分匹配`maxMatch`
- 根据K-Nearest匹配`KNeaMatch`


### App

- 训练一组掌纹图像并将其类别-特征入库`train`
- 预测一组/幅掌纹图像的类别`predict`

### palmprint_identify

- 主程序入口`main`
- 解析命令参数`parseCMD`


