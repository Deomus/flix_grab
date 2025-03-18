#include "video_frame_container.h"


VideoFrameContainer::VideoFrameContainer() : buffer_(NULL)
{

}

VideoFrameContainer::~VideoFrameContainer()
{
    if (buffer_) buffer_->Destroy();
}

void VideoFrameContainer::SetFormat(cdm::VideoFormat format)
{
    // std::cout << "Timestamp: " << cdm::GetVideoFormatName(format) << std::endl;
    //_format = format;
    frame_.format = format;
}

void VideoFrameContainer::SetSize(cdm::Size size)
{
    // std::cout << "Size: " << size.width << "x" << size.height << std::endl;

    frame_.width = size.width;
    frame_.height = size.height;
}

void VideoFrameContainer::SetFrameBuffer(cdm::Buffer* frame_buffer)
{
    if (buffer_) buffer_->Destroy();

    buffer_ = frame_buffer;
}

void VideoFrameContainer::SetPlaneOffset(cdm::VideoFrame::VideoPlane plane, uint32_t offset)
{
    plane_offset_[plane] = offset;

    //std::cout << "Frame Plane: " << plane << " Offset: " << offset << std::endl;
}

void VideoFrameContainer::SetStride(cdm::VideoFrame::VideoPlane plane, uint32_t stride)
{
    plane_stride_[plane] = stride;
    //std::cout << "Frame Plane: " << plane << " stride: " << stride << std::endl;
}

void VideoFrameContainer::SetTimestamp(int64_t ts)
{
    //std::cout << "Timestamp: " << ts << std::endl;
    frame_.timestamp = ts;
}

bool VideoFrameContainer::Commit(const FrameSource_fn& frame_sink)
{
    return frame_sink(&frame_, buffer_->Data(), buffer_->Size());
}
