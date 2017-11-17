/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "mbed.h"
#include "atomic_usage.h"
#include "LittleFileSystem.h"


#ifndef MBED_TEST_BLOCKDEVICE
#define MBED_TEST_BLOCKDEVICE SPIFBlockDevice
#endif

#ifndef MBED_TEST_BLOCKDEVICE_DECL
#define MBED_TEST_BLOCKDEVICE_DECL MBED_TEST_BLOCKDEVICE bd(D11,D12,D13,D10)
#endif

// declarations
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define INCLUDE(x) STRINGIZE(x.h)

#include INCLUDE(MBED_TEST_BLOCKDEVICE)

MBED_TEST_BLOCKDEVICE_DECL;


int main()
{
    printf("starting soak test\n");
    bool format = setup_atomic_operations(&bd, false);
    if (format) {
        printf("block device reformatted\n");
    } else {
        printf("using existing filesystem\n");
    }

    int64_t cycles = 0;
    int64_t total_cycles = 0;
    while (true) {
        int64_t ret = perform_atomic_operations(&bd);
        if (ret < 0) {
            // out of space
            break;
        }
        total_cycles = ret;
        cycles++;

        if ((cycles % 5) == 0) {
            check_atomic_operations(&bd);
            printf("Status\n"
                   "  Cycles since boot:   %lli\n"
                   "  Cycles since format: %lli\n"
                   "  Filesystem valid:    yes\n",
                   cycles, total_cycles);
        }
    }

    while (true) {
        printf("Gracefully failed:\n"
                "  Cycles since boot - %lli\n"
                "  Cycles since format - %lli\n",
                cycles, total_cycles);
        wait(5);
    }
}
