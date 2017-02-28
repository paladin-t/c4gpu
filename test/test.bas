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
write_to_file = true ' Change this to true to enable logging to file.

' Texture data.
print "Prepare texture data...";
tx0 = vec4b_array("s0")
tx0.push(128, 128, 128, 128)
tx0.push(128, 128, 128, 128)
tx0.push(128, 128, 128, 128)
tx0.push(128, 128, 128, 128)
txa0 = tex_array()
txa0.push(tx0, 2, 2)

' Uniform.
print "Prepare uniform data...";
un0 = vec4_array("u0")
un0.push(100, 100, 100, 100)
una0 = data_array()
una0.push(un0)

' Input data.
print "Prepare input buffer...";
in0 = vec4_array("v0")
for i = 1 to 10000
    in0.push(1, 2, 3, 4)
next
in1 = vec4_array("v1")
for i = 1 to 10000
    in1.push(1, 2, 3, 4)
next
ina0 = data_array()
ina0.push(in0)
ina0.push(in1)

' Output buffer.
print "Prepare output buffer...";
out0 = vec4_array("o0")
out1 = vec4_array("o1")
out0.resize(len(in0))
out1.resize(len(in0))
outa0 = data_array()
outa0.push(out0)
outa0.push(out1)

out2 = vec4_array("o0")
out3 = vec4_array("o1")
out2.resize(len(in0))
out3.resize(len(in0))
outa1 = data_array()
outa1.push(out2)
outa1.push(out3)

' Prepares.
print "Prepare context...";
rt = runtime()
rt.set_error_handler(lambda (_pass, _msg) ( print _msg; ))
rt.show_driver_info()
p0 = rt.add_pass()
p1 = rt.add_pass(p0)
rt.set_pass_pipe(p0, true, "o0", "v0", "o1", "v1")
rt.use_gpu_program_file(p0, "test.vert", "o0", "o1")
rt.use_gpu_program_file(p1, "test.vert", "o0", "o1")

rt.prepare_buffers(p0, len(txa0), len(ina0), len(outa0))
rt.prepare_buffers(p1, len(txa0), 0, len(outa1))
rt.prepare_tex(p0, txa0)
rt.prepare_tex(p1, txa0)
rt.prepare_uniform(p0, una0)
rt.prepare_uniform(p1, una0)
rt.prepare_in(p0, ina0)
rt.prepare_out(p0, outa0)
rt.prepare_out(p1, outa1)

' Computes.
print "Begin compute...";
t = ticks()
rt.compute(p0)
t = ticks() - t
print t, "ms cost.";

' Finishes.
print "Finish...";
rt.map_out(p0)
rt.map_out(p1)
rt.finish()

' Write to file?
if write_to_file then
    print "Write to file...";
    fp = file()
    fp.open("c4grt_eval.log", "w")
    fp.write_line("Output 0:")
    fp.write(out2)
    fp.write_line()
    fp.write_line("Output 1:")
    fp.write(out3)
    fp.close()
endif
print "Done.";

input
