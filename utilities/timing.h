#ifndef TIMING_H
#define TIMING_H

#include <string>
#include <chrono>
#include <map>
#include <vector>

class Timer{
public:
	typedef std::chrono::high_resolution_clock Clock_t;

protected:
	Clock_t::time_point start;
	Clock_t::time_point end;
	bool running;

public:
	Timer();

	void Reset();
	void Stop();

	double Get(); // Seconds
	double GetMillis(); // Milliseconds
	double GetMicros(); // Microseconds
	uint64_t GetNanos(); // Nanoseconds
};

// Probably move this somewhere else
class Profiler{
public:
	typedef std::chrono::high_resolution_clock Clock_t;
	typedef std::vector<double> SampleList_t;

protected:
	static std::map<std::string, SampleList_t> samples; // In ms
	
	Clock_t::time_point start;
	Clock_t::time_point end;
	std::string name;

public:
	Profiler(const std::string&);
	~Profiler();

	static SampleList_t& GetSamples(const std::string&);
	static double GetAverage(const std::string&);
	static double GetTotal(const std::string&);
	static double GetNumSamplesCollected(const std::string&);
};

#endif