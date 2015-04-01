#include "CCEventGesture.h"

NS_CC_BEGIN

EventGesture::EventGesture()
: Event(Event::Type::CUSTOM)
, _gestureType()
, _bodySideType()
, _handData(nullptr)
, _depthImage(nullptr)
, _rotationData(0,0,0)
, _scaleData(0)
{
}

NS_CC_END