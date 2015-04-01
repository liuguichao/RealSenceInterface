#ifndef __CCGestureAttribute_H__
#define __CCGestureAttribute_H__

#include "base\CCRef.h"
#include "pxchanddata.h"
#include  "math\Vec3.h"
NS_CC_BEGIN


/*
The GestureType enumerator itemizes the gesture type.
*/
enum class GestureType
{
	THUMB_UP,
	THUMB_DOWN,
	FIST,
	V_SIGN,
	SPREADFINGERS,	
	TWO_FINGERS_PINCH_OPEN,
	FULL_PINCH,
	TAP,
	WAVE,
	SWIPE,
	HandMove,
	DepthImage,
	HandDetected,
	HandLost,
	HandOpenned,
	HandClosed,
	Rotation,
	Scale,
	TwoHandsLost,
	TwoHandsDetected
};
/*
The JointType enumerator itemizes the supported joints
*/
enum class JointType
{
	//The wrist joint.
	JOINT_WRIST,
	//The palm center joint.
	JOINT_CENTER,

	//The thumb finger joints.
	JOINT_THUMB_BASE,
	JOINT_THUMB_JT1,
	JOINT_THUMB_JT2,
	JOINT_THUMB_TIP,
	

	//The index finger joints.
	JOINT_INDEX_BASE,
	JOINT_INDEX_JT1,
	JOINT_INDEX_JT2,
	JOINT_INDEX_TIP,
	
	//The middle finger joints.
	JOINT_MIDDLE_BASE,
	JOINT_MIDDLE_JT1,
	JOINT_MIDDLE_JT2,
	JOINT_MIDDLE_TIP,

	//The ring finger joints.
	JOINT_RING_BASE,
	JOINT_RING_JT1,
	JOINT_RING_JT2,
	JOINT_RING_TIP,
	

	//The pinky finger joints.
	JOINT_PINKY_BASE,
	JOINT_PINKY_JT1,
	JOINT_PINKY_JT2,
	JOINT_PINKY_TIP
};
/*
The BodySideType enumerator itemizes the side of a hand.
*/
enum class BodySideType
{
	UNKNOWN = PXCHandData::BODY_SIDE_UNKNOWN,
	LEFT = PXCHandData::BODY_SIDE_LEFT,
	RIGHT=PXCHandData::BODY_SIDE_RIGHT	
};


/** This class contains many useful data of a gesture event, including hand openness,22 joints data,etc.
 *	Each EventGesture contains one HandData* member, to describe the current hand data in HandMove event.
 */
class HandData : public Ref
{

public:	
		
	/** Get hand's coordinates.
	 *  @note The value _x, _y, _z are ranged from 0 to 1.
	 *        As hand moving from left to right, _x will increase from 0 to 1.
	 *        As hand moving from bottom to top, _y will increase from 0 to 1.
	 *        As hand far away from camera, _z will increase from 0 to 1.
	 */
	inline float getX() { return _jointData[(int)JointType::JOINT_CENTER].x; };
	inline float getY() { return _jointData[(int)JointType::JOINT_CENTER].y; };
	inline float getZ() { return _jointData[(int)JointType::JOINT_CENTER].z; };
	/**get the cooridnate of the specified joint */
	inline Vec3 getJointData(JointType joint){ return _jointData[(int)joint]; };
	/**Get the openness of hand*/
	inline float getOpenness(){ return _openness; };
	
	/** Get a copy of a HandData */
	HandData* clone();

private:

	/** Gesture openness is ranged form 0 to 100.
	 *  When the gesture is fist, the openness is 0.
	 *  As the hand open, the openness will increase form 0 to 100.
	 *  When the gesture is palm, the openness is 100.
	 */
	int _openness;

	/** Current gesture body type: left, right or unkown*/
	BodySideType _bodySideType;

	/** The 22 joints data retrieved from pipeline*/
	Vec3 _jointData[PXCHandData::NUMBER_OF_JOINTS];

	friend class HandMoveRule;
		
};
struct RotationMax2D
{
	int MaxNeg;
	int MaxPos;
};
struct Bool3D
{
	bool X;
	bool Y;
	bool Z;
};
class RotationConstraints
{
public:
	RotationMax2D XRotation;
	RotationMax2D YRotation;
	RotationMax2D ZRotation;
	Bool3D Freeze;
};
class RotationData
{
public:
	float Pitch;
	float Yaw;
	float Roll;
	bool Restart;
};

class ScaleConstraints 
{
public:
	float Min;
	float Max;
};

NS_CC_END
#endif  //defined(__CCGestureAttribute_H__)