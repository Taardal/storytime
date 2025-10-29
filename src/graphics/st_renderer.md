# Renderer

## Overview

The `Renderer` class is a 2D batch renderer built on Vulkan. It implements an instanced rendering approach with automatic batching to render large numbers of textured quads while minimizing draw calls and state changes.

### Key Features

- **Batch Rendering**: Automatically groups quads into batches to minimize draw calls.
- **Instanced Rendering**: Uses GPU instancing to render multiple quads in a single draw call.
- **Multi-Frame Rendering**: Supports multiple frames in flight for optimal GPU utilization.
- **Texture Management**: Handles up to 16 textures per batch.
- **High Throughput**: Handles up to 20,000 quads per batch across 8 batches per frame.

## Architecture

### Class Structure

```
Renderer
├── Vulkan Resources
│   ├── Swapchain
│   ├── Command Pools
│   ├── Descriptor Pools & Layouts
│   ├── Graphics Pipeline
│   └── Buffers & Samplers
└── Frame Management
    └── Frames (multiple)
        └── Batches (multiple per frame)
```

## Core Concepts

### 1. Batch Rendering System

The renderer groups quads into batches to minimize draw calls. Each batch can contain:

| Resource               | Limit  | Description                                                                        |
|------------------------|--------|------------------------------------------------------------------------------------|
| **Quads per Batch**    | 20,000 | Maximum number of quads in a single batch                                          |
| **Textures per Batch** | 16     | Maximum number of unique textures in a single batch                                |
| **Batches per Frame**  | 8      | Maximum number of batches in a single frame                                        |
| **Vertices per Quad**  | 4      | The number of vertices that make up a single quad                                  |
| **Indices per Quad**   | 6      | The number of indices to make up a single quad using two triangles (0,1,2 + 2,3,0) |

### 2. Instanced Rendering

The renderer uses two vertex buffers:

- **Base Vertex Buffer**: Contains the base quad geometry (4 vertices)
- **Instance Buffer**: Contains per-instance data (transform, color, texture info)

```
┌──────────────────────────────────────┐
│          Base Quad Vertices          │
│    (Shared by all quad instances)    │
│                                      │
│       TL (0,0)────────TR (1,0)       │
│          │               │           │
│          │               │           │
│       BL (0,1)────────BR (1,1)       │
└──────────────────────────────────────┘

┌──────────────────────────────────────┐
│        Instance Data Per Quad        │
│   ┌──────────────────────────────┐   │
│   │ Model Matrix (4x4)           │   │
│   │ Color (vec4)                 │   │
│   │ Texture Rectangle (vec4)     │   │
│   │ Texture Index (float)        │   │
│   └──────────────────────────────┘   │
└──────────────────────────────────────┘
```

### 3. Frame-in-Flight System

The renderer maintains multiple frames in flight to prevent CPU-GPU stalls:

```
Frame 0: CPU preparing ──┐
Frame 1: GPU rendering ──┼── Concurrent execution
Frame 2: Presenting ─────┘
```

Each frame contains:
- Command buffer for recording draw commands
- Descriptor set for view/projection matrices
- Uniform buffer for camera data
- Synchronization primitives (fences, semaphores)
- Multiple batches for organizing draw calls

## Rendering Pipeline

### Shader Pipeline

The renderer uses a two-stage shader pipeline:

#### Vertex Shader (quad.vert)

**Inputs:**
- **Set 0, Binding 0**: View-Projection uniform buffer
- **Binding 0**: Base vertex buffer (position, texture coordinates)
- **Binding 1**: Instance vertex buffer (model matrix, color, texture data)

**Outputs:**
- Transformed vertex position (gl_Position)
- Per-fragment data (color, texture coordinates, texture index)

**Processing:**
```glsl
void main() {
    // Transform vertex to clip space
    gl_Position = projection * view * model * position;
}
```

#### Fragment Shader (quad.frag)

**Inputs:**
- **Set 1, Binding 0**: Array of 16 texture samplers
- Per-vertex data from vertex shader

**Output:**
- Final fragment color (RGBA)

**Processing:**
```glsl
void main() {
    // Sample texture and multiply by vertex color
    fragment_color = texture(samplers[texture_index], uv) * color;
}
```

### Descriptor Set Layout

The renderer uses a two-set descriptor layout:

| Set               | Binding | Type                   | Count | Stage    | Purpose                  |
|-------------------|---------|------------------------|-------|----------|--------------------------|
| **0** (Per-Frame) | 0       | Uniform Buffer         | 1     | Vertex   | View/Projection matrices |
| **1** (Per-Batch) | 0       | Combined Image Sampler | 16    | Fragment | Texture array            |

## Detailed Workflow

### Frame Lifecycle

```
┌─────────────────────────────────────────────────────────┐
│                     begin_frame()                       │
├─────────────────────────────────────────────────────────┤
│  1. Wait for GPU to finish previous frame               │
│  2. Acquire next swapchain image                        │
│  3. Reset command buffer                                │
│  4. Begin command buffer recording                      │
│  5. Begin render pass                                   │
│  6. Bind graphics pipeline                              │
│  7. Bind per-frame descriptor set (camera)              │
└─────────────────────────────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────┐
│              set_view_projection(matrices)              │
├─────────────────────────────────────────────────────────┤
│  • Update uniform buffer with camera matrices           │
└─────────────────────────────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────┐
│               render_quad(quad) [Loop]                  │
├─────────────────────────────────────────────────────────┤
│  1. Add unique textures to current batch                │
│  2. Add quad instance data to current batch             │
│  3. If batch full: flush()                              │
│     ├─ Update instance vertex buffer with quads         │
│     ├─ Bind vertex buffers (base + instance)            │
│     ├─ Bind index buffer                                │
│     ├─ Write batch descriptor set (textures)            │
│     ├─ Bind batch descriptor set                        │
│     ├─ Draw indexed instanced                           │
│     └─ Move to next batch                               │
└─────────────────────────────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────┐
│                     end_frame()                         │
├─────────────────────────────────────────────────────────┤
│  1. Flush remaining batch (if not empty)                │
│  2. End render pass                                     │
│  3. End command buffer recording                        │
│  4. Submit command buffer to graphics queue             │
│  5. Present swapchain image                             │
│  6. Reset frame state (batch counters, textures)        │
│  7. Advance to next frame                               │
└─────────────────────────────────────────────────────────┘
```

### Batch Flushing Logic

A batch is flushed when:
1. **Quad limit reached**: Batch contains 20,000 quads
2. **Texture limit reached**: Batch references 16 unique textures
3. **Frame ends**: Any remaining quads are flushed at `end_frame()`

```
Current Batch State:
┌──────────────────────────┐
│ Quads: 15,230 / 20,000   │ ← Not full
│ Textures: 16 / 16        │ ← Full!
└──────────────────────────┘
         │
         ▼ FLUSH (texture limit hit)
         │
┌──────────────────────────┐
│ Quads: 0 / 20,000        │
│ Textures: 0 / 16         │
└──────────────────────────┘
```

## Resource Management

### Initialization Order

The renderer initializes resources in a specific order to satisfy dependencies:

1. **Swapchain**: Creates render targets and render pass
2. **Command Pools**: Allocates initialization and per-frame command buffers
3. **Descriptor Pools**: Allocates per-frame and per-batch descriptor sets
4. **Descriptor Set Layouts**: Defines descriptor structure used in shaders
5. **Graphics Pipeline**: Defines the pipeline to render quads
6. **Base Buffers**: Vertex and index buffers for the base quad
7. **Texture Sampler**: Defines how an image is read, including filtering, address modes, and mipmapping
8. **Placeholder Texture**: 1x1 white texture used for quads without textures
9. **Frames**: Per-frame resources (command buffers, sync objects, batches)

### Destruction Order

Resources are destroyed in reverse initialization order (RAII pattern).

## Advanced Features

### Texture Rectangle Mapping

The `texture_rectangle` parameter allows rendering sub-regions of textures, useful for:
- **Sprite sheets**: Define frame boundaries
- **Texture atlases**: Select specific atlas regions
- **UI elements**: Render button states or icons

```cpp
// Render top-left quarter of texture
quad.texture_rectangle = glm::vec4(0.0f, 0.0f, 0.5f, 0.5f);
```

The vertex shader interpolates texture coordinates:
```glsl
void main() {
    // Maps base UV (0,0 to 1,1) into rectangle bounds
    texture_coordinate = mix(rectangle.xy, rectangle.zw, base_uv);
}
```

### Color Blending

Quads support color tinting by multiplying the texture color:

```cpp
quad.color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f); // Red tint
```

Fragment shader applies:
```glsl
void main() {
    fragment_color = texture_sample * color;
}
```

### Placeholder Texture

A 1×1 white texture serves as default when no texture is provided:
- Allows rendering colored quads without texture data
- Ensures valid texture indices in shaders
- Initialized during renderer construction

## Error Handling

The renderer performs validation at key points:

1. **Configuration validation**: Ensures all required dependencies are provided
2. **Frame bounds checking**: Validates frame indices before access
3. **Batch limits**: Ignores new quads if batch limits exceeded
4. **Texture limits**: Automatically flushes when texture limit reached
5. **Vulkan result codes**: Checks all Vulkan calls and throws exception on failure

## Design Patterns

### Flyweight Pattern

Base quad geometry is shared across all instances, reducing memory:
- 4 vertices shared by all quads
- Each instance only stores transformation and appearance data

## Example Integration

```cpp
class Game {
    Renderer renderer;
    Camera camera;
    std::vector<Sprite> sprites;
    
public:
    void render() {
        if (!renderer.begin_frame()) {
            return; // Skip frame (e.g., window minimized)
        }
        
        // Update camera
        ViewProjection vp{};
        vp.view = camera.get_view();
        vp.projection = camera.get_projection();
        renderer.set_view_projection(vp);
        
        // Render sprites (automatically batched)
        for (auto& sprite : sprites) {
            Quad quad{};
            quad.model = sprite.get_transform();
            quad.color = sprite.get_color();
            quad.texture = sprite.get_texture();
            quad.texture_rectangle = sprite.get_uv_rect();
            
            renderer.render_quad(quad);
        }
        
        renderer.end_frame();
    }
};
```
