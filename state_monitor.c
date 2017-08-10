/*
 * state_monitor.c
 *
 * Script for monitoring the state of a block device.
 * Copyright (c) 2017 Fabian Baumanis <fbaumanis@suse.de>
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int);

int main(void){

  char buffer[16];
  int sel_ret, fd;
  fd_set rfds;

  fd = open("/sys/block/sdb/device/state", O_RDONLY);

  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  
  signal(SIGINT, signal_handler);

  while (1){
    sel_ret = select(fd+1, NULL, NULL, &rfds, NULL);
    printf("%d\n", sel_ret);
    if (sel_ret == 1){
      read(fd, buffer, 32);
      printf("Buffer value: %s\n", buffer);
      lseek(fd,0L,SEEK_SET);
    } else if (sel_ret == -1 ){
      printf("select syscall failed with %s\n", sel_ret);
    }
  }
return 0;
}

void signal_handler(int sig)
{
  printf("Interrupt was sent, exiting..\n");
  exit(1);
}
