#include "CCSimpleThread.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <process.h>


SimpleThread::SimpleThread()
{ 
	_cameraThread = 0; 
	_isRunning = false; 
}
SimpleThread::~SimpleThread()
{ 
	cocos2d::log("Camera Thread Exit");
}
unsigned PXCAPI SimpleThread::threadCallback(LPVOID hdl)
{	
	SimpleThread *pthis = (SimpleThread *)hdl;
	pthis->runThread();
	pthis->_cameraThread = 0;
	return 0;
}

void SimpleThread::start()
{
	onStart();
	if(!_cameraThread)
	{
		cocos2d::log("Camera Thread Started");
		_cameraThread = _beginthreadex(0,0,threadCallback,this,0,0);	
	}
}

void SimpleThread::stop(void)
{
	onStop();
}
void SimpleThread::runThread(void)
{
	
}