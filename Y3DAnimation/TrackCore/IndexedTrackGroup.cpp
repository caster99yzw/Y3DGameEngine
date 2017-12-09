#include "IndexedTrackGroup.h"
#include <assert.h>

INT32 ChildTrack::UniqueId = 0;

IndexedTrackGroup::IndexedTrackGroup()
{
}

IndexedTrackGroup::~IndexedTrackGroup()
{
}

INT32 IndexedTrackGroup::GetChildTrackNum() const
{
	return (INT32)m_ChildTracks.size();
}

ChildTrack* IndexedTrackGroup::GetChildTrack(INT32 index)
{
	return const_cast<ChildTrack*>(
		static_cast<IndexedTrackGroup const*>(this)->GetChildTrack(index)
		);
}

ChildTrack const* IndexedTrackGroup::GetChildTrack(INT32 index) const
{
	assert(0 <= index && index < m_ChildTracks.size());
	return &m_ChildTracks[index];
}

void IndexedTrackGroup::AddChildTrack(IndexedTrack* pTrack, std::string const& name)
{
	ChildTrack c;
	c.Track = pTrack;
	c.Name = name;
	pTrack->SetName(name.c_str());
	m_ChildTracks.push_back(c);
	//pTrack->SetGroup(this);
}

void IndexedTrackGroup::RemoveChildTrack(IndexedTrack* pTrack)
{
	for (INT32 i = 0; i < m_ChildTracks.size(); ++i)
	{
		if (m_ChildTracks[i].Track == pTrack)
		{
			m_ChildTracks.erase(m_ChildTracks.begin() + i);
			//pTrack->SetGroup(this);
		}
	}
}

void IndexedTrackGroup::InsertTrack(INT32 idx, IndexedTrack* pTrack, std::string const& name)
{
	ChildTrack c;
	c.Track = pTrack;
	c.Name = name;

	if (idx > m_ChildTracks.size())
	{
		m_ChildTracks.push_back(c);
	}
	else
	{
		m_ChildTracks.insert(m_ChildTracks.begin() + idx, c);
	}
}

void IndexedTrackGroup::Clear()
{
	for (INT32 i = 0; i < m_ChildTracks.size(); i++)
	{
		ChildTrack& childTrack = m_ChildTracks[i];
		//childTrack.Track->clear();
	}
}

INT32 IndexedTrackGroup::FindGroupIndexByID(INT32 idx)
{
	for (INT32 i = 0; i < m_ChildTracks.size(); ++i)
	{
		if (m_ChildTracks[i].GetID() == idx)
			return i;
	}
	return -1;
}

INT32 IndexedTrackGroup::FindGroupIndexByName(std::string const& name)
{
	for (INT32 i = 0; i < m_ChildTracks.size(); ++i)
	{
		if (m_ChildTracks[i].Name == name)
			return i;
	}
	return -1;
}

void IndexedTrackGroup::SetName(std::string const& name)
{
	m_strName = name;
}

std::string const& IndexedTrackGroup::GetName()
{
	return m_strName;
}

bool IndexedTrackGroup::HasChild(IndexedTrack* pTrack)
{
	for (INT32 i = 0; i < m_ChildTracks.size(); ++i)
	{
		if (m_ChildTracks[i].Track == pTrack)
			return true;

		IndexedTrackGroup* pGroup = dynamic_cast<IndexedTrackGroup*>(m_ChildTracks[i].Track);
		if (pGroup && pGroup->HasChild(pTrack))
			return true;
	}
	return false;
}

bool IndexedTrackGroup::IsAllChildEmpty()
{
	for (INT32 i = 0; i < m_ChildTracks.size(); ++i)
	{
		if (m_ChildTracks[i].Track->Num() > 0)
			return false;

		//IndexedTrackGroup* pGroup = dynamic_cast<IndexedTrackGroup*>(m_ChildTracks[i].Track);
		IndexedTrackGroup* pGroup =
			m_ChildTracks[i].Track->GetTrackKeyType() == eGroupTrack
			? (IndexedTrackGroup*)m_ChildTracks[i].Track
			: NULL;		
		if (pGroup && !pGroup->IsAllChildEmpty())
			return false;
	}
	return false;
}
