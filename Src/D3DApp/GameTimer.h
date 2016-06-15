//=======================================================================================
// GameTimer.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================
#pragma once

#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer
{
public:
	GameTimer();
	
	float GetGameTime()const;  // in seconds
	float GetDeltaTime()const; // in seconds
	
	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.
	
private:	
	// Make private to prevent copying of members of this class.
	GameTimer(const GameTimer& rhs);
	GameTimer& operator=(const GameTimer& rhs);

private:
	double mSecondsPerCount;
	double mDeltaTime;
	
	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;
	
	bool mStopped;

};

extern GameTimer* gTimer;

#endif // GAMETIMER_H