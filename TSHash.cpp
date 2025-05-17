//
// This file is part of the Terathon Container Library, by Eric Lengyel.
// Copyright 1999-2025, Terathon Software LLC
//
// This software is distributed under the MIT License.
// Separate proprietary licenses are available from Terathon Software.
//


#include "TSHash.h"

#ifdef TERATHON_DEBUG

	#include "TSTools.h"

#endif


using namespace Terathon;


HashTableElementBase::~HashTableElementBase()
{
	if (owningHashTableBucket)
	{
		owningHashTableBucket->RemoveBucketElement(this);
	}
}


HashTableBucket::~HashTableBucket()
{
	PurgeBucket();
}

void HashTableBucket::AppendBucketElement(HashTableElementBase *element)
{
	if (lastBucketElement)
	{
		lastBucketElement->nextBucketElement = element;
		element->prevBucketElement = lastBucketElement;
		lastBucketElement = element;
	}
	else
	{
		firstBucketElement = lastBucketElement = element;
	}

	element->owningHashTableBucket = this;
	owningHashTable->elementCount++;
}

void HashTableBucket::InsertBucketElementAfter(HashTableElementBase *element, HashTableElementBase *after)
{
	element->prevBucketElement = after;
	HashTableElementBase *before = after->nextBucketElement;
	element->nextBucketElement = before;

	if (before)
	{
		before->prevBucketElement = element;
	}
	else
	{
		lastBucketElement = element;
	}

	after->nextBucketElement = element;

	element->owningHashTableBucket = this;
	owningHashTable->elementCount++;
}

void HashTableBucket::RemoveBucketElement(HashTableElementBase *element)
{
	owningHashTable->elementCount--;

	HashTableElementBase *prev = element->prevBucketElement;
	HashTableElementBase *next = element->nextBucketElement;

	if (prev)
	{
		prev->nextBucketElement = next;
	}

	if (next)
	{
		next->prevBucketElement = prev;
	}

	if (firstBucketElement == element)
	{
		firstBucketElement = next;
	}

	if (lastBucketElement == element)
	{
		lastBucketElement = prev;
	}

	element->prevBucketElement = nullptr;
	element->nextBucketElement = nullptr;
	element->owningHashTableBucket = nullptr;
}

void HashTableBucket::RemoveAllBucketElements(void)
{
	HashTableElementBase *element = firstBucketElement;
	while (element)
	{
		HashTableElementBase *next = element->nextBucketElement;
		element->prevBucketElement = nullptr;
		element->nextBucketElement = nullptr;
		element->owningHashTableBucket = nullptr;
		element = next;
	}

	firstBucketElement = nullptr;
	lastBucketElement = nullptr;
}

void HashTableBucket::PurgeBucket(void)
{
	while (firstBucketElement)
	{
		delete firstBucketElement;
	}
}


HashTableBase::HashTableBase(int32 initialBucketCount, int32 maxAverageDepth)
{
	#ifdef TERATHON_TOOLS

		Assert((initialBucketCount & (initialBucketCount - 1)) == 0, "HashTableBase::HashTableBase(), bucket count must be a power of 2");

	#endif

	elementCount = 0;
	bucketCount = initialBucketCount;
	resizeLimit = initialBucketCount * maxAverageDepth;

	bucketTable = reinterpret_cast<HashTableBucket *>(new char[sizeof(HashTableBucket) * initialBucketCount]);
	for (machine a = 0; a < initialBucketCount; a++)
	{
		new(&bucketTable[a]) HashTableBucket(this);
	}
}

HashTableBase::~HashTableBase()
{
	for (machine a = bucketCount - 1; a >= 0; a--)
	{
		bucketTable[a].~HashTableBucket();
	}

	delete[] reinterpret_cast<char *>(bucketTable);
}

void HashTableBase::RemoveAllHashTableElements(void)
{
	for (machine a = bucketCount - 1; a >= 0; a--)
	{
		bucketTable[a].RemoveAllBucketElements();
	}

	elementCount = 0;
}

void HashTableBase::PurgeHashTable(void)
{
	for (machine a = bucketCount - 1; a >= 0; a--)
	{
		bucketTable[a].PurgeBucket();
	}

	elementCount = 0;
}

void HashTableBase::ResizeBucketTable(void)
{
	int32 newBucketCount = bucketCount * 2;
	HashTableBucket *newBucketTable = reinterpret_cast<HashTableBucket *>(new char[sizeof(HashTableBucket) * newBucketCount]);
	for (machine a = 0; a < newBucketCount; a++)
	{
		new(&newBucketTable[a]) HashTableBucket(this);
	}

	elementCount = 0;

	int32 count = bucketCount;
	for (machine a = count - 1; a >= 0; a--)
	{
		HashTableBucket *bucket = &bucketTable[a];
		HashTableElementBase *element = bucket->firstBucketElement;
		while (element)
		{
			HashTableElementBase *next = element->nextBucketElement;

			element->prevBucketElement = nullptr;
			element->nextBucketElement = nullptr;
			newBucketTable[element->hashValue & (newBucketCount - 1)].AppendBucketElement(element);

			element = next;
		}

		bucket->firstBucketElement = nullptr;
		bucket->lastBucketElement = nullptr;
		bucket->~HashTableBucket();
	}

	delete[] reinterpret_cast<char *>(bucketTable);

	bucketTable = newBucketTable;
	bucketCount = newBucketCount;
	resizeLimit *= 2;
}
