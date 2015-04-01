#include "CCEventListenerGesture.h"
#include "base\CCEventCustom.h"
#include "base\ccMacros.h"
NS_CC_BEGIN

const char * GESTURE_EVENT_NAME = "GestureEvent";

EventListenerGesture::EventListenerGesture(void)
: onGestureThumbUp(nullptr)
, onGestureThumbDown(nullptr)
, onGestureFist(nullptr)
, onGestureSpreadFingers(nullptr)
, onGestureVSign(nullptr)
, onGestureTwoFingersPinchOpen(nullptr)
, onGestureFullPinch(nullptr)
, onGestureSwipe(nullptr)
, onGestureWave(nullptr)
, onGestureTap(nullptr)
, onHandMove(nullptr)
, onDepthImage(nullptr)
, onHandDetected(nullptr)
, onHandLost(nullptr)
, onHandOpenned(nullptr)
, onHandClosed(nullptr)
, onGestureRotation(nullptr)
, onGestureScale(nullptr)
, onTwoHandsDetected(nullptr)
, onTwoHandsLost(nullptr)
{
}

EventListenerGesture::~EventListenerGesture(void)
{
}

bool EventListenerGesture::init()
{
	auto func = [this](EventCustom* event) -> void
	{
		onEvent(event);
	};

	return EventListenerCustom::init((std::string)(GESTURE_EVENT_NAME), func);		
}
EventListenerGesture* EventListenerGesture::create()
{
	auto ret = new EventListenerGesture();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}
bool EventListenerGesture::checkAvailable()
{
	
	if (onGestureThumbUp == nullptr && onGestureThumbDown == nullptr
		&& onGestureFist == nullptr && onGestureSpreadFingers == nullptr
		&& onGestureTap == nullptr && onGestureSwipe == nullptr && onGestureWave == nullptr
		&& onGestureVSign == nullptr && onGestureTwoFingersPinchOpen == nullptr && onGestureFullPinch==nullptr
		&& onHandMove == nullptr &&onDepthImage == nullptr&&onHandDetected==nullptr&&onHandLost==nullptr
		&&onHandOpenned==nullptr&&onHandClosed==nullptr&&onGestureRotation==nullptr&&onGestureScale==nullptr
		&&onTwoHandsDetected==nullptr&&onTwoHandsLost==nullptr)
		
	{
		CCASSERT(false, "Invalid EventListenerGesture!");
		return false;
	}

	return true;
}
EventListenerGesture* EventListenerGesture::clone()
{
	auto ret = new EventListenerGesture();
	if (ret && ret->init())
	{
		ret->autorelease();

		ret->onGestureThumbUp = onGestureThumbUp;
		ret->onGestureThumbDown = onGestureThumbDown;
		ret->onGestureFist = onGestureFist;
		ret->onGestureSpreadFingers = onGestureSpreadFingers;
		ret->onGestureTap = onGestureTap;
		ret->onGestureSwipe = onGestureSwipe;
		ret->onGestureWave = onGestureWave;
		ret->onGestureVSign = onGestureVSign;
		ret->onGestureTwoFingersPinchOpen = onGestureTwoFingersPinchOpen;	
		ret->onGestureFullPinch = onGestureFullPinch;		
		ret->onHandMove = onHandMove;		
		ret->onDepthImage = onDepthImage;
		ret->onHandDetected = onHandDetected;
		ret->onHandLost = onHandLost;
		ret->onHandOpenned = onHandOpenned;
		ret->onHandClosed = onHandClosed;
		ret->onGestureRotation = onGestureRotation;
		ret->onGestureScale = onGestureScale;
		ret->onTwoHandsDetected = onTwoHandsDetected;
		ret->onTwoHandsLost = onTwoHandsLost;
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

void EventListenerGesture::onEvent(EventCustom *event)
{
	auto gestureEvent = static_cast<EventGesture *>(event->getUserData());
	
	switch (gestureEvent->getGestureType())
	{
	case GestureType::THUMB_UP:
		if (onGestureThumbUp != nullptr)
			onGestureThumbUp(event);
		break;
	case GestureType::THUMB_DOWN:
		if (onGestureThumbDown != nullptr)
			onGestureThumbDown( event);
		break;
	case GestureType::FIST:
		if (onGestureFist != nullptr)
			onGestureFist(event);
		break;
	case GestureType::SPREADFINGERS:
		if (onGestureSpreadFingers != nullptr)
			onGestureSpreadFingers(event);
		break;
	case GestureType::V_SIGN:
		if (onGestureVSign != nullptr)
			onGestureVSign( event);
		break;
	case GestureType::TWO_FINGERS_PINCH_OPEN:
		if (onGestureTwoFingersPinchOpen != nullptr)
			onGestureTwoFingersPinchOpen(event);
		break;
	case GestureType::FULL_PINCH:
		if (onGestureFullPinch != nullptr)
			onGestureFullPinch(event);
		break;
	case GestureType::TAP:
		if (onGestureTap != nullptr)
			onGestureTap(event);
		break;	
	case GestureType::WAVE:
		if (onGestureWave != nullptr)
			onGestureWave(event);
		break;
	case GestureType::SWIPE:
		if (onGestureSwipe != nullptr)
			onGestureSwipe(event);
		break;	
	case GestureType::HandMove:
		if (onHandMove != nullptr)
			onHandMove(event);
		break;	
	case GestureType::HandDetected:
		if (onHandDetected != nullptr)
			onHandDetected(event);
		break;
	case GestureType::HandLost:
		if (onHandLost != nullptr)
			onHandLost(event);
		break;
	case GestureType::HandOpenned:
		if (onHandOpenned != nullptr)
			onHandOpenned(event);
		break;
	case GestureType::HandClosed:
		if (onHandClosed != nullptr)
			onHandClosed(event);
		break;
	case GestureType::DepthImage:
		if (onDepthImage != nullptr)
			onDepthImage(event);
		break;	
	case GestureType::Rotation:
		if (onGestureRotation != nullptr)
			onGestureRotation(event);
		break;
	case GestureType::Scale:
		if (onGestureScale != nullptr)
			onGestureScale(event);
		break;
	case GestureType::TwoHandsDetected:
		if (onTwoHandsDetected != nullptr)
			onTwoHandsDetected(event);
		break;
	case GestureType::TwoHandsLost:
		if (onTwoHandsLost != nullptr)
			onTwoHandsLost(event);
		break;
	} 

}

NS_CC_END