// ======================================================================
// \title VxWorks/Os/Console.cpp
// \brief VxWorks implementation for Os::Console
// ======================================================================
#include "Console.hpp"
#include <Fw/Types/Assert.hpp>
#include <cstdio>
#include <limits>

namespace Os {
namespace VxWorks {
namespace Console {

void VxWorksConsole::writeMessage(const CHAR* message, const FwSizeType size) {
    // size_t is defined as different sizes on different platforms. Since FwSizeType is likely larger than size_t
    // on these platforms, and the user is unlikely to console-log more than size_t-max data, we cap the total
    // size at the limit of the interface.
    FwSizeType capped_size = (size < std::numeric_limits<size_t>::max())
                                 ? size
                                 : static_cast<FwSizeType>(std::numeric_limits<size_t>::max());
    if (message != nullptr) {
        (void)::fwrite(message, sizeof(CHAR), static_cast<size_t>(capped_size), this->m_handle.m_file_descriptor);
        (void)::fflush(this->m_handle.m_file_descriptor);
    }
}

ConsoleHandle* VxWorksConsole::getHandle() {
    return &this->m_handle;
}

void VxWorksConsole ::setOutputStream(Stream stream) {
    switch (stream) {
        case STANDARD_OUT:
            this->m_handle.m_file_descriptor = stdout;
            break;
        case STANDARD_ERROR:
            this->m_handle.m_file_descriptor = stderr;
            break;
        default:
            FW_ASSERT(0, static_cast<FwAssertArgType>(stream));
            break;
    }
}

}  // namespace Console
}  // namespace VxWorks
}  // namespace Os
