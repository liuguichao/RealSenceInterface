#include "CCGesturePipeline.h"
#include "CCGestureAttribute.h"
#include <process.h>
#include "pxchandconfiguration.h"
#include <cstdlib>


NS_CC_BEGIN


const int CAMERA_INTERVAL_TIME = 20;

GesturePipeline::GesturePipeline(void)
:_pxcHandData(nullptr)
, _enabled(false)
, _interval(50)

{
	memset(_moduleName, 0, sizeof(_moduleName));
	memset(_deviceName, 0, sizeof(_deviceName));	
}

GesturePipeline::~GesturePipeline(void)
{
	delete _drawImageRule;
	delete _twoHandsDetectedRule;
	delete _twoHandsLostRule;
	delete _handClosedRule;
	delete _handOpennedRule;
	delete _handDetectedRule;
	delete _handLostRule;
	delete  _gestureDetectedRule;
	delete  _handMoveRule;
	delete _rotateRule;
	delete _scaleRule;
	_pxcHandData->Release();
	_pxcSenseManager->Close();
	_pxcSenseManager->Release();
}
bool GesturePipeline::init()
{
	_pxcSenseManager = PXCSenseManager::CreateInstance();
	/* Set Mode & Source */
	pxcCHAR * deviceName = retrieveDeviceName();
	_pxcSenseManager->QueryCaptureManager()->FilterByDeviceInfo(deviceName, 0, 0);


	pxcStatus status = _pxcSenseManager->EnableHand();
	_pxcHandModule = _pxcSenseManager->QueryHand();

	if (_pxcHandModule == NULL || status != pxcStatus::PXC_STATUS_NO_ERROR)
	{
		cocos2d::log("Failed to pair the gesture module with I/O");
		return 0;
	}

	/* Init */
	if (_pxcSenseManager->Init() >= PXC_STATUS_NO_ERROR)
	{
		_pxcHandData = _pxcHandModule->CreateOutput();
				
		initRules(_pxcHandData);		
		

		// IF IVCAM Set the following properties
		PXCCapture::Device *device = _pxcSenseManager->QueryCaptureManager()->QueryDevice();
		PXCCapture::DeviceInfo dinfo;
		device->QueryDeviceInfo(&dinfo);
		if (dinfo.model == PXCCapture::DEVICE_MODEL_IVCAM)
		{
			device->SetDepthConfidenceThreshold(1);
			device->SetMirrorMode(PXCCapture::Device::MIRROR_MODE_DISABLED);
			device->SetIVCAMFilterOption(6);
		}
		
		// Hand Module Configuration
		PXCHandConfiguration* config = _pxcHandModule->CreateActiveConfiguration();		
		config->SetTrackingMode(PXCHandData::TrackingModeType::TRACKING_MODE_FULL_HAND);
		config->EnableSegmentationImage(true);		

		config->ApplyChanges();
		config->Update();
		config->Release();
	
		return 1;
	}
	else
	{
		cocos2d::log("Init Failed");
		return 0;
	}

}
void GesturePipeline::initRules(PXCHandData *_pxcHandData)
{
	_handOpennedRule = new HandOpennedRule(_pxcHandData);
	rules.push_back(_handOpennedRule);
	_handClosedRule = new HandClosedRule(_pxcHandData);
	rules.push_back(_handClosedRule);
	_handLostRule = new HandLostRule(_pxcHandData);
	rules.push_back(_handLostRule);
	_handDetectedRule = new HandDetectedRule(_pxcHandData);
	rules.push_back(_handDetectedRule);
	_gestureDetectedRule = new GestureDetectedRule(_pxcHandData);
	rules.push_back(_gestureDetectedRule);
	_handMoveRule = new HandMoveRule(_pxcHandData);
	rules.push_back(_handMoveRule);
	_rotateRule = new HandRotateRule(_pxcHandData);
	rules.push_back(_rotateRule);
	_scaleRule = new HandScaleRule(_pxcHandData);
	rules.push_back(_scaleRule);
	_drawImageRule = new DrawImageRule(_pxcSenseManager);
	_twoHandsDetectedRule = new TwoHandsDetectedRule(_pxcHandData);
	rules.push_back(_twoHandsDetectedRule);
	_twoHandsLostRule = new TwoHandsLostRule(_pxcHandData);
	rules.push_back(_twoHandsLostRule);
}
void GesturePipeline::enableGestureDetectedRule(bool enabled, bool continuousTracking)
{
	// Hand Module Configuration
	PXCHandConfiguration* config = _pxcHandModule->CreateActiveConfiguration();

	if (enabled)
		config->EnableAllGestures(true);
	else config->DisableAllGestures();

	config->ApplyChanges();
	config->Update();
	config->Release();

	_gestureDetectedRule->enabled = enabled;
	_gestureDetectedRule->_continuousTracking = continuousTracking;
}
void GesturePipeline::enableAllRules(bool enabled, bool continuousTracking)
{
	enableDrawImageRule(enabled);
	enableRotateRule(enabled);
	enableScaleRule(enabled);
	enableTwoHandsDetectedRules(enabled);
	enableTwoHandsLostRules(enabled);
	enableHandClosedRule(enabled, continuousTracking);
	enableHandOpennedRule(enabled, continuousTracking);
	enableHandDetectedRule(enabled, continuousTracking);
	enableHandLostRule(enabled, continuousTracking);
	enableHandMoveRule(enabled, continuousTracking);
	enableHandDetectedRule(enabled, continuousTracking);
	enableGestureDetectedRule(enabled, continuousTracking);
}

bool GesturePipeline::isEnabled()
{
	return _enabled;
}

void GesturePipeline::onStop()
{
	_enabled = false;
}

void GesturePipeline::onStart()
{
	_enabled = true;
}

void PXCAPI GesturePipeline::handleFrame(void)
{
	pxcStatus sts = _pxcSenseManager->AcquireFrame(true);

	if (sts < PXC_STATUS_NO_ERROR) return;
	_pxcHandData->Update();

	for each (auto rule in rules)
	{
		if (rule->enabled)		
			rule->process();		
	}	
	
	if (_drawImageRule->enabled)
		_drawImageRule->process();
		

	_pxcSenseManager->ReleaseFrame();

}

pxcCHAR* GesturePipeline::retrieveModuleFriendlyName()
{
	PXCSession::ImplDesc desc, desc1;
	memset(&desc, 0, sizeof(desc));
	desc.cuids[0] = PXCHandModule::CUID;
	if (_pxcSenseManager->QuerySession()->QueryImpl(&desc, 0, &desc1) < PXC_STATUS_NO_ERROR)
		return nullptr;
	wcscpy_s(_moduleName, desc1.friendlyName);
	return _moduleName;
}

pxcCHAR* GesturePipeline::retrieveDeviceName()
{
	PXCSession::ImplDesc desc;
	memset(&desc, 0, sizeof(desc));
	desc.group = PXCSession::IMPL_GROUP_SENSOR;
	desc.subgroup = PXCSession::IMPL_SUBGROUP_VIDEO_CAPTURE;
	for (int i = 0;; i++) {
		PXCSession::ImplDesc desc1;
		if (_pxcSenseManager->QuerySession()->QueryImpl(&desc, i, &desc1) < PXC_STATUS_NO_ERROR) break;
		PXCCapture * capture = nullptr;
		if (_pxcSenseManager->QuerySession()->CreateImpl<PXCCapture>(&desc1, &capture) < PXC_STATUS_NO_ERROR) continue;
		for (int j = 0;; j++) {//may be should use QueryDeviceNum() if there are more than one device connected
			PXCCapture::DeviceInfo dinfo;
			if (capture->QueryDeviceInfo(j, &dinfo) < PXC_STATUS_NO_ERROR) break;
			wcscpy_s(_deviceName[i], dinfo.name);
		}
		capture->Release();
	}
	return _deviceName[0];
}


void GesturePipeline::setInterval(unsigned int interval)
{
	if (!interval)
		interval = CAMERA_INTERVAL_TIME;
	this->_interval = interval;
}

void PXCAPI GesturePipeline::runThread(void)
{
	if (!_enabled)
		return;
	_isRunning = true;
	log("Camera started\n");
	while (_isRunning && _enabled)
	{
		if (_pxcSenseManager->IsConnected())
			handleFrame();
		Sleep(_interval);
	}
	_isRunning = false;
	_endthreadex(0);
}

NS_CC_END