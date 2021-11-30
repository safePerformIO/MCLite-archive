/**
 * @file Scene.hpp
 * @author H14
 * @brief Scene contains all QGraphisItem of the game; when Scene updates, all QGraphisItem will automatically be repainted
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <QGraphicsScene>
#include <QRectF>

// the world is added to scene
class Scene : public QGraphicsScene
{
    Q_OBJECT
private:
    /**
     * @brief records the coordinate of the mouse
     *
     */
    QPointF mouse;

public:
    /**
     * @brief Construct a new Scene object
     *
     * @param screen the screen size will be recorded
     */
    Scene(QRectF screen);
    ~Scene();

    /**
     * @brief update the mouse coordinate
     *
     * @param mouse
     */
    void updateMouse(const QPointF &mouse);

    /**
     * @brief Get the Mouse object
     *
     * @return QPointF
     */
    QPointF getMouse() const;

    /**
     * @brief Get the direction the player looking at
     *
     * @return float
     */
    float getAngle() const;

    const QRectF screen; // screen size of the current device
};
