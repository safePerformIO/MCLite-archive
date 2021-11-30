/**
 * @file World.hpp
 * @author H14
 * @brief the world
 *        draws the map and background image
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <QGraphicsItem>
#include <QObject>
#include <vector>

struct Channel;

/*
    The World
 */
class World : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
private:
    Channel *channel;
    const QImage background;        // background image
    const QImage blockDamage;       // damange image
    double offset;
public:
    World(Channel *channel);
    ~World() = default;

    // set channel
    void setChannel(Channel *c);

public:
    // functions for painting the map and background images
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void paintBackground(QPainter *painter);
    void paintMap(QPainter *painter);

};
