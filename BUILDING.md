# Installing dependencies:

## Already prepared submodules
`.gitmodules` file already contains dependencies **(not all)** that you can install with:
`git submodule update --init --recursive`

You still need to install GraphicsMagick manually though.

## ImGui
- Download ImGui ([GitHub](https://github.com/ocornut/imgui/archive/refs/heads/master.zip))
- Unpack ImGui to `external`
- Rename directory to `ImGui`
- **Dependency installed 👍**

## FFmpeg
- Download FFmpeg ([GitHub](https://github.com/FFmpeg/FFmpeg/archive/refs/heads/master.zip))
- Unpack FFmpeg to `external`
- Rename directory to `FFmpeg`
- **Dependency installed 👍**

## GraphicsMagick
- Download GraphicsMagick ([SourceForge](https://sourceforge.net/projects/graphicsmagick/files/latest/download))
- Unpack GraphicsMagick to `external`
- Rename directory to `GraphicsMagick`
- Head to `GraphicsMagick/VisualMagick/configure`
- Run `configure.exe`
- Press Next, Next, Next, Done
- **Dependency installed 👍**

