# CAD concept files

- `component-placement.svg` — clear top and side view of the proposed part placement, battery position, estimated CG, and dimensions.
- `component-placement.dxf` — 2D top layout in millimetres. SolidWorks can open/import this as a drawing or sketch; save it as a native SolidWorks file after opening if needed.
- `component-layout.scad` — simple parametric 3D arrangement of the chassis and component envelopes. Open with OpenSCAD, press F5 to preview, or F6 then export STL. The STL can be imported into SolidWorks as a mesh reference.
- `chassis-layout.svg` — earlier suspension concept sketch.

The DXF and SCAD model are layout references, not fabrication-ready models. Component blocks are simplified envelopes. Confirm actual dimensions, suspension motion, fastener positions, and electrical clearances before fabrication. SolidWorks is not installed in this workspace, so no native `.SLDPRT` or `.SLDASM` could be generated here.
