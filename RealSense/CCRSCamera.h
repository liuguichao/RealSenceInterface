#ifndef __CCRSCamera_H__
#define __CCRSCamera_H__

#include "CCGesturePipeline.h"

/**
This singleton class is defined to control RealSense camera, such as start and pause
Note this class inherit from GesturePipeline which does the real interaction works with Intel's camera
*/
NS_CC_BEGIN
//NS_CC_EXT_BEGIN

class RSCamera : public GesturePipeline
{
public:

	/** Returns a shared instance of the camera */
	static RSCamera *getInstance();
	
private:

	/** Constructor of Camera */
	RSCamera();

	/** Destructor of Camera */
	~RSCamera(void);
};

NS_CC_END
#endif  //defined(__CCRSCamera_H__)