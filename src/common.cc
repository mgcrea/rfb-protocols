#include "common.h"

using namespace v8;

void VException(const char *msg) {
  Isolate* isolate = args.GetIsolate();
  // Throw an Error that is passed back to JavaScript
  isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, msg)));
  return;
}
