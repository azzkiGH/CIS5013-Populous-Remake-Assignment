
#include "GUClock.h"
#include <Windows.h>

using namespace std;



class GUFrameCounter {

private:

	int					frame;

	gu_seconds			fpsRefTimeIndex;

	unsigned long		fpsCounts;

	int					_framesPerSecond, _minimumFPS, _maximumFPS, _averageFPS;
	gu_seconds			_secondsPerFrame, _minimumSPF, _maximumSPF, _averageSPF;

public:
	
	int					_alt_frame;
	gu_seconds			_alt_fpsRefTimeIndex;
	unsigned long		_alt_fpsCounts;
	double				_alt_framesPerSecond, _alt_minimumFPS, _alt_maximumFPS, _alt_averageFPS;
	gu_seconds			_alt_secondsPerFrame, _alt_minimumSPF, _alt_maximumSPF, _alt_averageSPF;


public:

	GUFrameCounter(gu_time_index baseTime = 0) {

		resetCounter(baseTime);
	}

	void resetCounter(gu_time_index resetTime = 0) {

		frame = 0;

		fpsRefTimeIndex = 0.0;

		fpsCounts = 0;

		_framesPerSecond = _minimumFPS = _maximumFPS = _averageFPS = 0;
		_secondsPerFrame = _minimumSPF = _maximumSPF = _averageSPF = 0.0;


	

		_alt_frame = 0;

		_alt_fpsRefTimeIndex = 0.0;

		_alt_fpsCounts = 0;

		_alt_framesPerSecond = _alt_minimumFPS = _alt_maximumFPS = _alt_averageFPS = 0.0;
		_alt_secondsPerFrame = _alt_minimumSPF = _alt_maximumSPF = _alt_averageSPF = 0.0;
	}


	void updateFrameCounterForElaspsedTime(gu_seconds gameTimeElapsed) {

		
		frame++;
		_alt_frame++;


		

		gu_seconds time_delta = gameTimeElapsed - fpsRefTimeIndex;

		
		if (time_delta >= 1.0) {

			_framesPerSecond = frame;
			_secondsPerFrame = 1000.0 / (gu_seconds)_framesPerSecond; 

			if (fpsCounts == 0) {

				_minimumFPS = _maximumFPS = _averageFPS = _framesPerSecond;
				_minimumSPF = _maximumSPF = _averageSPF = _secondsPerFrame;

			}
			else {

				
				if (_framesPerSecond < _minimumFPS)
					_minimumFPS = _framesPerSecond;
				else if (_framesPerSecond > _maximumFPS)
					_maximumFPS = _framesPerSecond;

				_averageFPS += _framesPerSecond;

				
				if (_secondsPerFrame < _minimumSPF)
					_minimumSPF = _secondsPerFrame;
				else if (_secondsPerFrame > _maximumSPF)
					_maximumSPF = _secondsPerFrame;

				_averageSPF += _secondsPerFrame;
			}

			
			frame = 0;
			fpsRefTimeIndex += 1.0;

			fpsCounts++;
		}


		

		gu_seconds _alt_time_delta = gameTimeElapsed - _alt_fpsRefTimeIndex;

		if (_alt_time_delta >= 1.0) {

		
			_alt_framesPerSecond = (double)_alt_frame / _alt_time_delta;
			_alt_secondsPerFrame = _alt_time_delta / (double)_alt_frame;

			if (_alt_fpsCounts == 0) {

				
				_alt_minimumFPS = _alt_maximumFPS = _alt_averageFPS = _alt_framesPerSecond;
				_alt_minimumSPF = _alt_maximumSPF = _alt_averageSPF = _alt_secondsPerFrame;

			}
			else {

				
				if (_alt_framesPerSecond < _alt_minimumFPS)
					_alt_minimumFPS = _alt_framesPerSecond;
				else if (_alt_framesPerSecond > _alt_maximumFPS)
					_alt_maximumFPS = _alt_framesPerSecond;

				_alt_averageFPS += _alt_framesPerSecond;

				
				if (_alt_secondsPerFrame < _alt_minimumSPF)
					_alt_minimumSPF = _alt_secondsPerFrame;
				else if (_alt_secondsPerFrame > _alt_maximumSPF)
					_alt_maximumSPF = _alt_secondsPerFrame;

				_alt_averageSPF += _alt_secondsPerFrame;
			}

			
			_alt_frame = 0;

			_alt_fpsRefTimeIndex = gameTimeElapsed;

			_alt_fpsCounts++;
		}

	}


	int framesPerSecond() const { return _framesPerSecond; }
	int minimumFPS() const { return _minimumFPS; }
	int maximumFPS() const { return _maximumFPS; }
	gu_seconds averageFPS() const { return ((gu_seconds)_averageFPS) / (gu_seconds)fpsCounts; }
	gu_seconds secondsPerFrame() const { return _secondsPerFrame; }
	gu_seconds minimumSPF() const { return _minimumSPF; }
	gu_seconds maximumSPF() const { return _maximumSPF; }
	gu_seconds averageSPF() const { return _averageSPF / (gu_seconds)fpsCounts; }

	double altFramesPerSecond() const { return _alt_framesPerSecond; }
	double altMinimumFPS() const { return _alt_minimumFPS; }
	double altMaximumFPS() const { return _alt_maximumFPS; }
	gu_seconds altAverageFPS() const { return ((gu_seconds)_alt_averageFPS) / (gu_seconds)_alt_fpsCounts; }
	gu_seconds altSecondsPerFrame() const { return _alt_secondsPerFrame; }
	gu_seconds altMinimumSPF() const { return _alt_minimumSPF; }
	gu_seconds altMaximumSPF() const { return _alt_maximumSPF; }
	gu_seconds altAverageSPF() const { return _alt_averageSPF / (gu_seconds)_alt_fpsCounts; }

};





gu_time_index GUClock::actualTime() {

	gu_time_index t;

	QueryPerformanceCounter((LARGE_INTEGER*)&t);

	return t;
}




void GUClock::resetClockAttributes() {

	baseTime = GUClock::actualTime();

	prevTimeIndex = baseTime;

	deltaTime = 0;

	stopTimeIndex = 0;
	totalStopTime = 0;

	_clockStopped = false;
}



void GUClock::invalidateClock() {

	baseTime = 0;
	prevTimeIndex = 0;
	currentTimeIndex = 0;

	deltaTime = 0;

	stopTimeIndex = 0;
	totalStopTime = 0;

	_clockStopped = true;

	frameCounter = NULL;
}



gu_seconds GUClock::convertTimeIntervalToSeconds(gu_time_interval t) const {

	return (gu_seconds)t * timeRecip;
}




GUClock::GUClock(void) {

	QueryPerformanceFrequency((LARGE_INTEGER*)&performanceFrequency);

	if (performanceFrequency != 0) {

		timeRecip = 1.0 / (gu_seconds)performanceFrequency;

		resetClockAttributes();
		frameCounter = new GUFrameCounter();

	}
	else {

		cout << "clock initialisation error" << endl;

		timeRecip = 0.0; 

		invalidateClock();
	}
}


GUClock::~GUClock(void) {

	if (frameCounter)
		delete frameCounter;
}




void GUClock::start() {

	
	if (_clockStopped && performanceFrequency != 0) {

		gu_time_index		restartTimeIndex = GUClock::actualTime();

		totalStopTime += restartTimeIndex - stopTimeIndex;
		prevTimeIndex = restartTimeIndex;

		_clockStopped = false;
	}
}


void GUClock::stop() {

	if (!_clockStopped)
	{
		stopTimeIndex = GUClock::actualTime();

		_clockStopped = true;
	}
}


void GUClock::tick() {

	if (_clockStopped) {

		deltaTime = 0;
		return;
	}

	currentTimeIndex = GUClock::actualTime();
	deltaTime = currentTimeIndex - prevTimeIndex;
	prevTimeIndex = currentTimeIndex;

	if (frameCounter)
		frameCounter->updateFrameCounterForElaspsedTime(convertTimeIntervalToSeconds((currentTimeIndex - baseTime) - totalStopTime));
}


void GUClock::reset() {

	resetClockAttributes();

	if (frameCounter)
		frameCounter->resetCounter();
}




gu_seconds GUClock::actualTimeElapsed() const {

	return convertTimeIntervalToSeconds(GUClock::actualTime() - baseTime);
}


gu_seconds GUClock::gameTimeElapsed() const {

	return convertTimeIntervalToSeconds((((_clockStopped) ? stopTimeIndex : GUClock::actualTime()) - baseTime) - totalStopTime);
}


gu_seconds GUClock::gameTimeDelta() const {

	return convertTimeIntervalToSeconds(deltaTime);
}


bool GUClock::clockStopped() const {

	return _clockStopped;
}


void GUClock::reportTimingData() const {

	cout << "max FPS = " << maximumFPS() << endl;
	cout << "min FPS = " << minimumFPS() << endl;
	cout << "average FPS = " << averageFPS() << endl;

	cout << "max SPF = " << (maximumSPF() / 1000.0) << endl;
	cout << "min SPF = " << (minimumSPF() / 1000.0) << endl;
	cout << "average SPF = " << (averageSPF() / 1000.0) << endl;

	if (frameCounter) {

		cout << "ALT max FPS = " << frameCounter->altMaximumFPS() << endl;
		cout << "ALT min FPS = " << frameCounter->altMinimumFPS() << endl;
		cout << "ALT average FPS = " << frameCounter->altAverageFPS() << endl;

		cout << "ALT max SPF = " << (frameCounter->altMaximumSPF() ) << endl;
		cout << "ALT min SPF = " << (frameCounter->altMinimumSPF() ) << endl;
		cout << "ALT average SPF = " << (frameCounter->altAverageSPF() ) << endl;
	}
}


int GUClock::framesPerSecond() const {

	return (frameCounter) ? frameCounter->framesPerSecond() : 0;
}


int GUClock::minimumFPS() const {

	return (frameCounter) ? frameCounter->minimumFPS() : 0;
}


int GUClock::maximumFPS() const {

	return (frameCounter) ? frameCounter->maximumFPS() : 0;
}


gu_seconds GUClock::averageFPS() const {

	return (frameCounter) ? frameCounter->averageFPS() : 0.0;
}


gu_seconds GUClock::secondsPerFrame() const {

	return (frameCounter) ? frameCounter->secondsPerFrame() : 0.0;
}


gu_seconds GUClock::minimumSPF() const {

	return (frameCounter) ? frameCounter->minimumSPF() : 0.0;
}


gu_seconds GUClock::maximumSPF() const {

	return (frameCounter) ? frameCounter->maximumSPF() : 0.0;
}


gu_seconds GUClock::averageSPF() const {

	return (frameCounter) ? frameCounter->averageSPF() : 0.0;
}
