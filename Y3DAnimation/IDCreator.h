#pragma once

template <typename IdType>
class IdCreate
{
	IdType			CreatorID();

private:
	static IdType	UniqueID;
};
