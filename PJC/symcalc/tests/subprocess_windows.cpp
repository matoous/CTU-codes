#include "subprocess.hpp"

#include <string>
#include <stdexcept>
#include <system_error>

#include <windows.h>
#include <io.h>
#include <fcntl.h>

namespace subprocess {

template <typename F>
class scope_guard {
public:
    scope_guard(F func)
        : f(std::move(func))
    {
    }

    scope_guard(const scope_guard&) = delete;
    scope_guard& operator=(const scope_guard&) = delete;

    scope_guard(scope_guard &&o) : f(std::move(o.f)) {
        o.dismiss();
    }
    scope_guard& operator=(scope_guard &&o) {
        f = std::move(o.f);
        o.dismiss();
    }

    ~scope_guard() {
        if (enabled) {
            f();
        }
    }
    void dismiss() {
        enabled = false;
    }
private:
    bool enabled = true;
    F f;
};

template <typename F>
auto make_scope_guard(F &&f) -> scope_guard<F> {
    return {std::forward<F>(f)};
}


void throw_windows_error(std::string msg)
{
    DWORD dwErrVal = GetLastError();
    std::error_code ec(dwErrVal, std::system_category());
    throw std::system_error(ec, std::move(msg));
}

void throw_stdc_error(std::string msg)
{
    std::error_code ec(errno, std::generic_category());
    throw std::system_error(ec, std::move(msg));
}


// based on https://stackoverflow.com/a/7369662
std::unique_ptr<FILE, file_closer>
handle_to_file(HANDLE hnd, int mode, const char *mode2)
{
    int fd = _open_osfhandle((intptr_t) hnd, mode);
    if (fd == -1) {
        CloseHandle(hnd);
        throw_windows_error("_open_osfhandle");
    }

    FILE *f = _fdopen(fd, mode2);
    if (f == nullptr) {
        _close(fd);
        throw_stdc_error("_fdopen");
    }
    return std::unique_ptr<FILE, file_closer>(f);
}

Process::Process(const char *cmd)
{
    SECURITY_ATTRIBUTES saAttr;

    // Set the bInheritHandle flag so pipe handles are inherited.
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE infd[2], outfd[2];

    // Create a pipe for the child process's STDOUT.
    if (!CreatePipe(&outfd[0], &outfd[1], &saAttr, 0))
        throw_windows_error("StdoutRd CreatePipe");
    auto sg1 = make_scope_guard([&outfd](){CloseHandle(outfd[0]); CloseHandle(outfd[1]);});

    // Ensure the read handle to the pipe for STDOUT is not inherited.
    if (!SetHandleInformation(outfd[0], HANDLE_FLAG_INHERIT, 0))
        throw_windows_error("Stdout SetHandleInformation");

    // Create a pipe for the child process's STDIN.
    if (!CreatePipe(&infd[0], &infd[1], &saAttr, 0))
        throw_windows_error("Stdin CreatePipe");
    auto sg2 = make_scope_guard([&outfd](){CloseHandle(outfd[0]); CloseHandle(outfd[1]);});

    // Ensure the write handle to the pipe for STDIN is not inherited.
    if (!SetHandleInformation(infd[1], HANDLE_FLAG_INHERIT, 0))
        throw_windows_error("Stdin SetHandleInformation");

    sg1.dismiss();
    m_stdout = handle_to_file(outfd[0], _O_RDONLY, "rt");
    sg2.dismiss();
    m_stdin = handle_to_file(infd[1], _O_WRONLY, "wt");

    // Create the child process.
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

// Set up members of the PROCESS_INFORMATION structure.

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

// Set up members of the STARTUPINFO structure.
// This structure specifies the STDIN and STDOUT handles for redirection.

    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    siStartInfo.hStdOutput = outfd[1];
    siStartInfo.hStdInput = infd[0];
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

// Create the child process.

    bSuccess = CreateProcess(NULL,
                             const_cast<LPSTR>(cmd),           // command line
                             NULL,          // process security attributes
                             NULL,          // primary thread security attributes
                             TRUE,          // handles are inherited
                             0,             // creation flags
                             NULL,          // use parent's environment
                             NULL,          // use parent's current directory
                             &siStartInfo,  // STARTUPINFO pointer
                             &piProcInfo);  // receives PROCESS_INFORMATION
    if (!bSuccess)
        throw_windows_error(std::string("CreateProcess: ") + cmd);
    CloseHandle(piProcInfo.hThread);

    m_handle = piProcInfo.hProcess;
}

int Process::wait()
{
    DWORD res;

    res = WaitForSingleObject(m_handle, INFINITE);
    if (res == -1)
        throw_windows_error("WaitForSingleObject");
    else if (res != 1)
        throw std::runtime_error("WaitForSingleObject failed: "+res);

    DWORD errcode;
    BOOL ok = GetExitCodeProcess(m_handle, &errcode);
    if (ok == FALSE)
        throw_windows_error("GetExitCodeProcess");

    m_waited = true;
    return errcode;
}

Process::~Process()
{
    close_stdin();
    close_stdout();
    if (!m_waited)
        wait();
}

} // namespace subprocess
