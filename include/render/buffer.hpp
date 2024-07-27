#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <log.hpp>

#include <tiny_gltf.h>

namespace Engine {

    class BufferView;

    class Buffer {
        public:
            Buffer(std::vector<uint8_t> &data)  {
                LOG_INFO("Creating buffer: \t size %d", data.size());

                this->buffer.resize(data.size());
                memcpy(&this->buffer[0], &data[0], data.size());
            }

            ~Buffer() {
                LOG_INFO("Deleting buffer: \t size %d", buffer.size());
            }

            uint8_t* get(int offset) {
                return &buffer[offset];
            }

        private:
            std::vector<uint8_t> buffer;
    };

    class BufferView {
        public:
            BufferView(std::shared_ptr<Buffer> &buffer, tinygltf::BufferView &bv) : BufferView(buffer, bv.byteOffset, bv.byteLength, bv.target, GL_STATIC_DRAW) {}

            BufferView(std::shared_ptr<Buffer> &buffer, uint32_t offset, uint32_t size, uint32_t type, uint32_t mode) : offset(offset), size(size), type(type) {
                glGenBuffers(1, &ptr);

                bind();
                glBufferData(type, size, buffer->get(offset), mode);
                unbind();
            }

            ~BufferView() {
                glDeleteBuffers(1, &ptr);
            }

            void bind() {
                glBindBuffer(type, ptr);
            }

            void unbind() {
                glBindBuffer(type, 0);
            }

            uint32_t get_size() {
                return size;
            }

        private:
            GLuint ptr;

            uint32_t type;
            uint32_t offset;
            uint32_t size;
    };

}