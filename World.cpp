#include "World.hpp"
#include "Channel.hpp"

void World::setChannel(Channel *c)
{
    channel = c;
}

World::World(Channel *channel) : channel(channel), background(":/backGround/res/BGI.jpg"), blockDamage(":/blocks/res/damage_effects.png")
{
    // load background image
    channel->scene->addItem(this);
    offset = channel->scene->screen.width() / 1920.0;
}

void World::paintBackground(QPainter *painter)
{
    painter->drawImage(channel->scene->screen, background,
                       QRectF(1920 * (channel->player->x() / (channel->map->MAXWIDTH * 32.0)),
                              1080 * (1 - channel->player->y() / (Chunk::HEIGHT * 32.0)),
                              1920, 1080));
}

void World::paintMap(QPainter *painter)
{
    /* paint map */
    painter->save(); // focus on the center of the screen
    for (int y = -2 - channel->scene->screen.height() / 64; y <= channel->scene->screen.height() / 64; y++)
    {
        for (int x = -1 - channel->scene->screen.width() / 64; x <= channel->scene->screen.width() / 64 + 1; x++)
        {
            QRectF target(channel->scene->screen.width() / 2 + x * 32 + (channel->player->mx() * 32 - channel->player->x()),
                          channel->scene->screen.height() / 2 + y * 32 - (channel->player->my() * 32 - channel->player->y()), 32.0, 32.0);
            {
                auto pos = QPoint(x + channel->player->mx(), -y + channel->player->my());
                if (channel->map->inBound(pos))
                {
                    auto &b = channel->map->get(QPoint(x + channel->player->mx(), -y + channel->player->my()));
                    auto md = b->acquireModel(channel->texture);
                    if (md)
                    {
                        painter->drawImage(target, *md);
                        if (b->getHp() <= 59)
                            painter->drawImage(target, blockDamage, QRectF((59 - b->getHp()) / 6 * 64, 0, 64, 64));
                    }
                }
            }
        }
    }
    painter->restore();
}

void World::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    paintBackground(painter);
    paintMap(painter);
}

QRectF World::boundingRect() const
{
    return (QRectF(0, 0, channel->scene->screen.width(), channel->scene->screen.height()));
}
