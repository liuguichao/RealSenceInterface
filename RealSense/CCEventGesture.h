#ifndef __CCEventGesture_H__
#define __CCEventGesture_H__

#include "CCGestureAttribute.h"
#include "base\CCEvent.h"
#include "platform\CCImage.h"
#include "CCHandRules.h"
NS_CC_BEGIN


/**
This class defines a customized gesture event
All gesture events are automatically created, analyzed, and dispatched in GesturePipeline class.
*/
class EventGesture : public Event
{
public:		
	
	/** Constructor of EventGesture */
	EventGesture(void);

	/** Get the type of gesture*/
	inline GestureType getGestureType() const { return _gestureType; };

	/** Get joints data of hand.(coordinates, openness, type etc.)*/
	inline HandData* getHandData() const { return _handData; };

	/** Get depth image data from intel 3d camera*/
	inline Image* getDepthImage() const{ return _depthImage; };

	/** Get the side of body, left hand or right hand*/
	inline BodySideType getBodySideType() const { return _bodySideType; };
	
	/** Get two hand rotation data*/
	inline Vec3 getRotationData() const{ return _rotationData; };

	/** Get two hand scale data*/
	inline float getScaleData() const{ return _scaleData; };
private:	

	/** The type of the gesture*/
	GestureType _gestureType;

	/** The side of body, indicate left hand or right hand*/
	BodySideType _bodySideType;

	/** The hand joints data*/
	HandData * _handData;

	/** The cocos2dx image filled with 3d camera depth image data*/
	Image * _depthImage;	

	/** rotation data*/
	Vec3 _rotationData;

	/** scale data*/
	float _scaleData;

	
	friend class EventListenerGesture;	
	friend class RotationAction;
	friend class GestureDetectedRule;
	friend class HandDetectedRule;
	friend class HandLostRule;
	friend class HandClosedRule;
	friend class HandOpennedRule;
	friend class DrawImageRule;
	friend class HandMoveRule;
	friend class TwoHandsLostRule;
	friend class TwoHandsDetectedRule;
	friend class ScaleAction;
	friend class HandScaleRule;
};

NS_CC_END

#endif //defined(__CCEventGesture_H__) 