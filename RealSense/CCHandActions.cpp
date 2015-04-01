#include "CCHandActions.h"
#include "CCEventGesture.h"
#include "CCRealSenseEventCenter.h"

NS_CC_BEGIN

RotationAction::RotationAction(RotationData &rotData)
:_rotData(rotData)
, _xRot(0), _yRot(0), _zRot(0)
, _lastX(0), _lastY(0), _lastZ(0)
, _rotationAngleX(0), _rotationAngleY(0), _rotationAngleZ(0)
, _actionTriggered(false)
, RotateDumpingFactor(0.5f)
, ContinuousRotationSpeed(0)
, ContinuousRotation(false)
{
	Constraints.Freeze.X = false;
	Constraints.Freeze.Y = false;
	Constraints.Freeze.Z = false;

	Constraints.XRotation.MaxPos = 360;
	Constraints.YRotation.MaxPos = 360;
	Constraints.ZRotation.MaxPos = 360;

	Constraints.XRotation.MaxNeg = -360;
	Constraints.YRotation.MaxNeg = -360;
	Constraints.ZRotation.MaxNeg = -360;
}
int sign(float f)
{
	return f > 0 ? 1 : -1;
}
void RotationAction::update()
{	
	if (!_actionTriggered)
	{
		_actionTriggered = true;
		_rotData.Restart = true;
		return;
	}
	if (RotateDumpingFactor == 0)
	{
		cocos2d::log("RotateDumpingFactor must not be zero. Changing it to 1");
		RotateDumpingFactor = 1;
	}
	_xRot = _rotData.Pitch / RotateDumpingFactor;
	_yRot = -_rotData.Yaw / RotateDumpingFactor;
	_zRot = -_rotData.Roll / RotateDumpingFactor;
	//Keep it relative - for each axis, if rotation angle is 0 or if this is the first "frame" after a rotation angle is detected, save the data for next frame		
	if (!ContinuousRotation)
	{
		if (_xRot == 0 || _lastX == 0)		
			_lastX = _xRot;		
		else		
			_xRot = _xRot - _lastX;
		

		if (_yRot == 0 || _lastY == 0)		
			_lastY = _yRot;		
		else		
			_yRot = _yRot - _lastY;
		

		if (_zRot == 0 || _lastZ == 0)	
			_lastZ = _zRot;	
		else		
			_zRot = _zRot - _lastZ;	
	}

	if (ContinuousRotation && ContinuousRotationSpeed != 0)
	{
		_xRot = sign(_xRot) * ContinuousRotationSpeed;
		_yRot = sign(_yRot) * ContinuousRotationSpeed;
		_zRot = sign(_zRot) * ContinuousRotationSpeed;
	}
	
	//Make sure we didn't pass the Maximum Rotation Angles
	_xRot = ((_xRot + _rotationAngleX) > Constraints.XRotation.MaxPos) ? (Constraints.XRotation.MaxPos - _rotationAngleX) : _xRot;
	_yRot = ((_yRot + _rotationAngleY) > Constraints.YRotation.MaxPos) ? (Constraints.YRotation.MaxPos - _rotationAngleY) : _yRot;
	_zRot = ((_zRot + _rotationAngleZ) > Constraints.ZRotation.MaxPos) ? (Constraints.ZRotation.MaxPos - _rotationAngleZ) : _zRot;

	_xRot = ((_xRot + _rotationAngleX) < Constraints.XRotation.MaxNeg) ? (Constraints.XRotation.MaxNeg - _rotationAngleX) : _xRot;
	_yRot = ((_yRot + _rotationAngleY) < Constraints.YRotation.MaxNeg) ? (Constraints.YRotation.MaxNeg - _rotationAngleY) : _yRot;
	_zRot = ((_zRot + _rotationAngleZ) < Constraints.ZRotation.MaxNeg) ? (Constraints.ZRotation.MaxNeg - _rotationAngleZ) : _zRot;

	//Enable / Disable Axis
	_xRot = Constraints.Freeze.X ? 0 : _xRot;
	_yRot = Constraints.Freeze.Y ? 0 : _yRot;
	_zRot = Constraints.Freeze.Z ? 0 : _zRot;

	Vec3 rot(_xRot,_yRot,_zRot);	
	//send rotation message
	EventGesture gestureEvent;	
	gestureEvent._handData = nullptr;
	gestureEvent._gestureType = GestureType::Rotation;
	gestureEvent._rotationData = rot;	
	RealSenseEventCenter::getInstance()->pushGestureEvent(gestureEvent);

	//Update new rotation angle
	_rotationAngleX = _rotationAngleX + _xRot;
	_rotationAngleY = _rotationAngleY + _yRot;
	_rotationAngleZ = _rotationAngleZ + _zRot;

	//Enable Full Rotations
	if (_rotationAngleX >= 360) { _rotationAngleX = _rotationAngleX - 360; };
	if (_rotationAngleY >= 360) { _rotationAngleY = _rotationAngleY - 360; };
	if (_rotationAngleZ >= 360) { _rotationAngleZ = _rotationAngleZ - 360; };

	//Enable Full Rotations
	if (_rotationAngleX <= -360) { _rotationAngleX = _rotationAngleX + 360; };
	if (_rotationAngleY <= -360) { _rotationAngleY = _rotationAngleY + 360; };
	if (_rotationAngleZ <= -360) { _rotationAngleZ = _rotationAngleZ + 360; };

	//Update last rotation
	_lastX = _rotData.Pitch / RotateDumpingFactor;
	_lastY = -_rotData.Yaw / RotateDumpingFactor;
	_lastZ = -_rotData.Roll / RotateDumpingFactor;
	

}


NS_CC_END