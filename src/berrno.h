#pragma once

unsigned char BERRNO = 0;

typedef enum : unsigned char {
	BEFINE = 0,
	BEARGNULL = 1,
	BEMEMNULL = 2,
	BENODIFF = 3,
	BENOMEM = 4,
	BEEXPECTNULL = 5,
	BEBUFFNULL = 6,
} BERRNO_CODE;

const char* const BERRNO_NAMES[] = {
	"BEFINE",
	"BEARGNULL",
	"BEMEMNULL",
	"BENODIFF",
	"BENOMEM",
	"BEEXPECTNULL",
	"BEBUFFNULL",
};

const char* const BERRNO_NAMES_FULL[] = {
	"BEN_ERROR_FINE",
	"BEN_ERROR_ARGUMENT_NULL",
	"BEN_ERROR_MEMBER_NULL",
	"BEN_ERROR_NO_DIFFERENCE",
	"BEN_ERROR_NO_MEMORY",
	"BEN_ERROR_EXPECTED_NULL",
	"BEN_ERROR_BUFFER_NULL",
};

const char* const BERRNO_MESSAGES[] = {
	"no error",
	"specified function argument was null",
	"specified function argument had an unexpected null member",
	"function exited early because one or more parameters implied that nothing else needed to be done",
	"could not allocate new memory because the program ran out of heap space",
	"function expected parameter to be null, but it wasn't",
	"the pointer for a stack allocated buffer turned out to be null",
};

typedef enum : unsigned char {
	BERROR_SEVERITY_UNKNOWN = 0 << 5,
	BERROR_SEVERITY_NONE = 1 << 5,
	BERROR_SEVERITY_NOTIFY = 2 << 5,
	BERROR_SEVERITY_LOW = 3 << 5,
	BERROR_SEVERITY_MEDIUM = 4 << 5,
	BERROR_SEVERITY_HIGH = 5 << 5,
} BERROR_SEVERITY_LEVEL;

const char* const BERRNO_SEVERITY_MESSAGES[] = {
	"SEVERITY_UNKNOWN",
	"SEVERITY_NONE",
	"SEVERITY_NOTIFY",
	"SEVERITY_LOW",
	"SEVERITY_MEDIUM",
	"SEVERITY_HIGH",
};

#define BERRNO_SEVERITY BERRNO >> 5
#define BERRNO_CODE     BERRNO & 31