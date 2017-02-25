REM C4GPU.
REM
REM For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
REM
REM Copyright (C) 2017 Wang Renxin. All rights reserved.
REM
REM This is a MY-BASIC powered script.

import "@c4g"
import "@file"

' Config.
write_to_file = false ' Change this to true to enable logging to file.

' Uniform.
print "Prepare uniform data...";
un0 = vec4_array("u0")
un0.push(100, 100, 100, 100)
una0 = data_array()
una0.push(un0)

' Input data.
print "Prepare input buffer...";
in0 = vec4_array("v0")
for i = 1 to 1000000
    in0.push(1, 2, 3, 4)
next
ina0 = data_array()
ina0.push(in0)

' Output buffer.
print "Prepare output buffer...";
out0 = vec4_array("o0")
out0.resize(len(in0))
outa0 = data_array()
outa0.push(out0)

' Prepares.
print "Prepare context...";
rt = runtime()
rt.show_driver_info()
p0 = rt.add_pass()
rt.use_gpu_program_file(p0, "prog.vert", "o0")

rt.prepare_buffers(p0, 0, len(ina0), len(outa0))
rt.prepare_uniform(p0, una0)
rt.prepare_in(p0, ina0)
rt.prepare_out(p0, outa0)

' Computes.
print "Begin compute...";
t = ticks()
rt.compute(p0)
t = ticks() - t
print t, "ms cost.";

' Finishes.
print "Finish...";
rt.map_out(p0)
rt.finish()

' Write to file?
if write_to_file then
    print "Write to file...";
    fp = file()
    fp.open("c4grt_eval.log", "w")
    fp.write_line("Output 0:")
    fp.write(out0)
    fp.close()
endif
print "Done.";

input
