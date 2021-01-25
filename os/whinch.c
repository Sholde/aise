//
#include <stdio.h>

// ioctl
#include <sys/ioctl.h>

// signal
#include <signal.h>

//
void term_size(void)
{
  struct winsize ws;
  ioctl(0, TIOCGWINSZ, &ws);
  printf("Size is %d row and %d col\n", ws.ws_row, ws.ws_col);
}

//
void signal_handler(int signal)
{
  term_size();
}

//
int main(void)
{
  //
  signal(SIGWINCH, signal_handler);

  //
  while (1)
    {

    }
  
  //
  return 0;
}
