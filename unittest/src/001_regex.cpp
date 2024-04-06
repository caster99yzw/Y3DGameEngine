#include <iostream>
#include "gtest/gtest.h"
#include "regex/tiny_regex.h"

#define OK "ok"
#define NK "nk"

constexpr const char* matching_str[][3] = {
	{ NK, "[0-9]", "  - " },
	{ OK, "[^0-9]", "  - " },
};

TEST(regex, tiny)
{
	for (const auto& item : matching_str) {
		const bool result = re::re_match(item[1], item[2]);
		if (::strcmp(item[0], OK) == 0) {
			ASSERT_TRUE(result);
		}
		else {
			ASSERT_FALSE(result);
		}
	}
}