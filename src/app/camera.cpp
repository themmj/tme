/** @file */

#include "app/camera.hpp"

namespace tme {
    namespace app {

        Camera::Camera(uint32_t width, uint32_t height) {
            m_position = {0.0f, 0.0f};
            double baseW = (double)width;
            double baseH = (double)height;
            double baseMin;
            if (baseW < baseH)
                baseMin = baseW;
            else
                baseMin = baseH;
            m_dimensions.x = baseMin;
            m_dimensions.y = baseMin;
        }

        void Camera::scaleWidth(double scale) {
            m_dimensions.x *= scale;
        }

        void Camera::scaleHeight(double scale) {
            m_dimensions.y *= scale;
        }

        void Camera::addX(double diff) {
            m_position.x += diff;
        }
        void Camera::addY(double diff) {
            m_position.y += diff;
        }

        glm::mat4 Camera::getProjection() const {
            return glm::ortho(0.0f, static_cast<float>(m_dimensions.x), 0.0f, static_cast<float>(m_dimensions.y));
        }

        glm::mat4 Camera::getView() const {
            return glm::translate(glm::mat4(1.0f), glm::vec3(static_cast<float>(m_position.x), static_cast<float>(m_position.y), 0.0f));
        }

        glm::mat4 Camera::getMVP() const {
            return getProjection() *  getView();
        }

    }
}

