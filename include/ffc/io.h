/**
 * Created by Tqcq Ann on 11/15/22.
**/
#ifndef FCOMPUTER_INCLUDE_IO_H_
#define FCOMPUTER_INCLUDE_IO_H_

inline int read_mem(void* io_addr) __attribute__((optnone)) {
  return *(int*)io_addr;
}

inline void write_mem(void* io_addr, int data) __attribute__((optnone)) {
  *(int*)io_addr = data;
}

#endif //FCOMPUTER_INCLUDE_IO_H_
