//
//  CanonSDKWrapper.h
//  canon_no_addon_test
//
//  Created by Wouter Verweirder on 20/06/13.
//
//

#pragma once

#ifdef __APPLE__
#define __MACOS__   //needed for EDSDK
#endif

#include "ofMain.h"
#include "Observer.h"
#include "EDSDK.h"
#include "CameraEventListener.h"
#include "CameraModelLegacy.h"

class CanonSDKWrapper : public Observer {
    
public:
    
    CanonSDKWrapper(){
        ofAddListener(ofEvents().update, this, &CanonSDKWrapper::update);
    }
    ~CanonSDKWrapper(){
        ofRemoveListener(ofEvents().update, this, &CanonSDKWrapper::update);
        stop();
    }
    
    void start(){
        if(!_started) {
            _started = true;
            _err = EDS_ERR_OK;
            _cameraList = NULL;
            _camera = NULL;
            _count = 0;
            _isSDKLoaded = false;
            
            _err = EdsInitializeSDK();
            cout << "EdsInitializeSDK " << _err << endl;
            if(_err == EDS_ERR_OK) {
                _isSDKLoaded = true;
                _err = EdsGetCameraList(&_cameraList);
            }
            if( _err == EDS_ERR_OK) {
                _err = EdsGetChildCount(_cameraList, &_count);
                if( _count == 0) {
                    _err = EDS_ERR_DEVICE_NOT_FOUND;
                }
            }
            if( _err == EDS_ERR_OK) {
                _err = EdsGetChildAtIndex(_cameraList , 0 , &_camera);
            }
            EdsDeviceInfo deviceInfo;
            if(_err == EDS_ERR_OK) {
                _err = EdsGetDeviceInfo(_camera , &deviceInfo);
                if(_err == EDS_ERR_OK && _camera == NULL) {
                    _err = EDS_ERR_DEVICE_NOT_FOUND;
                }
            }
            //Release camera list
            if(_cameraList != NULL) {
                EdsRelease(_cameraList);
            }
            
            //Create Camera model
            if(_err == EDS_ERR_OK )
            {
                _model = cameraModelFactory(_camera, deviceInfo);
                if(_model == NULL)
                {
                    _err = EDS_ERR_DEVICE_NOT_FOUND;
                }
            }
            
            if(_err == EDS_ERR_OK ) {
                
                _model->addObserver(static_cast<Observer*>(this));
                
                _controller = new CameraController();
                _controller->setCameraModel(_model);
                
                if(_err == EDS_ERR_OK) {
                    _err = EdsSetPropertyEventHandler( _camera, kEdsPropertyEvent_All, CameraEventListener::handlePropertyEvent , (EdsVoid *)_controller);
                }
                
                if(_err == EDS_ERR_OK) {
                    _err = EdsSetObjectEventHandler( _camera, kEdsObjectEvent_All, CameraEventListener::handleObjectEvent , (EdsVoid *)_controller);
                }
                
                if(_err == EDS_ERR_OK) {
                    _err = EdsSetCameraStateEventHandler( _camera, kEdsStateEvent_All, CameraEventListener::handleStateEvent , (EdsVoid *)_controller);
                }
                
                _controller->run();
            } else {
                stop();
            }
        }
    }
    
    void stop(){
        if(_camera != NULL) {
            EdsRelease(_camera);
            _camera = NULL;
        }
            
        if(_isSDKLoaded){
            EdsTerminateSDK();
        }
            
        if(_controller != NULL) {
            delete _controller;
            _controller = NULL;
        }
        
        if(_model != NULL) {
            delete _model;
            _model = NULL;
        }
            
        _started = false;
    }
    
    void takePicture(){
        if( _controller != NULL) {
            ActionEvent event("TakePicture", 0);
            _controller->actionPerformed(event);
        }
    }
    
    void update(Observable* from, CameraEvent *e){
        std::string event = e->getEvent();
        cout << event << endl;
        if(event == "DownloadComplete") {
            string* arg = (string *) e->getArg();
            _mutex.lock();
            _newFiles.push_back(*arg);
            _mutex.unlock();
        } else if(event == "shutDown") {
            stop();
        }
    }
    
    void update(ofEventArgs& args){
        _mutex.lock();
        for(int i = 0; i < _newFiles.size(); i++){
            ofNotifyEvent(onFileAdded, _newFiles[i]);
        }
        _newFiles.clear();
        _mutex.unlock();
    }
    
    bool isStarted(){
        return _started;
    }
    
    ofEvent<string> onFileAdded;
    
protected:
    
    bool _started;
    
    EdsError _err;
    EdsCameraListRef _cameraList;
    EdsCameraRef _camera;
    EdsUInt32 _count;
    bool _isSDKLoaded;
    
    CameraModel*		_model;
	CameraController*	_controller;
    ofMutex _mutex;
    
    vector<string> _newFiles;
    
    CameraModel* cameraModelFactory(EdsCameraRef camera, EdsDeviceInfo deviceInfo)
    {
        // if Legacy protocol.
        if(deviceInfo.deviceSubType == 0)
        {
            return new CameraModelLegacy(camera);
        }
        
        // PTP protocol.
        return new CameraModel(camera);
    }
    
};