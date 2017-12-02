#pragma once

#include "../CoreTypes.h"

/**
* Tests if a From* is convertible to a To*
**/
template <typename From, typename To>
struct PointerIsConvertibleFromTo
{
private:
	static UINT8	Test(...);
	static UINT16	Test(To*);

public:
	enum { Value = sizeof(Test((From*)nullptr)) - 1 };
};

class PointerIsConvertibleFromTo_TestBase
{
};


class PointerIsConvertibleFromTo_TestDerived : public PointerIsConvertibleFromTo_TestBase
{
};

class PointerIsConvertibleFromTo_Unrelated
{
};

static_assert(PointerIsConvertibleFromTo<bool, bool>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(PointerIsConvertibleFromTo<void, void>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(PointerIsConvertibleFromTo<bool, void>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(PointerIsConvertibleFromTo<const bool, const void>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(PointerIsConvertibleFromTo<PointerIsConvertibleFromTo_TestDerived, PointerIsConvertibleFromTo_TestBase>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(PointerIsConvertibleFromTo<PointerIsConvertibleFromTo_TestDerived, const PointerIsConvertibleFromTo_TestBase>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(PointerIsConvertibleFromTo<const PointerIsConvertibleFromTo_TestDerived, const PointerIsConvertibleFromTo_TestBase>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(PointerIsConvertibleFromTo<PointerIsConvertibleFromTo_TestBase, PointerIsConvertibleFromTo_TestBase>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(PointerIsConvertibleFromTo<PointerIsConvertibleFromTo_TestBase, void>::Value, "Platform PointerIsConvertibleFromTo test failed.");

static_assert(!PointerIsConvertibleFromTo<PointerIsConvertibleFromTo_TestBase, PointerIsConvertibleFromTo_TestDerived>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(!PointerIsConvertibleFromTo<PointerIsConvertibleFromTo_Unrelated, PointerIsConvertibleFromTo_TestBase>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(!PointerIsConvertibleFromTo<bool, PointerIsConvertibleFromTo_TestBase>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(!PointerIsConvertibleFromTo<void, PointerIsConvertibleFromTo_TestBase>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(!PointerIsConvertibleFromTo<PointerIsConvertibleFromTo_TestBase, bool>::Value, "Platform PointerIsConvertibleFromTo test failed.");
static_assert(!PointerIsConvertibleFromTo<void, bool>::Value, "Platform PointerIsConvertibleFromTo test failed.");
