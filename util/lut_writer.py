# 4KB sine wave lut
import math
size = 1024
with open('dsp_lut.h', 'w') as f:
    f.write('#include <stdint.h>\n')
    f.write('#define DSY_LUT_SIN_SIZE ' + str(size) + '\n')
    f.write('namespace daisysp {\n')
    f.write('extern const float dsy_lut_sin[];\n')
    f.write('} // namespacde daisysp\n')

with open('dsp_lut.cpp', 'w') as f:
    f.write('#include "dsp_lut.h"\n')
    f.write('namespace daisysp {\n')
    f.write('const float dsy_lut_sin[DSY_LUT_SIN_SIZE] = {\n')
    for i in range(0, size):
        phs = (i / size) * (2.0 * math.pi)
        val = math.sin(phs)
        f.write(str(val) + 'f, ')
        if i % 8 == 0:
            f.write('\n')
    f.write('};\n')
    f.write('} // namespacde daisysp\n')
