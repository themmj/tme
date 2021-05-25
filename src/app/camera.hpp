#ifndef _APP_CAMERA_H
#define _APP_CAMERA_H
/** @file */

#include "glm/ext/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "core/log.hpp"

namespace tme {
    namespace app {

        /**//**
         * \brief Abstraction for MVP matrix generation.
         */
        class Camera {
            public:
            /**//**
             * \brief Coordinate pair.
             */
            struct Coordinates {
                /// x coordinate
                double x;
                /// y coordinate
                double y;
            };

            private:
            Coordinates m_position;
            Coordinates m_dimensions;

            public:
            /**//**
             * \brief Construct Camera with initial dimensions.
             *
             * @param width initial width of the projection
             * @param height initial height of the projection
             */
            Camera(uint32_t width, uint32_t height);

            /**//**
             * \brief Scale the width of the projection.
             *
             * Updates view to keep the camera in the middle.
             *
             * @param scale factor to scale the width of the projection with
             */
            void scaleWidth(double scale);
            /**//**
             * \brief Scale the height of the projection.
             *
             * Updates view to keep the camera in the middle.
             *
             * @param scale factor to scale the height of the projection with
             */
            void scaleHeight(double scale);

            /**//**
             * \brief Move view of the camera in x direction.
             *
             * @param diff distance to move the camera
             */
            void addX(double diff);
            /**//**
             * \brief Move view of the camera in y direction.
             *
             * @param diff distance to move the camera
             */
            void addY(double diff);

            /**//**
             * \brief Get projection matrix.
             *
             * @return projection matrix of the camera
             */
            glm::mat4 getProjection() const;

            /**//**
             * \brief Get view matrix.
             *
             * @return view matrix of the camera
             */
            glm::mat4 getView() const;

            /**//**
             * \brief Get MVP matrix.
             *
             * @return model view projection matrix of the camera
             */
            glm::mat4 getMVP() const;

            /**//**
             * \brief Get current camera position.
             *
             * @return Coordinates of the current position
             */
            inline Coordinates getPosition() const { return m_position; }
            /**//**
             * \brief Get current camera dimensions.
             *
             * @return Coordinates of the current dimensions
             */
            inline Coordinates getDimensions() const { return m_dimensions; }

        };

    }
}

#endif

