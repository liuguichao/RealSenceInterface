#include "CCRealSenseEventCenter.h"
#include "base\CCEventCustom.h"
#include "base\CCDirector.h"
NS_CC_BEGIN

extern const char* GESTURE_EVENT_NAME;

static RealSenseEventCenter *eventCenter = nullptr;

RealSenseEventCenter* RealSenseEventCenter::getInstance()
{
	if(eventCenter == nullptr)
	{
		eventCenter = new RealSenseEventCenter();
	}
	return eventCenter;
}
RealSenseEventCenter::RealSenseEventCenter(void)
{
}


RealSenseEventCenter::~RealSenseEventCenter(void)
{
}


void RealSenseEventCenter::pushGestureEvent(const EventGesture event)
{
	_gestureEventQueueMutex.lock();
	_gestureEventQueue.push(event);
	_gestureEventQueueMutex.unlock();
}

void RealSenseEventCenter::tick(float dt) 
{	
	//dispatch Gesture event
	_gestureEventQueueMutex.lock();
	while(!_gestureEventQueue.empty())
	{
		EventCustom event(GESTURE_EVENT_NAME);
		event.setUserData(&_gestureEventQueue.front());
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

		EventGesture &gestureEventToDelete = _gestureEventQueue.front();
		HandData *handData = gestureEventToDelete.getHandData();		
		if ( handData!= nullptr)
		{
			handData->release();
		}
		_gestureEventQueue.pop();
	}
	_gestureEventQueueMutex.unlock();
}

NS_CC_END