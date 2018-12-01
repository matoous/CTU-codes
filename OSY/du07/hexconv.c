// by Matous Dzivjak <dzivjak@matous.me>
#define SYSCALL_EXIT 1
#define SYSCALL_READ 3
#define SYSCALL_WRITE 4

#define STDOUT 1
#define STDIN 0

#define uint64 unsigned long long int

void sys_exit(int error_code){
    asm volatile("int $0x80" : : "a"(SYSCALL_EXIT), "b"(error_code));
}

int sys_write(char *buf, unsigned count){
    unsigned ret;
    asm volatile("int $0x80": "=a"(ret) : "a"(SYSCALL_WRITE), "b"(STDOUT), "c"(buf), "d"(count) : "memory");
    return ret;
}

int sys_read(char *buf, unsigned count){
    unsigned ret;
    asm volatile("int $0x80"
   : "=a" (ret)
   : "0" (SYSCALL_READ), "b" (STDIN), "c" (buf), "d" (sizeof(buf))
   : "memory", "cc");
    return ret;
}

void print(uint64 x){
    char tmp[26];
    int tmpcnt = 0;
    do{
      tmp[tmpcnt++] = "0123456789abcdef"[x%16];
    }while((x /= 16));
    int cnt = 0;
    char buffer[tmpcnt+2];
    buffer[cnt++] = '0';
    buffer[cnt++] = 'x';
    while(--tmpcnt >= 0)
      buffer[cnt++] = tmp[tmpcnt];
    buffer[cnt++] = '\n';
    buffer[cnt++] = '\0';

    sys_write(buffer, cnt);
}

int isnum(char ch){
  return ch >= '0' && ch <= '9';
}

int isspc(char ch){
  return ch == ' ' || ch == '\n';
}

int _start()
{
    char buf[20];
    uint64 num = 0;
    int i;
    int num_digits = 0;
    unsigned chars_in_buffer = 0;

    for ( ; ; i++, chars_in_buffer--) {
      if (chars_in_buffer == 0) {
        int ret = sys_read(buf, sizeof(buf));
        if (ret < 0)
          sys_exit(1);
        i = 0;
        chars_in_buffer = ret;
      }
      if (num_digits > 0 && (chars_in_buffer == 0 || !isnum(buf[i]))) {
        print(num);
        num_digits = 0;
        num = 0;
      }
      if (chars_in_buffer == 0 || (!isspc(buf[i]) && !isnum(buf[i])))
        sys_exit(0);

      if (isnum(buf[i])) {
        num = num * 10 + (unsigned long)(buf[i] - '0');
        num_digits++;
      }
    }

    sys_exit(0);
}
