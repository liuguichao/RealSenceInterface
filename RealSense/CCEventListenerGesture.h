#ifndef __CCEventListenerGesture_H__
#define __CCEventListenerGesture_H__


#include "base\CCEventListenerCustom.h"
#include "CCEventGesture.h"
#include "RealSense\CCHandTrackingLayer.h"

/**
 * This class defines a customized gesture event listener.
 * 
 *
 *  Usage:
 *
 *     Adds a listener:
 *	      auto dispatcher = Director::getInstance()->getEventDispatcher();
 *        auto listener = EventListenerGesture::create();
 *        dispatcher->addEventListenerWithSceneGraphPriority(listener, one_node);
 *
 *	   Binding listener with callback function:
 *		  auto callback = [](EventCustom* event){ do_some_thing(); };
 *		  listener->one_callback_function = callback;
 *
 *     Dispatches a gesture event:
 *		  extern const char* GESTURE_EVENT_NAME;
 *        dispatcher->dispatchCustomEvent(GESTURE_EVENT_NAME, one_EventGesture);
 *
 *     Removes a listener:
 *        dispatcher->removeEventListener(listener);
 */
NS_CC_BEGIN

class EventListenerGesture : public EventListenerCustom
{

public:		

	 /** Creates a gesture event listener.*/
	static EventListenerGesture* create();

	/// Overrides
	virtual bool checkAvailable() override;

	virtual EventListenerGesture* clone() override;

public:

	std::function<void(EventCustom*)> onGestureThumbUp;
	std::function<void(EventCustom*)> onGestureThumbDown;
	std::function<void(EventCustom*)> onGestureFist;
	std::function<void(EventCustom*)> onGestureSpreadFingers;
	std::function<void(EventCustom*)> onGestureVSign;
	std::function<void(EventCustom*)> onGestureTwoFingersPinchOpen;
	std::function<void(EventCustom*)> onGestureFullPinch;
	std::function<void(EventCustom*)> onGestureSwipe;
	std::function<void(EventCustom*)> onGestureWave;
	std::function<void(EventCustom*)> onGestureTap;
	std::function<void(EventCustom*)> onHandMove;		
	std::function<void(EventCustom*)> onHandDetected;
	std::function<void(EventCustom*)> onHandLost;
	std::function<void(EventCustom*)> onHandOpenned;
	std::function<void(EventCustom*)> onHandClosed;
	std::function<void(EventCustom*)> onGestureRotation;
	std::function<void(EventCustom*)> onGestureScale;
	std::function<void(EventCustom*)> onTwoHandsLost;
	std::function<void(EventCustom*)> onTwoHandsDetected;
protected:

	/** Init a gesture event listener*/
	bool init();

	/** Callback function of EventListenerCustom which will encapsulate EventListenerGesture*/
	void onEvent(EventCustom *event);

	/** Constructor of EventListenerGesture */
	EventListenerGesture(void);

	/** Destructor of EventListenerGesture */
	~EventListenerGesture(void);

	/** Hide this callback from general class,only expose to HandTrackingLayer interface */
	std::function<void(EventCustom*)> onDepthImage;

	friend class HandTrackingLayer;
};
NS_CC_END

#endif //defined(__CCEventListenerGesture_H__)