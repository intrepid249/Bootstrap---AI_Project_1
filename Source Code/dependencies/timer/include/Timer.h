#pragma once
#include <functional>

/** 
Handle timing and trigger a callback function when the timer is complete
* @author Jack McCall*/
class Timer {
public:
	Timer();
	~Timer();

	/** A do-once execution of a callback function upon completion
	* @param callback - the function to execute when the timer finishes
	* @param delay - the amount of time in milliseconds that the timer will take to complete*/
	void schedule(std::function<void()> callback, unsigned int delay);

	/** A repeated execution of a callback function at the set intervals
	* @param callback - the function to execute when the timer finishes
	* @param initialDelay - initial amount of time to wait in milliseconds
	* @param repeatDelay - time in milliseconds to wait between intervals*/
	void scheduleAtFixedRate(std::function<void()> callback, unsigned int initialDelay, unsigned int repeatDelay);
	
	/** Stops repeated execution*/
	void cancel();

private:
	bool isRepeating;
};