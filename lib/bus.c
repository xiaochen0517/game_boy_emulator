#include <bus.h>

u8 bus_read(u16 addr)
{
  if (addr < 0x8000)
  {
    return cart_read(addr);
  }

  NOT_IMPLEMENTED
}

void bus_write(u16 addr, u8 data)
{
  if (addr < 0x8000)
  {
    cart_write(addr, data);
    return;
  }

  NOT_IMPLEMENTED
}
