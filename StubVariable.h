#ifndef StubVariable_h
#define StubVariable_h

#include <ModularSensors.h>

// This is a fake variable that can be used for testing.

class StubVariable : public Variable
{
public:
    explicit StubVariable(const char *name, const float *values, int size, const char *uuid = "");
};
#endif
