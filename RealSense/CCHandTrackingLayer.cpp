#include "CCHandTrackingLayer.h"
//#include "CCRSCamera.h"
#include "CCEventListenerGesture.h"
#include "CCGestureAttribute.h"

NS_CC_BEGIN

const int CLICK_DOWN_THRESHOLD = 30;//if hand openness value is less than this value, it will send a touch begin event
const int CLICK_UP_THRESHOLD = 60;//if hand openness value is greater than this value,meanwhile there had send a touch begin event, it will send a touch end event
const int OPENNESS_CHANGE_THRESHOLD = 5;//threshold to determine whether openness changed

namespace {

	static Touch* touches[EventTouch::MAX_TOUCHES] = { nullptr };
	static unsigned int indexBitsUsed = 0;
	// System touch pointer ID (It may not be ascending order number) <-> Ascending order number from 0
	static std::map<int, int> touchIdReorderMap;
	static int getUnusedIndex()
	{
		int i;
		int temp = indexBitsUsed;
		for (i = 0; i < EventTouch::MAX_TOUCHES; i++) {
			if (! (temp & 0x00000001)) {
				indexBitsUsed |= (1 <<  i);
				return i;
			}
			temp >>= 1;
		}

		// all bits are used
		return -1;
	}
	static void removeUsedIndexBit(int index)
	{
		if (index < 0 || index >= EventTouch::MAX_TOUCHES)
		{
			return;
		}
		unsigned int temp = 1 << index;
		temp = ~temp;
		indexBitsUsed &= temp;
	}
}

HandTrackingLayer* HandTrackingLayer::create(const std::string &cursorUpImgPath, const std::string &cursorDownImgPath, const std::string &progressBarResPath)
{
	HandTrackingLayer *cursor = new HandTrackingLayer();
	if(cursor && cursor->init(cursorUpImgPath, cursorDownImgPath, progressBarResPath))
	{
		cursor->autorelease();
		return cursor;
	}
	CC_SAFE_DELETE(cursor);
	return nullptr;
}

bool HandTrackingLayer::init(const std::string &cursorUpImgPath, const std::string &cursorDownImgPath, const std::string &progressBarResPath)
{

	if ( !Layer::init() )
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	_handTrackingLayerWidth = visibleSize.width;
	_handTrackingLayerHeight = visibleSize.height;
	
	
	_cursorDelayFrameNum = DELAY_TIME; 
	_isActionRunning = false;
	_isTouchBegin = false;
	_cursorUpImgPath = cursorUpImgPath;
	_cursorDownImgPath = cursorDownImgPath;

	//init cursor sprite and cursorLayer
	_trackingLayer = LayerColor::create(Color4B(0xFF, 0xFF, 0xFF, 0), _handTrackingLayerWidth, _handTrackingLayerHeight);
	_cursor = Sprite::create(_cursorUpImgPath);
	_cursor->setOpacity(255);
	_cursor->setScale(0.5);
	_cursor->setPosition(_handTrackingLayerWidth / 2, _handTrackingLayerHeight / 2);
	_trackingLayer->addChild(_cursor, 2);
		

	//init progressBar
	_progressBar = Sprite::create();
	std::stringstream ss;
	std::string str;
	Vector<SpriteFrame*> frames;

	//create an animation of progressBar
	int progressBarResNum = 14;
	for (int i = 0; i < progressBarResNum; i++)  
	{  
		ss<<i;
		ss>>str;
		std::string fileName = progressBarResPath + "/bar" + str + ".png";
		Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(fileName);
		Rect rect = Rect::ZERO;
		rect.size = texture->getContentSize();
		SpriteFrame *frame = SpriteFrame::createWithTexture(texture, rect);
		frames.pushBack(frame);
		ss.clear();
	} 
	
	Animation *animation = Animation::createWithSpriteFrames(frames,1 / 5.0);  
	auto cache = AnimationCache::getInstance();
	cache->addAnimation(animation,"progressBar");

	_progressBar->setPosition(_handTrackingLayerWidth/2, _handTrackingLayerHeight/2);
	_progressBar->resume();
	_progressBar->setScale(0.75);
	_trackingLayer->addChild(_progressBar);

	this->dispatch = CC_CALLBACK_0(HandTrackingLayer::dispatchTouchBeginAndTouchEndEvent, this);

	//register eventListener
	EventListenerGesture *gestureListener = EventListenerGesture::create();
	gestureListener->onHandMove = CC_CALLBACK_1(HandTrackingLayer::onHandMove, this);
	gestureListener->onDepthImage = CC_CALLBACK_1(HandTrackingLayer::onDepthImage, this);
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(gestureListener, this);

	this->addChild(_trackingLayer);
	this->resume();

	//init variable 
	_depthImage = nullptr;
	_tex = nullptr;
	_preOpenness = -1;
	_dpPosition = DepthImagePosition::LeftBottom;
	_dpScale = 0.3f;

	return true;
}

void HandTrackingLayer::onDepthImage(EventCustom* event)
{
	EventGesture *eventGesture = static_cast<EventGesture*>(event->getUserData());

	Image *image = eventGesture->getDepthImage();

	if (_depthImage){
		_depthImage->removeFromParentAndCleanup(false);
	}
	
	Texture2D *tex = new Texture2D();                                                                                                                                                    
	
	tex->initWithImage(image);
	_depthImage = Sprite::createWithTexture(tex);
	_depthImage->setRotationSkewY(180);
	float imageWidth = _depthImage->getContentSize().width;
	float imageHeight = _depthImage->getContentSize().height;
	
	float scaleX = _handTrackingLayerWidth / imageWidth;
	float scaleY = _handTrackingLayerHeight / imageHeight;
	float min = scaleX < scaleY ? scaleX : scaleY;
	if (_dpScale<min)
	{
		scaleX = scaleY = _dpScale;
	}
	_depthImage->setScale(scaleX, scaleY);		
	
	
	if (_dpPosition==DepthImagePosition::LeftBottom)
		_depthImage->setPosition(Point(scaleX*imageWidth / 2, scaleX*imageHeight/2));	
	else if (_dpPosition == DepthImagePosition::RightBottom)
		_depthImage->setPosition(Point(_handTrackingLayerWidth - scaleX*imageWidth / 2, scaleX*imageHeight / 2));
	else if (_dpPosition == DepthImagePosition::LeftTop)
		_depthImage->setPosition(Point(scaleX*imageWidth / 2, _handTrackingLayerHeight - scaleX*imageHeight / 2));
	else if (_dpPosition == DepthImagePosition::RightTop)
		_depthImage->setPosition(Point(_handTrackingLayerWidth - scaleX*imageWidth / 2, _handTrackingLayerHeight - scaleX*imageHeight / 2));
	else if (_dpPosition == DepthImagePosition::Center)
		_depthImage->setPosition(Point(_handTrackingLayerWidth / 2, _handTrackingLayerHeight / 2));

	_trackingLayer->addChild(_depthImage);

	tex->release();
	image->release();
}
void HandTrackingLayer::onOpennessChange(EventGesture* event)
{	
	HandData *gestureData = event->getHandData();
	if (gestureData->getOpenness()<CLICK_DOWN_THRESHOLD&&!_isTouchBegin)
	{
		_isTouchBegin = true;
		GLView *glview = Director::getInstance()->getOpenGLView();
		float singleTouchX = (float)gestureData->getX() * glview->getFrameSize().width;
		float singleTouchY = glview->getFrameSize().height - (float)gestureData->getY() * glview->getFrameSize().height;
		if (glview->getViewPortRect().equals(Rect::ZERO) || glview->getViewPortRect().containsPoint(Point(singleTouchX, singleTouchY)))
		{
			int id = 0;
			handleTouchesBegin(1, &id, &singleTouchX, &singleTouchY);
		}

		//change cursor img
		_cursor->setTexture(_cursorDownImgPath);
		_cursor->setScale(0.3f);
		//stop processBar actions.
		_progressBar->stopAllActions();
		_cursorDelayFrameNum = DELAY_TIME;
		_isActionRunning = false;
		_progressBar->setOpacity(0);
	}
	if (gestureData->getOpenness()>CLICK_UP_THRESHOLD &&_isTouchBegin)
	{
		//dispatch touch end event
		_isTouchBegin = false;
		GLView *glview = Director::getInstance()->getOpenGLView();
		float singleTouchX = (float)gestureData->getX() * glview->getFrameSize().width;
		float singleTouchY = glview->getFrameSize().height - (float)gestureData->getY() * glview->getFrameSize().height;

		if (glview->getViewPortRect().equals(Rect::ZERO) || glview->getViewPortRect().containsPoint(Point(singleTouchX, singleTouchY)))
		{
			int id = 0;
			handleTouchesEnd(1, &id, &singleTouchX, &singleTouchY);
		}
		_cursor->setTexture(_cursorUpImgPath);
		_cursor->setScale(0.5f);
	}
}
void HandTrackingLayer::onHandMove(EventCustom* event)
{
	EventGesture * primaryGesture = static_cast<EventGesture*>(event->getUserData());
	HandData *handData = primaryGesture->getHandData();
	double currentPosX = handData->getX() * _handTrackingLayerWidth;
	double currentPosY = handData->getY() * _handTrackingLayerHeight;
	
	//dispach opennessChange event
	if (_preOpenness > 0)
	{
		int diff = handData->getOpenness() - _preOpenness;
		if (diff > OPENNESS_CHANGE_THRESHOLD||diff<-OPENNESS_CHANGE_THRESHOLD)
			onOpennessChange(primaryGesture);
	}
	_preOpenness = handData->getOpenness();	

	_cursor->setPosition(Point(currentPosX, currentPosY));
	_progressBar->setPosition(Point(currentPosX, currentPosY));

	double differX = abs(currentPosX - _cursorPosX);
	double differY = abs(currentPosY - _cursorPosY);

	if ((differX < CURSOR_ONCLICK_RANGE) && (differY < CURSOR_ONCLICK_RANGE) && !_isActionRunning && !_isTouchBegin)
	{
		-- _cursorDelayFrameNum;
	}
	else if((differX >= CURSOR_ONCLICK_RANGE ) || (differY >= CURSOR_ONCLICK_RANGE) && !_isTouchBegin )
	{
		if (_isActionRunning)
		{
			_progressBar->stopAllActions();
			_cursorDelayFrameNum = DELAY_TIME;
			_isActionRunning = false;
			_progressBar->setOpacity(0);
		}
		else
		{
			_cursorDelayFrameNum = DELAY_TIME;
		}
	}
	
	if(_cursorDelayFrameNum <= 0 && !_isActionRunning && !_isTouchBegin)
	{
		_progressBar->setOpacity(255);
		auto cache = AnimationCache::getInstance();
		Animation *animation = cache->getAnimation("progressBar");
		Animate *progressBarAnimate = Animate::create(animation);
		Sequence *sequence = Sequence::create(Show::create(), progressBarAnimate, Hide::create(), CallFunc::create(dispatch), NULL);
 		_progressBar->runAction(sequence);
		_progressBar->resume();
		_isActionRunning = true;
		_cursorDelayFrameNum = DELAY_TIME;
	}
	_cursorPosX = currentPosX;
	_cursorPosY = currentPosY;

	//dispatch touch move event
	if(_isTouchBegin)
	{
		//dispatchTouchEvent(EventTouch::EventCode::MOVED);

		GLView *glview = Director::getInstance()->getOpenGLView();
		float singleTouchX = (float)primaryGesture->getHandData()->getX() * glview->getFrameSize().width;
		float singleTouchY = glview->getFrameSize().height - (float)primaryGesture->getHandData()->getY() * glview->getFrameSize().height;

		if (glview->getViewPortRect().equals(Rect::ZERO) || glview->getViewPortRect().containsPoint(Point(singleTouchX,singleTouchY)))
		{
			int id = 0;
			handleTouchesMove(1, &id, &singleTouchX, &singleTouchY);
		}
	}
}

void HandTrackingLayer::dispatchTouchBeginAndTouchEndEvent()
{
	GLView *glview = Director::getInstance()->getOpenGLView();

	float singleTouchX = (_cursorPosX / _handTrackingLayerWidth) * glview->getFrameSize().width;
	float singleTouchY =  glview->getFrameSize().height - (_cursorPosY / _handTrackingLayerHeight) * glview->getFrameSize().height;	

	if(!_isTouchBegin)
	{	
		if (glview->getViewPortRect().equals(Rect::ZERO) || glview->getViewPortRect().containsPoint(Point(singleTouchX, singleTouchY)))
		{
			EventTouch touchEvent;
			Touch* touch = new Touch();
			touch->setTouchInfo(0, (singleTouchX - glview->getViewPortRect().origin.x) / glview->getScaleX(),
							   (singleTouchY - glview->getViewPortRect().origin.y) / glview->getScaleY());

			std::vector<Touch*> touches = touchEvent.getTouches();
			touches.push_back(touch);
			touchEvent.setTouches(touches);

			touchEvent.setEventCode(EventTouch::EventCode::BEGAN);
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&touchEvent);
			touchEvent.setEventCode(EventTouch::EventCode::ENDED);
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&touchEvent);
			touch->release();
		}
	}

	_cursorDelayFrameNum = DELAY_TIME;
	_isActionRunning = false;

}


void HandTrackingLayer::handleTouchesBegin(int num, int ids[], float xs[], float ys[])
{	

	GLView *glview = Director::getInstance()->getOpenGLView();
	if (glview == nullptr)
	{
		return;
	}	
	int id = 0;
	float x = 0.0f;
	float y = 0.0f;
	int unusedIndex = 0;
	EventTouch touchEvent;
	for (int i = 0; i < num; ++i)
	{
		id = ids[i];
		x = xs[i];
		y = ys[i];
		auto iter = touchIdReorderMap.find(id);
		unusedIndex = 0;
		// it is a new touch
		if (iter == touchIdReorderMap.end())
		{
			unusedIndex = getUnusedIndex();
			// The touches is more than MAX_TOUCHES ?
			if (unusedIndex == -1) {
				log("The touches is more than MAX_TOUCHES, unusedIndex = %d", unusedIndex);
				continue;
			}
			Touch* touch = touches[unusedIndex] = new Touch();
			touch->setTouchInfo(unusedIndex, (x - glview->getViewPortRect().origin.x) / glview->getScaleX(),
				(y - glview->getViewPortRect().origin.y) / glview->getScaleY());
			touchIdReorderMap.insert(std::make_pair(id, unusedIndex));
			std::vector<Touch*> touches = touchEvent.getTouches();
			touches.push_back(touch);
			touchEvent.setTouches(touches);
		}
	}
	if (touchEvent.getTouches().size() == 0)
	{
		log("touchesBegan: size = 0");
		return;
	}
	touchEvent.setEventCode(EventTouch::EventCode::BEGAN);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&touchEvent);
}


void HandTrackingLayer::handleTouchesEnd(int num, int ids[], float xs[], float ys[])
{
	int id = 0;
	float x = 0.0f;
	float y = 0.0f;
	EventTouch touchEvent;
	for (int i = 0; i < num; ++i)
	{
		id = ids[i];
		x = xs[i];
		y = ys[i];
		auto iter = touchIdReorderMap.find(id);
		if (iter == touchIdReorderMap.end())
		{
			log("if the index doesn't exist, it is an error");
			continue;
		}
		/* Add to the set to send to the director */
		Touch* touch = touches[iter->second];
		GLView *glview = Director::getInstance()->getOpenGLView();
		if (touch)
		{
			CCLOGINFO("Ending touches with id: %d, x=%f, y=%f", id, x, y);
			touch->setTouchInfo(iter->second, (x - glview->getViewPortRect().origin.x) / glview->getScaleX(),
				(y - glview->getViewPortRect().origin.y) / glview->getScaleY());
			std::vector<Touch*> touches = touchEvent.getTouches();
			touches.push_back(touch);
			touchEvent.setTouches(touches);
			touches[iter->second] = nullptr;
			removeUsedIndexBit(iter->second);
			touchIdReorderMap.erase(id);
		} 
		else
		{
			log("Ending touches with id: %d error", id);
			return;
		} 

	}
	if (touchEvent.getTouches().size() == 0)
	{
		log("touchesEnded or touchesCancel: size = 0");
		return;
	}

	touchEvent.setEventCode(EventTouch::EventCode::ENDED);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&touchEvent);
}
void HandTrackingLayer::handleTouchesMove(int num, int ids[], float xs[], float ys[])
{
	int id = 0;
	float x = 0.0f;
	float y = 0.0f;
	EventTouch touchEvent;

	for (int i = 0; i < num; ++i)
	{
		id = ids[i];
		x = xs[i];
		y = ys[i];
		auto iter = touchIdReorderMap.find(id);
		if (iter == touchIdReorderMap.end())
		{
			log("if the index doesn't exist, it is an error");
			continue;
		}
		CCLOGINFO("Moving touches with id: %d, x=%f, y=%f", id, x, y);
		Touch* touch = touches[iter->second];
		GLView *glview = Director::getInstance()->getOpenGLView();
		if (touch)
		{
			touch->setTouchInfo(iter->second, (x - glview->getViewPortRect().origin.x) / glview->getScaleX(),
				(y - glview->getViewPortRect().origin.y) / glview->getScaleY());

			std::vector<Touch*> touches = touchEvent.getTouches();
			touches.push_back(touch);
			touchEvent.setTouches(touches);
		}
		else
		{
			// It is error, should return.
			log("Moving touches with id: %d error", id);
			return;
		}
	}

	if (touchEvent.getTouches().size() == 0)
	{
		log("touchesMoved: size = 0");
		return;
	}

	touchEvent.setEventCode(EventTouch::EventCode::MOVED);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&touchEvent);
}

NS_CC_END