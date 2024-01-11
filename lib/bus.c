#include <bus.h>
#include <cart.h>

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

u16 bus_read16(u16 addr)
{
  return bus_read(addr) | (bus_read(addr + 1) << 8);
}

void bus_write16(u16 addr, u16 data)
{
  bus_write(addr + 1, (data >> 8) & 0xFF);
  bus_write(addr, data & 0xFF);
}
