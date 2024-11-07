// ======================================================================
// \title VxWorks/Os/Task.hpp
// \brief definitions of VxWorks implementation of Os::Task
// ======================================================================
#ifndef OS_VXWORKS_TASK_HPP
#define OS_VXWORKS_TASK_HPP

// VxWorks headers
#include <sysLib.h>
#include <taskLib.h>  // need it for VX_FP_TASK
#include <tickLib.h>
#include <vxWorks.h>

#include <atomic>

#include <Os/Task.hpp>

#include <FpConfig.hpp>
#include <Fw/Deprecate.hpp>
#include <Fw/Types/Serializable.hpp>
#include <Os/Mutex.hpp>
#include <Os/TaskString.hpp>

namespace Os {
namespace VxWorks {
namespace Task {
//! TaskHandle class definition for VxWorks implementations.
//!
struct VxWorksTaskHandle : public TaskHandle {
    static constexpr PlatformIntType SUCCESS = 0;

    //! VxWorks task descriptor
    TASK_ID m_task_descriptor = TASK_ID_NULL;
};

//! VxWorks task implementation as driven by pthreads implementation
class VxWorksTask : public TaskInterface {
  public:
    //! Enumeration of permission expectations
    enum PermissionExpectation {
        EXPECT_PERMISSION,    //!< Expect that you hold necessary permissions
        EXPECT_NO_PERMISSION  //!< Expect that you do not hold necessary permissions
    };

    //! \brief default constructor
    VxWorksTask() = default;

    //! \brief default virtual destructor
    ~VxWorksTask() = default;

    //! \brief copy constructor is forbidden
    VxWorksTask(const VxWorksTask& other) = delete;

    //! \brief assignment operator is forbidden
    VxWorksTask& operator=(const VxWorksTask& other) = delete;

    //! \brief perform required task start actions
    void onStart() override;

    //! \brief start the task
    //!
    //! Starts the task given the supplied arguments. This is done via the a task routine wrapper intermediary that
    //! ensures that `setStarted` is called once the task has actually started to run. The task then runs the user
    //! routine. This function may return before the new task begins to run.
    //
    //! It is illegal for arguments.m_routine to be null.
    //!
    //! \param arguments: arguments supplied to the task start call
    //! \return status of the task start
    Status start(const Arguments& arguments) override;

    //! \brief block until the task has ended
    //!
    //! Blocks the current (calling) task until this task execution has ended. Callers should ensure that any
    //! signals required to stop this task have already been emitted or will be emitted by another task.
    //!
    //! \return status of the block
    Status join() override;  //!< Wait for task to finish

    //! \brief suspend the task given the suspension type
    //!
    //! suspensionType argument is not used.
    //!
    //! \param suspensionType intentionality of the suspension
    void suspend(SuspensionType suspensionType) override;

    //! \brief resume a suspended task
    //!
    //! Resumes this task. Not started, running, and exited tasks take no action.
    //!
    void resume() override;

    //! \brief delay the current task
    //!
    //! Delays, or sleeps, the current task by the supplied time interval. In non-preempting os implementations
    //! the task will resume no earlier than expected but an exact wake-up time is not guaranteed.
    //!
    //! \param interval: delay time
    //! \return status of the delay
    Status _delay(Fw::TimeInterval interval) override;

    //! \brief return the underlying task handle (implementation specific)
    //! \return internal task handle representation
    TaskHandle* getHandle() override;
    PRIVATE :
        //! \brief create a configured pthread
        //!
        //! Creates, and configures, but does not start a pthread. This may be called twice, once to try setting
        //! permissions and once to fallback to no permissions.
        //!
        //! \param arguments: arguments used to set priority, affinity, etc
        //! \param permissions: whether to expect permissions or not
        //! \return OP_OK on success, or an error
        Status
        create(const Os::Task::Arguments& arguments, const VxWorksTask::PermissionExpectation permissions);

    VxWorksTaskHandle m_handle;                       //!< VxWorks task tracking
    static std::atomic<bool> s_permissions_reported;  //!< Permission errors have been reported
};
}  // end namespace Task
}  // end namespace VxWorks
}  // end namespace Os
#endif
