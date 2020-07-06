#pragma once

#include "core\TypeAbstractions.h"
#include "core\ExportMacro.h"
#include "core\Subsystem.h"

#include <mutex>

namespace Horizon
{

#define EngineAllocate(typeToAllocate)\
(typeToAllocate*)Horizon::EngineAllocator::GetInstance()->Allocate(sizeof(typeToAllocate))

#define EngineAllocateC(typeToAllocate, constructorCall)\
new(Horizon::EngineAllocator::GetInstance()->Allocate(sizeof(typeToAllocate))) constructorCall

#define EngineAllocateAlligned(typeToAllocate, alignment)\
(typeToAllocate*)Horizon::EngineAllocator::GetInstance()->Allocate(sizeof(typeToAllocate), alignment)

	//A really basic implementation of a linear allocator. 
	//This is just for development purposes, will definitely be replaced later down the line.
	//Subsystem memory class instances are not allocated using this allocator.
	class HORIZON_API EngineAllocator : public Subsystem<EngineAllocator>
	{
	public:
		//Initialize the allocator with a size in bytes.
		bool Init(uint64 size);

		//Shutdown the allocator and free all the memory allocated by it.
		void Shutdown();
		
		//Resets the allocator current pointer.
		void Reset();

		//Allocate a block of memory with the specified alignment
		void* Allocate(const uint64 size, const uint64 alignment = 0);

		uint64 GetBytesAllocated() const;
		uint64 GetInitialAllocationSize() const;
		uint64 GetAllocationCount() const;

	private:
		uint64 m_totalBytesAllocated;
		uint64 m_initialAllocation;

		uint64 m_allocationCount;

		uint64 m_currentOffset;
		void* m_start;

		std::mutex m_allocationMutex;
	};
}