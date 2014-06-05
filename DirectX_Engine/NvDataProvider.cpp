#define NVPM_INITGUID
#include "NvPmApi.Manager.h"

// Simple singleton implementation for grabbing the NvPmApi
static NvPmApiManager S_NVPMManager;
extern NvPmApiManager *GetNvPmApiManager() {return &S_NVPMManager;}  
const NvPmApi *GetNvPmApi() {return S_NVPMManager.Api();}

#include "NvDataProvider.h"
#include "UnmanagedPerf.h"

NVDataProvider *	NVDataProvider::instance = nullptr;

NVDataProvider::NVDataProvider() :
	NVPMInitialized(false),
	performAnalyze(false),
	feedbackCpt(0.0f)
{
	this->NVPMContext = 0;
}

NVDataProvider::~NVDataProvider()
{
}

int	NVDataProvider::myEnumFunc(NVPMCounterID counterIndex, char const * counterName)
{
	char		string[200];
	char		line[400];
	NVPMUINT	len;

	len = 200u;
	if (GetNvPmApi()->GetCounterName(counterIndex, string, &len) == NVPM_OK)
	{
		sprintf_s(line, "Counter %d [%s] : ", counterIndex, string);

		len = 200u;
		if (GetNvPmApi()->GetCounterName(counterIndex, string, &len) == NVPM_OK)
		{
			strcat_s(line, string);
		}
		else
		{
			strcat_s(line, "Error retrieving short name");
		}

		OutputDebugStringA(line);
	}

	return NVPM_OK;
}


#ifdef _WIN64
#define PATH_TO_NVPMAPI_CORE L"..\\DLLs\\win7_x64\\NvPmApi.Core.dll"
#else
#define PATH_TO_NVPMAPI_CORE L"..\\DLLs\\win7_x86\\NvPmApi.Core.dll"
#endif

NVPMRESULT	NVDataProvider::setupCounter(ID3D11Device * device)
{
	try
	{
		this->traceNames.push_back("Primitive Count");
		this->traceNames.push_back("% GPU Idle");
		this->traceNames.push_back(nullptr);

		this->counterNames.push_back("D3D primitive count");
		this->counterNames.push_back("gpu_idle");
		this->counterNames.push_back(nullptr);
	}
	catch (std::bad_alloc &)
	{
		return NVPM_ERROR_INTERNAL;
	}

	if (GetNvPmApiManager()->Construct(PATH_TO_NVPMAPI_CORE) != S_OK)
		return NVPM_ERROR_INTERNAL;

	NVPMRESULT	result;
	if ((result = GetNvPmApi()->Init()) != NVPM_OK)
		return result;

	this->NVPMInitialized = true;

	if ((result = GetNvPmApi()->CreateContextFromD3D11Device(device, &this->NVPMContext)) != NVPM_OK)
		return result;

	GetNvPmApi()->EnumCountersByContext(this->NVPMContext, NVDataProvider::myEnumFunc);

	int	index = 0;
	while (this->counterNames[index] != nullptr)
	{
		GetNvPmApi()->AddCounterByName(this->NVPMContext, this->counterNames[index]);
		index++;
	}

	for (auto index = 0; index < 2; ++index)
		this->trace[index].name(this->traceNames[index]);

	this->trace[1].min(0);
	this->trace[1].max(100);

	return NVPM_OK;
}

void	NVDataProvider::updateCounterValue(float dt)
{
	static float	lastTime;

	if (!this->NVPMInitialized)
		return ;

	if (dt != lastTime)
	{
		NVPMUINT64	value;
		NVPMUINT64	cycle;

		if (this->performAnalyze)
		{
		}
		else
		{
			NVPMUINT	count;

			GetNvPmApi()->Sample(this->NVPMContext, nullptr, &count);

			for (auto index = 0; index < 2; ++index)
			{
				switch (index)
				{
				case NVDataProvider::Traces::TN_PRIMITIVE_COUNT:
					GetNvPmApi()->GetCounterValueByName(this->NVPMContext, this->counterNames[0], 0, &value, &cycle);
					//this->trace[index].insert((float)value);
					this->primitives = (int)value;
					break;

				case NVDataProvider::Traces::TN_PERCENT_GPU_IDLE:
					GetNvPmApi()->GetCounterValueByName(this->NVPMContext, this->counterNames[1], 0, &value, &cycle);
					//float	tmp = (float)value / (float)cycle * 100.0f;
					//this->trace[index].insert(tmp);
					this->gpuIdle = (float)value / (float)cycle * 100.0f;
					if (this->gpuIdle > 100.0f)
						this->gpuIdle = 100.0f;
					if (this->gpuIdle < 0.0f)
						this->gpuIdle = 0.0f;
					break;
				}
			}
		}

		this->feedbackCpt += dt;
		lastTime = dt;

		if (this->feedbackCpt > 0.25f)
		{
			this->updateUnmanaged();
			this->feedbackCpt = 0.0f;
		}
	}
}

int		NVDataProvider::getPrimitiveCount() const
{
	if (!this->NVPMInitialized)
		return 0;

	//this->unmanaged->setPrimitives((int)this->trace[0](0));

	//return (int)this->trace[0](this->trace[0].size() - 1);
	return this->primitives;
}

float	NVDataProvider::getGpuIdle() const
{
	if (!this->NVPMInitialized)
		return 0.0f;

	//this->unmanaged->setGpuIdle((float)this->trace[0](0));
	//return (float)this->trace[1](0);
	return this->gpuIdle;
}

void	NVDataProvider::setUnmanaged(UnmanagedPerf * unmanaged)
{
	this->unmanaged = unmanaged;
}

NVDataProvider *	NVDataProvider::get()
{
	if (instance == nullptr)
	{
		instance = new (std::nothrow) NVDataProvider();
		if (instance == nullptr)
			return nullptr;
	}
	return instance;
}

void	NVDataProvider::updateUnmanaged()
{
	this->unmanaged->_feedback->OnBasicUpdate(this->getPrimitiveCount(), this->getGpuIdle());
}