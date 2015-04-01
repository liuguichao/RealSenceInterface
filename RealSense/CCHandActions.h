#ifndef __CCHandActions_H__
#define __CCHandActions_H__
#include "CCGestureAttribute.h"

NS_CC_BEGIN


class RotationAction
{
public:
	RotationAction(RotationData &rotData);
	void update();
public:
	/**The min/max rotation angles constraints*/
	RotationConstraints Constraints;
	/**When enabled the object will continue to rotate in a constant speed (set in the Continuous Rotation Speed parameter) as long as the Rotation Trigger is fired*/
	bool ContinuousRotation;	
	/**Sets the continuous rotation speed when enabled*/
	float ContinuousRotationSpeed;
	/** The rotate dumping factor. More means less rotation.*/
	float RotateDumpingFactor;	
private:
	
	float _xRot;
	float _yRot;
	float _zRot;

	float _lastX;
	float _lastY;
	float _lastZ;

	float _rotationAngleX;
	float _rotationAngleY;
	float _rotationAngleZ;
	
	RotationData &_rotData;
	 bool _actionTriggered;
};


NS_CC_END
#endif  //defined(__CCHandRules_H__)