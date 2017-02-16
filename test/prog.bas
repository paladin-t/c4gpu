' Texture data.
tx0 = vec4b_array("s0")
tx0.push(128, 128, 128, 128)
tx0.push(128, 128, 128, 128)
tx0.push(128, 128, 128, 128)
tx0.push(128, 128, 128, 128)
tx = tex_array()
tx.push(tx0, 2, 2)

' Uniform.
un0 = vec4_array("u0")
un0.push(100, 100, 100, 100)
un = data_array()
un.push(un0)

' Input data.
in0 = vec4_array("v0")
for i = 0 to 1000000
    in0.push(1, 2, 3, 4)
next
in1 = vec4_array("v1")
for i = 0 to 1000000
    in1.push(1, 2, 3, 4)
next
inp = data_array()
inp.push(in0)
inp.push(in1)

' Output buffer.
out0 = vec4_array("")
out1 = vec4_array("")
for i = 0 to len(in0)
    out0.push(0, 0, 0, 0)
    out1.push(0, 0, 0, 0)
next
out = data_array()
out.push(out0)
out.push(out1)

' Prepares.
rt = c4grt()
p0 = add_pass(rt)
rt.use_gpu_program_file(p0, "prog.vert", "o0", "o1")

rt.prepare_buffers(p0, len(tx), len(inp), len(out))
rt.prepare_tex(p0, tx)
rt.prepare_uniform(p0, un)
rt.prepare_in(p0, inp)
rt.prepare_out(p0, out)

' Computes
t = ticks()
rt.compute(p0)
t = ticks() - t
print t, "ms cost.";

' Finishes.
rt.finish()
