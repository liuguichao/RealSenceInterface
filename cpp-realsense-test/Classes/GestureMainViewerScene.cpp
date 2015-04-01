#include "GestureMainViewerScene.h"
#include "AppMacros.h"
#include "pxcsensemanager.h"
#include <string>

USING_NS_CC;

//define for gesture sprite tag
const int GESTURE_TAG = 0x1000;

Scene* GestureMainViewerScene::scene()
{	
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	GestureMainViewerScene *layer = GestureMainViewerScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GestureMainViewerScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	 visibleSize = Director::getInstance()->getVisibleSize();
	 origin = Director::getInstance()->getVisibleOrigin(); 
	 

	/////////////////////////////
	// 3. add your codes below...
	 	
	 //init sprites
	 initSprites();	

	 //define and register event listener
	auto gestureListener = EventListenerGesture::create();
	gestureListener->onGestureThumbUp = CC_CALLBACK_1(GestureMainViewerScene::onGestureThumbUp, this);
	gestureListener->onGestureThumbDown = CC_CALLBACK_1(GestureMainViewerScene::onGestureThumbDown, this);
	gestureListener->onGestureFist = CC_CALLBACK_1(GestureMainViewerScene::onGestureFist, this);
	gestureListener->onGestureSpreadFingers = CC_CALLBACK_1(GestureMainViewerScene::onGestureSpreadFingers, this);	
	gestureListener->onGestureVSign = CC_CALLBACK_1(GestureMainViewerScene::onGestureVSign, this);
	gestureListener->onGestureTwoFingersPinchOpen = CC_CALLBACK_1(GestureMainViewerScene::onGestureOK, this);
	gestureListener->onHandMove = CC_CALLBACK_1(GestureMainViewerScene::onHandMove, this);
	gestureListener->onGestureFullPinch = CC_CALLBACK_1(GestureMainViewerScene::onGestureFullPinch, this);
	gestureListener->onHandDetected = CC_CALLBACK_1(GestureMainViewerScene::onHandDetected, this);
	gestureListener->onHandLost = CC_CALLBACK_1(GestureMainViewerScene::onHandLost, this);
	gestureListener->onHandOpenned = CC_CALLBACK_1(GestureMainViewerScene::onHandOpenned, this);
	gestureListener->onHandClosed = CC_CALLBACK_1(GestureMainViewerScene::onHandClosed, this);
	gestureListener->onGestureRotation = CC_CALLBACK_1(GestureMainViewerScene::onGestureRotation, this);
	gestureListener->onGestureTap = CC_CALLBACK_1(GestureMainViewerScene::onGestureTap, this);
	gestureListener->onGestureWave = CC_CALLBACK_1(GestureMainViewerScene::onGestureWave, this);
	gestureListener->onGestureScale = CC_CALLBACK_1(GestureMainViewerScene::onGestureScale, this);
	gestureListener->onTwoHandsDetected = CC_CALLBACK_1(GestureMainViewerScene::onTwoHandsDetected, this);
	gestureListener->onTwoHandsLost = CC_CALLBACK_1(GestureMainViewerScene::onTwoHandsLost, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(gestureListener, this);
	
	return true;
}
void  GestureMainViewerScene::onTwoHandsDetected(EventCustom *event)
{	
	log("twoHandsDectected");
}
void  GestureMainViewerScene::onTwoHandsLost(EventCustom *event)
{
	log("twoHandsLost");
}
void  GestureMainViewerScene::onGestureScale(EventCustom *event)
{
	
	EventGesture *eventGesture = static_cast<EventGesture*>(event->getUserData());
	float scale = eventGesture->getScaleData();
	ActionInterval * scaleBy = ScaleBy::create(0.1,scale);
	logo->runAction(scaleBy);
	log("Func::onGestureScale:%f",scale);
}

void GestureMainViewerScene::onGestureRotation(EventCustom *event)
{
	//log("Func::onGestureRotation");
	EventGesture *eventGesture = static_cast<EventGesture*>(event->getUserData());
	Vec3 rot=eventGesture->getRotationData();
	ActionInterval * rotBy = RotateBy::create(0, rot);
	logo->runAction(rotBy);
	//log("rot(%f,%f,%f)",rot.x,rot.y,rot.z);
}
void GestureMainViewerScene::onGestureFullPinch(EventCustom *event)
{	
	log("Func::onFullPinch");
}
void GestureMainViewerScene::onHandMove(EventCustom *event)
{
	
	/*EventGesture * primaryGesture = static_cast<EventGesture*>(event->getUserData());
	HandData *handData = primaryGesture->getHandData();
	double currentPosX = handData->getX() * visibleSize.width;
	double currentPosY = handData->getY() * visibleSize.height;
	log("Func::onMove,%f,%f",currentPosX,currentPosY);*/
}
void GestureMainViewerScene::onHandDetected(EventCustom *event)
{
	log("Func::detected");
}
void GestureMainViewerScene::onHandLost(EventCustom *event)
{
	log("Func::lose");
	currentGesture->setLocalZOrder(-1);
}
void GestureMainViewerScene::onHandOpenned(EventCustom *event)
{
	log("Func::HandOpenned");
}
void GestureMainViewerScene::onHandClosed(EventCustom *event)
{
	log("Func::HandClosed");
}

void GestureMainViewerScene::displayGesturePic(EventCustom *event)
{
	EventGesture *eventGesture = static_cast<EventGesture*>(event->getUserData());	
	int tag = GESTURE_TAG | (int)eventGesture->getGestureType();
	if (preGestureType != tag)
	{
		preGestureType = tag;
		currentGesture->setLocalZOrder(-1);
		currentGesture = static_cast<Sprite*>(this->getChildByTag(tag));
		currentGesture->setLocalZOrder(1);
	}
}
void GestureMainViewerScene::onGestureWave(EventCustom *event)
{
	log("Func::onGestureWave");
	displayGesturePic(event);
}
void GestureMainViewerScene::onGestureTap(EventCustom *event)
{
	log("Func::onGestureTap");
	displayGesturePic(event);
}
void GestureMainViewerScene::onGestureThumbUp(EventCustom* event)
{
	log("Func::onThumbUp");
	displayGesturePic(event);	   
}
void GestureMainViewerScene::onGestureThumbDown(EventCustom* event)
{
	log("Func::onThumbDown");
	displayGesturePic(event);
}
void GestureMainViewerScene::onGestureFist(EventCustom* event)
{
	log("Func::onFist");
	displayGesturePic(event);
}
void GestureMainViewerScene::onGestureSpreadFingers(EventCustom* event)
{
	log("Func::onSpreadFingers");
	displayGesturePic(event);
}
void GestureMainViewerScene::onGestureVSign(EventCustom* event)
{
	log("Func::onGestureVSign");
	displayGesturePic(event);
}
void GestureMainViewerScene::onGestureOK(EventCustom* event)
{
	log("Func::onGestureOK");
	displayGesturePic(event);
}

void GestureMainViewerScene::initSprites()
{
	std::string dir = "GestureMainViewerSceneRes/";
	
	logo = Sprite::create(dir+"cocosdx.png");
	logo->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));

	this->addChild(logo,2);
	
	Sprite *background = Sprite::create(dir+"background.jpg");	
	background->setAnchorPoint(Point(0.5, 0.5));
	background->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background);	


	rec = Sprite::create(dir+"none.png");
	rec->setAnchorPoint(Point(0.5, 0.5));
	rec->setPosition(Point(visibleSize.width *0.75 + (rec->getContentSize().width)*1.1, visibleSize.height* 0.1));
	
	
	this->addChild(rec);
	std::map<std::string, GestureType> imageMap;
	imageMap["spreadfingers.png"] = GestureType::SPREADFINGERS;
	imageMap["fist.png"] = GestureType::FIST;
	imageMap["thumb_up.png"] = GestureType::THUMB_UP;
	imageMap["thumb_down.png"] = GestureType::THUMB_DOWN;
	imageMap["v_sign.png"] = GestureType::V_SIGN;
	imageMap["two_fingers_pinch_open.png"] = GestureType::TWO_FINGERS_PINCH_OPEN;
	imageMap["tap.png"] = GestureType::TAP;
	imageMap["wave.png"] = GestureType::WAVE;
	for each (auto item in imageMap)
	{
		currentGesture = Sprite::create(dir+item.first);
		currentGesture->setAnchorPoint(Point(0.5, 0.5));
		currentGesture->setPosition(Point(rec->getPositionX(), rec->getPositionY()));
		this->addChild(currentGesture, -1, GESTURE_TAG | (int)item.second);
	}		
	
}
