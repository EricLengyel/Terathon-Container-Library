//
// This file is part of the Terathon Container Library, by Eric Lengyel.
// Copyright 1999-2025, Terathon Software LLC
//
// This software is distributed under the MIT License.
// Separate proprietary licenses are available from Terathon Software.
//


#ifndef TSGraph_h
#define TSGraph_h


/// \component	Utility Library
/// \prefix		Utilities/


#include "TSList.h"


#define TERATHON_GRAPH 1


namespace Terathon
{
	class GraphElementBase;

	template <class, class>
	class GraphElement;

	template <class, class>
	class Graph;


	class GraphRelationStart : public ListElement<GraphRelationStart>
	{
		friend class GraphElementBase;

		private:

			GraphElementBase	*startElement;

			GraphRelationStart(const GraphRelationStart&) = delete;
			GraphRelationStart& operator =(const GraphRelationStart&) = delete;

		protected:

			GraphRelationStart(GraphElementBase *start);

			~GraphRelationStart()
			{
			}

			GraphElementBase *GetStartElement(void) const
			{
				return (startElement);
			}

			void SetStartElement(GraphElementBase *start);
			void AttachRelation(void);
	};


	class GraphRelationFinish : public GraphRelationStart, public ListElement<GraphRelationFinish>
	{
		friend class GraphElementBase;
		friend class GraphBase;

		private:

			GraphElementBase	*finishElement;

		protected:

			GraphRelationFinish(GraphElementBase *start, GraphElementBase *finish);

			~GraphRelationFinish()
			{
			}

			GraphElementBase *GetFinishElement(void) const
			{
				return (finishElement);
			}

			void SetFinishElement(GraphElementBase *finish);

		public:

			void DetachRelation(void)
			{
				ListElement<GraphRelationStart>::Detach();
				ListElement<GraphRelationFinish>::Detach();
			}

			void AttachRelation(void);
	};


	class GraphElementBase : public ListElement<GraphElementBase>
	{
		friend class GraphRelationStart;
		friend class GraphRelationFinish;
		friend class GraphBase;

		private:

			List<GraphRelationStart>	outgoingRelationList;
			List<GraphRelationFinish>	incomingRelationList;

			GraphElementBase(const GraphElementBase&) = delete;
			GraphElementBase& operator =(const GraphElementBase&) = delete;

		protected:

			TERATHON_API GraphElementBase();
			TERATHON_API ~GraphElementBase();

			GraphRelationStart *GetFirstOutgoingRelation(void) const
			{
				return (outgoingRelationList.GetFirstListElement());
			}

			GraphRelationStart *GetLastOutgoingRelation(void) const
			{
				return (outgoingRelationList.GetLastListElement());
			}

			GraphRelationFinish *GetFirstIncomingRelation(void) const
			{
				return (incomingRelationList.GetFirstListElement());
			}

			GraphRelationFinish *GetLastIncomingRelation(void) const
			{
				return (incomingRelationList.GetLastListElement());
			}

			GraphRelationStart *GetOutgoingRelation(int32 index) const
			{
				return (outgoingRelationList[index]);
			}

			GraphRelationFinish *GetIncomingRelation(int32 index) const
			{
				return (incomingRelationList[index]);
			}

			TERATHON_API GraphRelationStart *FindOutgoingRelation(const GraphElementBase *finish) const;
			TERATHON_API static GraphRelationStart *FindNextOutgoingRelation(const GraphElementBase *finish, const GraphRelationStart *previous);

			TERATHON_API GraphRelationFinish *FindIncomingRelation(const GraphElementBase *start) const;
			TERATHON_API static GraphRelationFinish *FindNextIncomingRelation(const GraphElementBase *start, const GraphRelationFinish *previous);

		public:

			int32 GetOutgoingRelationCount(void) const
			{
				return (outgoingRelationList.GetListElementCount());
			}

			int32 GetIncomingRelationCount(void) const
			{
				return (incomingRelationList.GetListElementCount());
			}

			bool IsolatedGraphElement(void) const
			{
				return ((outgoingRelationList.Empty()) && (incomingRelationList.Empty()));
			}

			void AddOutgoingRelation(GraphRelationStart *relation)
			{
				outgoingRelationList.AppendListElement(relation);
				relation->startElement = this;
			}

			void AddIncomingRelation(GraphRelationFinish *relation)
			{
				incomingRelationList.AppendListElement(relation);
				relation->finishElement = this;
			}

			void PurgeOutgoingRelations(void)
			{
				outgoingRelationList.PurgeList();
			}

			void PurgeIncomingRelations(void)
			{
				incomingRelationList.PurgeList();
			}
	};


	/// \class	GraphRelation		The base class for relations connecting elements in a directed graph.
	///
	/// The $GraphRelation$ class is the base class for relations connecting elements in a directed graph.
	///
	/// \def	template <class elementType, class relationType> class GraphRelation : public GraphRelationFinish
	///
	/// \tparam		elementType		The type of the class that can be stored as an element in the graph.
	/// \tparam		relationType	The type of the class that represents the relations in the graph. This parameter should
	///								be the type of the class that inherits directly from the $GraphRelation$ class.
	///
	/// \ctor	GraphRelation(GraphElement<elementType, relationType> *start, GraphElement<elementType, relationType> *finish);
	///
	/// \param	start	A pointer to the element at the start of the relation.
	/// \param	finish	A pointer to the element at the finish of the relation.
	///
	/// \desc
	/// The $GraphRelation$ class should be declared as a base class for objects that represent relations in a directed graph.
	/// The $relationType$ template parameter should match the class type of such objects. The $elementType$ template
	/// parameter should match the type of the class representing the elements in the graph.
	///
	/// The $start$ and $finish$ parameters specify the graph elements at the start and finish of the directed relation.
	/// These may not be $nullptr$. When a $GraphRelation$ instance is constructed, the relation automatically becomes an
	/// outgoing relation for the element specified by the $start$ parameter and an incoming relation for the element specified
	/// by the $finish$ parameter.
	///
	/// \privbase	GraphRelationFinish		Used internally to encapsulate common functionality that is independent of the template parameter.
	///
	/// \also	$@GraphElement@$
	/// \also	$@Graph@$


	/// \function	GraphRelation::GetStartElement		Returns the graph element at which a relation starts.
	///
	/// \proto		elementType *GetStartElement(void) const;
	///
	/// \desc
	/// The $GetStartElement$ function returns the graph element at which a relation starts.
	///
	/// \also	$@GraphRelation::SetStartElement@$
	/// \also	$@GraphRelation::GetFinishElement@$
	/// \also	$@GraphRelation::SetFinishElement@$


	/// \function	GraphRelation::SetStartElement		Sets the graph element at which a relation starts.
	///
	/// \proto		void SetStartElement(elementType *start);
	///
	/// \param	start	The new start element. This may not be $nullptr$.
	///
	/// \desc
	/// The $SetStartElement$ function sets the graph element at which a relation starts to that specified by the $start$ parameter.
	/// The relation is removed from the list of outgoing relations for its previous start element, and it is added to the list of
	/// outgoing relations for its new start element.
	///
	/// \also	$@GraphRelation::GetStartElement@$
	/// \also	$@GraphRelation::GetFinishElement@$
	/// \also	$@GraphRelation::SetFinishElement@$


	/// \function	GraphRelation::GetFinishElement		Returns the graph element at which a relation finishes.
	///
	/// \proto		elementType *GetFinishElement(void) const;
	///
	/// \desc
	/// The $GetFinishElement$ function returns the graph element at which a relation finishes.
	///
	/// \also	$@GraphRelation::SetFinishElement@$
	/// \also	$@GraphRelation::GetStartElement@$
	/// \also	$@GraphRelation::SetStartElement@$


	/// \function	GraphRelation::SetFinishElement		Sets the graph element at which a relation finishes.
	///
	/// \proto		void SetFinishElement(elementType *finish);
	///
	/// \param	finish	The new finish element. This may not be $nullptr$.
	///
	/// \desc
	/// The $SetFinishElement$ function sets the graph element at which a relation finishes to that specified by the $finish$ parameter.
	/// The relation is removed from the list of incoming relations for its previous finish element, and it is added to the list of
	/// incoming relations for its new finish element.
	///
	/// \also	$@GraphRelation::GetFinishElement@$
	/// \also	$@GraphRelation::GetStartElement@$
	/// \also	$@GraphRelation::SetStartElement@$


	/// \function	GraphRelation::GetPreviousOutgoingRelation		Returns the previous outgoing relation for a graph element.
	///
	/// \proto		relationType *GetPreviousOutgoingRelation(void) const;
	///
	/// \desc
	/// The $GetPreviousOutgoingRelation$ function returns the previous outgoing relation for the graph element
	/// at which the relation for which the function is called starts. If there is no previous outgoing relation,
	/// then the return value is $nullptr$.
	///
	/// To iterate through all outgoing relations for a graph element using this function, first call the
	/// $@GraphElement::GetLastOutgoingRelation@$ function for the graph element, and then call the $GetPreviousOutgoingRelation$
	/// function iteratively for the returned relations until $nullptr$ is returned.
	///
	/// \also	$@GraphRelation::GetNextOutgoingRelation@$
	/// \also	$@GraphRelation::GetPreviousIncomingRelation@$
	/// \also	$@GraphRelation::GetNextIncomingRelation@$
	/// \also	$@GraphElement::GetLastOutgoingRelation@$


	/// \function	GraphRelation::GetNextOutgoingRelation		Returns the next outgoing relation for a graph element.
	///
	/// \proto		relationType *GetNextOutgoingRelation(void) const;
	///
	/// \desc
	/// The $GetNextOutgoingRelation$ function returns the next outgoing relation for the graph element
	/// at which the relation for which the function is called starts. If there is no next outgoing relation,
	/// then the return value is $nullptr$.
	///
	/// To iterate through all outgoing relations for a graph element using this function, first call the
	/// $@GraphElement::GetFirstOutgoingRelation@$ function for the graph element, and then call the $GetNextOutgoingRelation$
	/// function iteratively for the returned relations until $nullptr$ is returned.
	///
	/// \also	$@GraphRelation::GetPreviousOutgoingRelation@$
	/// \also	$@GraphRelation::GetPreviousIncomingRelation@$
	/// \also	$@GraphRelation::GetNextIncomingRelation@$
	/// \also	$@GraphElement::GetFirstOutgoingRelation@$


	/// \function	GraphRelation::GetPreviousIncomingRelation		Returns the previous incoming relation for a graph element.
	///
	/// \proto		relationType *GetPreviousIncomingRelation(void) const;
	///
	/// \desc
	/// The $GetPreviousIncomingRelation$ function returns the previous incoming relation for the graph element
	/// at which the relation for which the function is called finishes. If there is no previous incoming relation,
	/// then the return value is $nullptr$.
	///
	/// To iterate through all incoming relations for a graph element using this function, first call the
	/// $@GraphElement::GetLastIncomingRelation@$ function for the graph element, and then call the $GetPreviousIncomingRelation$
	/// function iteratively for the returned relations until $nullptr$ is returned.
	///
	/// \also	$@GraphRelation::GetNextIncomingRelation@$
	/// \also	$@GraphRelation::GetPreviousOutgoingRelation@$
	/// \also	$@GraphRelation::GetNextOutgoingRelation@$
	/// \also	$@GraphElement::GetLastIncomingRelation@$


	/// \function	GraphRelation::GetNextIncomingRelation		Returns the next incoming relation for a graph element.
	///
	/// \proto		relationType *GetNextIncomingRelation(void) const;
	///
	/// \desc
	/// The $GetNextIncomingRelation$ function returns the next incoming relation for the graph element
	/// at which the relation for which the function is called finishes. If there is no next incoming relation,
	/// then the return value is $nullptr$.
	///
	/// To iterate through all incoming relations for a graph element using this function, first call the
	/// $@GraphElement::GetFirstIncomingRelation@$ function for the graph element, and then call the $GetNextIncomingRelation$
	/// function iteratively for the returned relations until $nullptr$ is returned.
	///
	/// \also	$@GraphRelation::GetPreviousIncomingRelation@$
	/// \also	$@GraphRelation::GetPreviousOutgoingRelation@$
	/// \also	$@GraphRelation::GetNextOutgoingRelation@$
	/// \also	$@GraphElement::GetFirstIncomingRelation@$


	template <class elementType, class relationType>
	class GraphRelation : public GraphRelationFinish
	{
		protected:

			GraphRelation(GraphElement<elementType, relationType> *start, GraphElement<elementType, relationType> *finish) : GraphRelationFinish(start, finish)
			{
			}

		public:

			elementType *GetStartElement(void) const
			{
				return (static_cast<elementType *>(static_cast<GraphElement<elementType, relationType> *>(GraphRelationStart::GetStartElement())));
			}

			void SetStartElement(elementType *start)
			{
				GraphRelationStart::SetStartElement(start);
			}

			elementType *GetFinishElement(void) const
			{
				return (static_cast<elementType *>(static_cast<GraphElement<elementType, relationType> *>(GraphRelationFinish::GetFinishElement())));
			}

			void SetFinishElement(elementType *finish)
			{
				GraphRelationFinish::SetFinishElement(finish);
			}

			relationType *GetPreviousOutgoingRelation(void) const
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(ListElement<GraphRelationStart>::GetPreviousListElement())));
			}

			relationType *GetNextOutgoingRelation(void) const
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(ListElement<GraphRelationStart>::GetNextListElement())));
			}

			relationType *GetPreviousIncomingRelation(void) const
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(ListElement<GraphRelationFinish>::GetPreviousListElement())));
			}

			relationType *GetNextIncomingRelation(void) const
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(ListElement<GraphRelationFinish>::GetNextListElement())));
			}
	};


	/// \class	GraphElement		The base class for elements in a directed graph.
	///
	/// The $GraphElement$ class is the base class for elements in a directed graph.
	///
	/// \def	template <class elementType, class relationType> class GraphElement : public GraphElementBase
	///
	/// \tparam		elementType		The type of the class that can be stored as an element in the graph. This parameter should
	///								be the type of the class that inherits directly from the $GraphElement$ class.
	/// \tparam		relationType	The type of the class that represents the relations in the graph.
	///
	/// \ctor	GraphElement();
	/// \ctor	GraphElement(Graph<elementType, relationType> *graph);
	///
	/// \param	graph	The graph to which the element should be added.
	///
	/// \desc
	/// The $GraphElement$ class should be declared as a base class for objects that represent elements in a directed graph.
	/// The $elementType$ template parameter should match the class type of such objects. The $relationType$ template
	/// parameter should match the type of the class representing the relations in the graph.
	///
	/// If the $graph$ parameter is specified, then the element is added to the graph object that it points to.
	///
	/// \privbase	GraphElementBase	Used internally to encapsulate common functionality that is independent of the template parameter.
	///
	/// \also	$@GraphRelation@$
	/// \also	$@Graph@$


	/// \function	GraphElement::GetFirstOutgoingRelation		Returns the first outgoing relation for a graph element.
	///
	/// \proto		relationType *GetFirstOutgoingRelation(void) const;
	///
	/// \desc
	/// The $GetFirstOutgoingRelation$ function returns the first outgoing relation for a graph element.
	/// If there are no outgoing relations, then the return value is $nullptr$.
	///
	/// To iterate through all outgoing relations for a graph element, first call the $@GraphElement::GetFirstOutgoingRelation@$
	/// function, and then call the $GraphRelation::GetNextOutgoingRelation$ function iteratively for the returned relations
	/// until $nullptr$ is returned.
	///
	/// \also	$@GraphElement::GetLastOutgoingRelation@$
	/// \also	$@GraphElement::GetFirstIncomingRelation@$
	/// \also	$@GraphElement::GetLastIncomingRelation@$
	/// \also	$@GraphRelation::GetNextOutgoingRelation@$


	/// \function	GraphElement::GetLastOutgoingRelation		Returns the last outgoing relation for a graph element.
	///
	/// \proto		relationType *GetLastOutgoingRelation(void) const;
	///
	/// \desc
	/// The $GetLastOutgoingRelation$ function returns the last outgoing relation for a graph element.
	/// If there are no outgoing relations, then the return value is $nullptr$.
	///
	/// To iterate through all outgoing relations for a graph element, first call the $@GraphElement::GetLastOutgoingRelation@$
	/// function, and then call the $GraphRelation::GetPreviousOutgoingRelation$ function iteratively for the returned relations
	/// until $nullptr$ is returned.
	///
	/// \also	$@GraphElement::GetFirstOutgoingRelation@$
	/// \also	$@GraphElement::GetFirstIncomingRelation@$
	/// \also	$@GraphElement::GetLastIncomingRelation@$
	/// \also	$@GraphRelation::GetPreviousOutgoingRelation@$


	/// \function	GraphElement::GetFirstIncomingRelation		Returns the first incoming relation for a graph element.
	///
	/// \proto		relationType *GetFirstIncomingRelation(void) const;
	///
	/// \desc
	/// The $GetFirstIncomingRelation$ function returns the first incoming relation for a graph element.
	/// If there are no incoming relations, then the return value is $nullptr$.
	///
	/// To iterate through all incoming relations for a graph element, first call the $@GraphElement::GetFirstIncomingRelation@$
	/// function, and then call the $GraphRelation::GetNextIncomingRelation$ function iteratively for the returned relations
	/// until $nullptr$ is returned.
	///
	/// \also	$@GraphElement::GetLastIncomingRelation@$
	/// \also	$@GraphElement::GetFirstOutgoingRelation@$
	/// \also	$@GraphElement::GetLastOutgoingRelation@$
	/// \also	$@GraphRelation::GetNextIncomingRelation@$


	/// \function	GraphElement::GetLastIncomingRelation		Returns the last incoming relation for a graph element.
	///
	/// \proto		relationType *GetLastIncomingRelation(void) const;
	///
	/// \desc
	/// The $GetLastIncomingRelation$ function returns the last incoming relation for a graph element.
	/// If there are no incoming relations, then the return value is $nullptr$.
	///
	/// To iterate through all incoming relations for a graph element, first call the $@GraphElement::GetLastIncomingRelation@$
	/// function, and then call the $GraphRelation::GetPreviousIncomingRelation$ function iteratively for the returned relations
	/// until $nullptr$ is returned.
	///
	/// \also	$@GraphElement::GetFirstIncomingRelation@$
	/// \also	$@GraphElement::GetFirstOutgoingRelation@$
	/// \also	$@GraphElement::GetLastOutgoingRelation@$
	/// \also	$@GraphRelation::GetPreviousIncomingRelation@$


	/// \function	GraphElement::FindOutgoingRelation		Returns the outgoing relation with a specific finish element.
	///
	/// \proto		relationType *FindOutgoingRelation(const GraphElementBase *finish) const;
	///
	/// \param	finish		The graph element at which the relation finishes.
	///
	/// \desc
	/// The $FindOutgoingRelation$ function searches for an outgoing relation starting at the graph element for which
	/// the function is called and finishing at the graph element specified by the $finish$ parameter. If such
	/// a relation is found, then it is returned. Otherwise, the return value is $nullptr$.
	///
	/// \also	$@GraphElement::FindIncomingRelation@$


	/// \function	GraphElement::FindIncomingRelation		Returns the incoming relation with a specific start element.
	///
	/// \proto		relationType *FindIncomingRelation(const GraphElementBase *start) const;
	///
	/// \param	start		The graph element at which the relation start.
	///
	/// \desc
	/// The $FindIncomingRelation$ function searches for an incoming relation starting at the graph element specified
	/// by the $start$ parameter and finishing at the graph element for which the function is called. If such
	/// a relation is found, then it is returned. Otherwise, the return value is $nullptr$.
	///
	/// \also	$@GraphElement::FindOutgoingRelation@$


	template <class elementType, class relationType>
	class GraphElement : public GraphElementBase
	{
		protected:

			GraphElement()
			{
			}

			GraphElement(Graph<elementType, relationType> *graph)
			{
				graph->AddGraphElement(this);
			}

		public:

			elementType *GetPreviousGraphElement(void) const
			{
				return (static_cast<elementType *>(static_cast<GraphElement<elementType, relationType> *>(GraphElementBase::GetPreviousListElement())));
			}

			elementType *GetNextGraphElement(void) const
			{
				return (static_cast<elementType *>(static_cast<GraphElement<elementType, relationType> *>(GraphElementBase::GetNextListElement())));
			}

			relationType *GetFirstOutgoingRelation(void) const
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(GraphElementBase::GetFirstOutgoingRelation())));
			}

			relationType *GetLastOutgoingRelation(void) const
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(GraphElementBase::GetLastOutgoingRelation())));
			}

			relationType *GetFirstIncomingRelation(void) const
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(GraphElementBase::GetFirstIncomingRelation())));
			}

			relationType *GetLastIncomingRelation(void) const
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(GraphElementBase::GetLastIncomingRelation())));
			}

			relationType *GetOutgoingRelation(int32 index) const
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(GraphElementBase::GetOutgoingRelation(index))));
			}

			relationType *GetIncomingRelation(int32 index) const
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(GraphElementBase::GetIncomingRelation(index))));
			}

			relationType *FindOutgoingRelation(const GraphElementBase *finish) const
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(GraphElementBase::FindOutgoingRelation(finish))));
			}

			static relationType *FindNextOutgoingRelation(const GraphElementBase *finish, const relationType *previous)
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(GraphElementBase::FindNextOutgoingRelation(finish, previous))));
			}

			relationType *FindIncomingRelation(const GraphElementBase *start) const
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(GraphElementBase::FindIncomingRelation(start))));
			}

			static relationType *FindNextIncomingRelation(const GraphElementBase *start, const relationType *previous)
			{
				return (static_cast<relationType *>(static_cast<GraphRelation<elementType, relationType> *>(GraphElementBase::FindNextIncomingRelation(start, previous))));
			}
	};


	class GraphBase
	{
		private:

			List<GraphElementBase>		elementList;

		protected:

			GraphBase() {}
			~GraphBase() {}

			GraphElementBase *GetFirstGraphElement(void) const
			{
				return (elementList.GetFirstListElement());
			}

			GraphElementBase *GetLastGraphElement(void) const
			{
				return (elementList.GetLastListElement());
			}

			void AddGraphElement(GraphElementBase *element)
			{
				elementList.AppendListElement(element);
			}

			void RemoveGraphElement(GraphElementBase *element)
			{
				element->PurgeIncomingRelations();
				element->PurgeOutgoingRelations();
				elementList.RemoveListElement(element);
			}

			void DetachGraphElement(GraphElementBase *element)
			{
				elementList.RemoveListElement(element);
			}

		public:

			bool Empty(void) const
			{
				return (elementList.Empty());
			}

			int32 GetGraphElementCount(void) const
			{
				return (elementList.GetListElementCount());
			}

			void PurgeGraph(void)
			{
				elementList.PurgeList();
			}

			TERATHON_API bool Predecessor(const GraphElementBase *first, const GraphElementBase *second);
	};


	template <class elementType, class relationType>
	class GraphIterator
	{
		private:

			elementType		*iteratorElement;

		public:

			GraphIterator(elementType *element) : iteratorElement(element) {}

			elementType *operator *(void) const
			{
				return (iteratorElement);
			}

			GraphIterator& operator ++(void)
			{
				iteratorElement = iteratorElement->GraphElement<elementType, relationType>::GetNextGraphElement();
				return (*this);
			}

			bool operator ==(const GraphIterator& iterator) const
			{
				return (iteratorElement == iterator.iteratorElement);
			}

			bool operator !=(const GraphIterator& iterator) const
			{
				return (iteratorElement != iterator.iteratorElement);
			}
	};


	/// \class	Graph		Encapsulates a directed graph.
	///
	/// The $Graph$ class encapsulates a directed graph.
	///
	/// \def	template <class elementType, class relationType> class Graph : public GraphBase
	///
	/// \tparam		elementType		The type of the class that can be stored as an element in the graph.
	/// \tparam		relationType	The type of the class that represents the relations in the graph.
	///
	/// \ctor	Graph();
	///
	/// \desc
	/// The $Graph$ class is used to organize a set of elements and relations arranged as a directed graph.
	///
	/// \privbase	GraphBase	Used internally to encapsulate common functionality that is independent of the template parameter.
	///
	/// \also	$@GraphElement@$
	/// \also	$@GraphRelation@$


	/// \function	Graph::Empty	Returns a boolean value indicating whether any elements exist in a graph.
	///
	/// \proto		bool Empty(void) const;
	///
	/// \desc
	/// The $Empty$ function returns $true$ if no elements belong to a graph. If at least one element belongs to
	/// a graph, then the return value is $false$.
	///
	/// \also	$@Graph::GetGraphElementCount@$
	/// \also	$@Graph::PurgeGraph@$


	/// \function	Graph::GetGraphElementCount		Returns the number of elements in a graph.
	///
	/// \proto		int32 GetGraphElementCount(void) const;
	///
	/// \desc
	/// The $GetGraphElementCount$ function returns the number of elements in a graph.
	///
	/// If the only information needed is whether a graph contains a nonzero number of elements, then it is
	/// more efficient to call the $@Graph::Empty@$ function.
	///
	/// \also	$@Graph::Empty@$
	/// \also	$@Graph::PurgeGraph@$


	/// \function	Graph::PurgeGraph	Destroys all elements belonging to a graph.
	///
	/// \proto		void PurgeGraph(void);
	///
	/// \desc
	/// The $PurgeGraph$ function destroys all of the elements belonging to a graph. This process also
	/// causes every relation belonging to the graph to be destroyed. The graph is completely
	/// empty after this function is called.
	///
	/// \also	$@Graph::Empty@$
	/// \also	$@Graph::GetGraphElementCount@$
	/// \also	$@Graph::RemoveGraphElement@$


	/// \function	Graph::AddGraphElement		Adds an element to a graph.
	///
	/// \proto		void AddGraphElement(GraphElement<elementType, relationType> *element);
	///
	/// \param	element		The element to add to the graph.
	///
	/// \desc
	/// The $AddGraphElement$ function adds the element specified by the $element$ parameter to a graph.
	/// Initially, the element has no incoming or outgoing relations.
	///
	/// \also	$@Graph::RemoveGraphElement@$
	/// \also	$@Graph::PurgeGraph@$


	/// \function	Graph::RemoveGraphElement		Removes an element from a graph.
	///
	/// \proto		void RemoveGraphElement(GraphElement<elementType, relationType> *element);
	///
	/// \param	element		The element to remove from the graph.
	///
	/// \desc
	/// The $RemoveGraphElement$ function removes the element specified by the $element$ parameter from a graph.
	/// This function destroys all incoming and outgoing relations for the element.
	///
	/// \also	$@Graph::AddGraphElement@$
	/// \also	$@Graph::PurgeGraph@$


	/// \function	Graph::Predecessor		Returns a boolean value indicating whether one element precedes another.
	///
	/// \proto		bool Predecessor(const GraphElementBase *first, const GraphElementBase *second);
	///
	/// \param	first	The first element in the graph.
	/// \param	second	The second element in the graph.
	///
	/// \desc
	/// The $Predecessor$ function returns $true$ if the element specified by the $first$ parameter precedes the
	/// element specified by the $second$ parameter in a graph, and it returns $false$ otherwise. In order for the
	/// first element to precede the second, there must be a sequence of relations starting at the first element that can
	/// be followed through a sequence of elements that ultimately finishes at the second element.
	///
	/// \also	$@GraphElement::GetFirstOutgoingRelation@$
	/// \also	$@GraphElement::GetLastOutgoingRelation@$
	/// \also	$@GraphElement::GetFirstIncomingRelation@$
	/// \also	$@GraphElement::GetLastIncomingRelation@$


	template <class elementType, class relationType>
	class Graph : public GraphBase
	{
		public:

			inline Graph() = default;
			inline ~Graph() = default;

			elementType *GetFirstGraphElement(void) const
			{
				return (static_cast<elementType *>(static_cast<GraphElement<elementType, relationType> *>(GraphBase::GetFirstGraphElement())));
			}

			elementType *GetLastGraphElement(void) const
			{
				return (static_cast<elementType *>(static_cast<GraphElement<elementType, relationType> *>(GraphBase::GetLastGraphElement())));
			}

			GraphIterator<elementType, relationType> begin(void) const
			{
				return (GraphIterator<elementType, relationType>(static_cast<elementType *>(static_cast<GraphElement<elementType, relationType> *>(GraphBase::GetFirstGraphElement()))));
			}

			GraphIterator<elementType, relationType> end(void) const
			{
				return (GraphIterator<elementType, relationType>(nullptr));
			}

			void AddGraphElement(GraphElement<elementType, relationType> *element)
			{
				GraphBase::AddGraphElement(element);
			}

			void RemoveGraphElement(GraphElement<elementType, relationType> *element)
			{
				GraphBase::RemoveGraphElement(element);
			}

			void DetachGraphElement(GraphElement<elementType, relationType> *element)
			{
				GraphBase::DetachGraphElement(element);
			}
	};


	inline GraphRelationStart::GraphRelationStart(GraphElementBase *start)
	{
		startElement = start;
		start->outgoingRelationList.AppendListElement(this);
	}

	inline void GraphRelationStart::SetStartElement(GraphElementBase *start)
	{
		startElement = start;
		start->outgoingRelationList.AppendListElement(this);
	}

	inline void GraphRelationStart::AttachRelation(void)
	{
		startElement->outgoingRelationList.AppendListElement(this);
	}


	inline GraphRelationFinish::GraphRelationFinish(GraphElementBase *start, GraphElementBase *finish) : GraphRelationStart(start)
	{
		finishElement = finish;
		finish->incomingRelationList.AppendListElement(this);
	}

	inline void GraphRelationFinish::SetFinishElement(GraphElementBase *finish)
	{
		finishElement = finish;
		finish->incomingRelationList.AppendListElement(this);
	}

	inline void GraphRelationFinish::AttachRelation(void)
	{
		GraphRelationStart::AttachRelation();
		finishElement->incomingRelationList.AppendListElement(this);
	}
}


#endif
