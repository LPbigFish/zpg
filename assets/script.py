
import bpy
import math
import re
from pathlib import Path

# ── Configuration ────────────────────────────────────────

MODEL_NAME = "Shrek"
OUTPUT_DIR = bpy.path.abspath("//")

VERTEX_LAYOUT = "PosNormalUv"

# Supported layouts:
#   PosNormalColor  - position, normal, color
#   PosColorNormal  - position, color, normal
#   PosColor        - position, color
#   PosNormal       - position, normal
#   PosNormalUv     - position, normal, uv

# ── Vertex layouts ───────────────────────────────────────

LAYOUTS = {
    "PosNormalColor": ("position", "normal", "color"),
    "PosColorNormal": ("position", "color", "normal"),
    "PosColor":       ("position", "color"),
    "PosNormal":      ("position", "normal"),
    "PosNormalUv":    ("position", "normal", "uv"),
}

SIZES = {
    "position": 3,
    "normal": 3,
    "color": 3,
    "uv": 2,
}


def cpp_float(value):
    """Convert a Python float to a valid C++ float literal."""
    value = float(value)

    if not math.isfinite(value):
        raise ValueError("Mesh contains a non-finite float")

    result = format(value, ".9g")

    if "." not in result and "e" not in result:
        result += ".0"

    return result + "f"


def cpp_identifier(name):
    """Convert object name to a valid C++ identifier."""
    name = re.sub(r"[^a-zA-Z0-9_]", "_", name).lower()

    if not name or name[0].isdigit():
        name = "_" + name

    return name


def export_model(model_name, output_dir, layout):
    if layout not in LAYOUTS:
        raise ValueError(f"Unsupported vertex layout: {layout}")

    obj = bpy.data.objects.get(model_name)

    if obj is None:
        raise ValueError(f"Object '{model_name}' not found")

    if obj.type != "MESH":
        raise TypeError(f"Object '{model_name}' is not a mesh")

    components = LAYOUTS[layout]
    stride = sum(SIZES[c] for c in components)

    depsgraph = bpy.context.evaluated_depsgraph_get()
    evaluated = obj.evaluated_get(depsgraph)

    mesh = evaluated.to_mesh(
        preserve_all_data_layers=True,
        depsgraph=depsgraph,
    )

    if mesh is None:
        raise RuntimeError("Failed to evaluate mesh")

    try:
        mesh.calc_loop_triangles()

        uv_layer = mesh.uv_layers.active
        color_layer = mesh.color_attributes.active_color

        if "uv" in components and uv_layer is None:
            raise ValueError("Mesh has no active UV map")

        if "color" in components:
            if color_layer is None:
                raise ValueError("Mesh has no active color attribute")
            if color_layer.domain not in {"CORNER", "POINT"}:
                raise ValueError("Unsupported color attribute domain")

        if "normal" in components:
            if not hasattr(mesh, "corner_normals"):
                mesh.calc_normals_split()

        lines = []
        vertex_count = 0

        for triangle in mesh.loop_triangles:
            for loop_index in triangle.loops:
                loop = mesh.loops[loop_index]
                vertex = mesh.vertices[loop.vertex_index]

                values = []

                for component in components:
                    if component == "position":
                        values.extend(vertex.co[:3])

                    elif component == "normal":
                        if hasattr(mesh, "corner_normals"):
                            normal = mesh.corner_normals[loop_index].vector
                        else:
                            normal = loop.normal

                        values.extend(normal[:3])

                    elif component == "uv":
                        uv = uv_layer.data[loop_index].uv
                        values.extend(uv[:2])

                    elif component == "color":
                        index = (
                            loop_index
                            if color_layer.domain == "CORNER"
                            else loop.vertex_index
                        )
                        color = color_layer.data[index].color
                        values.extend(color[:3])

                lines.append(
                    "    " + ", ".join(map(cpp_float, values)) + ","
                )
                vertex_count += 1

        if vertex_count == 0:
            raise ValueError("Mesh contains no triangles")

        identifier = cpp_identifier(model_name)
        float_count = vertex_count * stride

        content = (
            "#pragma once\n"
            "#include <array>\n"
            '#include "zpg/graphics/MeshData.hpp"\n\n'
            "namespace models {\n\n"
            f"inline constexpr std::array<float, {float_count}> "
            f"{identifier}_data{{\n"
            + "\n".join(lines)
            + "\n};\n\n"
            f"inline constexpr zpg::graphics::MeshData {identifier}{{\n"
            f"    {identifier}_data,\n"
            f"    zpg::graphics::VertexLayout::{layout}\n"
            "};\n\n"
            "} // namespace models\n"
        )

        output_path = Path(output_dir) / f"{model_name}.hpp"
        output_path.parent.mkdir(parents=True, exist_ok=True)
        output_path.write_text(content, encoding="utf-8")

        print(f"Exported: {output_path}")
        print(f"Layout: {layout}")
        print(f"Stride: {stride} floats")
        print(f"Vertices: {vertex_count}")
        print(f"Triangles: {len(mesh.loop_triangles)}")
        print(f"Total floats: {float_count}")

    finally:
        evaluated.to_mesh_clear()


export_model(MODEL_NAME, OUTPUT_DIR, VERTEX_LAYOUT)
