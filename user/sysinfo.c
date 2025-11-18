#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

// Kiểm tra: gọi system call sysinfo có hoạt động không?
void testcall() {
  struct sysinfo info;
  printf("testcall: calling sysinfo()...\n");
  if(sysinfo(&info) < 0){
    printf("sysinfotest: sysinfo() failed\n");
    exit(1);
  }
  printf("testcall: sysinfo() succeeded\n");
  printf("  -> Current free memory: %ld bytes\n", info.freemem);
  printf("  -> Current processes: %ld\n", info.nproc);
  printf("testcall: OK\n\n");
}

// Kiểm tra bộ nhớ trống giảm khi cấp phát
void testmem() {
  struct sysinfo si_before, si_after;
  printf("testmem: checking memory allocation...\n");
  
  if(sysinfo(&si_before) < 0){
    printf("sysinfotest: sysinfo() failed\n");
    exit(1);
  }
  printf("  -> Free memory BEFORE malloc: %ld bytes\n", si_before.freemem);
  
  printf("  -> Allocating 10 pages (40960 bytes)...\n");
  void *a = malloc(4096 * 10);  // cấp phát 10 trang
  
  if(sysinfo(&si_after) < 0){
    printf("sysinfotest: sysinfo() failed\n");
    exit(1);
  }
  printf("  -> Free memory AFTER malloc: %ld bytes\n", si_after.freemem);
  
  uint64 diff = si_before.freemem - si_after.freemem;
  printf("  -> Memory decreased by: %ld bytes\n", diff);
  
  if(si_after.freemem >= si_before.freemem){
    printf("sysinfotest: FAILED - memory did not decrease!\n");
    exit(1);
  }
  
  free(a);
  printf("testmem: OK\n\n");
}

// Kiểm tra số tiến trình tăng khi tạo process
void testproc() {
  struct sysinfo si_before, si_after;
  printf("testproc: checking process count...\n");
  
  if(sysinfo(&si_before) < 0){
    printf("sysinfotest: sysinfo() failed\n");
    exit(1);
  }
  printf("  -> Processes BEFORE fork: %ld\n", si_before.nproc);
  
  printf("  -> Creating child process with fork()...\n");
  int pid = fork();
  if(pid < 0){
    printf("sysinfotest: fork() failed\n");
    exit(1);
  }
  
  if(pid == 0){
    // Child process
    sleep(10); 
    exit(0);
  }
  
  // Parent process
  if(sysinfo(&si_after) < 0){
    printf("sysinfotest: sysinfo() failed\n");
    exit(1);
  }
  printf("  -> Processes AFTER fork: %ld\n", si_after.nproc);
  printf("  -> Process count increased by: %ld\n", si_after.nproc - si_before.nproc);
  
  if(si_after.nproc <= si_before.nproc){
    printf("sysinfotest: FAILED - number of processes did not increase!\n");
    exit(1);
  }
  
  wait(0);
  printf("testproc: OK\n\n");
}

int main() {
  printf("sysinfotest: STARTING\n");

  
  testcall();   // test 1
  testmem();    // test 2
  testproc();   // test 3
  
  printf("sysinfotest: OK\n");
  exit(0);
}