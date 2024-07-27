#include <utils.hpp>

namespace Engine::Utils {
    glm::quat quat_from_btQuat(btQuaternion btQuat) {
        return glm::quat(btQuat.getX(), btQuat.getY(), btQuat.getZ(), btQuat.getW());
    }

    tinygltf::Model array2model(std::vector<float> vertices, std::vector<unsigned short> indices) {
        tinygltf::Model model;
        tinygltf::Scene scene;
        tinygltf::Mesh mesh;
        tinygltf::Node node;
        
        // Create a buffer
        tinygltf::Buffer buffer;
        buffer.data.resize(vertices.size() * sizeof(float) + indices.size() * sizeof(unsigned short));

        // Fill the buffer with vertex data
        memcpy(buffer.data.data(), vertices.data(), vertices.size() * sizeof(float));
        
        // Fill the buffer with index data after vertices
        memcpy(buffer.data.data() + vertices.size() * sizeof(float), indices.data(), indices.size() * sizeof(unsigned short));

        // Add the buffer to the model
        model.buffers.push_back(buffer);

        // Create a buffer view for vertices
        tinygltf::BufferView vertexBufferView;
        vertexBufferView.buffer = 0; // Index of the buffer
        vertexBufferView.byteOffset = 0;
        vertexBufferView.byteLength = vertices.size() * sizeof(float);
        vertexBufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;

        // Create a buffer view for indices
        tinygltf::BufferView indexBufferView;
        indexBufferView.buffer = 0; // Index of the buffer
        indexBufferView.byteOffset = vertices.size() * sizeof(float);
        indexBufferView.byteLength = indices.size() * sizeof(unsigned short);
        indexBufferView.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

        // Add buffer views to the model
        model.bufferViews.push_back(vertexBufferView);
        model.bufferViews.push_back(indexBufferView);

        // Create an accessor for vertices
        tinygltf::Accessor vertexAccessor;
        vertexAccessor.bufferView = 0; // Index of the vertex buffer view
        vertexAccessor.byteOffset = 0;
        vertexAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        vertexAccessor.count = vertices.size() / 3; // Assuming 3 floats per vertex (x, y, z)
        vertexAccessor.type = TINYGLTF_TYPE_VEC3;

        // Create an accessor for indices
        tinygltf::Accessor indexAccessor;
        indexAccessor.bufferView = 1; // Index of the index buffer view
        indexAccessor.byteOffset = 0;
        indexAccessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
        indexAccessor.count = indices.size();
        indexAccessor.type = TINYGLTF_TYPE_SCALAR;

        // Add accessors to the model
        model.accessors.push_back(vertexAccessor);
        model.accessors.push_back(indexAccessor);

        // Create a primitive and add to mesh
        tinygltf::Primitive primitive;
        primitive.indices = 1; // Index of the index accessor
        primitive.attributes["POSITION"] = 0; // Index of the vertex accessor
        primitive.mode = TINYGLTF_MODE_TRIANGLES;
        mesh.primitives.push_back(primitive);

        // Add the mesh to the model
        model.meshes.push_back(mesh);

        // Add a node to reference the mesh
        node.mesh = 0; // Index of the mesh
        model.nodes.push_back(node);

        // Set up the default scene
        scene.nodes.push_back(0); // Index of the node
        model.scenes.push_back(scene);
        model.defaultScene = 0;

        return model;
    }

    tinygltf::Model array2model(std::vector<float> vertices) {
        // Assuming each vertex has 3 components (x, y, z), this version will create a simple model without indices
        std::vector<unsigned short> indices;
        for (size_t i = 0; i < vertices.size() / 3; ++i) {
            indices.push_back(static_cast<unsigned short>(i));
        }
        return array2model(vertices, indices);
    }

    tinygltf::Model lines2model(std::vector<std::vector<float>> linesVertices, std::vector<std::array<float, 4>> colors) {
        tinygltf::Model model;
        tinygltf::Scene scene;
        tinygltf::Node node;
        tinygltf::Mesh mesh;

        size_t bufferIndex = 0;

        for (size_t i = 0; i < linesVertices.size(); ++i) {
            const auto& vertices = linesVertices[i];
            const auto& color = colors[i];

            // Create a buffer for the current line
            tinygltf::Buffer buffer;
            buffer.data.resize(vertices.size() * sizeof(float) + vertices.size() / 3 * sizeof(color));

            // Fill the buffer with vertex data
            memcpy(buffer.data.data(), vertices.data(), vertices.size() * sizeof(float));

            // Fill the buffer with color data
            for (size_t j = 0; j < vertices.size() / 3; ++j) {
                memcpy(buffer.data.data() + vertices.size() * sizeof(float) + j * sizeof(color), color.data(), sizeof(color));
            }

            // Add the buffer to the model
            model.buffers.push_back(buffer);

            // Create a buffer view for vertices
            tinygltf::BufferView vertexBufferView;
            vertexBufferView.buffer = bufferIndex;
            vertexBufferView.byteOffset = 0;
            vertexBufferView.byteLength = vertices.size() * sizeof(float);
            vertexBufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;

            // Create a buffer view for colors
            tinygltf::BufferView colorBufferView;
            colorBufferView.buffer = bufferIndex;
            colorBufferView.byteOffset = vertices.size() * sizeof(float);
            colorBufferView.byteLength = vertices.size() / 3 * sizeof(color);
            colorBufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;

            // Add buffer views to the model
            model.bufferViews.push_back(vertexBufferView);
            model.bufferViews.push_back(colorBufferView);

            // Create an accessor for vertices
            tinygltf::Accessor vertexAccessor;
            vertexAccessor.bufferView = 2 * bufferIndex; // Index of the vertex buffer view
            vertexAccessor.byteOffset = 0;
            vertexAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
            vertexAccessor.count = vertices.size() / 3; // Assuming 3 floats per vertex (x, y, z)
            vertexAccessor.type = TINYGLTF_TYPE_VEC3;

            // Create an accessor for colors
            tinygltf::Accessor colorAccessor;
            colorAccessor.bufferView = 2 * bufferIndex + 1; // Index of the color buffer view
            colorAccessor.byteOffset = 0;
            colorAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
            colorAccessor.count = vertices.size() / 3; // Number of vertices
            colorAccessor.type = TINYGLTF_TYPE_VEC4;

            // Add accessors to the model
            model.accessors.push_back(vertexAccessor);
            model.accessors.push_back(colorAccessor);

            // Create a primitive for the line and add to mesh
            tinygltf::Primitive primitive;
            primitive.attributes["POSITION"] = 2 * bufferIndex; // Index of the vertex accessor
            primitive.attributes["COLOR_0"] = 2 * bufferIndex + 1; // Index of the color accessor
            primitive.mode = TINYGLTF_MODE_LINE;

            mesh.primitives.push_back(primitive);

            // Update the buffer index for the next line
            bufferIndex++;
        }

        // Add the mesh to the model
        model.meshes.push_back(mesh);

        // Add a node to reference the mesh
        node.mesh = 0; // Index of the mesh
        model.nodes.push_back(node);

        // Set up the default scene
        scene.nodes.push_back(0); // Index of the node
        model.scenes.push_back(scene);
        model.defaultScene = 0;

        return model;
    }
}