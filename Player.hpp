/**
 * @file Player.hpp
 * @author H14
 * @brief the player class
 * @version 0.1
 * @date 2021-11-30
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include "Texture/IHasModel.hpp"
#include <QGraphicsItem>
#include <QObject>
#include <vector>
#include "Block/Block.hpp"
#include "Texture/IHasModel.hpp"

struct Channel;

// Inventory holds the blocks that the player can place
struct Inventory
{
    std::vector<std::string> items;
    Inventory();
    // select items from inventory
    std::string select(size_t) const;
};

/* The player */
class Player : public QObject, public QGraphicsItem, virtual public IHasModel
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    static constexpr QRect rect = QRect(0, 0, 32, 64); // takes up 1x2 blocks
    static constexpr int32_t range = 4;                // how far the player can reach
    static constexpr int32_t maxHP = 100;              // max hp of player
    static constexpr size_t hotbarSize = 9;            // length of the hotbar

private:
    const QImage model; // model of the player
    const QImage GGimage_0, GGimage_1;
    Channel *channel;

    QRectF Head_L_i, Head_R_i, Head_F_i, Head;
    QRectF Body_F_i, Body_L_i, Body_R_i, Body_F, Body_L;
    QRectF Arm_i;
    QRectF Leg_L_i, Leg_R_i, Leg_L_s, Leg_R_s, Leg_L, Leg_R;
    double offset; // adapt to different screen

    int32_t hp = maxHP; // hp of the player
    QPointF position;   // player position
    size_t holding;     // item the player is holding
    // the inventiry, we have infinite items
    Inventory inventory = Inventory(); // inventory of the player
    // motion related
    double vx;
    double vy, ay;
    double fallTime;

    int distanceToGround;
    int distanceToCeiling;

    // state related

    int attack_progress = 0;
    bool attacking = false;

    int place_progress = 0;
    bool placing = false;

    int move_progress = 0;
    bool moving = false;

    int jump_progress = 0;
    bool jumping = false;

    int dying = 0;
    bool dead = false;

    QPointF punchPos;
    HarvestLevel weapon;

    /**
     * @brief Get the Block By Pos object
     *
     * @param dp block position
     * @return const Block::Ptr&
     */
    const Block::Ptr &getBlockByPos(QPointF dp) const;

public:
    // default constructor with channel init
    Player(Channel *channel);
    ~Player() = default;

    // update player status: position, hp, speed, item holding, pick item
    // evoked by World
    void update();

    // render the player
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    virtual const QImage *acquireModel(Texture::Ptr texture) const override;

    // player actions
    void attack(const bool &attacking = true);
    void place(const bool &placing = true);
    void move(const bool &moving = true);
    void jump(const bool &jumping = true); // jump

    // store data to save
    std::string store() const;
    // load data from save
    void load(int32_t _hp, QPointF _position, double _dx, double _dy, double _ay);

    /**
     * @brief break block/attack, return true on success
     *
     * @param pos position
     */
    void punchAt(QPointF pos);
    /**
     * @brief player place a block, evoke updateBlock in Map
     *
     * @param pos position
     */
    void place(QPointF pos);
    /**
     * @brief respawn the player when dead
     *
     */
    void respawn();

    /**
     * @brief  can reach block
     *
     * @param pos position
     * @return true can reach
     * @return false cannot
     */
    bool canReach(QPointF pos) const;
    /**
     * @brief Get the Position object
     *
     * @return QPointF
     */
    QPointF getPosition() const { return position; }
    /**
     * @brief if player is in the world
     *
     * @return true in
     * @return false not in
     */
    bool isInbound() const;
    /**
     * @brief if player is dead
     *
     * @return true dead
     * @return false live
     */
    bool isDead() const;
    /**
     * @brief draw image when player is dead
     *
     * @param painter
     */
    void die(QPainter *painter);
    /**
     * @brief if the player is falling
     *
     * @return true is falling
     * @return false not falling
     */
    bool isFalling() const { return distanceToGround > 0; }

    // set channel
    void setChannel(Channel *c);
    // set dx,dy
    void setDX(int32_t vx) { this->vx = vx; }
    void setDY(int32_t vy) { this->vy = vy; }
    // set hp
    void setHP(int32_t h) { hp = h; }
    // set item holding, via inventory index. 0 <= i <= 8
    void setHolding(int i) { holding = i; }
    // get item holding
    int getHolding() const { return holding; }
    // set pos
    void setPosition(QPointF pos) { position = pos; }
    // set punch position
    void setPunch(QPointF p) { punchPos = p; }
    // set weapon
    void setWeapon(HarvestLevel h) { weapon = h; }

    // getter functions
    int32_t getDX() const { return vx; }
    int32_t x() const { return position.x(); }
    int32_t mx() const { return position.x() / 32; }
    int32_t getDY() const { return vy; }
    int32_t y() const { return position.y(); }
    int32_t my() const { return position.y() / 32; }
    HarvestLevel getWeapon() const { return weapon; }
    int32_t getHP() const { return hp; }
    const Inventory &getInventory() const { return inventory; }
};
