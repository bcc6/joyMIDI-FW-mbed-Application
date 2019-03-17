#include "MXC4005.h"

MXC4005::MXC4005(I2C &i2c, InterruptIn &acc_int, int i2c_addr)
    : _i2c(i2c),
      _acc_int(acc_int),
      _i2c_addr(i2c_addr),
      _sensitivity(MXC4005_RANGE_2G_SENS),
      _calib_xyz() {
  /* Software reset */
  _reg_write(MXC4005_REG_INT_CLR1, (1 << 4));
  /* All interrupts disabled */
  _reg_write(MXC4005_REG_INT_MASK0, 0x00);
  _reg_write(MXC4005_REG_INT_MASK1, 0x00);
  /* Set Power-down disabled and default range (+/-)2G */
  _reg_write(MXC4005_REG_CONTROL, 0x00);
}

void MXC4005::speed(int hz) { _i2c.frequency(hz); }

void MXC4005::set_sensitivity(int16_t s) {
  _sensitivity = s;
  if (s == MXC4005_RANGE_2G_SENS) _reg_write(MXC4005_REG_CONTROL, 0x00);
  if (s == MXC4005_RANGE_4G_SENS) _reg_write(MXC4005_REG_CONTROL, 0x20);
  if (s == MXC4005_RANGE_8G_SENS) _reg_write(MXC4005_REG_CONTROL, 0x40);
  wait(0.1);
}

uint8_t MXC4005::get_device_id() {
  char id;
  _reg_read(MXC4005_REG_DEVICE_ID, &id, 1);
  return id;
}

void MXC4005::get_raw(int16_t *xyz) {
  char val[6];
  _reg_read(MXC4005_REG_XOUT_UPPER, val, 6);

  /* Convert ADC sign-12bits to sign-16bits */
  int16_t x = ((int16_t)val[0] << 4) | (val[1] >> 4);
  if (x & 0x800) x |= 0xF000;  // Padding sign bits if negative
  int16_t y = ((int16_t)val[2] << 4) | (val[3] >> 4);
  if (y & 0x800) y |= 0xF000;  // Padding sign bits if negative
  int16_t z = ((int16_t)val[4] << 4) | (val[5] >> 4);
  if (z & 0x800) z |= 0xF000;  // Padding sign bits if negative

  xyz[0] = x;
  xyz[1] = y;
  xyz[2] = z;
}

void MXC4005::get_vector(float *xyz) {
  int16_t raw_xyz[3];
  get_raw(raw_xyz);

  /* Calibration */
  raw_xyz[0] -= _calib_xyz[0];
  raw_xyz[1] -= _calib_xyz[1];
  raw_xyz[2] -= _calib_xyz[2];
  
  /* Convert raw to vector */
  xyz[0] = (float)raw_xyz[0] / (float)_sensitivity;
  xyz[1] = (float)raw_xyz[1] / (float)_sensitivity;
  xyz[2] = (float)raw_xyz[2] / (float)_sensitivity;
}

void MXC4005::calibrate(int16_t *xyz) {
  _calib_xyz[0] = xyz[0];
  _calib_xyz[1] = xyz[1];
  _calib_xyz[2] = xyz[2] + _sensitivity;
}

void MXC4005::_reg_write(char reg, char val) {
  char data[2];
  data[0] = reg;
  data[1] = val;
  _i2c.write(_i2c_addr, data, 2, 0);
}

void MXC4005::_reg_read(char reg, char *val, int len) {
  _i2c.write(_i2c_addr, &reg, 1, 0);
  _i2c.read(_i2c_addr, val, len);
}