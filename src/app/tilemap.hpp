#ifndef _APP_TILEMAP_H
#define _APP_TILEMAP_H
/** @file */

#include <vector>
#include "core/storage.hpp"
#include "core/events/handler.hpp"
#include "core/graphics/common.hpp"
#include "core/layers/layer.hpp"
#include "app/graphics/tile.hpp"

namespace tme {
    namespace app {

        /**//**
         * \brief Cursor of a Tilemap.
         *
         * Stores currently selected layer and tile factory.
         * Additionally stores state information about mouse input.
         */
        struct Cursor {
            /// currently active TileFactory to palce tiles
            core::Handle<graphics::TileFactory> tileFactory;
            /// currently selected layer
            size_t layer = 0;
            /// indicates if the mouse is within the bounds of the tilemap
            bool inBounds = false;
            /// indicates that tiles should be placed
            bool placeTile = false;
            /// indicates that tiles should be erased
            bool eraseTile = false;
        };

        /**//**
         * \brief Tilemap with multiple layers which can be edited using its cursor.
         *
         * Keeps track of the associated shader and texture ids, its layers and the cursor.
         */
        class Tilemap final : public core::Mappable, public core::events::Handler, public core::graphics::Renderable {
            using IdentifierVector = std::vector<core::Identifier>;
            core::Identifier m_id;
            uint32_t m_tileSize;
            uint32_t m_width, m_height;
            IdentifierVector m_shaderIds;
            IdentifierVector m_textureIds;
            size_t m_layerCount = 0;
            core::layers::Stack m_layers;
            core::Handle<Cursor> m_cursor;

            public:
            /**//**
             * \brief Construct Tilemap.
             *
             * @param width number of tiles in the x direction
             * @param height number of tiles in the y direction
             * @param tileSize size of a tile dimension in pixels
             */
            Tilemap(uint32_t width, uint32_t height, uint32_t tileSize);
            ~Tilemap();

            core::Identifier getId() const override { return m_id; }
            void onEvent(core::events::Event& e) override;
            void render() override;

            /**//**
             * \brief Get width of Tilemap.
             *
             * @return width of the map in full tiles
             */
            inline uint32_t getWidth() const { return m_width; }
            /**//**
             * \brief Get height of Tilemap.
             *
             * @return height of the map in full tiles
             */
            inline uint32_t getHeight() const { return m_height; }
            /**//**
             * \brief Get size of a single tile.
             *
             * @return size of a single tile in pixels
             */
            inline uint32_t getTileSize() const { return m_tileSize; }
            /**//**
             * \brief Get Cursor of Tilemap.
             *
             * @return Handle to the Cursor
             */
            inline core::Handle<Cursor> getCursor() const { return m_cursor; }

            /**//**
             * \brief Adds shaderId to list of associated shaders.
             *
             * @param shaderId id of the shader in question
             */
            void addShader(core::Identifier shaderId);
            /**//**
             * \brief Get associated shader ids of Tilemap.
             *
             * @return reference to vector of global Identifier of associated Shader
             */
            inline const IdentifierVector& getShaderIds() { return m_shaderIds; }

            /**//**
             * \brief Adds textureId to list of associated textures.
             *
             * @param textureId id of the texture in question
             */
            void addTexture(core::Identifier textureId);
            /**//**
             * \brief Get associated texture ids of Tilemap.
             *
             * @return reference to vector of global Identifier of associated Texture
             */
            inline const IdentifierVector& getTextureIds() { return m_textureIds; }

            /**//**
             * \brief Get number of layers.
             *
             * @return number of MapLayer in the Tilemap
             */
            size_t getLayerCount() const { return m_layerCount; }
            /**//**
             * \brief Add MapLayer.
             */
            void addLayer();
            /**//**
             * \brief Remove MapLayer.
             */
            void removeLayer();
        };

    }
}

#endif

