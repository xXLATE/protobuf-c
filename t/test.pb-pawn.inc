/* Generated by the protocol buffer compiler for Pawn. DO NOT EDIT! */
/* source: t/test.proto */

#if defined __T_TEST_PB_PAWN_INC__
#endinput
#endif
#define __T_TEST_PB_PAWN_INC__

#include "protobuf-c/protobuf-c.pb-pawn.inc"

// Message: Person
enum E_PERSON_DATA {
    P_NAME = 0,
    P_ID = 1,
    P_EMAIL = 2,
    P_PHONE = 3,
}

// Enum: PhoneType
#define MOBILE 0
#define HOME 1
#define WORK 2

// Message: PhoneNumber
enum E_PHONENUMBER_DATA {
    P_NUMBER = 0,
    P_TYPE = 1,
    P_COMMENT = 2,
}

// Message: Comment
enum E_COMMENT_DATA {
    P_COMMENT = 0,
}

// Message: LookupResult
enum E_LOOKUPRESULT_DATA {
    P_PERSON = 0,
}

// Message: Name
enum E_NAME_DATA {
    P_NAME = 0,
}

// Service Descriptors
// Service: DirLookup
// Method count: 1
