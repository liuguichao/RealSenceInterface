#include "CCRSCamera.h"

NS_CC_BEGIN


/** singleton camera */
static RSCamera *_camera = nullptr;

RSCamera::RSCamera()
{
}

RSCamera::~RSCamera(void)
{
}

RSCamera *RSCamera::getInstance()
{
	if (_camera == nullptr)
	{
		_camera = new RSCamera();			
		if (!_camera->init())
		{
			log("Camera init failed");
			_exit(0);
		}
	}
	return _camera;
}

NS_CC_END