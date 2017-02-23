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
 * parameter0:	./main
 * parameter1:	Train/Test/Predict(If the parameter is 'Train', you should \
 *		enter the 'TrainList' which store the path of each trained image later) \
 *		(If the parameter is 'Test', you should enter the 'TestList' which store \
 *		the path of each tested image later)	\
 *		(If the parameter is 'Predict', you should enter the 'PredictData' which is \
 *		a path of one picture or one video)
 * parameter2:	TrainList/TestList/PredictData
 * parameter3:	Image/Video(Type to be processed)
 * parameter4:	CameraImage/DatabaseImage(CameraImage is shotted by phone. DatabaseImage is provided by Polyu)
 */

void print_usage( void );

void process( int argc, char **argv );


#endif 



