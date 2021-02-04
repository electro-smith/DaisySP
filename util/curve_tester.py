import math
import matplotlib.pyplot as plt

def triangle(size):
    shape = []
    for i in range(0, size):
        val = 1.0 - abs((-0.5 + (i/size)) * 2.0)
        shape.append(val)
    return shape

gval = 0.0
gcurve = 0.0

class tseg:
    gval = 0.0
    gcurve = 0.0
    curx = 0.0

    def transeg(self, inval, pos, size, curve):
        val = self.gval
        if pos == (size / 2) or pos == 0:
            # Reset stuff on segment change
            self.curx = 0.0
            self.gval = val

        if pos < (size / 2):
            beg = 0.0
            end = 1.0
        else:
            beg = 1.0
            end = 0.0

        if (curve == 0.0):
            c1 = ((end - beg) / (size/2))
        else:
            c1 = ((end - beg) / ((1.0 - math.exp(curve))))

        inc = c1
        print("Increment:\t" + str(c1))
        if (curve == 0):
            out = val
            val = val + inc
        else:
            out = val
            self.curx += (curve / (size/2))
            val = beg + inc * (1.0 - math.exp(self.curx))

        self.gval = val

        return out

t = tseg()
test_size = 512
out_shape = list(range(test_size))
x_scale = list(range(test_size))
test_curve = -15.0
linear_shape = triangle(test_size)
for i in range(0, test_size):
    out_shape[i] = t.transeg(linear_shape[i], i, test_size, test_curve)
    x_scale[i] = i

plt.plot(x_scale,linear_shape, label="lin")
plt.plot(x_scale,out_shape, label="transeg")
plt.title('transeg test')
plt.legend() 
plt.show()

