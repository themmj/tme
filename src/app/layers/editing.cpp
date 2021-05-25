/** @file */

#include "app/layers/editing.hpp"
#include "core/events/event.hpp"
#include "core/events/dispatcher.hpp"
#include "core/events/mouse.hpp"
#include "core/events/window.hpp"
#include "core/graphics/shader.hpp"
#include "core/storage.hpp"

namespace tme {
    namespace app {
        namespace layers {

            Editing::Editing(core::Handle<Tilemap> tilemap)
                : core::layers::Layer("Editing"),
                Dispatcher(this),
                m_tilemap(tilemap),
                m_camera(tilemap->getWidth(), tilemap->getHeight()),
                m_cameraMoving(false) {}
            Editing::~Editing() {}

            void Editing::render() {
                for (auto shaderId : m_tilemap->getShaderIds()) {
                    if (auto shader = core::Storage<core::graphics::Shader>::global()->get(shaderId); shader) {
                        shader->bind();
                        shader->setUniformMat4f("u_mvp", m_camera.getMVP());
                    }
                }
                m_tilemap->render();
            }

            void Editing::onEvent(core::events::Event& event) {
                dispatchEvent<core::events::MouseKeyPress>(event, &Editing::handleMouseKeyPress);
                dispatchEvent<core::events::MouseKeyRelease>(event, &Editing::handleMouseKeyReleased);
                dispatchEvent<core::events::WindowResize>(event, &Editing::handleWindowResize);
                dispatchEvent<core::events::MouseMove>(event, &Editing::handleMouseMove);
                dispatchEvent<core::events::MouseScroll>(event, &Editing::handleMouseScroll);
                m_tilemap->onEvent(event);
            }

            bool Editing::handleMouseMove(core::events::MouseMove& event) {
                if (m_cameraMoving) {
                    m_camera.addX((event.getXPosRel() - m_prevCamX) * m_camera.getDimensions().x);
                    m_camera.addY((event.getYPosRel() - m_prevCamY) * m_camera.getDimensions().y);
                    m_prevCamX = event.getXPosRel();
                    m_prevCamY = event.getYPosRel();
                    return true;
                }
                m_prevCamX = event.getXPosRel();
                m_prevCamY = event.getYPosRel();
                double onScreenX = m_camera.getDimensions().x * event.getXPosRel();
                double onScreenY = m_camera.getDimensions().y * event.getYPosRel();
                double onMapX = onScreenX - m_camera.getPosition().x;
                double onMapY = onScreenY - m_camera.getPosition().y;
                if (onMapX >= 0.0 && onMapY >= 0.0) {
                    uint32_t tilePosX = static_cast<uint32_t>(onMapX);
                    uint32_t tilePosY = static_cast<uint32_t>(onMapY);
                    if (tilePosX < m_tilemap->getWidth() && tilePosY < m_tilemap->getHeight()) {
                        m_tilemap->getCursor()->tileFactory->setPosition(tilePosX, tilePosY);
                        m_tilemap->getCursor()->inBounds = true;
                        return true;
                    }
                }
                m_tilemap->getCursor()->inBounds = false;
                return false;
            }

            bool Editing::handleWindowResize(core::events::WindowResize& event) {
                m_camera.scaleWidth(event.getWidthFactor());
                m_camera.scaleHeight(event.getHeightFactor());
                return false;
            }

            bool Editing::handleMouseKeyPress(core::events::MouseKeyPress& event) {
                if (event.getKey().isKey(TME_MOUSE_BUTTON_MIDDLE)) {
                    m_cameraMoving = true;
                    return true;
                }
                if (event.getKey().isKey(TME_MOUSE_BUTTON_LEFT)) {
                    m_tilemap->getCursor()->placeTile = true;
                    return true;
                }
                if (event.getKey().isKey(TME_MOUSE_BUTTON_RIGHT)) {
                    m_tilemap->getCursor()->eraseTile = true;
                    return true;
                }
                return false;
            }

            bool Editing::handleMouseKeyReleased(core::events::MouseKeyRelease& event) {
                if (event.getKey().isKey(TME_MOUSE_BUTTON_MIDDLE)) {
                    m_cameraMoving = false;
                    return true;
                }
                if (event.getKey().isKey(TME_MOUSE_BUTTON_LEFT)) {
                    m_tilemap->getCursor()->placeTile = false;
                    return true;
                }
                if (event.getKey().isKey(TME_MOUSE_BUTTON_RIGHT)) {
                    m_tilemap->getCursor()->eraseTile = false;
                    return true;
                }
                return false;
            }

            bool Editing::handleMouseScroll(core::events::MouseScroll& event) {
                double scale = (m_camera.getDimensions().y - event.getYOffset()) / m_camera.getDimensions().y;
                if (scale > 0.0) {
                    m_camera.scaleWidth(scale);
                    m_camera.scaleHeight(scale);
                }
                return false;
            }

        }
    }
}

