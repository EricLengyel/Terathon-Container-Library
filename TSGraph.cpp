//
// This file is part of the Terathon Container Library, by Eric Lengyel.
// Copyright 1999-2022, Terathon Software LLC
//
// This software is distributed under the MIT License.
// Separate proprietary licenses are available from Terathon Software.
//


#include "TSGraph.h"


using namespace Terathon;


GraphElementBase::GraphElementBase()
{
}

GraphElementBase::~GraphElementBase()
{
}

GraphRelationStart *GraphElementBase::FindOutgoingRelation(const GraphElementBase *finish) const
{
	GraphRelationStart *relation = outgoingRelationList.GetFirstListElement();
	while (relation)
	{
		if (static_cast<GraphRelationFinish *>(relation)->GetFinishElement() == finish)
		{
			return (relation);
		}

		relation = relation->GetNextListElement();
	}

	return (nullptr);
}

GraphRelationStart *GraphElementBase::FindNextOutgoingRelation(const GraphElementBase *finish, const GraphRelationStart *previous)
{
	GraphRelationStart *relation = previous->GetNextListElement();
	while (relation)
	{
		if (static_cast<GraphRelationFinish *>(relation)->GetFinishElement() == finish)
		{
			return (relation);
		}

		relation = relation->GetNextListElement();
	}

	return (nullptr);
}

GraphRelationFinish *GraphElementBase::FindIncomingRelation(const GraphElementBase *start) const
{
	GraphRelationFinish *relation = incomingRelationList.GetFirstListElement();
	while (relation)
	{
		if (relation->GetStartElement() == start)
		{
			return (relation);
		}

		relation = relation->ListElement<GraphRelationFinish>::GetNextListElement();
	}

	return (nullptr);
}

GraphRelationFinish *GraphElementBase::FindNextIncomingRelation(const GraphElementBase *start, const GraphRelationFinish *previous)
{
	GraphRelationFinish *relation = previous->ListElement<GraphRelationFinish>::GetNextListElement();
	while (relation)
	{
		if (relation->GetStartElement() == start)
		{
			return (relation);
		}

		relation = relation->ListElement<GraphRelationFinish>::GetNextListElement();
	}

	return (nullptr);
}


bool GraphBase::Predecessor(const GraphElementBase *first, const GraphElementBase *second)
{
	List<GraphElementBase>		readyList;
	List<GraphElementBase>		visitedList;

	readyList.AppendListElement(const_cast<GraphElementBase *>(first));
	bool result = false;

	for (;;)
	{
		GraphElementBase *element = readyList.GetFirstListElement();
		if (!element)
		{
			break;
		}

		visitedList.AppendListElement(element);

		const GraphRelationStart *relation = element->GetFirstOutgoingRelation();
		while (relation)
		{
			GraphElementBase *finish = static_cast<const GraphRelationFinish *>(relation)->GetFinishElement();
			if (!visitedList.Member(finish))
			{
				if (finish == second)
				{
					result = true;
					goto end;
				}

				readyList.AppendListElement(finish);
			}

			relation = relation->GetNextListElement();
		}
	}

	end:
	for (;;)
	{
		GraphElementBase *element = readyList.GetFirstListElement();
		if (!element)
		{
			break;
		}

		elementList.AppendListElement(element);
	}

	for (;;)
	{
		GraphElementBase *element = visitedList.GetFirstListElement();
		if (!element)
		{
			break;
		}

		elementList.AppendListElement(element);
	}

	return (result);
}
