#ifndef __GESTUREMAINVIEWER_SCENE_H__
#define __GESTUREMAINVIEWER_SCENE_H__

#include "cocos2d.h"
#include "base\CCEventCustom.h"
#include "RealSense\CCEventGesture.h"
#include "RealSense\CCEventListenerGesture.h"

USING_NS_CC;
class GestureMainViewerScene : public cocos2d::Layer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(GestureMainViewerScene);	

private:	
	//create sprites, add to layer
	void initSprites();

	//event handle functions
	void onGestureThumbUp(EventCustom* event);
	void onGestureThumbDown(EventCustom* event);
	void onGestureFist(EventCustom* event);
	void onGestureSpreadFingers(EventCustom* event);	
	void onGestureVSign(EventCustom* event);
	void onGestureOK(EventCustom* event);
	void onHandMove(EventCustom* event);	
	void onGestureFullPinch(EventCustom* event);
	void onGestureWave(EventCustom* event);
	void onGestureTap(EventCustom* event);
	void onHandDetected(EventCustom* event);
	void onHandLost(EventCustom* event);
	void onHandOpenned(EventCustom* event);
	void onHandClosed(EventCustom* event);
	void onGestureRotation(EventCustom* event);
	void onGestureScale(EventCustom* event);
	void onTwoHandsDetected(EventCustom *event);
	void onTwoHandsLost(EventCustom *event);

	void displayGesturePic(EventCustom *event);
private:

	//layer size and origin
	Size visibleSize;	
	Point origin;	
	
	//which area to display gesture image
	Sprite *rec;	
	//which gesture image to dispaly
	Sprite *currentGesture;
	//previous gesture name tag
	int preGestureType;

	//item to demostrate rotation and scale
	Sprite *logo;
};

#endif // __GESTUREMAINVIEWER_SCENE_H__
