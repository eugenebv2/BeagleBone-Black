from periphery import I2C

# Open i2c-0 controller
i2c = I2C("/dev/i2c-2")

# Read byte at address 0x0000 of EEPROM at 0x50
msgs = [I2C.Message([0x00, 0x00]), I2C.Message([0x00], read=True)]
i2c.transfer(0x50, msgs)
print("0x0000: 0x{:02x}".format(msgs[1].data[0]))

i2c.close()
