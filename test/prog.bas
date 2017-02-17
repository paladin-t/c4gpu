import "@c4g"

' Config.
write_to_file = false ' Change this to true to enable logging to file.

' Uniform.
print "Prepare uniform data...";
un0 = vec4_array("u0")
un0.push(100, 100, 100, 100)
un = data_array()
un.push(un0)

' Input data.
print "Prepare input buffer...";
in0 = vec4_array("v0")
for i = 1 to 1000000
    in0.push(1, 2, 3, 4)
next
inp = data_array()
inp.push(in0)

' Output buffer.
print "Prepare output buffer...";
out0 = vec4_array("")
for i = 1 to len(in0)
    out0.push(0, 0, 0, 0)
next
out = data_array()
out.push(out0)

' Prepares.
print "Prepare context...";
rt = runtime()
p0 = add_pass(rt)
use_gpu_program_file(rt, p0, "prog.vert", "o0")

prepare_buffers(rt, p0, 0, len(inp), len(out))
prepare_uniform(rt, p0, un)
prepare_in(rt, p0, inp)
prepare_out(rt, p0, out)

' Computes
print "Begin compute...";
t = ticks()
compute(rt, p0)
t = ticks() - t
print t, "ms cost.";

' Finishes.
print "Finish...";
map_out(rt, p0)
finish(rt)

' Write to file?
if write_to_file then
    print "Write to file...";
    fp = file.file()
    file.open(fp, "c4grt_eval.log", "w")
    file.write_line(fp, "Output 0:")
    file.write(fp, out0)
    file.write_line(fp)
    file.write_line(fp, "Output 1:")
    file.write(fp, out1)
    file.close(fp)
endif
print "Done.";

input
