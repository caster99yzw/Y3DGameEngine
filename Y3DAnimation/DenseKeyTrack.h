#pragma once
#include "TrackCore/IndexedKeyTrack.h"
#include "Y3DQuaternion.h"
#include "Y3DVector.h"
using namespace Y3D;

class DenseQuatFloatTrack : public IndexedKeyTrack<QuatF32>
{
	typedef IndexedKeyTrack<QuatF32>	Super;

public:

	virtual UINT32						GetTrackKeyType() const override;
	virtual DenseQuatFloatTrack*		Clone() const override;

	inline KeyType						Evaluate(TimeType time) const;
	inline KeyType						Evaluate(TimeType time, KeyType const& fallbackVal) const;
	inline QuatF32						EvaluateTangent(TimeType t);
};

class DenseVec3FloatTrack : public IndexedKeyTrack<Vec3F32>
{
	typedef IndexedKeyTrack<Vec3F32>	Super;

public:

	explicit							DenseVec3FloatTrack(INT32 keyCount = 0);

public:

	virtual UINT32						GetTrackKeyType() const override;
	virtual DenseVec3FloatTrack*		Clone() const override;

	inline Vec3F32						EvaluateTangent(TimeType t);
	inline void							RemoveRedundentKeys();
};

class DenseTransformTrack
{
public:

	virtual void						EvaluatePosition(AnimTimeType t, Vec3F32 & pos) = 0;
	virtual void						EvaluateRotationEuler(AnimTimeType t, QuatF32 & euler) = 0;
	//virtual void						EvaluateTransform(AnimTimeType t, Transform & pos) = 0;

	virtual void						AddOrUpdatePosition(AnimTimeType t, Vec3F32 & pos) = 0;
	virtual void						AddOrUpdateRotationEuler(AnimTimeType t, QuatF32 & euler) = 0;
	//virtual void						AddOrUpdateTransform(AnimTimeType t, Transform & pos) = 0;
};

class 