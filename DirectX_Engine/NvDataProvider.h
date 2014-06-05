#pragma once

#include <vector>
#include <string>

#include <d3d11.h>

#include "NvPmApi.h"
#include "trace.h"

class UnmanagedPerf;

class NVDataProvider
{
public:
	enum class Traces
	{
		TN_PRIMITIVE_COUNT,
		TN_PERCENT_GPU_IDLE,
		TN_NUM_TRACES
	};

	~NVDataProvider();

	void	setUnmanaged(UnmanagedPerf* unmanaged);

	NVPMRESULT	setupCounter(ID3D11Device * device);
	void	updateCounterValue(float dt);

	float	getGpuIdle() const;
	int		getPrimitiveCount() const;

	static NVDataProvider *	get();

protected:
	NVDataProvider();

private:
	UnmanagedPerf *	unmanaged;

	std::vector<char *>	traceNames;
	std::vector<char *>	counterNames;

	NVPMContext	NVPMContext;
	bool		NVPMInitialized;
	bool		performAnalyze;

	int			primitives;
	float		gpuIdle;

	float		feedbackCpt;

	void	updateUnmanaged();

	CTrace<float>	trace[Traces::TN_NUM_TRACES];

	static int		myEnumFunc(NVPMCounterID counterIndex, char const * counterName);

	static NVDataProvider *	instance;
};