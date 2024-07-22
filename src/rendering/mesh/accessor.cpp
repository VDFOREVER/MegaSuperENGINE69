#include <rendering/mesh/accessor.hpp>

namespace Engine::Mesh {

    Accessor::Accessor(std::shared_ptr<Buffer> buffer, tinygltf::BufferView &bufferView, tinygltf::Accessor &accessor) : buffer(buffer, bufferView) {
        size = tinygltf::GetNumComponentsInType(accessor.type);
        count = accessor.count;
        offset = accessor.byteOffset;
        stride = accessor.ByteStride(bufferView);
        type = accessor.componentType;
        normalized = accessor.normalized;
    }

    void Accessor::bind(uint32_t index) {
        buffer.bind();

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized ? GL_TRUE : GL_FALSE, stride, ((char*)NULL + offset));
    }

    void Accessor::bind() {
        buffer.bind();
    }

    void Accessor::draw() {
        buffer.bind();
        glDrawElements(GL_TRIANGLES, count, type, ((char*)NULL + offset));
    }

    void Accessor::unbind() {
        buffer.unbind();
    }
}