#pragma once

#include <iostream>
#include <vector>
#include <memory>

template<typename T>
class FutureResult;

struct AbstractTask {
  AbstractTask() = default;
  virtual void CalculateResult() = 0;
};

template<typename T>
auto GetRequiredValue(T& task) -> decltype(task) {
  return task;
}

template<typename T>
auto GetRequiredValue(FutureResult<T>& task) -> decltype(task.GetResult()) {
  return task.GetResult();
}

template<typename T>
struct FunctionalTask: public AbstractTask {
  FunctionalTask() = default;
  virtual T GetResult() = 0;
};

template<typename ResultType>
struct AbstractFunctionHolder {
  virtual ResultType GetResult() = 0;
  virtual void CalculateResult() = 0;
};

template<typename ResultType, typename T1, typename T2>
class TaskTwoArgs: public FunctionalTask<ResultType> {
public:
  TaskTwoArgs() = default;

  template<typename Function>
  TaskTwoArgs(const Function& function, const T1& a, const T2& b)
  : holder_(std::make_unique<FunctionHolder<Function>>(FunctionHolder<Function>(function, a, b)))
  , a_(a)
  , b_(b) {
  }

  TaskTwoArgs(const TaskTwoArgs<ResultType, T1, T2>& task)
  : holder_(task.holder_)
  , a_(task.a_)
  , b_(task.b_) {
  }

  TaskTwoArgs<ResultType, T1, T2>& operator=(const TaskTwoArgs<ResultType, T1, T2>& task) {
    holder_ = task.holder_;
    a_ = task.a_;
    b_ = task.b_;
  }

  void CalculateResult() override final {
    holder_->CalculateResult();
  }

  ResultType GetResult() override final {
    return holder_->GetResult();
  }
  
private:
  template<typename Function>
  class FunctionHolder: public AbstractFunctionHolder<ResultType> {
  public:
    FunctionHolder() = default;

    FunctionHolder(const Function& function, const T1& a, const T2& b)
    : function_(function)
    , a_(a)
    , b_(b) {
    }

    FunctionHolder(const FunctionHolder<Function>& holder)
    : function_(holder.function_)
    , a_(holder.a_)
    , b_(holder.b_)
    , result_(holder.result_)
    , is_result_set_(holder.is_result_set_) {
    }

    FunctionHolder<Function>& operator=(const FunctionHolder<Function>& holder) {
      function_ = holder.function_;
      a_ = holder.a_;
      b_ = holder.b_;
      result_ = holder.result_;
      is_result_set_ = holder.is_result_set_;
    }

    void CalculateResult() override final {
      result_ = is_result_set_ ? result_ : function_(GetRequiredValue(a_), GetRequiredValue(b_));
      is_result_set_ = true;
    }

    void GnaTehaKhtsi() {
      
    }

    ResultType GetResult() override final {
      CalculateResult();

      return result_;
    }

  private:
    Function function_;
    T1 a_;
    T2 b_;
    ResultType result_;
    bool is_result_set_ = false;
  };

  std::unique_ptr<AbstractFunctionHolder<ResultType>> holder_;
  T1 a_;
  T2 b_;
};

template<typename ResultType, typename T>
class TaskOneArg: public FunctionalTask<ResultType> {
public:
  TaskOneArg() = default;

  template<typename Function>
  TaskOneArg(Function& function, const T& a)
  : holder_(std::make_unique<FunctionHolder<Function>>(FunctionHolder<Function>(function, a)))
  , a_(a) {
  }

  TaskOneArg(const TaskOneArg<ResultType, T>& task)
  : holder_(task.holder_)
  , a_(task.a_) {
  }

  TaskOneArg<ResultType, T>& operator=(const TaskOneArg<ResultType, T>& task) {
    holder_ = task.holder_;
    a_ = task.a_;
  }

  void CalculateResult() override final {
    holder_->CalculateResult();
  }

  ResultType GetResult() override final {
    return holder_->GetResult();
  }
  
private:
  template<typename Function>
  class FunctionHolder: public AbstractFunctionHolder<ResultType> {
  public:
    FunctionHolder() = default;

    FunctionHolder(const Function& function, const T& a)
    : function_(function)
    , a_(a) {
    }

    FunctionHolder(const FunctionHolder<Function>& holder)
    : function_(holder.function_)
    , a_(holder.a_)
    , result_(holder.result_)
    , is_result_set_(holder.is_result_set_) {
    }

    FunctionHolder<Function>& operator=(const FunctionHolder<Function>& holder) {
      function_ = holder.function_;
      a_ = holder.a_;
      result_ = holder.result_;
      is_result_set_ = holder.is_result_set_;
    }

    void CalculateResult() override final {
      result_ = is_result_set_ ? result_ : function_(GetRequiredValue(a_));
      is_result_set_ = true;
    }

    ResultType GetResult() override final {
      CalculateResult();

      return result_;
    }

  private:
    Function function_;
    T a_;
    ResultType result_;
    bool is_result_set_ = false;
  };

  std::unique_ptr<AbstractFunctionHolder<ResultType>> holder_;
  T a_;
};

template<typename ResultType>
class TaskNoArgs: public FunctionalTask<ResultType> {
public:
  TaskNoArgs() = default;

  template<typename Function>
  TaskNoArgs(const Function& function)
  : holder_(std::make_unique<FunctionHolder<Function>>(FunctionHolder<Function>(function))) {
  }

  TaskNoArgs(const TaskNoArgs<ResultType>& task)
  : holder_(task.holder_) {
  }

  TaskNoArgs<ResultType>& operator=(const TaskNoArgs<ResultType>& task) {
    holder_ = task.holder_;
  }

  void CalculateResult() override final {
    holder_->CalculateResult();
  }

  ResultType GetResult() override final {
    return holder_->GetResult();
  }
  
private:
  template<typename Function>
  class FunctionHolder: public AbstractFunctionHolder<ResultType> {
  public:
    FunctionHolder() = default;

    FunctionHolder(const Function& function)
    : function_(function) {
    }

    FunctionHolder(const FunctionHolder<Function>& holder)
    : function_(holder.function_)
    , result_(holder.result_)
    , is_result_set_(holder.is_result_set_) {
    }

    FunctionHolder<Function>& operator=(const FunctionHolder<Function>& holder) {
      function_ = holder.function_;
      result_ = holder.result_;
      is_result_set_ = holder.is_result_set_;
    }

    void CalculateResult() override final {
      result_ = is_result_set_ ? result_ : function_();
      is_result_set_ = true;
    }

    ResultType GetResult() override final {
      CalculateResult();

      return result_;
    }

  private:
    Function function_;
    ResultType result_;
    bool is_result_set_ = false;
  };

  std::unique_ptr<AbstractFunctionHolder<ResultType>> holder_;
};

template<typename ResultType>
class FutureResult {
public:
  FutureResult() = default;

  FutureResult(std::unique_ptr<AbstractTask>& task)
  : task_(*dynamic_cast<FunctionalTask<ResultType>*>(task.get())) {
  }

  ResultType GetResult() {
    return task_.GetResult();
  }

private:
  FunctionalTask<ResultType>& task_;
};

class TTaskScheduler {
public:
  TTaskScheduler() = default;

  template<typename Function, typename T1, typename T2>
  const std::size_t add(Function func, T1 a, T2 b) {
    TaskTwoArgs<decltype(func(GetRequiredValue(a), GetRequiredValue(b))), T1, T2> task(func, a, b);
    tasks_.push_back(std::make_unique<TaskTwoArgs<decltype(func(GetRequiredValue(a),
                                                                GetRequiredValue(b))), T1, T2>>(func, a, b));

    return tasks_.size() - 1;
  }

  template<typename Function, typename T>
  const std::size_t add(Function func, T a) {
    TaskOneArg<decltype(func(GetRequiredValue(a))), T> task(func, a);
    tasks_.push_back(std::make_unique<TaskOneArg<decltype(func(GetRequiredValue(a))), T>>(func, a));

    return tasks_.size() - 1;
  }

  template<typename Function>
  const std::size_t add(const Function& func) {
    TaskNoArgs<decltype(func())> task(func);
    tasks_.push_back(std::make_unique<TaskNoArgs<decltype(func())>>(func));

    return tasks_.size() - 1;
  }

  template<typename T>
  T getResult(const std::size_t sz) {
    return FutureResult<T>(tasks_[sz]).GetResult();
  }

  template<typename T>
  FutureResult<T> getFutureResult(const std::size_t sz) {
    return FutureResult<T>(tasks_[sz]);
  }

  void executeAll() const {
    for (const auto& task: tasks_) {
      task->CalculateResult();
    }
  }

private:
  std::vector<std::unique_ptr<AbstractTask>> tasks_;
};

