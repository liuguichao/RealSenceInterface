#ifndef __CCGesturePipeline_H__
#define __CCGesturePipeline_H__


#include "CCSimpleThread.h"
#include "CCRealSenseEventCenter.h"
#include "pxcsensemanager.h"
#include "pxcdefs.h"
#include <map>
#include<string>
#include "CCHandRules.h"
//#include "CCHandActions.h"
/**
This class creates a RealSense camera manager and it can do following things synchronously by extending SimpleThread:

	*1.Gets all raw data from camera.
	*2.Analyzes and classifies the raw data.
	*3.Transforms and encapsulates the raw data into HandData while creating corresponding EventGesture.
	*4.Dispatches gesture events to RealSenseEventCenter with hand tracking data.
*/

NS_CC_BEGIN

class GesturePipeline : public SimpleThread
{

public:	

	/** Constructor of GesturePipeline */
	GesturePipeline(void);	

	/** Gets whether enable the gesture pipeline*/
	bool isEnabled();

	/** Set the interval time of pipeline thread  
     *  @param interval The milliseconds of interval time.
     */
	void setInterval(unsigned int interval);

	/** Init a gesture pipeline
	*  @return If init succeed, return true.
	*/
	virtual bool init(void);
	

	/** Whether enable the special rules
	*  @param enabled, the switch label.
	*  @param continuousTracking, the index of the hand can change once more hands are visible. Should continuously track the detected hand no matter its' index.	
	*/
	inline void enableHandDetectedRule(bool enabled, bool continuousTracking){ _handDetectedRule->enabled = enabled; _handDetectedRule->_continuousTracking = continuousTracking; };
	/** Whether enable the special rules
	*  @param enabled, the switch label.
	*  @param continuousTracking, the index of the hand can change once more hands are visible. Should continuously track the detected hand no matter its' index.
	*/
	inline void enableHandLostRule(bool enabled, bool continuousTracking){ _handLostRule->enabled = enabled; _handLostRule->_continuousTracking = continuousTracking; };
	/** Whether enable the special rules
	*  @param enabled, the switch label.
	*  @param continuousTracking, the index of the hand can change once more hands are visible. Should continuously track the detected hand no matter its' index.
	*/
	inline void enableHandOpennedRule(bool enabled, bool continuousTracking){ _handOpennedRule->enabled = enabled; _handOpennedRule->_continuousTracking = continuousTracking; };
	/** Whether enable the special rules
	*  @param enabled, the switch label.
	*  @param continuousTracking, the index of the hand can change once more hands are visible. Should continuously track the detected hand no matter its' index.
	*/
	inline void enableHandClosedRule(bool enabled, bool continuousTracking){ _handClosedRule->enabled = enabled; _handClosedRule->_continuousTracking = continuousTracking; };
	/** Whether enable the special rules
	*  @param enabled, the switch label.
	*  @param continuousTracking, the index of the hand can change once more hands are visible. Should continuously track the detected hand no matter its' index.
	*/
	inline void enableHandMoveRule(bool enabled, bool continuousTracking){ _handMoveRule->enabled = enabled; _handMoveRule->_continuousTracking = continuousTracking; };

	/** Whether enable the special rules
	*  @param enabled, the switch label.
	*  @param continuousTracking, the index of the hand can change once more hands are visible. Should continuously track the detected hand no matter its' index.
	*/
	inline void enableGestureDetectedRule(bool enabled, bool continuousTracking);

	/** Whether enable the special rules
	*  @param enabled, the switch label.
	*/
	inline void enableDrawImageRule(bool enabled){ _drawImageRule->enabled = enabled; };
	//set depth image opacity£¬range from 0 to 255.
	inline void setImageOpacity(unsigned char imageOpacity){ _drawImageRule->setImageOpacity(imageOpacity); };

	/** Whether enable the special rules
	*  @param enabled, the switch label.
	*/
	inline void enableRotateRule(bool enabled){ _rotateRule->enabled = true; };
	//set whether freeze the rotate orientation X ,Y or Z.
	inline void setRotateFreeze(bool freezeX, bool freezeY, bool freezeZ){ _rotateRule->setFreeze(freezeX, freezeZ, freezeY); };
	/** Whether enable the special rules
	*  @param enabled, the switch label.
	*/
	inline void enableTwoHandsDetectedRules(bool enabled){ _twoHandsDetectedRule->enabled = enabled; };
	/** Whether enable the special rules
	*  @param enabled, the switch label.
	*/
	inline void enableTwoHandsLostRules(bool enabled){ _twoHandsLostRule->enabled = enabled; };
	/** Whether enable the special rules
	*  @param enabled, the switch label.
	*/
	inline void enableScaleRule(bool enabled){ _scaleRule->enabled = enabled; };

	/** Whether enable the all rules
	*  @param enabled, the switch label.
	*  @param continuousTracking, the index of the hand can change once more hands are visible. Should continuously track the detected hand no matter its' index.
	*/
	void enableAllRules(bool enabled, bool continuousTracking);
protected:

	/** Destructor of GesturePipeline */
	~GesturePipeline(void);

	/// Overrides
	virtual void onStop() override;
	virtual void onStart() override;
	virtual void PXCAPI runThread(void) override;	

private:
	

	/** Executes in the main loop of this thread. Catches the raw data from RealSense camera*/
	void PXCAPI handleFrame(void);	
	
	/** Get module friendly name*/
	pxcCHAR* retrieveModuleFriendlyName(void);

	/** Get device name*/
	pxcCHAR* retrieveDeviceName(void);			

	/** init rules with pxcHandata */
	void initRules(PXCHandData * pxcHandData);
protected:
	
	/** Whether enable the gesture pipeline*/
	bool _enabled;

	/** The interval time of thread*/
	unsigned int _interval;

private:		
	
	/** intel 3d camera, module name and device name*/
	pxcCHAR _moduleName[256];
	pxcCHAR _deviceName[2][256];	

	/** Standard interface for gesture recognition*/
	PXCHandData *_pxcHandData;

	/** Interface for hand module configuration*/
	PXCHandModule *_pxcHandModule;

	/** Interface to control the pipeline execution */
	PXCSenseManager *_pxcSenseManager;
	

	
	HandClosedRule *_handClosedRule;
	HandOpennedRule *_handOpennedRule;
	HandDetectedRule *_handDetectedRule;
	HandLostRule *_handLostRule;
	GestureDetectedRule * _gestureDetectedRule;
	HandMoveRule * _handMoveRule;
	DrawImageRule * _drawImageRule;
	HandRotateRule *_rotateRule;
	HandScaleRule * _scaleRule;
	TwoHandsDetectedRule * _twoHandsDetectedRule;
	TwoHandsLostRule * _twoHandsLostRule;
	std::vector<BaseRule *> rules;	


};

NS_CC_END
#endif  // defined(__CCGesturePipeline_H__) 
