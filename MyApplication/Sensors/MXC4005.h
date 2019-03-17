#ifndef MXC4005_H
#define MXC4005_H

#include "mbed.h"

#define MXC4005_REG_INT_SRC0    0x00    // Shake & change in orientation interrupt sources (read only)
#define MXC4005_REG_INT_CLR0    0x00    // Shake & change in orientation interrupt clear (write only)
#define MXC4005_REG_INT_SRC1    0x01    // Data ready interrupt sources, tilt & orientation status (read only)
#define MXC4005_REG_INT_CLR1    0x01    // Data ready, tilt & data ready interrupt clear (write only)
#define MXC4005_REG_STATUS      0x02    // Instantaneous orientation status (read only) 
#define MXC4005_REG_XOUT_UPPER  0x03    // X-axis acceleration output MSB
#define MXC4005_REG_XOUT_LOWER  0x04    // X-axis acceleration output LSB
#define MXC4005_REG_YOUT_UPPER  0x05    // Y-axis acceleration output MSB
#define MXC4005_REG_YOUT_LOWER  0x06    // Y-axis acceleration output LSB
#define MXC4005_REG_ZOUT_UPPER  0x07    // Z-axis acceleration output MSB
#define MXC4005_REG_ZOUT_LOWER  0x08    // Z-axis acceleration output LSB
#define MXC4005_REG_TOUT        0x09    // Temperature output
#define MXC4005_REG_INT_MASK0   0x0A    // Shake & orientation detection interrupt mask (write only)
#define MXC4005_REG_INT_MASK1   0x0B    // Data ready, tilt & data ready interrupt mask (write only)
#define MXC4005_REG_DETECTION	  0x0C    // Orientation and shake detection parameters (read/write)
#define MXC4005_REG_CONTROL		  0x0D    // Operating mode control and full-scale range (read/write)
#define MXC4005_REG_DEVICE_ID   0x0E    // Currently has a value of 0x02
#define MXC4005_REG_WHO_AM_I    0x0F    // Is a read-only register to identify the MXC400xXC

enum MXC4005_RangeSensitivity {
  MXC4005_RANGE_2G_SENS = 1024, // Range_2G: Sensitivity 1024 LSB/g
  MXC4005_RANGE_4G_SENS = 512,  // Range_4G: Sensitivity  512 LSB/g
  MXC4005_RANGE_8G_SENS = 256,  // Range_8G: Sensitivity  256 LSB/g
};

class MXC4005 {
 public:

  /** Create a MXC4005
   *
   *  @param i2c I2C object
   *  @param acc_int InterruptIn object
   *  @param i2c_addr I2C address
   *  @param hz I2C speed
   */
  MXC4005(I2C &i2c, InterruptIn &acc_int, int i2c_addr = (0x15 << 1));

  /**
   * Set the frequency of the I2C interface
   *
   * @param hz The bus frequency in hertz
   */
  void speed(int hz);

  /** Set sensitivity
   *
   * @param s MXC4005_RangeSensitivity
   */
  void set_sensitivity(int16_t s);

  /** Get device ID
   *
   * @returns ID(0x02)
   */
  uint8_t get_device_id();

  /** Get raw data
   *
   * @param xyz Output X, Y and Z raw data, respectively range [-2048, 2047]
   */
  void get_raw(int16_t *xyz);

  /** Get vector
   *
   * @param xyz Output X, Y and Z vector, unit: G
   */
  void get_vector(float *xyz);

  /** calibrate
   *
   * @param xyz Input X, Y and Z raw data
   */
  void calibrate(int16_t *xyz);

 private:
  I2C &_i2c;
  InterruptIn &_acc_int;

  uint8_t _i2c_addr;
  int16_t _sensitivity;
  int16_t _calib_xyz[3];

  void _reg_write(char reg, char val);
  void _reg_read(char reg, char *val, int len);
};

#endif