# Use EDCC library in python project

## EDCC API Adapter

According to the type of platform (Windows / Linux), select the dynamic library to load, you can refer to the example EDCCApiAdapter.py call method.

## Create a palm print image factory

For generating a triples (ID, instanceID, imagePath) associated with the palmprint image encoded data, where the ID can be a human ID, such as a name; instanceID is an instance ID corresponding to the palmprint image, for example, R_01; The image path, either absolute or relative to the EDCC dynamic library.

## Create palmprint image storage

Used to access the palmprint image encoded data.