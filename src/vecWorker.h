
#ifndef VEC_WORKER_H
#define VEC_WORKER_H

#include <nan.h>
#include "wrapper.h"

class VecWorker : public Nan::AsyncWorker
{
public:
  VecWorker(std::string query, Wrapper *wrapper)
      : Nan::AsyncWorker(new Nan::Callback()),
        query_(query),
        wrapper_(wrapper),
        result_(){};

  ~VecWorker(){};

  void Execute();
  void HandleOKCallback();
  void HandleErrorCallback();

private:
  std::string query_;
  Wrapper *wrapper_;
  std::vector<double> result_;
};

#endif