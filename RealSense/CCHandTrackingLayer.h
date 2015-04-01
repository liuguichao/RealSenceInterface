#ifndef __HandTrackingLayer_H__
#define __HandTrackingLayer_H__

#include "cocos2d.h"
#include "CCEventGesture.h"

/**
 *  This class define a cursor node with an image which can track hand's position and simulate mouse interaction, and a additional area to display depth stream.
 *
 *  Usage:
 *     Adds a cursor to an application (you need to set the cursorUpImgPath, cursorDownImgPath and progressBarResPath in the your AppDelegate.cpp):
 *
 *			HandTrackingLayer *trackingLayer = HandTrackingLayer::create(const std::string &cursorUpImgPath, const std::string &cursorDownImgPath, const std::string &progressBarResPath);
 *			trackingLayer->setDepthImagePositon(Cursor::DepthImagePosition::Center,0.5);// optional setting,needed only if want to display depth stream realtime.
 *			auto pDirector = Director::getInstance(); 
 *			pDirector->setNotificationNode(trackingLayer);
 */

NS_CC_BEGIN

/** If cursor position's change is no more than this range in DELAY_TIME, it will
 *  automatically dispatch a touch begin event and touch end event on current cursor position.
 */
const static float CURSOR_ONCLICK_RANGE = 20.0f;
const static int DELAY_TIME = 30;


class HandTrackingLayer : public Layer
{
public:

	/// Overrides
	virtual ~HandTrackingLayer(){};

	/** Create a HandTrackingLayer
	 *  @param cursorUpImgPath The path of click up image. eg.(cursor/click_up.png)
	 *  @param cursorDownImgPath  The path of click down image. eg.(cursor/click_down.png)
	 *  @param progressBarResPath The directory to store progressBar images. This path is different because we need more than one images
     *							  to create an animation of progressBar. eg.(cursor). CCCursor.cpp shows how to create an animation of a 
	 *							  progressBar.
	 */
	static HandTrackingLayer *create(const std::string &cursorUpImgPath, const std::string &cursorDownImgPath, const std::string &progressBarResPath);

	/**Depth image' display position */
	enum class DepthImagePosition
	{
		LeftTop,
		RightTop,
		LeftBottom,
		RightBottom,
		Center
	};

	/** Set depth image' display position on screen
	 *  @param position, the relative position to anchor.
	 *  @param scale, the scale to zoom depth image. if the scale factor exceed the fullscreen,it will keep fullscreen scale to display.  
	*/
	inline void setDepthImagePositon(DepthImagePosition position, float scale){	_dpPosition = position;	_dpScale = scale;	};

private:

	/** Callback function onHandMove of gesture listener*/
	void onHandMove(EventCustom* event);		

	/** Callback function onDepthImage of gesture listener*/
	void onDepthImage(EventCustom *event);
	
	/** Callback function onOpennessChange of gesture listener*/
	void onOpennessChange(EventGesture* event);	
	

	/// Overrides
	virtual bool init(const std::string &cursorUpImgPath, const std::string &cursorDownImgPath, const std::string &progressBarResPath);

	/** This function will create and dispatch a touch begin event and a touch end event*/
	void dispatchTouchBeginAndTouchEndEvent();

	/** Handle and dispatch touch begin events*/
	void handleTouchesBegin(int num, int ids[], float xs[], float ys[]);

	/** Handle and dispatch touch move events*/
	void handleTouchesMove(int num, int ids[], float xs[], float ys[]);

	/** Handle and dispatch touch end events*/
	void handleTouchesEnd(int num, int ids[], float xs[], float ys[]);

private:
	
	std::function<void()> dispatch;
	/** The width of hand tracking layer
	 *  @note Cursor sprite is a child of hand tracking layer
	 */
	float _handTrackingLayerWidth;
	/** The height of hand tracking layer
	 *  @note Cursor sprite is a child of hand tracking layer
	 */
	float _handTrackingLayerHeight;

	//The sprite to draw depth image
	Sprite* _depthImage;
	//The relative position to anchor depth image sprite
	DepthImagePosition _dpPosition;
	//The scale factor to zoom depth image sprite
	float _dpScale;
	//Texture to render depth image
	Texture2D* _tex;

	//Previous openness 
	int _preOpenness;
	
	//The cursor display as a small hand
	Sprite *_cursor;

	//The progressbar to simulate button click
	Sprite *_progressBar;

	//hyaline layer to hold cursor and progressBar
	Layer *_trackingLayer;	
	double _cursorPosX;
	double _cursorPosY;
	int _cursorDelayFrameNum;
	bool _isActionRunning;
	bool _isTouchBegin;
	std::string _cursorUpImgPath, _cursorDownImgPath;

};

NS_CC_END

#endif  //defined(__HandTrackingLayer_H__)