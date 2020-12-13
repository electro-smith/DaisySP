Import("env")

# General options that are passed to the C compiler (C only; not C++).
env.Append(
    CFLAGS = [
        "-std=gnu11"
    ]
)
# General options that are passed to the C and C++ compilers
env.Append(
    CCFLAGS=[
        "-finline-functions"
    ]
)
# General options that are passed to the C++ compiler
env.Append(
    CXXFLAGS=[
        "-std=gnu++14",
        "-fno-exceptions",  
        "-fno-rtti"
    ]
)
# print("printing env:")
# print(env)
# print(env.Dump())

print("running supplimental link script")    

