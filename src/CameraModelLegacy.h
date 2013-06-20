#pragma once

#include "CameraModel.h"

class CameraModelLegacy : public CameraModel
{
    
public:
	// Constructor
	CameraModelLegacy(EdsCameraRef camera):CameraModel(camera){}
    
    
	virtual bool isLegacy()
	{
		return true;
	}
};