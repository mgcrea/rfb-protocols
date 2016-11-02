#ifndef HEXTILENODE_H
#define HEXTILENODE_H

#include <node.h>
#include <node_object_wrap.h>
#include <node_buffer.h>

#include "hextile.h"

namespace addon {

class HextileNode : public node::ObjectWrap {

  public:
    static void Init(v8::Local<v8::Object> exports);

  private:
    explicit HextileNode(int width, int height);
    ~HextileNode();
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void ColorRect(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void DecodeSync(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void FgBgRect(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Rect(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetBackground(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetForeground(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void PlusOne(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Persistent<v8::Function> constructor;
    HextileDecoder hextile_;
    double value_;
    int width_;
    int height_;
};

}  // namespace addon

#endif
