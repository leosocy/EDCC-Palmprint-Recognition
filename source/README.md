# 代码架构

## API

- EDCC.h中提供的API对应的实现

## Pub

- 一些公用的函数和宏

## IO

- 加载EDCC算法的配置文件`loadConfig`(配置文件为JSON格式)
- 加载掌纹训练集`loadPalmprintGroup`
- 加载掌纹训练集特征库`loadPalmprintFeatureData`
- 保存掌纹训练集特征库`savePalmprintFeatureData`

## Check

- 检查EDCC算法参数合法性`checkConfigValid`
- 检查训练集掌纹路径合法性`checkPalmprintGroupValid`
- 检查特征库合法性`checkPalmprintFeatureData`

## Core

- 手掌`Palmprint`
- EDCC编码`EDCCoding`
- Gabor滤波器`GaborFilter`
- 掌纹编码`PalmprintCode`

## Match

- 两个掌纹编码点对点匹配`matchP2P`