#pragma once

#include <cstdio> // fclose
#include <memory>

namespace subprocess {

struct file_closer {void operator()(FILE *f){std::fclose(f);}};

class Process {
public:
    Process(const char *cmd);
    ~Process();

    FILE* pstdin() {return m_stdin.get();} // process' stdin
    FILE* pstdout() {return m_stdout.get();} // process' stdout
    void close_stdin() {m_stdin.reset(nullptr);}
    void close_stdout() {m_stdout.reset(nullptr);}
    int wait();
private:
    std::unique_ptr<FILE, file_closer> m_stdin;
    std::unique_ptr<FILE, file_closer> m_stdout;
    union {
        int m_pid; // pid_t
        void *m_handle; // HANDLE
    };
    bool m_waited = false;
};


} // namespace subprocess
