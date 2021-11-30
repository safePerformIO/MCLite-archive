#include <QPointF>
#include <cmath>
#include "Player.hpp"
#include "Channel.hpp"
#include "Block/BlockList.hpp"
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <sstream>
#include <QMessageBox>

Inventory::Inventory()
{
    items = {
        "dirt",
        "stone",
        "cobblestone",
        "log",
        "leaves",
        "glass",
        "plank"};
}

std::string Inventory::select(size_t i) const
{
    if (i >= items.size())
        return std::string();
    return items.at(i);
}

Player::Player(Channel *channel) : model(":/player/res/model.png"),
                                   GGimage_0(":/player/res/desmond.jpg"), GGimage_1(":/player/res/ggnai.png"),
                                   channel(channel), hp(maxHP), position(QPointF(50, 50)), vx(0), vy(0), ay(0), weapon(HarvestLevel::Hand)
{
    channel->scene->addItem(this);

    offset = channel->scene->screen.width() / 1920.0;

    // head
    Head_L_i = QRectF(108, 72, 71, 71);
    Head_R_i = QRectF(36, 72, 71, 71);
    Head_F_i = QRectF(72, 72, 71, 71);
    Head = QRectF(-16, -32, 32, 32);

    // body
    Body_F = QRectF(-12, -32, 24, 48);
    Body_F_i = QRectF(179, 143, 71, 142);
    Body_L = QRectF(-8, -32, 16, 48);
    Body_L_i = QRectF(166, 143, 47, 142);
    Body_R_i = QRectF(213, 143, 47, 142);

    // arm
    Arm_i = QRectF(356, 179, 36, 106);

    // legs
    Leg_L_i = QRectF(1, 143, 71, 142);
    Leg_R_i = QRectF(72, 143, 71, 142);
    Leg_L_s = QRectF(-12, -8, 12, 48);
    Leg_R_s = QRectF(0, -8, 12, 48);
    Leg_L = QRectF(-7, -8, 12, 48);
    Leg_R = QRectF(-5, -8, 12, 48);

    //    QRectF Body_F_i, Body_L_i, Body_F, Body_L;
    //    QRectF Arm_i, Arm;
    //    QRectF Leg_L_i, Leg_R_i, Leg_L, Leg_R;
    auto fa = channel->map->firstAir();
    setPosition(QPointF(fa.x() * 32 + 16, fa.y() * 32 + 16));
}

bool Player::isDead() const
{
    return hp <= 0;
}

void Player::die(QPainter *painter)
{
    if (!dead)
    {
        dead = true;
        dying = 5000;
    }
    else if (dying > 0)
        dying -= channel->dt;
    else
        dying = 2800;

    if (dying > 3800)
    {
        painter->save();

        painter->setOpacity((dying - 3800) / 1200.0);
        painter->translate(QPointF(channel->scene->screen.width() / 2, channel->scene->screen.height() / 2 + 100 * (dying - 3800) / 1200.0));
        painter->drawImage(Leg_L_s, model, Leg_L_i);
        painter->drawImage(Leg_R_s, model, Leg_R_i);
        painter->drawImage(Body_F, model, Body_F_i);
        painter->drawImage(QRectF(-24 * offset, -24 * offset, 12 * offset, 24 * offset), model, Arm_i);
        painter->drawImage(QRectF(12 * offset, -24 * offset, 12 * offset, 24 * offset), model, Arm_i);
        painter->drawImage(QRectF(-16 * offset, -48 * offset, 32 * offset, 32 * offset), model, Head_F_i);

        painter->restore();
    }
    else if (dying > 2800)
    {
        painter->save();

        painter->setOpacity(1 - (dying - 2800) / 1000.0);
        painter->drawImage(QRectF(channel->scene->screen.width() / 2 - 320 * offset, channel->scene->screen.height() / 2 - 320 * offset, 640 * offset, 640.0 * offset), GGimage_0);
        painter->drawImage(QRectF(channel->scene->screen.width() / 2 - 600 * offset, channel->scene->screen.height() / 2 - 180 * offset, 1200 * offset, 600 * offset), GGimage_1);

        painter->restore();
    }
    else
    {
        painter->drawImage(QRectF(channel->scene->screen.width() / 2 - 320 * offset, channel->scene->screen.height() / 2 - 320 * offset, 640 * offset, 640.0 * offset), GGimage_0);
        painter->setOpacity((rand() % 40 + 60) / 100.0);
        if (dying > rand() % 800)
            painter->drawImage(QRectF(channel->scene->screen.width() / 2 - 600 * offset, channel->scene->screen.height() / 2 - 180 * offset, 1200 * offset, 600 * offset), GGimage_1);
    }
}

const Block::Ptr &Player::getBlockByPos(QPointF dp) const
{
    return channel->map->get(dp);
}

const QImage *Player::acquireModel(Texture::Ptr) const
{
    return &model;
}

void Player::setChannel(Channel *c)
{
    channel = c;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    update();

    if (isDead())
    {
        die(painter);
        return;
    }

    painter->save();                                                                                      // focus on the center of the screen
    painter->translate(QPointF(channel->scene->screen.width() / 2, channel->scene->screen.height() / 2)); // change the origin to the middle of the screen
    auto angle = channel->scene->getAngle();                                                              // get the angle of arm and head
    auto item = channel->texture->getTexture(inventory.select(holding));                                  // get current holding item

    // leg
    painter->save();
    painter->translate(0, 8 + 16 * jump_progress / 200.0);

    if (!moving)
    {
        if ((angle >= -30.0 && angle <= 30.0) || (angle >= 150.0 || angle <= -150.0)) // standing
        {
            painter->drawImage(Leg_L_s, model, Leg_L_i);
            painter->drawImage(Leg_R_s, model, Leg_R_i);
        }
        else if (angle < -30.0 && angle > -150.0) // standing right
        {
            painter->drawImage(Leg_L, model, Leg_L_i);
            painter->drawImage(Leg_R, model, Leg_R_i);
        }
        else // standing left
        {
            painter->drawImage(Leg_R, model, Leg_R_i);
            painter->drawImage(Leg_L, model, Leg_L_i);
        }
    }
    else
    {
        auto moving_phase = move_progress / 100;
        auto leg_angle = 40.0 * (move_progress % 100) / 100.0;
        if (moving_phase == 1)
            leg_angle = 40.0 - leg_angle;
        else if (moving_phase == 2)
            leg_angle = -leg_angle;
        else if (moving_phase == 3)
            leg_angle = -40 + leg_angle;

        if (vx > 0) // moving right
        {
            painter->rotate(leg_angle);
            painter->drawImage(Leg_R, model, Leg_R_i);
            painter->rotate(-2 * leg_angle);
            painter->drawImage(Leg_L, model, Leg_L_i);
        }
        else // moving left
        {
            painter->rotate(-leg_angle);
            painter->drawImage(Leg_L, model, Leg_L_i);
            painter->rotate(2 * leg_angle);
            painter->drawImage(Leg_R, model, Leg_R_i);
        }
    }

    painter->restore();

    // body
    painter->save();
    if ((angle >= -30.0 && angle <= 30.0) || (angle >= 150.0 || angle <= -150.0)) // front
        painter->drawImage(Body_F, model, Body_F_i);
    else if ((angle < -30.0 && angle > -150.0)) // left
        painter->drawImage(Body_L, model, Body_L_i);
    else // right
        painter->drawImage(Body_L, model, Body_R_i);
    painter->restore();

    // arm
    painter->save();
    painter->translate(0, -16);
    if ((angle >= -30.0 && angle <= 30.0) || (angle >= 150.0 || angle <= -150.0))
    {
        // print arm
        painter->drawImage(QRectF(-24, -8, 12, 24 + ((angle >= -30.0 && angle <= 30.0) ? 1 : -1) * 8.0 * (attack_progress + place_progress) / 200.0), model, Arm_i);
        painter->drawImage(QRectF(12, -8, 12, 24 + ((angle >= -30.0 && angle <= 30.0) ? 1 : -1) * 8.0 * (attack_progress + place_progress) / 200.0), model, Arm_i);
        // print holding item
        if (place_progress < 100)
            painter->drawImage(QRectF(-14, 6 + ((angle >= -30.0 && angle <= 30.0) ? 1 : -1) * 8.0 * (attack_progress + place_progress) / 200.0, 28, 28), *item);
    }
    else
    {
        painter->rotate(angle + ((angle < -30.0 && angle > -150.0) ? 1 : -1) * 40.0 * attack_progress / 200.0);

        // print holding item
        if (place_progress < 100)
            painter->drawImage(QRectF(-4 + ((angle < -30.0 && angle > -150.0) ? 0 : -20), 18 + 12 * place_progress / 200.0, 28, 28), *item);
        // print arm
        painter->drawImage(QRectF(-8, -8, 16, 48 + 12 * place_progress / 200.0), model, Arm_i);
    }
    painter->restore();

    // head
    painter->save();
    painter->translate(0, -16);
    if ((angle >= -30.0 && angle <= 30.0) || (angle >= 150.0 || angle <= -150.0))
        painter->drawImage(Head, model, Head_F_i);
    else if (angle < -30.0 && angle > -150.0)
    {
        painter->rotate(angle + 90);
        painter->drawImage(Head, model, Head_R_i);
    }
    else
    {
        painter->rotate(angle - 90);
        painter->drawImage(Head, model, Head_L_i);
    }
    painter->restore();

    painter->restore(); // cancel focus on the center of the screen

    // show currently holding item
    if (item)
    {
        auto i = QRectF(channel->scene->screen.width() * 0.74 + 8 * offset, channel->scene->screen.height() * 0.052 + 8 * offset, 64 * offset, 64 * offset);
        auto b = QRectF(channel->scene->screen.width() * 0.74, channel->scene->screen.height() * 0.052, 80 * offset, 80 * offset);
        painter->fillRect(b, Qt::white);
        painter->drawImage(i, *item);
    }
}

void Player::update()
{
    if (isDead())
        return;

    // suffocate damage
    if (channel->map->inBound(position / 32 + QPointF(-0.5, 1)) &&
        channel->map->inBound(position / 32 + QPointF(-0.5, 2)) &&
        channel->map->inBound(position / 32 + QPointF(0.5, 1)) &&
        channel->map->inBound(position / 32 + QPointF(0.5, 2)))
    {
        if (!getBlockByPos(position / 32 + QPointF(-0.5, 1))->checkCanPass() &&
            !getBlockByPos(position / 32 + QPointF(-0.5, 2))->checkCanPass() &&
            !getBlockByPos(position / 32 + QPointF(0.5, 1))->checkCanPass() &&
            !getBlockByPos(position / 32 + QPointF(0.5, 2))->checkCanPass())
            hp -= 0.005 * channel->dt;
    }

    /* update position */
    // update x
    if (channel->map->inBound(position / 32 + QPointF(vx, 0)) && channel->map->inBound(position / 32 + QPointF(vx, 1)) && channel->map->inBound(position / 32 + QPointF(vx, 2)))
    {
        if (getBlockByPos(position / 32 + QPointF(vx, 0))->checkCanPass() &&
            getBlockByPos(position / 32 + QPointF(vx, 1))->checkCanPass() &&
            getBlockByPos(position / 32 + QPointF(vx, 2))->checkCanPass())
            position += channel->dt * QPointF(vx, 0);
    }

    // update y
    // update gravity
    if (isFalling())
        vy -= 0.008 * channel->dt;

    bool dtg = false;
    for (int i = 0; i < 20; i++)
    {
        if (channel->map->inBound(QPointF((position.x()) / 32, position.y() / 32 - i)) &&
            !getBlockByPos(QPointF((position.x()) / 32, position.y() / 32 - i))->checkCanPass())
        {
            distanceToGround = position.y() - 32 * (int(position.y()) / 32 - i) - 48;
            dtg = true;
            break;
        }
    }
    if (!dtg)
        distanceToGround = channel->scene->screen.height() / 2;

    bool dtc = false;
    for (int i = 0; i < 20; i++)
    {
        if (channel->map->inBound(QPointF((position.x()) / 32, position.y() / 32 + i)))
        {
            if (!getBlockByPos(QPointF((position.x()) / 32, position.y() / 32 + i))->checkCanPass())
            {
                distanceToCeiling = 32 * (int(position.y()) / 32 + i) + 90 - position.y() - 170;
                dtc = true;
                break;
            }
        }
        else
        {
            distanceToCeiling = 32 * (int(position.y()) / 32 + i) + 90 - position.y() - 170;
            dtc = true;
            break;
        }
    }
    if (!dtc)
        distanceToCeiling = channel->scene->screen.height() / 2;

    if (channel->map->inBound(position / 32 + QPointF(0, vy)))
    {
        if (vy > 0) // jumping up
        {
            if (channel->dt * vy < distanceToCeiling)
                position += channel->dt * QPointF(0, vy);
            else
            {
                position += QPointF(0, distanceToCeiling);
                vy = 0;
            }
        }
        else if (vy < 0) // falling down
        {
            if (-channel->dt * vy < distanceToGround)
                position += channel->dt * QPointF(0, vy);
            else
            {
                position += QPointF(0, -distanceToGround);
                // calculate fall damage
                if (vy < -3)
                {
                    hp += vy * 0.5 * exp(-vy / 3);
                    if (hp < 0)
                        hp = 0;
                }
                vy = (vy > -0.02 * channel->dt) ? 0 : -vy / 4.0;
            }
        }
    }

    // attack
    if (attacking)
        punchAt(punchPos);
    // place
    if (placing)
        place(punchPos);

    // move
    if (vx == 0)
        move(false);
    else
        move();

    /* update body status*/
    // update legs
    if (jump_progress > 0)
        jump_progress -= channel->dt;
    else if (jumping)
        jump_progress = 200;
    else
        jump_progress = 0;

    if (move_progress > 0)
        move_progress -= channel->dt;
    else if (moving)
        move_progress = 400;
    else
        move_progress = 0;

    // update arms
    if (attack_progress > 0)
        attack_progress -= channel->dt;
    if (place_progress > 0)
        place_progress -= channel->dt;
}

void Player::punchAt(QPointF pos)
{
    if (!canReach(pos))
        return;

    QRectF screen = QGuiApplication::primaryScreen()->geometry();

    auto z = QPointF(screen.width(), screen.height()) / 2 - pos;
    auto v = QPointF(-z.x(), z.y() + 32);
    auto vec = v / sqrt(pow(v.x(), 2.0) + pow(v.y(), 2.0));

    // check from near to far, break the nearest block
    for (auto i = 32; i <= 6 * 32; i += 32)
    {
        auto d = (position + i * vec) / 32;
        if (channel->map->inBound(QPointF(d.x(), d.y())))
        {
            if (channel->map->get(d)->get() != Block::BlockType::AIR)
            {
                channel->map->get(d)->updateState(weapon, channel->dt);
                channel->map->updateBlock(QPoint(d.x(), d.y()));
                if (attack_progress <= 0)
                    attack_progress = 200;
                return;
            }
        }
    }
}

void Player::place(QPointF pos)
{
    QRectF screen = QGuiApplication::primaryScreen()->geometry();
    auto z = (QPointF(screen.width(), screen.height()) / 2 - pos);
    auto d = (position + QPointF(-z.x(), z.y() + 32)) / 32;
    if (!channel->map->inBound(QPoint(d.x(), d.y())))
        return;
    if (canReach(pos) && channel->map->get(d)->get() == Block::BlockType::AIR && channel->map->blockNear(d))
    {
        // can not place block in the player
        if (QPoint(d.x(), d.y()) == QPoint(position.x() / 32, position.y() / 32) ||
            QPoint(d.x(), d.y()) == QPoint(position.x() / 32, position.y() / 32 + 1) ||
            QPoint(d.x(), d.y()) == QPoint(position.x() / 32, position.y() / 32 + 2))
            return;
        // place block
        channel->map->updateBlock(QPoint(d.x(), d.y()), newBlock(inventory.select(holding)));
        if (place_progress <= 0)
            place_progress = 200;
    }
}

void Player::jump(const bool &jumping)
{
    if (!isFalling() && jumping)
        vy = 2;
    this->jumping = jumping;
}

void Player::respawn()
{
    if (!isDead())
        return;
    setHP(maxHP);
    auto fa = channel->map->firstAir();
    setPosition(QPointF(fa.x() * 32 + 16, fa.y() * 32 + 16));
    dying = 0;
}

bool Player::canReach(QPointF pos) const
{
    QRectF screen = QGuiApplication::primaryScreen()->geometry();
    auto d = QPointF(screen.width(), screen.height()) / 2 - pos;
    return std::sqrt(std::pow(d.x(), 2) + std::pow(d.y(), 2)) <= 6 * 32;
}

bool Player::isInbound() const
{
    return position.x() >= 0 && position.x() < Map::MAXWIDTH && position.y() >= 0 && position.y() < Chunk::HEIGHT;
}

QRectF Player::boundingRect() const
{
    return (QRectF(0, 0, 16, 48));
}

void Player::attack(const bool &attacking)
{
    this->attacking = attacking;
}

void Player::place(const bool &placing)
{
    this->placing = placing;
}

void Player::move(const bool &moving)
{
    this->moving = moving;
}

std::string Player::store() const
{
    std::stringstream fmt;
    fmt << (int)position.x() << " " << (int)position.y() << " ";
    fmt << (int)hp << " " << (int)ay << " ";
    fmt << (int)vx << " " << (int)vy << std::endl;
    return fmt.str();
}

void Player::load(int32_t _hp, QPointF _position, double _dx, double _dy, double _ay)
{
    hp = _hp;
    position = _position;
    holding = 0;
    vx = _dx;
    vy = _dy;
    ay = _ay;
}
