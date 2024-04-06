#include "tiny_regex.h"
#include <cstdio>
#include <cctype>

namespace re {

#define MAX_REGEXP_OBJECTS      30    /* Max number of regex symbols in expression. */
#define MAX_CHAR_CLASS_LEN      40    /* Max length of character-class buffer in.   */

enum
{
	UNUSED,
	DOT,
	BEGIN,
	END,
	QUESTIONMARK,
	STAR,
	PLUS,
	CHAR,
	CHAR_CLASS,
	INV_CHAR_CLASS,
	DIGIT,
	NOT_DIGIT,
	ALPHA,
	NOT_ALPHA,
	WHITESPACE,
	NOT_WHITESPACE, /* BRANCH */
};

typedef struct regex_t
{
	unsigned char  type;   /* CHAR, STAR, etc.                      */
	union
	{
		unsigned char  ch;   /*      the character itself             */
		unsigned char* ccl;  /*  OR  a pointer to characters in class */
	} u;
} regex_t;

regex_t* re_compile(const char* pattern)
{
	static regex_t re_compiled[MAX_REGEXP_OBJECTS];
	static unsigned char ccl_buf[MAX_CHAR_CLASS_LEN];
	int ccl_bufidx = 1;

	char c;     /* current char in pattern   */
	int i = 0;  /* index into pattern        */
	int j = 0;  /* index into re_compiled    */

	while (pattern[i] != '\0' && (j + 1 < MAX_REGEXP_OBJECTS)) {
		c = pattern[i];

		switch (c) {
			/* Meta-characters: */
			case '^': { re_compiled[j].type = BEGIN; } break;
			case '$': { re_compiled[j].type = END; } break;
			case '.': { re_compiled[j].type = DOT; } break;
			case '*': { re_compiled[j].type = STAR; } break;
			case '+': { re_compiled[j].type = PLUS; } break;
			case '?': { re_compiled[j].type = QUESTIONMARK; } break;

			case '[':
			{
				/* Remember where the char-buffer starts. */
				int buf_begin = ccl_bufidx;

				/* Look-ahead to determine if negated */
				if (pattern[i + 1] == '^') {
					re_compiled[j].type = INV_CHAR_CLASS;
					i += 1; /* Increment i to avoid including '^' in the char-buffer */
					if (pattern[i + 1] == 0) {
						return nullptr;
					}
				}
				else {
					re_compiled[j].type = CHAR_CLASS;
				}

				/* Copy characters inside [..] to buffer */
				while ((pattern[++i] != ']') && (pattern[i] != '\0')) {
					if (ccl_bufidx >= MAX_CHAR_CLASS_LEN) {
						return nullptr;
					}
					ccl_buf[ccl_bufidx++] = pattern[i];
				}

				if (ccl_bufidx >= MAX_CHAR_CLASS_LEN) {
					return nullptr;
				}
				/* Null-terminate string end */
				ccl_buf[ccl_bufidx++] = 0;
				re_compiled[j].u.ccl = &ccl_buf[buf_begin];

			} break;

			default:
			{
				re_compiled[j].type = CHAR;
				re_compiled[j].u.ch = c;
			}
		}

		if (pattern[i] == 0) {
			return nullptr;
		}

		i += 1;
		j += 1;
	}

	re_compiled[j].type = UNUSED;
	return re_compiled;
}

static void re_print(regex_t* pattern)
{
	const char* types[] = {
		"UNUSED",
		"DOT",
		"BEGIN",
		"END",
		"QUESTIONMARK",
		"STAR",
		"PLUS",
		"CHAR",
		"CHAR_CLASS",
		"INV_CHAR_CLASS",
		"DIGIT",
		"NOT_DIGIT",
		"ALPHA",
		"NOT_ALPHA",
		"WHITESPACE",
		"NOT_WHITESPACE",
		"BRANCH"
	};

	int i;
	int j;
	char c;
	for (i = 0; i < MAX_REGEXP_OBJECTS; ++i) {
		if (pattern[i].type == UNUSED) {
			break;
		}

		::printf("\ntype: %s", types[pattern[i].type]);
		if (pattern[i].type == CHAR_CLASS || pattern[i].type == INV_CHAR_CLASS) {
			::printf(" [");
			for (j = 0; j < MAX_CHAR_CLASS_LEN; ++j) {
				c = pattern[i].u.ccl[j];
				if ((c == '\0') || (c == ']')) {
					break;
				}
				::printf("%c", c);
			}
			::printf("]");
		}
		else if (pattern[i].type == CHAR) {
			::printf(" '%c'", pattern[i].u.ch);
		}
	}
	::printf("\n");
}

static int matchdigit(char c)
{
	return ::isdigit(c);
}

static int matchalpha(char c)
{
	return ::isalpha(c);
}

static int matchwhitespace(char c)
{
	return ::isspace(c);
}

static int matchalphanum(char c)
{
	return ((c == '_') || matchalpha(c) || matchdigit(c));
}

static int matchrange(char c, const char* str)
{
	return ((c != '-')
		&& (str[0] != '\0')
		&& (str[0] != '-')
		&& (str[1] == '-')
		&& (str[2] != '\0')
		&& ((c >= str[0]) && (c <= str[2]))
		);
}

static int matchcharclass(char c, const char* str)
{
	do {
		if (matchrange(c, str)) {
			return 1;
		}
		else if (c == str[0]) {
			if (c == '-') {
				return ((str[-1] == '\0') || (str[1] == '\0'));
			}
			else {
				return 1;
			}
		}
	} while (*str++ != '\0');

	return 0;
}

static int matchone(regex_t p, char c)
{
	switch (p.type) {
		case CHAR_CLASS:     return  matchcharclass(c, (const char*)p.u.ccl);
		case INV_CHAR_CLASS: return !matchcharclass(c, (const char*)p.u.ccl);
		case DIGIT:          return  matchdigit(c);
		case NOT_DIGIT:      return !matchdigit(c);
		case ALPHA:          return  matchalphanum(c);
		case NOT_ALPHA:      return !matchalphanum(c);
		case WHITESPACE:     return  matchwhitespace(c);
		case NOT_WHITESPACE: return !matchwhitespace(c);
		default:             return  (p.u.ch == c);
	}
}

static int matchpattern(regex_t* pattern, const char* text, int* matchlength)
{
	int pre = *matchlength;

	do {
		if (pattern[0].type == UNUSED) {
			return 1;
		}
		else if ((pattern[0].type == END) && pattern[1].type == UNUSED) {
			return (text[0] == '\0');
		}
		(*matchlength)++;
	} while ((text[0] != '\0') && matchone(*pattern++, *text++));

	*matchlength = pre;
	return 0;
}

static int re_matchp(regex_t* pattern, const char* text, int* matchlength)
{
	*matchlength = 0;
	if (pattern != 0) {
		if (pattern[0].type == BEGIN) {
			return ((matchpattern(&pattern[1], text, matchlength)) ? 0 : -1);
		}
		else {
			int idx = -1;

			do {
				idx += 1;

				if (matchpattern(pattern, text, matchlength)) {
					if (text[0] == '\0')
						return -1;

					return idx;
				}
			} while (*text++ != '\0');
		}
	}
	return -1;
}

bool re_match(const char* pattern, const char* txt)
{
	int matchlength = 0;
	return re_matchp(re_compile(pattern), txt, &matchlength) != -1;
}

} // namespace re
