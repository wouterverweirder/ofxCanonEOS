/******************************************************************************
 *                                                                             *
 *   PROJECT : EOS Digital Software Development Kit EDSDK                      *
 *      NAME : Processor.h	                                                  *
 *                                                                             *
 *   Description: This is the Sample code to show the usage of EDSDK.          *
 *                                                                             *
 *                                                                             *
 *******************************************************************************
 *                                                                             *
 *   Written and developed by Camera Design Dept.53                            *
 *   Copyright Canon Inc. 2006-2008 All Rights Reserved                        *
 *                                                                             *
 *******************************************************************************/


#pragma once


#include <deque>
#include "Command.h"
#include "Poco/Condition.h"

class Processor : public ofThread
{
    
protected:
	// Que
	std::deque<Command*>  _queue;
    
	// Command when ending
	Command*	_closeCommand;
    
	// Synchronized Object
    Poco::Condition     _condition;
    
    
public:
	// Constructor
	Processor(): _closeCommand(0){ }
    
	// Destoracta
	virtual ~Processor(){
        clear();
    }
    
	// Set command when ending
	void setCloseCommand(Command* closeCommand){_closeCommand = closeCommand;}
    
	
	void enqueue(Command* command)
	{
		lock();
		_queue.push_back(command);
		_condition.signal();
		unlock();
	}
    
    
	void clear()
	{
		lock();
		std::deque<Command*>::iterator it = _queue.begin();
		while (it != _queue.end())
		{
			delete (*it);
			++it;
		}
		_queue.clear();
        
		unlock();
	}
    
    
public:
	virtual void run()
	{
		//When using the SDK from another thread in Windows,
		// you must initialize the COM library by calling CoInitialize
		//CoInitializeEx( NULL, COINIT_MULTITHREADED );
        
		while (isThreadRunning())
		{
            sleep(1);
            
			Command* command = take();
			if(command != NULL)
			{
				bool complete = command->execute();
				
				if(complete == false)
				{
					//If commands that were issued fail ( because of DeviceBusy or other reasons )
					// and retry is required , note that some cameras may become unstable if multiple
					// commands are issued in succession without an intervening interval.
					//Thus, leave an interval of about 500 ms before commands are reissued.
					sleep(500);
					enqueue(command);
				}
				else
				{
					delete command;
				}
			}
		}
		
		// Clear que
		clear();
        
		// Command of end
		if(_closeCommand != NULL)
		{
			_closeCommand->execute();
			delete _closeCommand;
			_closeCommand = NULL;
		}
        
		//CoUninitialize();
        
	}
    
protected:
    
	//The command is taken out of the queue
    
	
 	Command* take()
	{
        
		Command* command = NULL;
        
		lock();
        
		if (isThreadRunning() && !_queue.empty())
		{
			command = _queue.front();
			_queue.pop_front();
		}
        
		unlock();
        
		return command;
	}
    
    
	bool isEmpty()
	{
		lock();
		bool ret = _queue.empty();
		unlock();
		
		return ret;
	}
    
};