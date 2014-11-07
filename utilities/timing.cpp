#ifndef TIMING_CPP
#define TIMING_CPP

#include "timing.h"

using namespace std::chrono;
using std::string;

//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////

Timer::Timer(){
	start = Clock_t::now();
	running = true;
}
void Timer::Reset(){
	start = Clock_t::now();
	running = true;
}
void Timer::Stop(){
	running = false;
	end = Clock_t::now();
}

double Timer::Get(){
	auto nend = Clock_t::now();
	if(running) end = nend;
	auto d = duration_cast<duration<double>>(end-start).count();
	return d;
}

double Timer::GetMillis(){
	auto nend = Clock_t::now();
	if(running) end = nend;
	auto d = (double)duration_cast<nanoseconds>(end-start).count();
	return d/1000000.0;
}
double Timer::GetMicros(){
	auto nend = Clock_t::now();
	if(running) end = nend;
	auto d = (double)duration_cast<nanoseconds>(end-start).count();
	return d/1000.0;
}
uint64_t Timer::GetNanos(){
	auto nend = Clock_t::now();
	if(running) end = nend;
	uint64_t d = duration_cast<nanoseconds>(end-start).count();
	return d;
}

//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////

using std::map;
using std::vector;

map<string, Profiler::SampleList_t> Profiler::samples = {};

Profiler::Profiler(const string& _name) : name(_name){
	start = Clock_t::now();
}

Profiler::~Profiler(){
	end = Clock_t::now();

	if(samples.find(name) == samples.end()){
		samples[name] = {};
	}

	double d = duration_cast<duration<double, std::milli>>(end-start).count();

	samples[name].push_back(d);
}

Profiler::SampleList_t& Profiler::GetSamples(const string& n){
	if(samples.find(n) == samples.end()){
		samples[n] = {};
	}

	return samples[n];
}

double Profiler::GetAverage(const string& n){
	SampleList_t& v = GetSamples(n);

	int count = v.size();

	if(count == 0) return 0.0;

	double total = 0.0;
	for(double d : v){
		total += d;
	}

	return total/(float)count;
}

double Profiler::GetTotal(const string& n){
	SampleList_t& v = GetSamples(n);

	double total = 0.0;
	for(double d : v){
		total += d;
	}

	return total;
}

double Profiler::GetNumSamplesCollected(const string& n){
	SampleList_t& v = GetSamples(n);
	return v.size();
}

#endif