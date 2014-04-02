#include <stdio.h>
#include "graphics.h"
#include "device.h"
#include "private.h"
#include <vector>

/* Locals */
static int adapter_count = 0;
static std::vector<IAdapter*> adapters;

/* Summary */
/* Get the default system adapter */
extern "C" int DefaultAdapter(AdapterPtr *pAdapter)
{
	IAdapter *tmp;

	if ((pAdapter == NULL))
		return GI_ERROR;

	if ((adapters.size() <= 0))
		return GI_ERROR;

	if ((tmp = adapters.front()) == NULL)
		return GI_ERROR;

	(*pAdapter) = (AdapterPtr)tmp;

	return GI_SUCCESS;
}

/* Summary */
/* Enumerates the adapters (video cards). */
extern "C" int EnumAdapters(int index, AdapterPtr *pAdapter)
{
	if ((pAdapter == NULL))
	{
		return GI_ERROR;
	}

	if ((index < 0) || (index >= adapters.size()))
	{
		return GI_ERROR;
	}

	IAdapter *tmp;
	if ((tmp = adapters.at(index)) == NULL)
	{
		return GI_ERROR;
	}

	(*pAdapter) = (AdapterPtr)tmp;

	return 1;
}

/* Summary */
/* Registers an adapter to */
/* provide client-side driver. */
extern "C" int RegisterAdapter(IAdapter *adapter)
{
	if ((adapter == NULL))
	{
		return GI_ERROR;
	}

	adapters.push_back(adapter);

	return 1;
}



