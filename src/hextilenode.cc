#include "common.h"
#include "hextilenode.h"

namespace addon {

using v8::Context;
using v8::Exception;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;
using namespace node;

Persistent<Function> HextileNode::constructor;

HextileNode::HextileNode(int width, int height) : hextile_(width, height), width_(width), height_(height)  {
}

HextileNode::~HextileNode() {
}

void HextileNode::Init(Local<Object> exports) {
    Isolate* isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Hextile"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "colorRect", ColorRect);
    NODE_SET_PROTOTYPE_METHOD(tpl, "decodeSync", DecodeSync);
    NODE_SET_PROTOTYPE_METHOD(tpl, "fgbgRect", FgBgRect);
    // NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);
    NODE_SET_PROTOTYPE_METHOD(tpl, "rect", Rect);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setBackground", SetBackground);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setForeground", SetForeground);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "Hextile"),
                 tpl->GetFunction());
}

void HextileNode::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check the number of arguments passed.
  if (args.Length() < 2) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }
  // Check the type of arguments passed.
  if (!args[0]->IsInt32() || !args[1]->IsInt32()) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong type of arguments")));
    return;
  }

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    int width = args[0]->Int32Value();
    int height = args[1]->Int32Value();
    // double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    HextileNode* obj = new HextileNode(width, height);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Context> context = isolate->GetCurrentContext();
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Object> result =
        cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

// void HextileNode::PlusOne(const FunctionCallbackInfo<Value>& args) {
//   Isolate* isolate = args.GetIsolate();
//
//   HextileNode* hex = ObjectWrap::Unwrap<HextileNode>(args.Holder());
//   hex->value_ += 1;
//
//   args.GetReturnValue().Set(Number::New(isolate, hex->value_));
// }

void HextileNode::ColorRect(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 5) {
    VException("Five arguments required - color, x, y, w, h.");
    return;
  }

  if (!args[0]->IsInt32()) {
    VException("First argument must be integer pixel color.");
    return;
  }
  if (!args[1]->IsInt32()) {
    VException("Second argument must be integer x.");
    return;
  }
  if (!args[2]->IsInt32()) {
    VException("Third argument must be integer y.");
    return;
  }
  if (!args[3]->IsInt32()) {
    VException("Fourth argument must be integer w.");
    return;
  }
  if (!args[4]->IsInt32()) {
    VException("Fifth argument must be integer h.");
    return;
  }

  HextileNode* hex = ObjectWrap::Unwrap<HextileNode>(args.Holder());
  int color = args[0]->Int32Value();
  int x = args[1]->Int32Value();
  int y = args[2]->Int32Value();
  int w = args[3]->Int32Value();
  int h = args[4]->Int32Value();

  hex->hextile_.color_rect(color, x, y, w, h);

  args.GetReturnValue().Set(Undefined(isolate));
}

void HextileNode::DecodeSync(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  HextileNode* hex = ObjectWrap::Unwrap<HextileNode>(args.Holder());
  args.GetReturnValue().Set(Encode((char *)hex->hextile_.get(), hex->hextile_.get_len(), BINARY));
}

void HextileNode::Rect(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 5) {
    VException("Five arguments required - buffer, x, y, w, h.");
    return;
  }

  if (!Buffer::HasInstance(args[0])) {
    VException("First argument must be Buffer.");
    return;
  }
  if (!args[1]->IsInt32()) {
    VException("Second argument must be integer x.");
    return;
  }
  if (!args[2]->IsInt32()) {
    VException("Third argument must be integer y.");
    return;
  }
  if (!args[3]->IsInt32()) {
    VException("Fourth argument must be integer w.");
    return;
  }
  if (!args[4]->IsInt32()) {
    VException("Fifth argument must be integer h.");
    return;
  }

  HextileNode* hex = ObjectWrap::Unwrap<HextileNode>(args.Holder());
  char* bufferData = node::Buffer::Data(args[0]);
  size_t bufferLength = node::Buffer::Length(args[0]);
  int x = args[1]->Int32Value();
  int y = args[2]->Int32Value();
  int w = args[3]->Int32Value();
  int h = args[4]->Int32Value();

  hex->hextile_.rect((unsigned char *)bufferData, bufferLength, x, y, w, h);

  args.GetReturnValue().Set(Undefined(isolate));
}

void HextileNode::FgBgRect(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

    if (args.Length() != 4) {
      VException("Four arguments required - x, y, w, h.");
      return;
    }

    if (!args[0]->IsInt32()) {
      VException("First argument must be integer x.");
      return;
    }
    if (!args[1]->IsInt32()) {
      VException("Second argument must be integer y.");
      return;
    }
    if (!args[2]->IsInt32()) {
      VException("Third argument must be integer w.");
      return;
    }
    if (!args[3]->IsInt32()) {
      VException("Fourth argument must be integer h.");
      return;
    }

    HextileNode* hex = ObjectWrap::Unwrap<HextileNode>(args.Holder());
    int x = args[0]->Int32Value();
    int y = args[1]->Int32Value();
    int w = args[2]->Int32Value();
    int h = args[3]->Int32Value();

    hex->hextile_.fgbg_rect(x, y, w, h);

    args.GetReturnValue().Set(Undefined(isolate));
}

void HextileNode::SetBackground(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 1) {
    VException("One argument required - background color.");
    return;
  }
  if (!args[0]->IsInt32()) {
    VException("First argument must be integer color.");
    return;
  }

  int color = args[0]->Int32Value();

  HextileNode* hex = ObjectWrap::Unwrap<HextileNode>(args.Holder());
  hex->hextile_.set_background(color);

  args.GetReturnValue().Set(Undefined(isolate));
}

void HextileNode::SetForeground(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

    if (args.Length() != 1) {
      VException("One argument required - foreground color.");
      return;
    }
    if (!args[0]->IsInt32()) {
      VException("First argument must be integer color.");
      return;
    }

    int color = args[0]->Int32Value();

    HextileNode* hex = ObjectWrap::Unwrap<HextileNode>(args.Holder());
    hex->hextile_.set_foreground(color);

    args.GetReturnValue().Set(Undefined(isolate));
}

}  // namespace addon
