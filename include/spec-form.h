#pragma once

#include "object.h"

#define SPEC_DEFINE_ARGC 2
#define SPEC_DEFINE_DOT 1
scheme_object * Scheme_Special_Define(scheme_object ** objs, scheme_env* env, size_t count);

#define SPEC_IF_ARGC 3
#define SPEC_IF_DOT 0
scheme_object * Scheme_Special_If(scheme_object ** objs, scheme_env* env, size_t count);
