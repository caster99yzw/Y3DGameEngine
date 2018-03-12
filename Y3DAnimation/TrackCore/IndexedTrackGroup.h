#pragma once
#include "CoreTypes.h"
#include "TrackCore/IndexedTrack.h"

#define ABit( num )	(1 << ( num ))

enum EGroupType
{
	eBuiltInGroupBit = ABit(31),
	TransformGroup = 1 | eBuiltInGroupBit,
};

struct ChildTrack
{
	ChildTrack() : Id(++UniqueId), Track(nullptr) {}
	INT32 GetID() const { return Id; }

	IndexedTrack*	Track;
	std::string		Name;
private:
	static INT32 UniqueId;
	INT32 Id;
};


class IndexedTrackGroup
{
public:

	explicit						IndexedTrackGroup();
	virtual							~IndexedTrackGroup();

	virtual INT32					GetChildTrackNum() const;
	virtual ChildTrack*				GetChildTrack(INT32 index);
	virtual ChildTrack const*		GetChildTrack(INT32 index) const;

	virtual INT32					FindGroupIndexByID(INT32 id);
	virtual INT32					FindGroupIndexByName(std::string const& name);

	virtual void					AddChildTrack(IndexedTrack* pTrack, std::string const& name);
	virtual void					RemoveChildTrack(IndexedTrack* pTrack);
	virtual void					InsertTrack(INT32 idx, IndexedTrack* pTrack, std::string const& name);

	virtual void					Clear();

	virtual bool					Save() const = 0;
	virtual bool					Load() = 0;
	virtual bool					Serialize() = 0; 	//	This function is used in UI, such as unity

	inline void						multisetGrounType();
	inline INT32					GetGroupType();

	inline void						multisetGroupFlags();
	inline void						GetGroupFlags();

	inline void						multisetName(std::string const& name);
	inline std::string const&		GetName();

	// GetGroup
	// multisetGroup
	// RemoveFromGroup

	virtual bool					HasChild(IndexedTrack* pTrack);
	virtual bool					IsAllChildEmpty();

	template <typename T>
	inline void						TraverseChildren(T const& func);

protected:

	std::vector<ChildTrack>			m_ChildTracks;
	std::string						m_strName;
};


template <typename T>
void IndexedTrackGroup::TraverseChildren(T const& func)
{
	for (int i = 0; i < GetChildTrackNum(); ++i)
	{
		IndexedTrack* pTrack = GetChildTrack(i)->Track;
		if (Invoke(func, pTrack)) continue;
		if (pTrack->GetTrackKeyType() == eGroupTrack)
		{
			IndexedTrackGroup* pGroup = (IndexedTrackGroup*)pTrack;
			pGroup->TraverseChildren(func);
		}
	}
}