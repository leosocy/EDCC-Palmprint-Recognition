/*************************************************************************
	> File Name: PRVerify.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/28 15:37:42
 ************************************************************************/
#ifndef __PRVERIFY_H__
#define __PRVERIFY_H__

#include "../../extract_features/features_base.h"
#include "../../PRML/include/PRML.h"

class PRVerify
{
public:

	PRVerify();
	virtual ~PRVerify();

	void doPRPreprocess();
	void doPRROI();
	void doPRFeatures( PRFeatures *featureMethod );
	void doPRML( PRML *mlMethod );

	void doVerify( PRFeatures *featureMethod, PRML *mlMethod );

	PRFeatures *trainFeatures;
	PRFeatures *predictFeatures;
	
private:
};

#endif /* end of PRVerify.h */
