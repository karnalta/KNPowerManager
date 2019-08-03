#ifndef KNSwitchEvent_H
#define KNSwitchEvent_H

#include "Declarations.h"

/// <summary>
/// Digital pin switching event handler.
/// </summary>
class KNSwitchEvent
{
private:
	uint8_t _dPin = 0;
	long _startTime = 0;
	bool _started = false;
	bool _completed = false;
	int _duration = 0;

public:	
	inline void SetDuration(int duration) { _duration = duration; }

	/// <summary>
	/// Initializes a new instance of the <see cref="KNSwitchEvent"/> class.
	/// </summary>
	/// <param name="dPin">The d pin.</param>
	KNSwitchEvent(uint8_t dPin);
	
	/// <summary>
	/// Starts the switching event.
	/// </summary>
	void Start();
	
	/// <summary>
	/// Resets the switching event to default state.
	/// </summary>
	void Reset();
		
	/// <summary>
	/// Checks this instance.
	/// </summary>
	/// <returns>True if completed.</returns>
	bool Check();
};

#endif