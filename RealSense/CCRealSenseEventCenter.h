#ifndef __CCRealSenseEventCenter_H__
#define __CCRealSenseEventCenter_H__

#include <mutex>
#include <queue>
#include "CCEventGesture.h"
//#include "CCGestureAttribute.h"

/**
 *	This singleton class is defined to dispatch gesture events.
 *
 *	Note: Because OpenGL doesn't support two threads drawing synchronously, GesturePipeline and cocos2d::Director will
 *	      share the same event queues in the RealSenseEventCenter.
 *
 *  Usage:
 *     Schedule RealSenseEventCenter::tick(float) function in the AppDelegate.cpp:
 *
 *			auto pDirector = Director::getInstance(); 
 *			pDirector->getScheduler()->schedule(schedule_selector(RealSenseEventCenter::tick), RealSenseEventCenter::getInstance(), 0, false); 
 */

NS_CC_BEGIN
/** extend */
class RealSenseEventCenter : public Ref
{
public:

	/** Destructor of RealSenseEventCenter */
	~RealSenseEventCenter(void);

	/** Returns a shared instance of the RealSenseEventCenter */
	static RealSenseEventCenter* getInstance(); 
	

	/** Push a gesture event into EventGesture queue */
	void pushGestureEvent(const EventGesture event);

	/** Dispatch all the gesture events in the queue and clean the queue 
	 *  @note To dispatch all gesture event at each frame, you need to schedule this function in the AppDelegate.cpp.
	 */
	void tick(float);

private:

	/** Constructor of RealSenseEventCenter */
	RealSenseEventCenter(void);

	typedef std::queue<EventGesture> GestureEventQueueType;

	/** EventGesture queue */
	GestureEventQueueType _gestureEventQueue;

	/** EventGesture queue mutex */
	std::mutex _gestureEventQueueMutex;

};
NS_CC_END
#endif // defined(__CCRealSenseEventCenter_H__) 