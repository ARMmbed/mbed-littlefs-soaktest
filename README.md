## Littlefs Soak Test

The purpose of this application is to verify that littlefs properly handles flash exhaustion failure on real hardware. On first boot it will format the block device it is using with a littlefs filesystem. For there on it will perform non-stop flash operations. If reset the application will start where it left out without reformatting the filesystem. If any error occurs the program will exit with a message and start flashing the error pattern on its led.

### How to build

To build this application for spi flash connected to the default spi pins of an arduino form factor run the compile command:
```
> mbed compile -t <toolchain> -m <target>
```

If the pins you want to use differ from the default spi port then the pins can be set by defining ```MBED_TEST_BLOCKDEVICE_DECL``` on the command line. An example of this is building for the K82F which has an onboard spi flash chip:
```
> mbed compile -t gcc_arm -m k82f -D"MBED_TEST_BLOCKDEVICE_DECL=SPIFBlockDevice bd(PTE2, PTE4, PTE1, PTE5)"
```

### Expected output

After you have built and loaded the image you'll the if there was an existing filesystem on the spi flash or if it was reformatted when the device boots. After that you'll see status about the number of cycles that have run.
```
starting soak test
using existing filesystem
Status
  Cycles since boot:   5
  Cycles since format: 1082
  Filesystem valid:    yes
Status
  Cycles since boot:   10
  Cycles since format: 1087
  Filesystem valid:    yes
```


If the flash part becomes completely exhusted then and littlefs correctly handles this by returning out of space then the application will loop and print this every 5 seconds.
```
Gracefully failed:
  Cycles since boot - 123456
  Cycles since format - 12345678910
Gracefully failed:
  Cycles since boot - 123456
  Cycles since format - 12345678910
```

Finally, if an error occurs during the test then error information will be printed once and the error pattern with flash.