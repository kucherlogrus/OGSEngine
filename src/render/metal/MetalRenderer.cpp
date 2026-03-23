#include "MetalRenderer.h"
#include "backends/AppWindow.h"
#include "monitoring/Logsystem.h"
#include "render/TextureHandle.h"
#include "render/RenderQueue.h"
#include "storage/Texture.h"

#include <glm/gtc/type_ptr.hpp>

namespace ogs {

// -----------------------------------------------------------------------
// Test geometry (existing two-triangle demo)
// -----------------------------------------------------------------------
struct Vertex {
    float x, y;
    float r, g, b;
};

static const Vertex triangleVertices[] = {
    { -0.8f,  0.8f,  1.0f, 0.2f, 0.2f },
    { -0.2f,  0.8f,  1.0f, 0.2f, 0.2f },
    { -0.5f,  0.2f,  1.0f, 0.2f, 0.2f },
    {  0.2f, -0.2f,  0.2f, 0.4f, 1.0f },
    {  0.8f, -0.2f,  0.2f, 0.4f, 1.0f },
    {  0.5f, -0.8f,  0.2f, 0.4f, 1.0f },
};

// -----------------------------------------------------------------------
// Init / shutdown
// -----------------------------------------------------------------------
void MetalRenderer::init(AppWindow& window) {
    device = MTL::CreateSystemDefaultDevice();
    if (!device) {
        logerror("MetalRenderer", "init", "Failed to create Metal device");
        return;
    }

    metalLayer = reinterpret_cast<CA::MetalLayer*>(window.getMetalLayer());
    if (!metalLayer) {
        logerror("MetalRenderer", "init", "No Metal layer on window");
        return;
    }
    metalLayer->setDevice(device);
    metalLayer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

    commandQueue = device->newCommandQueue();

    buildPipeline();
    buildVertexBuffer();
    buildSpritePipeline();
    buildSpriteSampler();

    loginfo("MetalRenderer", "init", "Metal renderer initialized");
}

void MetalRenderer::buildPipeline() {
    NS::Error* error = nullptr;

    MTL::Library* library = device->newDefaultLibrary();
    if (!library) {
        logerror("MetalRenderer", "buildPipeline", "Failed to load default shader library");
        return;
    }

    MTL::Function* vertexFn   = library->newFunction(
        NS::String::string("vertex_main", NS::UTF8StringEncoding));
    MTL::Function* fragmentFn = library->newFunction(
        NS::String::string("fragment_main", NS::UTF8StringEncoding));

    MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init();
    desc->setVertexFunction(vertexFn);
    desc->setFragmentFunction(fragmentFn);
    desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

    pipelineState = device->newRenderPipelineState(desc, &error);
    if (!pipelineState)
        logerror("MetalRenderer", "buildPipeline", "Failed to create pipeline state");

    desc->release();
    vertexFn->release();
    fragmentFn->release();
    library->release();
}

void MetalRenderer::buildVertexBuffer() {
    vertexBuffer = device->newBuffer(triangleVertices,sizeof(triangleVertices),MTL::ResourceStorageModeShared);
}

void MetalRenderer::buildSpritePipeline() {
    NS::Error* error = nullptr;

    MTL::Library* library = device->newDefaultLibrary();
    if (!library) {
        logerror("MetalRenderer", "buildSpritePipeline", "Failed to load shader library");
        return;
    }

    MTL::Function* vertFn = library->newFunction(NS::String::string("sprite_vertex", NS::UTF8StringEncoding));
    MTL::Function* fragFn = library->newFunction(NS::String::string("sprite_fragment", NS::UTF8StringEncoding));

    if (!vertFn || !fragFn) {
        logerror("MetalRenderer", "buildSpritePipeline", "sprite shader functions not found");
        library->release();
        return;
    }

    // Vertex descriptor: position(float2), uv(float2), alpha(float)
    MTL::VertexDescriptor* vd = MTL::VertexDescriptor::alloc()->init();

    // attribute 0: position (float2, offset 0)
    vd->attributes()->object(0)->setFormat(MTL::VertexFormatFloat2);
    vd->attributes()->object(0)->setOffset(0);
    vd->attributes()->object(0)->setBufferIndex(0);

    // attribute 1: uv (float2, offset 8)
    vd->attributes()->object(1)->setFormat(MTL::VertexFormatFloat2);
    vd->attributes()->object(1)->setOffset(8);
    vd->attributes()->object(1)->setBufferIndex(0);

    // attribute 2: alpha (float, offset 16)
    vd->attributes()->object(2)->setFormat(MTL::VertexFormatFloat);
    vd->attributes()->object(2)->setOffset(16);
    vd->attributes()->object(2)->setBufferIndex(0);

    // layout 0: stride = sizeof(SpriteVertex) = 20 bytes
    vd->layouts()->object(0)->setStride(sizeof(SpriteVertex));

    MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init();
    desc->setVertexFunction(vertFn);
    desc->setFragmentFunction(fragFn);
    desc->setVertexDescriptor(vd);

    auto* ca = desc->colorAttachments()->object(0);
    ca->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    ca->setBlendingEnabled(true);
    ca->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
    ca->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    ca->setSourceAlphaBlendFactor(MTL::BlendFactorOne);
    ca->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);

    spritePipelineState = device->newRenderPipelineState(desc, &error);
    if (!spritePipelineState)
        logerror("MetalRenderer", "buildSpritePipeline", "Failed to create sprite pipeline");

    vd->release();
    desc->release();
    vertFn->release();
    fragFn->release();
    library->release();
}

void MetalRenderer::buildSpriteSampler() {
    MTL::SamplerDescriptor* sd = MTL::SamplerDescriptor::alloc()->init();
    sd->setMinFilter(MTL::SamplerMinMagFilterLinear);
    sd->setMagFilter(MTL::SamplerMinMagFilterLinear);
    sd->setSAddressMode(MTL::SamplerAddressModeClampToEdge);
    sd->setTAddressMode(MTL::SamplerAddressModeClampToEdge);
    spriteSampler = device->newSamplerState(sd);
    sd->release();
}

void MetalRenderer::shutdown() {
    for (auto& [id, tex] : textures) tex->release();
    textures.clear();

    if (spriteSampler)       { spriteSampler->release();       spriteSampler       = nullptr; }
    if (spritePipelineState) { spritePipelineState->release(); spritePipelineState = nullptr; }
    if (vertexBuffer)        { vertexBuffer->release();        vertexBuffer        = nullptr; }
    if (pipelineState)       { pipelineState->release();       pipelineState       = nullptr; }
    if (commandQueue)        { commandQueue->release();        commandQueue        = nullptr; }
    if (device)              { device->release();              device              = nullptr; }
}

MetalRenderer::~MetalRenderer() {
    shutdown();
}

// -----------------------------------------------------------------------
// Per-frame
// -----------------------------------------------------------------------
void MetalRenderer::beginFrame() {
    currentDrawable = metalLayer->nextDrawable();
    if (!currentDrawable) return;

    MTL::RenderPassDescriptor* pass = MTL::RenderPassDescriptor::alloc()->init();
    auto* ca = pass->colorAttachments()->object(0);
    ca->setTexture(currentDrawable->texture());
    ca->setLoadAction(MTL::LoadActionClear);
    ca->setClearColor(MTL::ClearColor(0.1, 0.1, 0.1, 1.0));
    ca->setStoreAction(MTL::StoreActionStore);

    currentCmd     = commandQueue->commandBuffer();
    currentEncoder = currentCmd->renderCommandEncoder(pass);
    pass->release();

    // Draw test triangles
    currentEncoder->setRenderPipelineState(pipelineState);
    currentEncoder->setVertexBuffer(vertexBuffer, 0, 0);
    currentEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(6));
}

void MetalRenderer::submit(const RenderQueue& queue) {
    if (!currentEncoder) return;

    // Process transparent queue (sprites live here)
    for (const auto& cmd : queue.transparent) {
        if (std::holds_alternative<SpriteBatchCommand>(cmd.data)) {
            submitSpriteBatch(std::get<SpriteBatchCommand>(cmd.data));
        }
        // Mesh3DCommand in transparent queue — TODO when 3D transparency is needed
    }

    // Process opaque queue (3D meshes)
    for (const auto& cmd : queue.opaque) {
        if (std::holds_alternative<Mesh3DCommand>(cmd.data)) {
            // TODO: dispatch 3D mesh draw calls
            (void)cmd;
        }
    }
}

void MetalRenderer::submitSpriteBatch(const SpriteBatchCommand& batch) {
    if (!spritePipelineState || !spriteSampler) return;
    if (batch.vertices.empty() || batch.indices.empty()) return;

    auto it = textures.find(batch.texture.id);
    if (it == textures.end()) return; // texture not uploaded

    MTL::Texture* mtlTex = it->second;

    // Upload vertex data into a transient shared buffer
    MTL::Buffer* vbuf = device->newBuffer(
        batch.vertices.data(),
        batch.vertices.size() * sizeof(SpriteVertex),
        MTL::ResourceStorageModeShared
    );

    // Upload index data
    MTL::Buffer* ibuf = device->newBuffer(
        batch.indices.data(),
        batch.indices.size() * sizeof(uint32_t),
        MTL::ResourceStorageModeShared
    );

    // Upload projection matrix
    glm::mat4 proj = batch.projection;
    MTL::Buffer* ubuf = device->newBuffer(
        glm::value_ptr(proj),
        sizeof(glm::mat4),
        MTL::ResourceStorageModeShared
    );

    currentEncoder->setRenderPipelineState(spritePipelineState);
    currentEncoder->setVertexBuffer(vbuf,  0, 0); // vertices  → buffer(0)
    currentEncoder->setVertexBuffer(ubuf,  0, 1); // projection → buffer(1)
    currentEncoder->setFragmentTexture(mtlTex, 0);
    currentEncoder->setFragmentSamplerState(spriteSampler, 0);

    currentEncoder->drawIndexedPrimitives(
        MTL::PrimitiveTypeTriangle,
        NS::UInteger(batch.indices.size()),
        MTL::IndexTypeUInt32,
        ibuf,
        NS::UInteger(0)
    );

    // autorelease: buffers are freed after the command buffer completes execution
    vbuf->autorelease();
    ibuf->autorelease();
    ubuf->autorelease();
}

void MetalRenderer::endFrame() {
    if (!currentEncoder) return;

    currentEncoder->endEncoding();
    currentCmd->presentDrawable(currentDrawable);
    currentCmd->commit();

    currentEncoder  = nullptr;
    currentCmd      = nullptr;
    currentDrawable = nullptr;
}

// -----------------------------------------------------------------------
// Texture management
// -----------------------------------------------------------------------
TextureHandle MetalRenderer::uploadTexture(const TextureData& data) {
    if (!device || !data.pixdata || data.pixlen == 0) return TextureHandle{};

    MTL::TextureDescriptor* td = MTL::TextureDescriptor::alloc()->init();
    td->setTextureType(MTL::TextureType2D);
    td->setWidth(NS::UInteger(data.width));
    td->setHeight(NS::UInteger(data.height));
    td->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    td->setUsage(MTL::TextureUsageShaderRead);
    td->setStorageMode(MTL::StorageModeShared);

    MTL::Texture* tex = device->newTexture(td);
    td->release();

    if (!tex) {
        logerror("MetalRenderer", "uploadTexture", "Failed to create MTLTexture");
        return TextureHandle{};
    }

    MTL::Region region = MTL::Region::Make2D(0, 0, data.width, data.height);
    tex->replaceRegion(region,
                       /*mipmapLevel=*/0,
                       data.pixdata,
                       /*bytesPerRow=*/NS::UInteger(data.width * 4));

    TextureHandle handle{ nextTextureId++ };
    textures[handle.id] = tex;

    loginfo("MetalRenderer", "uploadTexture", "Uploaded texture id:", handle.id,
            "size:", data.width, "x", data.height);
    return handle;
}

void MetalRenderer::releaseTexture(TextureHandle handle) {
    auto it = textures.find(handle.id);
    if (it == textures.end()) return;
    it->second->release();
    textures.erase(it);
}

} // ogs