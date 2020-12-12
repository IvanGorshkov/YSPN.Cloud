#pragma once

#include <memory>

class IObserverManger {
 public:
  virtual ~IObserverManger() = default;
  virtual void Update(int b) = 0;
};

class ISubjectManager {
 public:
  virtual ~ISubjectManager() = default;
  virtual void Attach(std::unique_ptr<IObserverManger>) = 0;
  virtual void Detach(std::unique_ptr<IObserverManger>) = 0;
  virtual void Notify() = 0;
};