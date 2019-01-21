

#include <iostream>
#include "nnWorker.h"
#include <v8.h>

void NnWorker::Execute()
{
    try
    {
        wrapper_->loadModel();
        wrapper_->precomputeWordVectors();
        result_ = wrapper_->nn(query_, k_);
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

void NnWorker::HandleErrorCallback()
{
    Nan::HandleScope scope;

    auto res = GetFromPersistent("key").As<v8::Promise::Resolver>();
    res->Reject(Nan::GetCurrentContext(), Nan::Error(ErrorMessage()));
    v8::Isolate::GetCurrent()->RunMicrotasks();
}

void NnWorker::HandleOKCallback()
{
    Nan::HandleScope scope;
    v8::Local<v8::Array> result = Nan::New<v8::Array>(result_.size());

    for (unsigned int i = 0; i < result_.size(); i++)
    {
        v8::Local<v8::Object> returnObject = Nan::New<v8::Object>();

        returnObject->Set(
            Nan::New<v8::String>("label").ToLocalChecked(),
            Nan::New<v8::String>(result_[i].label.c_str()).ToLocalChecked());

        returnObject->Set(
            Nan::New<v8::String>("value").ToLocalChecked(),
            Nan::New<v8::Number>(result_[i].value));

        result->Set(i, returnObject);
    }

    // promise resolver
    auto res = GetFromPersistent("key").As<v8::Promise::Resolver>();
    res->Resolve(Nan::GetCurrentContext(), result);
    v8::Isolate::GetCurrent()->RunMicrotasks();
}