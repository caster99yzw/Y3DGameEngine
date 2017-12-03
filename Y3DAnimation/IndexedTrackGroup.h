#pragma once
#include "..\Y3DSTLCore\CoreTypes.h"
#include "TrackCore\IndexedTrack.h"

class TrackNode
{
	INT32							tID;
	std::string						name;
	std::vector<IndexedTrack*>		track;

	std::vector<TrackNode*>			childTrackGroup;

	virtual INT32					GetTrackGroupNum() const;
	virtual TrackNode*				GetTrackNodeAtIndex();
	virtual TrackNode const*		GetTrackNodeAtIndex() const;

	virtual INT32					FindGroupIndexByID(INT32 id);
	virtual INT32					FindGroupIndexByName(std::string const& name);

	virtual void					AddTrack(IndexedTrack* pTrack, std::string const& name);
	virtual void					RemoveTrack(IndexedTrack* pTrack);
	virtual void					InsertTrack(INT32 idx, IndexedTrack* pTrack, std::string const& name);

	virtual void					Clear();

	virtual bool					Save() const = 0;
	virtual bool					Load() = 0;
	virtual bool					Serialize() = 0; 	//	This function is used in UI, such as unity

	inline void						SetGrounType();
	inline INT32					GetGroupType();

	inline void						SetGroupFlags();
	inline void						GetGroupFlags();

	inline void						SetName();
	inline std::string const&		GetName();

	// GetGroup
	// SetGroup
	// RemoveFromGroup

	// HasChild
	// IsAllChildEmpty

	// 

	virtual bool					HasChild(IndexedTrack* pChild);
	virtual	bool					IsAllChildEmpty();

	template <typename T>
	inline void						TraverseChildren(T const& func);
};

class IndexedTrackTree
{
protected:

	TrackNode*		m_rootTrackNode;
};