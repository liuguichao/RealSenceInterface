#include "CCHandRules.h"
#include "CCEventGesture.h"
#include "CCRealSenseEventCenter.h"

NS_CC_BEGIN

BaseRule::BaseRule(PXCHandData * pxcHandData)
: _pxcHandData(pxcHandData)
, _whichHand(PXCHandData::AccessOrderType::ACCESS_ORDER_NEAR_TO_FAR)
, _handIndex(0)
, _continuousTracking(false)
, _uniqueID(-1)
, _lastFrameDetected(false)
, enabled(false)
{

}
HandOpennedRule::HandOpennedRule(PXCHandData * pxcHandData)
:BaseRule(pxcHandData)
, _opennessFactor(50)
{
	
}
void HandOpennedRule::process()
{	
	PXCHandData::IHand *data = 0;

	if (_pxcHandData->QueryNumberOfHands() > 0 &&
		((_continuousTracking&&_pxcHandData->QueryHandDataById(_uniqueID, data) >= pxcStatus::PXC_STATUS_NO_ERROR)
		|| _pxcHandData->QueryHandData(PXCHandData::ACCESS_ORDER_NEAR_TO_FAR, _handIndex, data) >= pxcStatus::PXC_STATUS_NO_ERROR))
	{
		_uniqueID = data->QueryUniqueId();
		PXCHandData::JointData jointD;
		data->QueryTrackedJoint(PXCHandData::JointType::JOINT_PINKY_JT2, jointD);

		if (jointD.confidence < 100) return;			
		
		if (data->QueryOpenness() <= _opennessFactor)
			_lastFrameDetected = true;		
		else
		{
			if (_lastFrameDetected)
			{
				EventGesture gestureEvent;
				gestureEvent._gestureType = GestureType::HandOpenned;
				RealSenseEventCenter::getInstance()->pushGestureEvent(gestureEvent);
			}

			_lastFrameDetected = false;
		}
	}
	else
	{
		_lastFrameDetected = false;
	}

}
	
HandClosedRule::HandClosedRule(PXCHandData * pxcHandData)
:BaseRule(pxcHandData)
, _closeCounter(0)
, _opennessFactor(50)
{

}
void HandClosedRule::process()
{	
	PXCHandData::IHand *data = 0;

	if (_pxcHandData->QueryNumberOfHands() > 0 &&
		((_continuousTracking&&_pxcHandData->QueryHandDataById(_uniqueID, data) >= pxcStatus::PXC_STATUS_NO_ERROR)
		|| _pxcHandData->QueryHandData(PXCHandData::ACCESS_ORDER_NEAR_TO_FAR, _handIndex, data) >= pxcStatus::PXC_STATUS_NO_ERROR))
	{
		_uniqueID = data->QueryUniqueId();
		PXCHandData::JointData jointD;
		data->QueryTrackedJoint(PXCHandData::JointType::JOINT_INDEX_TIP, jointD);
		if (jointD.confidence < 100)
		{
			_closeCounter = 0;
			return;
		}
		if (data->QueryOpenness() <= _opennessFactor)
		{
			if (!_lastFrameDetected)
			{
				_closeCounter++;

				if (_closeCounter > 3)
				{
					EventGesture gestureEvent;
					gestureEvent._gestureType = GestureType::HandClosed;
					RealSenseEventCenter::getInstance()->pushGestureEvent(gestureEvent);
					
					_lastFrameDetected = true;
				}
			}
		}
		else
		{
			_closeCounter = 0;
			_lastFrameDetected = false;
		}
	}
	else
	{
		_closeCounter = 0;
		_lastFrameDetected = false;
	}
	
}
HandLostRule::HandLostRule(PXCHandData *pxcHandData)
:BaseRule(pxcHandData)
{

}
void HandLostRule::process()
{
	PXCHandData::IHand *data = 0;	

	if (_pxcHandData->QueryNumberOfHands() > 0 &&
		((_continuousTracking&&_pxcHandData->QueryHandDataById(_uniqueID, data) >= pxcStatus::PXC_STATUS_NO_ERROR)
		|| _pxcHandData->QueryHandData(PXCHandData::ACCESS_ORDER_NEAR_TO_FAR, _handIndex, data) >= pxcStatus::PXC_STATUS_NO_ERROR))
	{
		_uniqueID = data->QueryUniqueId();		
		_lastFrameDetected = true;
	}
	else
	{
		if (_lastFrameDetected)
		{
			EventGesture gestureEvent;
			gestureEvent._gestureType = GestureType::HandLost;
			RealSenseEventCenter::getInstance()->pushGestureEvent(gestureEvent);
		}
		_lastFrameDetected = false;
	}
	
}
HandDetectedRule::HandDetectedRule(PXCHandData *pxcHandData)
:BaseRule(pxcHandData)
{

}
void HandDetectedRule::process()
{
	PXCHandData::IHand *data = 0;
	

	if (_pxcHandData->QueryNumberOfHands() > 0 &&
		((_continuousTracking&&_pxcHandData->QueryHandDataById(_uniqueID, data) >= pxcStatus::PXC_STATUS_NO_ERROR)
		|| _pxcHandData->QueryHandData(PXCHandData::ACCESS_ORDER_NEAR_TO_FAR, _handIndex, data) >= pxcStatus::PXC_STATUS_NO_ERROR))
	{
		_uniqueID = data->QueryUniqueId();
		
		if (!_lastFrameDetected)
		{			
			EventGesture gestureEvent;
			gestureEvent._gestureType = GestureType::HandDetected;
			RealSenseEventCenter::getInstance()->pushGestureEvent(gestureEvent);
			
		}
		_lastFrameDetected = true;
	}
	else
	{
		_lastFrameDetected = false;
	}	

	
}
TwoHandsLostRule::TwoHandsLostRule(PXCHandData *pxcHandData)
:BaseRule(pxcHandData)
,_twoHandsDetected(false)
{
	

}
void TwoHandsLostRule::process()
{
	if (_pxcHandData->QueryNumberOfHands() > 1)
	{
		PXCHandData::IHand * leftHand = NULL;
		PXCHandData::IHand * rightHand = NULL;
		if (_pxcHandData->QueryHandData(PXCHandData::ACCESS_ORDER_LEFT_HANDS, 0, leftHand) >= pxcStatus::PXC_STATUS_NO_ERROR)
		{
			if (_pxcHandData->QueryHandData(PXCHandData::ACCESS_ORDER_RIGHT_HANDS, 0, rightHand) >= pxcStatus::PXC_STATUS_NO_ERROR)
			{
				if (!_twoHandsDetected)
				{
					_twoHandsDetected = true;
				}
				return;
			}
		}
	}

	if (_twoHandsDetected)
	{
		EventGesture gestureEvent;
		gestureEvent._gestureType = GestureType::TwoHandsLost;
		RealSenseEventCenter::getInstance()->pushGestureEvent(gestureEvent);

		_twoHandsDetected = false;
	}
}

TwoHandsDetectedRule::TwoHandsDetectedRule(PXCHandData *pxcHandData)
:BaseRule(pxcHandData)
, _twoHandsDetected(false)
{


}
void TwoHandsDetectedRule::process()
{
	if (_pxcHandData->QueryNumberOfHands() > 1)
	{
		PXCHandData::IHand * leftHand = NULL;
		PXCHandData::IHand * rightHand = NULL;
		if (_pxcHandData->QueryHandData(PXCHandData::ACCESS_ORDER_LEFT_HANDS, 0, leftHand) >= pxcStatus::PXC_STATUS_NO_ERROR)
		{
			if (_pxcHandData->QueryHandData(PXCHandData::ACCESS_ORDER_RIGHT_HANDS, 0, rightHand) >= pxcStatus::PXC_STATUS_NO_ERROR)
			{
				if (!_twoHandsDetected)
				{
					_twoHandsDetected = true;
					EventGesture gestureEvent;
					gestureEvent._gestureType = GestureType::TwoHandsDetected;
					RealSenseEventCenter::getInstance()->pushGestureEvent(gestureEvent);					
				}
				return;
			}
		}
	}
	_twoHandsDetected = false;
	
}

GestureDetectedRule::GestureDetectedRule(PXCHandData *pxcHandData)
:BaseRule(pxcHandData)
{
	//init a map to store gesture name and gestureType mapping
	
	_gestureMap.clear();
	_gestureMap[L"spreadfingers"] = GestureType::SPREADFINGERS;
	_gestureMap[L"thumb_up"] = GestureType::THUMB_UP;
	_gestureMap[L"thumb_down"] = GestureType::THUMB_DOWN;
	_gestureMap[L"two_fingers_pinch_open"] = GestureType::TWO_FINGERS_PINCH_OPEN;
	_gestureMap[L"v_sign"] = GestureType::V_SIGN;
	_gestureMap[L"fist"] = GestureType::FIST;
	_gestureMap[L"full_pinch"] = GestureType::FULL_PINCH;
	_gestureMap[L"swipe"] = GestureType::SWIPE;
	_gestureMap[L"tap"] = GestureType::TAP;
	_gestureMap[L"wave"] = GestureType::WAVE;
}
void GestureDetectedRule::process()
{
	if (_pxcHandData->QueryNumberOfHands() > 0)
	{
		int totalNumOfFiredGestures = _pxcHandData->QueryFiredGesturesNumber();
		//Iterate fired gestures
		for (unsigned int i = 0; i < totalNumOfFiredGestures; i++)
		{
			//Get fired gesture data
			PXCHandData::GestureData gestureData;
			if (_pxcHandData->QueryFiredGestureData(i, gestureData) == PXC_STATUS_NO_ERROR)
			{
				//Get hand data related to fired gesture
				PXCHandData::IHand* handData;
				if ((_continuousTracking && _pxcHandData->QueryHandDataById(_uniqueID, handData) >= PXC_STATUS_NO_ERROR)
					||
					_pxcHandData->QueryHandData(_whichHand, _handIndex, handData) >= PXC_STATUS_NO_ERROR)
				{
					_uniqueID = handData->QueryUniqueId();
					if (handData->QueryUniqueId() == gestureData.handId)
					{
						//dispatch gesture only if we know if its right/left hand
						if (!handData->QueryBodySide() == PXCHandData::BODY_SIDE_UNKNOWN)
						{
							std::wstring name(gestureData.name);
							EventGesture gestureEvent;
							gestureEvent._handData = nullptr;
							gestureEvent._gestureType = _gestureMap[name];// use in listener
							gestureEvent._bodySideType = (BodySideType)handData->QueryBodySide();//use in cocos
							RealSenseEventCenter::getInstance()->pushGestureEvent(gestureEvent);
							//return true;
						}
						
					}
				}
				
			}
		}

	}
	
}
HandMoveRule::HandMoveRule(PXCHandData * pxcHandData)
:BaseRule(pxcHandData)
, DEPTH_WIDTH(640)
, DEPTH_HEIGHT(480)
, GESTURE_RANGE_MIN_Z(0.1f)
, GESTURE_RANGE_MAX_Z(0.6f)
{
	enabled = true;
}

void HandMoveRule::process()
{
	PXCHandData::IHand *data = 0;
	PXCHandData::JointData nodes[PXCHandData::NUMBER_OF_JOINTS];

	if (_pxcHandData->QueryNumberOfHands() > 0 &&
		((_continuousTracking&&_pxcHandData->QueryHandDataById(_uniqueID, data) >= pxcStatus::PXC_STATUS_NO_ERROR)
		|| _pxcHandData->QueryHandData(_whichHand, _handIndex, data) >= pxcStatus::PXC_STATUS_NO_ERROR))
	{
		_uniqueID = data->QueryUniqueId();
		auto bodySide = data->QueryBodySide();
		if (bodySide == PXCHandData::BODY_SIDE_UNKNOWN) return;

		HandData gesture;
		gesture._bodySideType = (BodySideType)bodySide;
		gesture._openness = data->QueryOpenness();

		//Iterate Joints
		for (int j = 0; j < PXCHandData::NUMBER_OF_JOINTS; j++)
		{
			data->QueryTrackedJoint((PXCHandData::JointType)j, nodes[j]);
			PXCPoint3DF32 position = nodes[j].positionImage;
			position.z = nodes[j].positionWorld.z;
			convertCoordinates(position, gesture._jointData[j]);
		}


		//init EventGesture and push it into gesture event queue
		EventGesture gestureEvent;
		gestureEvent._handData =gesture.clone();
		gestureEvent._gestureType = GestureType::HandMove;// use in listener
		gestureEvent._bodySideType = (BodySideType)bodySide;//use in cocos
		RealSenseEventCenter::getInstance()->pushGestureEvent(gestureEvent);
	}
	
}
void HandMoveRule::convertCoordinates(const PXCPoint3DF32 &raw, Vec3 &target)
{	
	if (raw.x < 0)
		target.x = 1;
	else if (raw.x<DEPTH_WIDTH)
		target.x = (DEPTH_WIDTH - raw.x) / DEPTH_WIDTH;
	else target.x = 0;

	if (raw.y < 0)
		target.y = 1;
	else if (raw.y<DEPTH_HEIGHT)
		target.y = (DEPTH_HEIGHT - raw.y) / DEPTH_HEIGHT;
	else target.y = 0;

	if (raw.z < GESTURE_RANGE_MAX_Z)
		raw.z < GESTURE_RANGE_MIN_Z ? target.z = 0 : target.z = (raw.z - GESTURE_RANGE_MIN_Z) / (GESTURE_RANGE_MAX_Z - GESTURE_RANGE_MIN_Z);
	else
		target.z = 1.0f;
	
}
/**************************************HandRotateRule*************************************/
HandRotateRule::HandRotateRule(PXCHandData *pxcHandData)
: BaseRule(pxcHandData)
, DistanceThresholdX(0)
, DistanceThresholdY(0)
, DistanceThresholdZ(0)
{
	_rotationAction = new RotationAction(_rotationData);
}
HandRotateRule::~HandRotateRule()
{
	
	delete _rotationAction;
}
void HandRotateRule::process()
{
	if (_pxcHandData!=NULL&&_pxcHandData->QueryNumberOfHands() > 1)
	{
		PXCHandData::IHand * leftHand = NULL;
		PXCHandData::IHand * rightHand = NULL;
		if (_pxcHandData->QueryHandData(PXCHandData::ACCESS_ORDER_LEFT_HANDS, 0, leftHand) >= pxcStatus::PXC_STATUS_NO_ERROR)
		{
			if (_pxcHandData->QueryHandData(PXCHandData::ACCESS_ORDER_RIGHT_HANDS, 0, rightHand) >= pxcStatus::PXC_STATUS_NO_ERROR)
			{
				float yCurDiff = rightHand->QueryMassCenterWorld().y * 100 - leftHand->QueryMassCenterWorld().y * 100;
				float zCurDiff = rightHand->QueryMassCenterWorld().z * 100 - leftHand->QueryMassCenterWorld().z * 100;

				
				if (_rotationData.Restart)
				{
					_roll = yCurDiff;
					_pitch = atan2(yCurDiff, zCurDiff);
					_yaw = zCurDiff;
					_rotationData.Restart = false;
				}
				_rotationData.Roll = yCurDiff - _roll;
				_rotationData.Roll *= -1;
				if (abs(_rotationData.Roll) < DistanceThresholdX)				
					_rotationData.Roll = 0;
				

				_rotationData.Pitch = (float)(atan2(yCurDiff, zCurDiff) - _pitch);
				if (abs(_rotationData.Pitch) < DistanceThresholdZ)				
					_rotationData.Pitch = 0;
				

				_rotationData.Yaw = zCurDiff - _yaw;
				if (abs(_rotationData.Yaw) < DistanceThresholdY)				
					_rotationData.Yaw = 0;
				

				if ((_rotationData.Roll == _rotationData.Yaw) && (_rotationData.Yaw == _rotationData.Pitch) && (_rotationData.Pitch == 0))				
					return;
				

				_rotationAction->update();
			}
		}
	}
	
}
/**************************************HandRotateRule*************************************/

HandScaleRule::HandScaleRule(PXCHandData *pxcHandData)
:BaseRule(pxcHandData)
, DistanceThresholdX(3)
, _scale(0)
{
	
}
void HandScaleRule::process()
{	
	if (_pxcHandData != NULL&&_pxcHandData->QueryNumberOfHands() > 1)
	{
		PXCHandData::IHand * leftHand = NULL;
		PXCHandData::IHand * rightHand = NULL;
		if (_pxcHandData->QueryHandData(PXCHandData::ACCESS_ORDER_LEFT_HANDS, 0, leftHand) >= pxcStatus::PXC_STATUS_NO_ERROR)
		{
			if (_pxcHandData->QueryHandData(PXCHandData::ACCESS_ORDER_RIGHT_HANDS, 0, rightHand) >= pxcStatus::PXC_STATUS_NO_ERROR)
			{					
				
				float yCurDiff = rightHand->QueryMassCenterWorld().y * 100 - leftHand->QueryMassCenterWorld().y * 100;
				float zCurDiff = rightHand->QueryMassCenterWorld().z * 100 - leftHand->QueryMassCenterWorld().z * 100;
				
				if (!_begin)
				{
					//check whether two hand in the same plane and same height.
					if (abs(yCurDiff) < threshold && abs(zCurDiff) < threshold)
						--counter;//hold on 
					else if (abs(yCurDiff) > threshold || abs(zCurDiff) > threshold)//cancel
						counter = delay;

					if (counter == 0)
					{						
					
						 xCurDiff1 = rightHand->QueryMassCenterWorld().x * 100 - leftHand->QueryMassCenterWorld().x * 100;	

						// if (abs(xCurDiff2 - xCurDiff1) > DistanceThresholdX)
						// {
							 _begin = true;
							 counter = delay;
						// }
					}			

				}
				else
				{
					xCurDiff2 = rightHand->QueryMassCenterWorld().x * 100 - leftHand->QueryMassCenterWorld().x * 100;
					if (abs(xCurDiff2 - xCurDiff1) < DistanceThresholdX)// joggling 
						return;
				
					if (abs(yCurDiff) < threshold && abs(zCurDiff) < threshold)
						--counter;
					else if (abs(yCurDiff) > threshold || abs(zCurDiff) > threshold)
						counter = delay;
					if (counter == 0)
					{						
						float diff = xCurDiff2 - xCurDiff1;
						float scale;
						if (diff > 0)//zoom in
						{
							scale = 1 - abs(diff / xCurDiff1);
						}
						else
						{
							scale = 1 + abs(diff / xCurDiff1);
						}
						EventGesture gestureEvent;
						gestureEvent._handData = nullptr;
						gestureEvent._gestureType = GestureType::Scale;
						gestureEvent._scaleData = scale;
						RealSenseEventCenter::getInstance()->pushGestureEvent(gestureEvent);
						_begin = false;
						counter = delay;
					}
						
				}		



			}
		}
	}
}
DrawImageRule::DrawImageRule(PXCSenseManager *pxcSenseManager)
:_pxcSenseManager(pxcSenseManager)
,_dpImageOpacity(50)
, _buffer(NULL)
, _bufferSize(0)
, _charBuffer(NULL)
, _maxRangeValue(1000)
, NUMBER_OF_FRAMES_TO_DELAY(3)
, enabled(false)
{
	_maxRangeValue = _pxcSenseManager->QueryCaptureManager()->QueryDevice()->QueryDepthSensorRange().max;
}
DrawImageRule::~DrawImageRule()
{
	releaseVectorImages();
}
void DrawImageRule::clearBuffer(PXCImage::ImageInfo info)
{
	int bufferSize = info.width * info.height;
	if (bufferSize != _bufferSize)
	{
		_bufferSize = bufferSize;
		if (_buffer) delete[] _buffer;
		_buffer = new pxcI32[_bufferSize];
		if (_charBuffer) delete[] _charBuffer;
		_charBuffer = new unsigned char[info.width*info.height * 4];
	}

	if (_bufferSize > 0){
		memset(_buffer, 0, _bufferSize*sizeof(pxcI32));
	}
}
void  DrawImageRule::releaseVectorImages()
{
	for (std::vector<PXCImage *>::iterator it = _depthImages.begin(); it != _depthImages.end(); ++it)
	{
		(*it)->Release();
	}

	while (!_depthImages.empty())
	{
		_depthImages.pop_back();
	}
}
void DrawImageRule::convertTo8bpp(unsigned short * pSrc, int iSize, unsigned char * pDst)
{
	float fMaxValue = _maxRangeValue;
	unsigned char cVal;
	for (int i = 0; i < iSize; i++, pSrc++, pDst += 4)
	{
		cVal = (unsigned char)((*pSrc) / fMaxValue * 255);
		if (cVal != 0)
			cVal = 255 - cVal;

		pDst[0] = cVal;
		pDst[1] = cVal;
		pDst[2] = cVal;
		pDst[3] = _dpImageOpacity;
	}
}
void DrawImageRule::process()
{
	
	const PXCCapture::Sample *sample = _pxcSenseManager->QueryHandSample();

	if (sample && sample->depth)
	{
		PXCImage* image = sample->depth;
		PXCImage::ImageInfo imageInfo = image->QueryInfo();
		clearBuffer(imageInfo);
		//collecting 3 images inside a vector and displaying the oldest image
		PXCImage::ImageInfo iinfo;
		memset(&iinfo, 0, sizeof(iinfo));
		iinfo = image->QueryInfo();
		PXCImage *clonedImage;
		clonedImage = _pxcSenseManager->QuerySession()->CreateImage(&iinfo);
		clonedImage->CopyImage(image);
		_depthImages.push_back(clonedImage);

		if (_depthImages.size() == NUMBER_OF_FRAMES_TO_DELAY)
		{
			Image *ccImage = new Image();
			PXCImage::ImageInfo info = _depthImages.front()->QueryInfo();
			PXCImage::ImageData data;

			if (info.format == PXCImage::PIXEL_FORMAT_DEPTH || info.format == PXCImage::PIXEL_FORMAT_Y16)
			{
				if (image->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_DEPTH, &data) >= PXC_STATUS_NO_ERROR)
				{
					convertTo8bpp((unsigned short*)data.planes[0], info.width*info.height, _charBuffer);
					ccImage->initWithRawData(_charBuffer, info.width*info.height, info.width, info.height, 8);
					image->ReleaseAccess(&data);
				}
			}

			_depthImages.front()->Release();
			_depthImages.erase(_depthImages.begin());

			//init EventGesture and push it into gesture event queue
			EventGesture gestureEvent;
			gestureEvent._depthImage = ccImage;
			gestureEvent._gestureType = GestureType::DepthImage;
			RealSenseEventCenter::getInstance()->pushGestureEvent(gestureEvent);
		}
	}
}

NS_CC_END