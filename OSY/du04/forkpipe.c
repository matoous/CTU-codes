#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#ifdef DEBUG
#define MASK 0x00ff
#else
#define MASK 0xffff
#endif

#define DO_OR_DIE(x) if(x < 0){return 2;}

volatile int stop = 0;

void sig_handler(int signo)
{
  if (signo == SIGTERM)
    stop = 1;
}

int main(void)
{
    int P[2];
    int status = 0;
    int p1, p2;
    DO_OR_DIE(pipe(P))

    // GEN CHILD
    if ((p1 = fork()) == 0) {
        if (signal(SIGTERM, sig_handler) == SIG_ERR){
          perror("cant issue signal handler!\n");
          return 2;
        }
        DO_OR_DIE(close(P[0]))
        DO_OR_DIE(dup2(P[1], STDOUT_FILENO))
        DO_OR_DIE(close(P[1]))
        while (!stop) {
            DO_OR_DIE(printf("%d %d\n", rand() & MASK, rand() & MASK))
            fflush(stdout);
            DO_OR_DIE(sleep(1))
        }
        fprintf(stderr, "GEN TERMINATED\n");
        return 0;
    } else if (p1 < 0){
      return 2;
    }

    // NSD CHILD
    if (p1 && (p2 = fork()) == 0){
      DO_OR_DIE(close(P[1]))
      DO_OR_DIE(dup2(P[0], STDIN_FILENO))
      DO_OR_DIE(close(P[0]))
      DO_OR_DIE(execl("nsd", "nsd", NULL))
    } else if (p2 < 0){
      return 2;
    }

    // PARENT
    if ( p1 && p2 ) {
      DO_OR_DIE(close(P[0]))
      DO_OR_DIE(close(P[1]))
      DO_OR_DIE(sleep(5))
      int ex1 = 0, ex2 = 0;

      // terminate GEN
      DO_OR_DIE(kill(p1, SIGTERM))
      DO_OR_DIE(waitpid(p1, &status, 0))
      if(WIFEXITED(status))
        ex1 = WEXITSTATUS(status);

      // wait for NSD
      DO_OR_DIE(waitpid(p2, &status, 0))
      if (WIFEXITED(status))
        ex2 = WEXITSTATUS(status);

      if(ex1 || ex2){
        printf("ERROR\n");
        exit(1);
      } else {
        printf("OK\n");
        exit(0);
      }
    }

  // Shouldn't get there
  exit(1);
}
