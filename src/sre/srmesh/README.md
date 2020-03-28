## SR_Mesh
Capable of loading simple wavefront(.obj) files. Currently Supports:
* Geometric data(v)(vt)(vn)
* Face indexing
* Quad faces

There will be no support for free-form curves and surfaces as wel as grouping.
Faces up to quadliterals are supported.

Indexed Vertex Data constructs a continous array of vertex data of the form (v, vt, vn)
with stride 8 and offsets 0, 3, 5.

### TODO
* Convert quad faces to triangle faces
* Support optional v, w texture coordinates
* Support optional w vertex coordinate
* Support points and lines
* Support texture and material data
