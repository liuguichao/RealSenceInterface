#ifndef __CCSimpleThread_H__
#define __CCSimpleThread_H__

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "cocos2d.h"
#include "pxchanddata.h"

/**
This class defines a simple thread prototype 
This class is inherited by GesturePipeline and GestureDepthViewer class
*/
class SimpleThread 
{

public:

	/** Constructor of Thread */
	SimpleThread(void);	

	/** Destructor of Thread */
	~SimpleThread(void);

	/** Start thread */
	void start();

	/** Stop thread */
	void stop(void);	
	
	/** Check if thread is still running */
	bool isRuning(void){ return _isRunning; };

	/** This function be called first in the start() function */
	virtual void onStart() = 0;

	/** This function be called first in the stop() function */
	virtual void onStop() = 0;

protected:	

	/** This function be called in the threadCallback function */
	virtual void PXCAPI runThread(void);

	/** This function be called after the thread has successful created and started */
	static unsigned PXCAPI threadCallback(LPVOID hdl);

protected:

	/** The exit code of thread */
	volatile uintptr_t _cameraThread;

	/** The current status of thread  */
	volatile bool _isRunning;
};

#endif //defined(__CCSimpleThread_H__)