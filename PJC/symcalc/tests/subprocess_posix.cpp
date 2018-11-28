#include "subprocess.hpp"

#include <string>
#include <algorithm>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdexcept>
#include <system_error>

namespace subprocess {

void throw_stdc_error(std::string msg)
{
    std::error_code ec(errno, std::generic_category());
    throw std::system_error(ec, std::move(msg));
}

Process::Process(const char *cmd)
{
    int infd[2], outfd[2];
    pipe(infd);
    pipe(outfd);

    m_pid = fork();
    if (m_pid < 0) {
        throw_stdc_error("fork");
    } else if (m_pid == 0) { // child
        close(infd[1]);
        close(outfd[0]);
        close(0);
        close(1);
        dup2(infd[0], 0);
        dup2(outfd[1], 1);
        execlp("/bin/sh", "/bin/sh", "-c", cmd, nullptr);
        _exit(1);
    }
    close(infd[0]);
    close(outfd[1]);

    m_stdin.reset(fdopen(infd[1], "wt"));
    m_stdout.reset(fdopen(outfd[0], "rt"));
}

int Process::wait()
{
    int errcode = 1;
    int res = waitpid(m_pid, &errcode, 0);
    if (res == -1)
        throw_stdc_error("waitpid");
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
