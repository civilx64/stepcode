#ifndef  SDAIHEADER_SECTION_SCHEMA_SCHEMA_CC
#define  SDAIHEADER_SECTION_SCHEMA_SCHEMA_CC
// This file was generated by exp2cxx.  You probably don't want to edit
// it since your modifications will be lost if exp2cxx is used to
// regenerate it.

#include "editor/SdaiSchemaInit.h"

void HeaderSchemaInit( Registry & reg ) {
    HeaderInitSchemasAndEnts( reg );
    SdaiHEADER_SECTION_SCHEMAInit( reg );
    reg.SetCompCollect( 0 );
}

#endif
