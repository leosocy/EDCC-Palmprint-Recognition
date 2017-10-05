# 参数说明

- `-t/--train` 处理训练集掌纹图像，得到EDCC特征。
    - 如果有`-o`，则将掌纹库特征存储到对应的json文件中。
    - 如果没有`-c`，则返回处理失败，因为没有设置算法参数。
    - 不能和`-p`一起使用，只能单独处理训练集或者单独处理测试集。
    - 如果有`-m`，则忽略此参数。

- `-p/--predict` 处理测试集掌纹图像，得到EDCC特征。
    - 如果有`-o`，则将掌纹库特征存储到对应的json文件中。
    - 如果没有`-c`，则返回处理失败，因为没有设置算法参数。
    - 不能和`-t`一起使用，只能单独处理训练集或者单独处理测试集。
    - 如果有`-m`，则忽略此参数。

- `-m/--match`
- `-c/--config` specify config.json to train or predict
- `-i/--increment` train/predict with increment
- `-o/--output` train/predict output filename
- `-h/--help` log help info


# examples

## 处理训练集

- `parse_cmd -t trainPalmprintGroup.json -o trainFeatures.json -c config.json`

## 处理测试集

- `parse_cmd -p predictPalmprintGroup.json -o predictFeatures.json -c config.json`