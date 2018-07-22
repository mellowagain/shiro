/**
 * Copyright 2014-2015 Denis Blank <denis.blank@outlook.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SHIRO_TASKSCHEDULER_HH
#define SHIRO_TASKSCHEDULER_HH

#include <algorithm>
#include <chrono>
#include <functional>
#include <vector>
#include <queue>
#include <memory>
#include <utility>
#include <random>
#include <mutex>
#include <set>

namespace tsc
{

    class TaskContext;

/// The TaskScheduler class provides the ability to schedule std::function's in the near future.
/// Use TaskScheduler::Update to update the scheduler.
/// Popular methods are:
/// * Schedule (Schedules a std::function which will be executed in the near future).
/// * Schedules an asynchronous function which will be executed at the next update tick.
/// * Cancel, Delay & Reschedule (Methods to manipulate already scheduled tasks).
/// Tasks are organized in groups (uint), multiple tasks can have the same group id,
/// you can provide a group or not, but keep in mind that you can only manipulate specific tasks through its group id!
/// Tasks callbacks use the function signature void(TaskContext) where TaskContext provides
/// access to the function schedule plan which makes it possible to repeat the task
/// with the same duration or a new one.
/// It also provides access to the repeat counter which is useful for task that repeat itself often
/// but behave different every time (spoken event dialogs for example).
    class TaskScheduler
    {
        friend class TaskContext;

        // Time definitions (use steady clock)
        typedef std::chrono::steady_clock clock_t;
        typedef clock_t::time_point timepoint_t;

        // Duration calculator
        typedef std::function<clock_t::duration()> duration_calculator_t;

        // Static time
        template<typename _Rep, typename _Period>
        static duration_calculator_t MakeDurationCalculator(const std::chrono::duration<_Rep, _Period>& duration)
        {
            return [duration]
            {
                return duration;
            };
        }

        // Random time between min and max
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        static duration_calculator_t MakeDurationCalculator(
                const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                const std::chrono::duration<_RepRight, _PeriodRight>& max)
        {
            return std::bind(RandomDurationBetween<_RepLeft, _PeriodLeft, _RepRight, _PeriodRight>, min, max);
        }

        // Task group type
        typedef unsigned int group_t;
        // Task repeated type
        typedef unsigned int repeated_t;

        // Task handle type
        typedef std::function<void(TaskContext)> task_handler_t;
        // Predicate type
        typedef std::function<bool()> predicate_t;
        // Success handle type
        typedef std::function<void()> success_t;

        // C++11 std::make_unique workarround
        template<typename T, typename... Args>
        static std::unique_ptr<T> MakeUnique(Args&&... args)
        {
            return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
        }

        class Task
        {
            friend class TaskContext;
            friend class TaskScheduler;

            timepoint_t _end;
            duration_calculator_t _duration_calculator;
            std::unique_ptr<group_t> _group;
            repeated_t _repeated;
            task_handler_t _task;

        public:
            // All Argument construct
            Task(const timepoint_t& end, duration_calculator_t&& duration_calculator,
                 const group_t group,
                 const repeated_t repeated, const task_handler_t& task)
                    : _end(end), _duration_calculator(std::move(duration_calculator)),
                      _group(TaskScheduler::MakeUnique<group_t>(group)),
                      _repeated(repeated), _task(task) { }

            // Minimal Argument construct
            Task(const timepoint_t& end, duration_calculator_t&& duration_calculator,
                 const task_handler_t& task)
                    : _end(end), _duration_calculator(std::move(duration_calculator)),
                      _group(nullptr), _repeated(0), _task(task) { }

            // Copy construct
            Task(const Task&) = delete;
            // Move construct
            Task(Task&&) = delete;
            // Copy Assign
            Task& operator= (const Task&) = delete;
            // Move Assign
            Task& operator= (Task&& right) = delete;

            // Order tasks by its end
            inline bool operator< (const Task& other) const
            {
                return _end < other._end;
            }

            inline bool operator> (const Task& other) const
            {
                return _end > other._end;
            }

            // Compare tasks with its end
            inline bool operator== (const Task& other)
            {
                return _end == other._end;
            }

            // Returns true if the task is in the given group
            inline bool IsInGroup(const group_t group) const
            {
                return _group && (*_group == group);
            }
        };

        typedef std::shared_ptr<Task> TaskContainer;

        /// Container which provides Task order, insert and reschedule operations.
        struct Compare
        {
            bool operator() (const TaskContainer& left, const TaskContainer& right) const
            {
                return (*left.get()) < (*right.get());
            };
        };

        class TaskQueue
        {
            std::multiset<TaskContainer, Compare> container;

        public:
            // Pushes the task in the container
            void Push(TaskContainer&& task);

            /// Pops the task out of the container
            TaskContainer Pop();

            const TaskContainer& First() const;

            void Clear();

            void RemoveIf(const std::function<bool(const TaskContainer&)> & filter);

            void ModifyIf(const std::function<bool(const TaskContainer&)>& filter);

            bool IsEmpty() const;
        };

        /// Contains a self reference to track if this object was deleted or not.
        std::shared_ptr<TaskScheduler> self_reference;

        /// The current time point (now)
        timepoint_t _now;

        /// The Task Queue which contains all task objects.
        TaskQueue _task_holder;

        typedef std::queue<std::function<void()>> AsyncHolder;

        /// Contains all asynchronous tasks which will be invoked at
        /// the next update tick.
        AsyncHolder _asyncHolder;

        predicate_t _predicate;

        static bool EmptyValidator()
        {
            return true;
        }

        static void EmptyCallback()
        {
        }

    public:
        TaskScheduler()
                : self_reference(this, [](const TaskScheduler*) { }),
                  _now(clock_t::now()), _predicate(EmptyValidator) { }

        template<typename P>
        TaskScheduler(P&& predicate)
                : self_reference(this, [](const TaskScheduler*) { }),
                  _now(clock_t::now()), _predicate(std::forward<P>(predicate)) { }

        TaskScheduler(const TaskScheduler&) = delete;
        TaskScheduler(TaskScheduler&&) = delete;
        TaskScheduler& operator= (const TaskScheduler&) = delete;
        TaskScheduler& operator= (TaskScheduler&&) = delete;

        /// Sets a Validator which is asked if tasks are allowed to be executed.
        template<typename P>
        TaskScheduler& SetValidator(P&& predicate)
        {
            _predicate = std::forward<P>(predicate);
            return *this;
        }

        /// Clears the Validator which is asked if tasks are allowed to be executed.
        TaskScheduler& ClearValidator();

        /// Update the scheduler to the current time.
        /// Calls the optional callback on successfully finish.
        TaskScheduler& Update(const success_t& callback = EmptyCallback);

        /// Update the scheduler with a difftime in ms.
        /// Calls the optional callback on successfully finish.
        TaskScheduler& Update(const size_t milliseconds, const success_t& callback = EmptyCallback);

        /// Update the scheduler with a difftime.
        /// Calls the optional callback on successfully finish.
        template<typename _Rep, typename _Period>
        TaskScheduler& Update(const std::chrono::duration<_Rep, _Period>& difftime,
                              const success_t& callback = EmptyCallback)
        {
            _now += difftime;
            Dispatch(callback);
            return *this;
        }

        /// Schedule an callable function that is executed at the next update tick.
        /// Its safe to modify the TaskScheduler from within the callable.
        TaskScheduler& Async(const std::function<void()>& callable);

        /// Schedule an event with a fixed rate.
        /// Never call this from within a task context! Use TaskContext::Schedule instead!
        template<typename _Rep, typename _Period>
        TaskScheduler& Schedule(const std::chrono::duration<_Rep, _Period>& time,
                                const task_handler_t& task)
        {
            return ScheduleAt(_now, MakeDurationCalculator(time), task);
        }

        /// Schedule an event with a fixed rate.
        /// Never call this from within a task context! Use TaskContext::Schedule instead!
        template<typename _Rep, typename _Period>
        TaskScheduler& Schedule(const std::chrono::duration<_Rep, _Period>& time,
                                const group_t group, const task_handler_t& task)
        {
            return ScheduleAt(_now, MakeDurationCalculator(time), group, task);
        }

        /// Schedule an event with a randomized rate between min and max rate.
        /// Never call this from within a task context! Use TaskContext::Schedule instead!
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskScheduler& Schedule(const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                                const std::chrono::duration<_RepRight, _PeriodRight>& max,
                                const task_handler_t& task)
        {
            return ScheduleAt(_now, MakeDurationCalculator(min, max), task);
        }

        /// Schedule an event with a fixed rate.
        /// Never call this from within a task context! Use TaskContext::Schedule instead!
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskScheduler& Schedule(const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                                const std::chrono::duration<_RepRight, _PeriodRight>& max, const group_t group,
                                const task_handler_t& task)
        {
            return ScheduleAt(_now, MakeDurationCalculator(min, max), group, task);
        }

        /// Cancels all tasks.
        /// Never call this from within a task context! Use TaskContext::CancelAll instead!
        TaskScheduler& CancelAll();

        /// Cancel all tasks of a single group.
        /// Never call this from within a task context! Use TaskContext::CancelGroup instead!
        TaskScheduler& CancelGroup(const group_t group);

        /// Cancels all groups in the given std::vector.
        /// Hint: Use std::initializer_list for this: "{1, 2, 3, 4}"
        TaskScheduler& CancelGroupsOf(const std::vector<group_t>& groups);

        /// Delays all tasks with the given duration.
        template<typename _Rep, typename _Period>
        TaskScheduler& DelayAll(const std::chrono::duration<_Rep, _Period>& duration)
        {
            _task_holder.ModifyIf([&duration](const TaskContainer& task) -> bool
                                  {
                                      task->_end += duration;
                                      return true;
                                  });
            return *this;
        }

        /// Delays all tasks with a random duration between min and max.
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskScheduler& DelayAll(const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                                const std::chrono::duration<_RepRight, _PeriodRight>& max)
        {
            return DelayAll(RandomDurationBetween(min, max));
        }

        /// Delays all tasks of a group with the given duration.
        template<typename _Rep, typename _Period>
        TaskScheduler& DelayGroup(const group_t group, const std::chrono::duration<_Rep, _Period>& duration)
        {
            _task_holder.ModifyIf([&duration, group](const TaskContainer& task) -> bool
                                  {
                                      if (task->IsInGroup(group))
                                      {
                                          task->_end += duration;
                                          return true;
                                      }
                                      else
                                          return false;
                                  });
            return *this;
        }

        /// Delays all tasks of a group with a random duration between min and max.
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskScheduler& DelayGroup(const group_t group,
                                  const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                                  const std::chrono::duration<_RepRight, _PeriodRight>& max)
        {
            return DelayGroup(group, RandomDurationBetween(min, max));
        }

        /// Reschedule all tasks with a given duration.
        template<typename _Rep, typename _Period>
        TaskScheduler& RescheduleAll(const std::chrono::duration<_Rep, _Period>& duration)
        {
            return RescheduleAt(_now + duration);
        }

        /// Reschedule all tasks with a random duration between min and max.
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskScheduler& RescheduleAll(const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                                     const std::chrono::duration<_RepRight, _PeriodRight>& max)
        {
            return RescheduleAll(RandomDurationBetween(min, max));
        }

        /// Reschedule all tasks of a group with the given duration.
        template<typename _Rep, typename _Period>
        TaskScheduler& RescheduleGroup(const group_t group, const std::chrono::duration<_Rep, _Period>& duration)
        {
            return RescheduleAtWithPredicate(_now + duration, [&](const TaskContainer& task)
            {
                return task->IsInGroup(group);
            });
        }

        /// Reschedule all tasks of a group with a random duration between min and max.
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskScheduler& RescheduleGroup(const group_t group,
                                       const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                                       const std::chrono::duration<_RepRight, _PeriodRight>& max)
        {
            return RescheduleGroup(group, RandomDurationBetween(min, max));
        }

    private:
        /// Insert a new task to the enqueued tasks.
        TaskScheduler& InsertTask(TaskContainer task);

        TaskScheduler& ScheduleAt(const timepoint_t& end,
                                  duration_calculator_t&& duration_calculator, const task_handler_t& task)
        {
            return InsertTask(TaskContainer(new Task(end + duration_calculator(), std::move(duration_calculator), task)));
        }

        /// Schedule an event with a fixed rate.
        /// Never call this from within a task context! Use TaskContext::schedule instead!
        TaskScheduler& ScheduleAt(const timepoint_t& end,
                                  duration_calculator_t&& duration_calculator,
                                  const group_t group, const task_handler_t& task)
        {
            // FIXME Reuse std::unique_ptr
            static const repeated_t DEFAULT_REPEATED = 0;
            return InsertTask(TaskContainer(new Task(end + duration_calculator(), std::move(duration_calculator), group, DEFAULT_REPEATED, task)));
        }

        static bool AlwaysTruePredicate(const TaskContainer&)
        {
            return true;
        }

        TaskScheduler& RescheduleAt(const timepoint_t& end)
        {
            return RescheduleAtWithPredicate(end, AlwaysTruePredicate);
        }

        /// Reschedule all tasks with a given duration relative to the given time.
        TaskScheduler& RescheduleAtWithPredicate(const timepoint_t& end, const std::function<bool(const TaskContainer&)>& predicate)
        {
            _task_holder.ModifyIf([&](const TaskContainer& task) -> bool
                                  {
                                      if (predicate(task))
                                      {
                                          task->_end = end;
                                          return true;
                                      }
                                      else
                                          return false;
                                  });
            return *this;
        }

        // Returns a random duration between min and max
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        std::chrono::duration<_RepLeft, _PeriodLeft>
        static RandomDurationBetween(const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                                     const std::chrono::duration<_RepRight, _PeriodRight>& max)
        {
            if (!min.count() && !max.count())
                return std::chrono::duration<_RepLeft, _PeriodLeft>(0);

            using normalized_t = std::chrono::duration<_RepLeft, _PeriodLeft>;
            auto normalized = std::chrono::duration_cast<normalized_t>(max);

            if (min.count() > normalized.count())
                throw std::logic_error("min > max");

            static std::mutex _lock;
            std::lock_guard<std::mutex> guard(_lock);

            // Random engine
            static std::random_device rd;
            static std::mt19937 rng(rd());

            std::uniform_int_distribution<typename std::chrono::duration<_RepLeft, _PeriodLeft>::rep>
                    _distribution(min.count(), normalized.count());

            // Distribute
            return std::chrono::duration<_RepLeft, _PeriodLeft>(_distribution(rng));
        }

        /// Dispatch remaining tasks when the given condition fits.
        void Dispatch(const success_t& callback);
    };

    class TaskContext
    {
        friend class TaskScheduler;

        /// Associated task
        TaskScheduler::TaskContainer _task;

        /// Owner
        std::weak_ptr<TaskScheduler> _owner;

        /// Marks the task as consumed
        std::shared_ptr<bool> _consumed;

        /// Dispatches an action safe on the TaskScheduler
        TaskContext& Dispatch(const std::function<TaskScheduler&(TaskScheduler&)>& apply);

        // Copy assign
        // TODO Make TaskContext move only
        TaskContext& operator= (const TaskContext& right)
        {
            _task = right._task;
            _owner = right._owner;
            _consumed = right._consumed;
            return *this;
        }

        // Copy construct
        // TODO Make TaskContext move only
        TaskContext(const TaskContext& right)
                : _task(right._task), _owner(right._owner), _consumed(right._consumed) { }

    public:
        // Empty constructor
        TaskContext()
                : _task(), _owner(), _consumed(std::make_shared<bool>(true)) { }

        // Construct from task and owner
        explicit TaskContext(TaskScheduler::TaskContainer&& task, std::weak_ptr<TaskScheduler>&& owner)
                : _task(task), _owner(owner), _consumed(std::make_shared<bool>(false)) { }

        // Move construct
        TaskContext(TaskContext&& right)
                : _task(std::move(right._task)), _owner(std::move(right._owner)),
                  _consumed(std::move(right._consumed)) { }

        // Move assign
        TaskContext& operator= (TaskContext&& right)
        {
            _task = std::move(right._task);
            _owner = std::move(right._owner);
            _consumed = std::move(right._consumed);
            return *this;
        }

        /// Returns true if the owner was deallocated and this context has expired.
        bool IsExpired() const;

        /// Returns true if the event is in the given group
        bool IsInGroup(const TaskScheduler::group_t group) const;

        /// Sets the event in the given group
        TaskContext& SetGroup(const TaskScheduler::group_t group);

        /// Removes the group from the event
        TaskContext& ClearGroup();

        /// Returns the repeat counter which increases every time the task is repeated.
        TaskScheduler::repeated_t GetRepeatCounter() const;

        /// Repeats the event and sets a new duration.
        /// std::chrono::seconds(5) for example.
        /// This will consume the task context, its not possible to repeat the task again
        /// from the same task context!
        template<typename _Rep, typename _Period>
        TaskContext& Repeat(const std::chrono::duration<_Rep, _Period>& duration)
        {
            _task->_duration_calculator = TaskScheduler::MakeDurationCalculator(duration);
            return Repeat();
        }

        /// Repeats the event with the same duration.
        /// This will consume the task context, its not possible to repeat the task again
        /// from the same task context!
        TaskContext& Repeat()
        {
            ThrowOnConsumed();

            // Set new duration, in-context timing and increment repeat counter
            _task->_end += _task->_duration_calculator();
            _task->_repeated += 1;
            (*_consumed) = true;
            return Dispatch(std::bind(&TaskScheduler::InsertTask, std::placeholders::_1, std::cref(_task)));
        }

        /// Repeats the event and set a new duration that is randomized between min and max.
        /// std::chrono::seconds(5) for example.
        /// This will consume the task context, its not possible to repeat the task again
        /// from the same task context!
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskContext& Repeat(const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                            const std::chrono::duration<_RepRight, _PeriodRight>& max)
        {
            _task->_duration_calculator = TaskScheduler::MakeDurationCalculator(min, max);
            return Repeat();
        }

        /// Schedule a callable function that is executed at the next update tick from within the context.
        /// Its safe to modify the TaskScheduler from within the callable.
        TaskContext& Async(const std::function<void()>& callable);

        /// Schedule an event with a fixed rate from within the context.
        /// Its possible that the new event is executed immediately!
        /// Use TaskScheduler::Async to create a task
        /// which will be called at the next update tick.
        template<typename _Rep, typename _Period>
        TaskContext& Schedule(const std::chrono::duration<_Rep, _Period>& time,
                              const TaskScheduler::task_handler_t& task)
        {
            return Dispatch([&](TaskScheduler& scheduler) -> TaskScheduler&
                            {
                                return scheduler.ScheduleAt(_task->_end, TaskScheduler::MakeDurationCalculator(time), task);
                            });
        }

        /// Schedule an event with a fixed rate from within the context.
        /// Its possible that the new event is executed immediately!
        /// Use TaskScheduler::Async to create a task
        /// which will be called at the next update tick.
        template<typename _Rep, typename _Period>
        TaskContext& Schedule(const std::chrono::duration<_Rep, _Period>& time,
                              const TaskScheduler::group_t group, const TaskScheduler::task_handler_t& task)
        {
            return Dispatch([&](TaskScheduler& scheduler) -> TaskScheduler&
                            {
                                return scheduler.ScheduleAt(_task->_end, TaskScheduler::MakeDurationCalculator(time), group, task);
                            });
        }

        /// Schedule an event with a randomized rate between min and max rate from within the context.
        /// Its possible that the new event is executed immediately!
        /// Use TaskScheduler::Async to create a task
        /// which will be called at the next update tick.
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskContext& Schedule(const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                              const std::chrono::duration<_RepRight, _PeriodRight>& max, const TaskScheduler::task_handler_t& task)
        {
            return Dispatch([&](TaskScheduler& scheduler) -> TaskScheduler&
                            {
                                return scheduler.ScheduleAt(_task->_end, TaskScheduler::MakeDurationCalculator(min, max), task);
                            });
        }

        /// Schedule an event with a randomized rate between min and max rate from within the context.
        /// Its possible that the new event is executed immediately!
        /// Use TaskScheduler::Async to create a task
        /// which will be called at the next update tick.
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskContext& Schedule(const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                              const std::chrono::duration<_RepRight, _PeriodRight>& max, const TaskScheduler::group_t group,
                              const TaskScheduler::task_handler_t& task)
        {
            return Dispatch([&](TaskScheduler& scheduler) -> TaskScheduler&
                            {
                                return scheduler.ScheduleAt(_task->_end, TaskScheduler::MakeDurationCalculator(min, max), group, task);
                            });
        }

        /// Cancels all tasks from within the context.
        TaskContext& CancelAll();

        /// Cancel all tasks of a single group from within the context.
        TaskContext& CancelGroup(const TaskScheduler::group_t group);

        /// Cancels all groups in the given std::vector from within the context.
        /// Hint: Use std::initializer_list for this: "{1, 2, 3, 4}"
        TaskContext& CancelGroupsOf(const std::vector<TaskScheduler::group_t>& groups);

        /// Delays all tasks with the given duration from within the context.
        template<typename _Rep, typename _Period>
        TaskContext& DelayAll(const std::chrono::duration<_Rep, _Period>& duration)
        {
            return Dispatch(std::bind(&TaskScheduler::DelayAll<_Rep, _Period>, std::placeholders::_1, duration));
        }

        /// Delays all tasks with a random duration between min and max from within the context.
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskContext& DelayAll(const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                              const std::chrono::duration<_RepRight, _PeriodRight>& max)
        {
            return DelayAll(TaskScheduler::RandomDurationBetween(min, max));
        }

        /// Delays all tasks of a group with the given duration from within the context.
        template<typename _Rep, typename _Period>
        TaskContext& DelayGroup(const TaskScheduler::group_t group, const std::chrono::duration<_Rep, _Period>& duration)
        {
            return Dispatch(std::bind(&TaskScheduler::DelayGroup<_Rep, _Period>, std::placeholders::_1, group, duration));
        }

        /// Delays all tasks of a group with a random duration between min and max from within the context.
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskContext& DelayGroup(const TaskScheduler::group_t group,
                                const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                                const std::chrono::duration<_RepRight, _PeriodRight>& max)
        {
            return DelayGroup(group, TaskScheduler::RandomDurationBetween(min, max));
        }

        /// Reschedule all tasks with the given duration.
        template<typename _Rep, typename _Period>
        TaskContext& RescheduleAll(const std::chrono::duration<_Rep, _Period>& duration)
        {
            return Dispatch(std::bind(&TaskScheduler::RescheduleAt, std::placeholders::_1, _task->_end + duration));
        }

        /// Reschedule all tasks with a random duration between min and max.
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskContext& RescheduleAll(const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                                   const std::chrono::duration<_RepRight, _PeriodRight>& max)
        {
            return RescheduleAll(TaskScheduler::RandomDurationBetween(min, max));
        }

        /// Reschedule all tasks of a group with the given duration.
        template<typename _Rep, typename _Period>
        TaskContext& RescheduleGroup(const TaskScheduler::group_t group, const std::chrono::duration<_Rep, _Period>& duration)
        {
            return Dispatch(std::bind(&TaskScheduler::RescheduleAtWithPredicate, std::placeholders::_1, _task->_end + duration,
                                      [&](const TaskScheduler::TaskContainer& task)
                                      {
                                          return task->IsInGroup(group);
                                      }));
        }

        /// Reschedule all tasks of a group with a random duration between min and max.
        template<typename _RepLeft, typename _PeriodLeft, typename _RepRight, typename _PeriodRight>
        TaskContext& RescheduleGroup(const TaskScheduler::group_t group,
                                     const std::chrono::duration<_RepLeft, _PeriodLeft>& min,
                                     const std::chrono::duration<_RepRight, _PeriodRight>& max)
        {
            return RescheduleGroup(group, TaskScheduler::RandomDurationBetween(min, max));
        }

    private:
        /// Throws std::logic_error if the task was consumed already.
        void ThrowOnConsumed() const;

    public:

        /// Invokes the associated hook of the task.
        void Invoke();
    };

} // tsc

#endif //SHIRO_TASKSCHEDULER_HH
