#ifndef __CCHandRules_H__
#define __CCHandRules_H__
#include<pxcsensemanager.h>
#include <pxchanddata.h>
#include "CCGestureAttribute.h"
#include <map>
#include<string>
#include<vector>
#include "CCHandActions.h"
NS_CC_BEGIN

//Rule base - Base class for all rules.
class BaseRule
{
public:
	BaseRule(PXCHandData *pxcHandData);	
	//check whether the rule is triggered,if triggered,dispatch an event.
	virtual void process()=0;	
public:
	//which hand to detect or track first.
	PXCHandData::AccessOrderType _whichHand;
	// the index of the hand can change once more hands are visible. Should continuously track the detected hand no matter its' index
	bool _continuousTracking;	
	//whether enable this rule
	bool enabled;
protected:
	// interface manages the hand tracking module output data
	PXCHandData * _pxcHandData;
	//the index of the hand.
	int _handIndex;
	int _uniqueID;
	// store if in the last frame a hand was detected.
	bool _lastFrameDetected;	
};
class HandOpennedRule:public BaseRule
{
public:
	HandOpennedRule(PXCHandData * pxcHandData);	
	void process();
public:
	//the threshold to determine hand openned or closed
	int _opennessFactor;
};
class HandClosedRule :public BaseRule
{
public:
	HandClosedRule(PXCHandData * pxcHandData);
	void process();
public:
	//the threshold to determine hand openned or closed
	int _opennessFactor;
private:
	int _closeCounter;
	
};
class HandLostRule :public BaseRule
{
public:
	HandLostRule(PXCHandData * pxcHandData);
	void process();
};
class HandDetectedRule :public BaseRule
{
public:
	HandDetectedRule(PXCHandData * pxcHandData);
	void process();
};
class TwoHandsLostRule :public BaseRule
{
public:
	TwoHandsLostRule(PXCHandData * pxcHandData);
	void process();
private:
	bool _twoHandsDetected;
};
class TwoHandsDetectedRule :public BaseRule
{
public:
	TwoHandsDetectedRule(PXCHandData * pxcHandData);
	void process();
private:
	bool _twoHandsDetected;
};
class GestureDetectedRule: public BaseRule
{
public:
	GestureDetectedRule(PXCHandData * pxcHandData);
	void process();
private:
	/** Map for query GestureType by gesture name */
	std::map<std::wstring, GestureType> _gestureMap;
};
class HandRotateRule: public BaseRule
{
public:
	//A minimum distance between hands/fingers to start rotation around the X axis
	float DistanceThresholdX;
	//A minimum distance between hands/fingers to start rotation around the Y axis.
	float DistanceThresholdY;
	//A minimum distance between hands / fingers to start rotation around the Z axis.
	float DistanceThresholdZ;
	HandRotateRule(PXCHandData *pxcHandData);
	void process();
	inline void setFreeze(bool freezeX, bool freezeY, bool freezeZ){ _rotationAction->Constraints.Freeze.X = freezeX;  _rotationAction->Constraints.Freeze.Y = freezeY;  _rotationAction->Constraints.Freeze.Z = freezeZ; };
	~HandRotateRule();
private:
	float _roll;
	float _pitch;
	float _yaw;	
	//record roatation data and transfer to RotationAction
	RotationData _rotationData;
	//if detect the rotate movement,dispatch an event in RotationAction
	RotationAction * _rotationAction;
};
class HandScaleRule : public BaseRule
{
public:
	HandScaleRule(PXCHandData *pxcHandData);
	void process();
public:
	//A minimum distance between hands/fingers to start rotation around the X axis
	float DistanceThresholdX;
	float _scale;
		

	bool _begin = false, _end = false;
	int delay = 3;
	int counter = delay;
	int threshold = 5;
	int ScaleDumpingFactor = 10;
	float xCurDiff1, xCurDiff2;

};
class DrawImageRule
{
public:
	DrawImageRule(PXCSenseManager *pxcSenseManager);
	~DrawImageRule();

	void process();
	inline void setImageOpacity(unsigned char imageOpacity){ _dpImageOpacity = imageOpacity; };
private:
	/** Convert image format
	* @param pSrc, the source data address.
	* @param iSize, the source data size.
	* @param pDst, the destination address to store converted data.
	*/
	void convertTo8bpp(unsigned short * pSrc, int iSize, unsigned char * pDst);
	void clearBuffer(PXCImage::ImageInfo info);
	void releaseVectorImages();
public:
	bool enabled;
private:
	PXCSenseManager *_pxcSenseManager;
	/**The opacity of depth image ,range from 0 to 255,0 is transparent background,255 is black backgroud*/
	unsigned char _dpImageOpacity;
	/** vector containing depth image - for synchronization purposes*/
	std::vector<PXCImage *> _depthImages;
	/** image conversion buffer*/
	pxcI32* _buffer;
	pxcI32 _bufferSize;
	unsigned char *_charBuffer;
	float _maxRangeValue;
	//define for depth image display buffer
	 int NUMBER_OF_FRAMES_TO_DELAY;
};


class HandMoveRule:public BaseRule
{
public:
	HandMoveRule(PXCHandData * pxcHandData);
	void process();
private:
	void convertCoordinates(const PXCPoint3DF32 &raw, Vec3 &target);
private:
	//the camera depth stream size info
	const int DEPTH_WIDTH ;
	const int DEPTH_HEIGHT;
	//camera min and max traking distance, the unit is m
	const float GESTURE_RANGE_MIN_Z;	
	const float GESTURE_RANGE_MAX_Z;
	
};
NS_CC_END
#endif  //defined(__CCHandRules_H__)