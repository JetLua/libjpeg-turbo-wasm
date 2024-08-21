#include <stdio.h>
// #include <stdlib.h>
// #include <stdarg.h>
#include <emscripten.h>
#include "turbojpeg.h"

// char* format_string(const char* format, ...) {
//     // 预分配一个初始缓冲区
//     size_t size = 256;
//     char* buffer = malloc(size);
//     if (!buffer) return NULL;

//     va_list args;
//     va_start(args, format);

//     int length = vsnprintf(buffer, size, format, args);
//     va_end(args);

//     // 如果缓冲区不够大，重新分配内存
//     if (length >= size) {
//         size = length + 1;
//         buffer = realloc(buffer, size);
//         if (!buffer) return NULL;

//         va_start(args, format);
//         vsnprintf(buffer, size, format, args);
//         va_end(args);
//     }

//     return buffer;
// }
// EMSCRIPTEN_KEEPALIVE unsigned long _tidy (
//   uint8_t *raw,
//   int w, int h,
//   uint8_t **out,
//   unsigned long *size
// ) {
//   struct jpeg_compress_struct cinfo;
//   struct jpeg_error_mgr jerr;

//   cinfo.err = jpeg_std_error(&jerr);
//   jpeg_create_compress(&cinfo);
//   jpeg_mem_dest(&cinfo, out, size);

//   cinfo.image_width = w;
//   cinfo.image_height = h;
//   cinfo.input_components = 3;
//   cinfo.in_color_space = JCS_RGB;
//   cinfo.optimize_coding = false;
//   cinfo.arith_code = true;
//   cinfo.smoothing_factor = 100;
//   cinfo.raw_data_in = false;
//   cinfo.progressive_mode = true;
//   cinfo.write_Adobe_marker = false;
//   cinfo.write_JFIF_header = false;

//   jpeg_set_defaults(&cinfo);
//   jpeg_set_colorspace(&cinfo, JCS_RGB);
//   jpeg_set_quality(&cinfo, 70, true);


//   jpeg_start_compress(&cinfo, true);


//   int row_stride = w * 3;
//   JSAMPROW row_pointer[1];

//   while (cinfo.next_scanline < cinfo.image_height) {
//     row_pointer[0] = &raw[cinfo.next_scanline * row_stride];
//     jpeg_write_scanlines(&cinfo, row_pointer, 1);
//   }

//   jpeg_finish_compress(&cinfo);
//   jpeg_destroy_compress(&cinfo);

//   return *size;
// }
// EM_JS(void, log_message, (const char* message), {
//   console.log(UTF8ToString(message));
// });

EMSCRIPTEN_KEEPALIVE void tidy(
  uint8_t *raw,
  unsigned long size,
  int quanlity,
  uint8_t **_out,
  unsigned long *_ps
) {
  // 解压
  tjhandle handle = tjInitDecompress();

  int width, height, sub, cs;

  tjDecompressHeader3(handle, raw, size, &width, &height, &sub, &cs);

  int pixelSize = tjPixelSize[TJPF_RGB];
  uint8_t *dstBuf = malloc(width * height * pixelSize);

  tjDecompress2(handle, raw, size, dstBuf, width, 0, height, TJPF_RGB, 0);

  tjDestroy(handle);

  handle = tjInitCompress();

  uint8_t *out = NULL;
  unsigned long ps = 0;

  tjCompress2(
    handle,
    dstBuf,
    width,
    0,
    height,
    TJPF_RGB,
    &out, &ps,
    TJSAMP_420,
    quanlity,
    TJFLAG_ACCURATEDCT
  );

  *_out = out;
  *_ps = ps;

  free(dstBuf);

  tjDestroy(handle);
}
