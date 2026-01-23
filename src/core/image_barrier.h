#pragma once
#include <vulkan/vulkan.h>

struct  ImageLayoutTransition {
    VkImageLayout oldLayout;
    VkImageLayout newLayout;
    VkAccessFlags srcAccessMask;
    VkAccessFlags dstAccessMask;
    VkPipelineStageFlags srcStage;
    VkPipelineStageFlags dstStage;

    // Undefined状態から描画先としてのレイアウトへ
    static ImageLayoutTransition FromUndefinedToColorAttachment();

    // PresentSrc状態から描画先としてのレイアウトへ
    static ImageLayoutTransition FromPresentSrcToColorAttachment();

    // 描画先レイアウトからPresentSrcレイアウトへ
    static ImageLayoutTransition FromColorToPresent();
};