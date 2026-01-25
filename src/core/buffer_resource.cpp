#include "buffer_resource.h"

void* VertexBuffer::Map()
{
    return nullptr;
}

void VertexBuffer::Unmap()
{
}

bool VertexBuffer::Initialize(VkDeviceSize size, VkMemoryPropertyFlags memProps)
{
    return false;
}
