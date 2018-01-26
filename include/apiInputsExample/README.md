# Description

## Config

Used to configure the parameters of EDCC algorithm. There are five parameters:

- `imageSizeW`: Palmprint ROI image width.
- `imageSizeH`: Palmprint ROI image height.
- `gaborKernelSize`: The kernel size of 2DGabor wavelet.
- `gaborDirections`: The number of directions of 2DGabor wavelet.
- `laplaceKernelSize`: The kernel size of laplacian.

`Tips`: You can change these parameters to get the highest palmprint recognition accuracy.

## TrainingSet

Used to describe the correspondence between training set IDs and palmprint instances. The following rules need to be observed:

- An ID can correspond to n palm-print instances, but the palmprint image path can not be repeated.
- You can not have the same palmprint image path in two different IDs.

`Tips`: This file can be entered as a training set for a small data volume palmprint.

## Features

The palmprint features extracted by EDCC algorithm. This file is generated after calling `GetTrainingSetFeatures`API.

**Do not manually modify!**

`Tips`: This file can be used as a training set feature library for small data volume palm, but the storage space and read efficiency are relatively low.

If the amount of data is large, you need to call the primitive `GetEDCCCoding` to obtain the palmprint feature code and save it in the database.