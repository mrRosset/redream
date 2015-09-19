#ifndef TRACE_VIEWER_H
#define TRACE_VIEWER_H

#include <unordered_map>
#include <vector>
#include "hw/holly/tile_renderer.h"
#include "sys/system.h"
#include "trace/trace.h"

namespace dreavm {
namespace trace {

struct TextureInst {
  hw::holly::TSP tsp;
  hw::holly::TCW tcw;
  const uint8_t *palette;
  const uint8_t *texture;
  renderer::TextureHandle handle;
};

class TraceTextureCache : public hw::holly::TextureProvider {
 public:
  void AddTexture(const hw::holly::TSP &tsp, hw::holly::TCW &tcw,
                  const uint8_t *palette, const uint8_t *texture);
  void RemoveTexture(const hw::holly::TSP &tsp, hw::holly::TCW &tcw);

  renderer::TextureHandle GetTexture(
      const hw::holly::TSP &tsp, const hw::holly::TCW &tcw,
      hw::holly::RegisterTextureCallback register_cb);

 private:
  std::unordered_map<hw::holly::TextureKey, TextureInst> textures_;
};

class TraceViewer {
 public:
  TraceViewer();
  ~TraceViewer();

  void Run(const char *path);

 private:
  bool Init();
  bool Parse(const char *path);

  void PumpEvents();
  void RenderFrame();

  int GetNumFrames();
  void CopyCommandToContext(const TraceCommand *cmd,
                            hw::holly::TileContext *ctx);
  void PrevContext();
  void NextContext();

  sys::System sys_;
  TraceTextureCache texcache_;
  hw::holly::TileRenderer tile_renderer_;
  renderer::Backend *rb_;

  TraceReader reader_;
  TraceCommand *current_cmd_;
  int num_frames_;
  int current_frame_;
  hw::holly::TileContext current_ctx_;
};
}
}

#endif
