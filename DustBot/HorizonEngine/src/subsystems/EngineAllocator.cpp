#include "EngineAllocator.h"

#include <utilities\Logger.h>

#include <algorithm>
#include <string>
#include <stdlib.h>

bool Horizon::EngineAllocator::Init(uint64 size)
{
	m_initialAllocation = size;
	m_start = malloc(size);

	m_currentOffset = 0;
	m_allocationCount = 0;
	m_totalBytesAllocated = 0;

	return true;
}

void Horizon::EngineAllocator::Shutdown()
{
	m_allocationMutex.lock();
	free(m_start);
	m_start = nullptr;
	m_allocationMutex.unlock();
}

void Horizon::EngineAllocator::Reset()
{
	m_allocationMutex.lock();
	m_currentOffset = 0;
	m_allocationCount = 0;
	m_totalBytesAllocated = 0;
	m_allocationMutex.unlock();
}

void* Horizon::EngineAllocator::Allocate(const uint64 size, const uint64 requiredAlignment)
{
	m_allocationMutex.lock();

	const uint64 currentAddr = (uint64)m_start + m_currentOffset;

	uint64 requiredPadding = 0;
	if (requiredAlignment != 0 && m_currentOffset % requiredAlignment != 0)
	{
		const uint64 alignedAdr = ((currentAddr / requiredAlignment) + 1) * requiredAlignment;
		requiredPadding = alignedAdr - currentAddr;
	}

	if (size + requiredPadding + m_currentOffset > m_initialAllocation)
	{
		Logger::GetInstance()->LogError("[FATAL] Allocation of " + std::to_string(size) + " bytes failed as it exceeded initial allocation");
		return nullptr;
		m_allocationMutex.unlock();
	}

	m_currentOffset += requiredPadding;
	const uint64 allocAddr = currentAddr + requiredPadding;

	m_currentOffset += size;
	m_totalBytesAllocated = m_currentOffset;
	m_allocationCount++;

	m_allocationMutex.unlock();

	void* addr = (void*)allocAddr;

	return addr;
}

Horizon::uint64 Horizon::EngineAllocator::GetBytesAllocated() const
{
	return m_totalBytesAllocated;
}

Horizon::uint64 Horizon::EngineAllocator::GetInitialAllocationSize() const
{
	return m_initialAllocation;
}

Horizon::uint64 Horizon::EngineAllocator::GetAllocationCount() const
{
	return m_allocationCount;
}
