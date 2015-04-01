#include "CCGestureAttribute.h"
NS_CC_BEGIN
HandData* HandData::clone()
{
	HandData* handData = new HandData();//will release resource in RealSenseEventCenter pop()
	if (handData != nullptr)
	{		
		handData->_openness = _openness;
		handData->_bodySideType = _bodySideType;

		for (int i = 0; i != PXCHandData::NUMBER_OF_JOINTS; ++i)
			handData->_jointData[i] = _jointData[i];
	}
	return handData;
}


NS_CC_END