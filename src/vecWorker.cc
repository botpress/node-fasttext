

#include <iostream>
#include "vecWorker.h"
#include <v8.h>

void VecWorker::Execute()
{
  try
  {
    wrapper_->loadModel();
    wrapper_->precomputeWordVectors();
    result_ = wrapper_->getWordVector(query_);
  }
  catch (std::string errorMessage)
  {
    std::cout << "Exception: " << errorMessage << std::endl;
    SetErrorMessage(errorMessage.c_str());
  }
  catch (const char *str)
  {
    std::cout << "Exception: " << str << std::endl;
    SetErrorMessage(str);
  }
  catch (const std::exception &e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
    SetErrorMessage(e.what());
  }
}

void VecWorker::HandleErrorCallback()
{
  Nan::HandleScope scope;

  auto res = GetFromPersistent("key").As<v8::Promise::Resolver>();
  res->Reject(Nan::GetCurrentContext(), Nan::Error(ErrorMessage()));
  v8::Isolate::GetCurrent()->RunMicrotasks();
}

void VecWorker::HandleOKCallback()
{
  Nan::HandleScope scope;
  v8::Local<v8::Array> result = Nan::New<v8::Array>(result_.size());

  for (unsigned int i = 0; i < result_.size(); i++)
  {
    result->Set(i, Nan::New<v8::Number>(result_[i]));
  }

  // promise resolver
  auto res = GetFromPersistent("key").As<v8::Promise::Resolver>();
  res->Resolve(Nan::GetCurrentContext(), result);
  v8::Isolate::GetCurrent()->RunMicrotasks();
}