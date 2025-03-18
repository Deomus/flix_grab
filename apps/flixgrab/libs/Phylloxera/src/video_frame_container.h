#pragma once

#include "content_decryption_module.h"
#include "decrypt_protocol.h"
#include "decrypt_server.h"


class VideoFrameContainer : public cdm::VideoFrame {
public:
 
    virtual void SetFormat(cdm::VideoFormat format);;
    virtual cdm::VideoFormat Format() const { return frame_.format; }

    virtual void SetSize(cdm::Size size);
    virtual cdm::Size Size() const { return cdm::Size(frame_.width, frame_.height); }

    virtual void SetFrameBuffer(cdm::Buffer* frame_buffer);
    virtual cdm::Buffer* FrameBuffer() { return buffer_; }

    virtual void SetPlaneOffset(cdm::VideoFrame::VideoPlane plane, uint32_t offset);
    virtual uint32_t PlaneOffset(cdm::VideoFrame::VideoPlane plane) { return plane_offset_[plane]; }

    virtual void SetStride(cdm::VideoFrame::VideoPlane plane, uint32_t stride);
    virtual uint32_t Stride(cdm::VideoFrame::VideoPlane plane) { return plane_stride_[plane]; }

    virtual void SetTimestamp(int64_t ts);
    virtual int64_t Timestamp() const { return frame_.timestamp; }
    
public:
    
    VideoFrameContainer();
    virtual ~VideoFrameContainer();

    bool        Commit(const FrameSource_fn& frame_sink);

private:
    VideoFrameData                      frame_;
    cdm::Buffer*                        buffer_;
    uint32_t                            plane_offset_[cdm::VideoFrame::kMaxPlanes];
    uint32_t                            plane_stride_[cdm::VideoFrame::kMaxPlanes];

};
