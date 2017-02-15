/*************************************************************************
	> File Name: process_parameter.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/02/14 21:38:35
 ************************************************************************/

#ifndef __PROCESS_PARAMETER_H__
#define __PROCESS_PARAMETER_H__

/*
 * process parameters 
 * parameter1:	./main
 * parameter2:	Train/Test/Predict(If the parameter is 'Train', you should \
 *		enter the 'TrainList' which store the path of each trained image later) \
 *		(If the parameter is 'Test', you should enter the 'TestList' which store \
 *		the path of each tested image later)	\
 *		(If the parameter is 'Predict', you should enter the 'PredictData' which is \
 *		a path of one picture or one video)
 * parameter3:	TrainList/TestList/PredictData
 * parameter4:	Image/Video(Type to be processed)
 * parameter5:	CameraImage/DatabaseImage(CameraImage is shotted by phone. DatabaseImage is provided by Polyu)
 */

void print_usage();
void process( int argc, char **argv );


#endif 



