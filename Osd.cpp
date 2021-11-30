#include "Osd.hpp"
#include "Channel.hpp"
#include <QString>

Osd::Osd(Channel *channel) : channel(channel),                                                                                                                      // load channel
                             Weapon_pickaxe(":/tool/res/iron_pickaxe.png"), Weapon_shovel(":/tool/res/iron_shovel.png"), Weapon_sword(":/tool/res/iron_sword.png"), // load weapons
                             weapon_HUD(":/tool/res/weapon_HUD.png"), item_HUD(":/blocks/res/item_HUD.png"), FPS(0), sF(0), sT(0)
{
    double offset = channel->scene->screen.width() / 1920.0;

    // load weapon coordinates
    Weapon_pickaxe_i = QRectF(channel->scene->screen.width() * 0.8 + 8 * offset, channel->scene->screen.height() * 0.052 + 8 * offset, 64 * offset, 64 * offset);
    Weapon_pickaxe_b = QRectF(channel->scene->screen.width() * 0.8, channel->scene->screen.height() * 0.052, 80 * offset, 80 * offset);
    Weapon_shovel_i = QRectF(channel->scene->screen.width() * 0.86 + 8 * offset, channel->scene->screen.height() * 0.052 + 8 * offset, 64 * offset, 64 * offset);
    Weapon_shovel_b = QRectF(channel->scene->screen.width() * 0.86, channel->scene->screen.height() * 0.052, 80 * offset, 80 * offset);
    Weapon_sword_i = QRectF(channel->scene->screen.width() * 0.92 + 8 * offset, channel->scene->screen.height() * 0.052 + 8 * offset, 64 * offset, 64 * offset);
    Weapon_sword_b = QRectF(channel->scene->screen.width() * 0.92, channel->scene->screen.height() * 0.052, 80 * offset, 80 * offset);
    Weapon_1 = QRectF(0, 0, 336, 336);
    Weapon_2 = QRectF(493, 0, 336, 336);
    Weapon_3 = QRectF(986, 0, 336, 336);
    Weapon_1_b = QRectF(channel->scene->screen.width() * 0.8 - 8 * offset, channel->scene->screen.height() * 0.052 - 8 * offset, 96 * offset, 96 * offset);
    Weapon_2_b = QRectF(channel->scene->screen.width() * 0.86 - 8 * offset, channel->scene->screen.height() * 0.052 - 8 * offset, 96 * offset, 96 * offset);
    Weapon_3_b = QRectF(channel->scene->screen.width() * 0.92 - 8 * offset, channel->scene->screen.height() * 0.052 - 8 * offset, 96 * offset, 96 * offset);

    // load hp bar coordinates
    hpBar = QRectF(channel->scene->screen.width() * 0.025, channel->scene->screen.height() * 0.056, channel->scene->screen.width() * 0.7, channel->scene->screen.height() * 0.05);

    // load text for FPS
    FPSf.setFamily("times");
    FPSf.setPointSize(18);

    // add to scene
    channel->scene->addItem(this);
}

Osd::~Osd() = default;

QRectF Osd::boundingRect() const
{
    return QRectF(0, 0, channel->scene->screen.width(), channel->scene->screen.height());
}

void Osd::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    update();
    // paint hp bar
    painter->save();
    painter->fillRect(hpBar, Qt::white);
    if (hurt_process <= 400)
    {
        if (hurt_process != 0)
        {
            painter->setOpacity(0.5);
            painter->fillRect(QRectF(hpBar.x(), hpBar.y(), ((hp + (pre_hp - hp) * (hurt_process / 400.0)) / (double)channel->player->maxHP) * hpBar.width(), hpBar.height()), Qt::red);
        }
        painter->setOpacity(1.0);
        painter->fillRect(QRectF(hpBar.x(), hpBar.y(), (hp / (double)channel->player->maxHP) * hpBar.width(), hpBar.height()), Qt::red);
    }
    painter->restore();

    // paint weapons
    painter->save();
    painter->setOpacity(0.8);
    if (channel->player->getWeapon() == HarvestLevel::Pickaxe)
        painter->fillRect(Weapon_pickaxe_b, Qt::white);
    else if (Weapon_1_b.contains(channel->scene->getMouse()))
        painter->fillRect(Weapon_pickaxe_b, Qt::gray);
    else
        painter->fillRect(Weapon_pickaxe_b, Qt::black);

    if (channel->player->getWeapon() == HarvestLevel::Shovel)
        painter->fillRect(Weapon_shovel_b, Qt::white);
    else if (Weapon_2_b.contains(channel->scene->getMouse()))
        painter->fillRect(Weapon_shovel_b, Qt::gray);
    else
        painter->fillRect(Weapon_shovel_b, Qt::black);

    if (channel->player->getWeapon() == HarvestLevel::Sword)
        painter->fillRect(Weapon_sword_b, Qt::white);
    else if (Weapon_3_b.contains(channel->scene->getMouse()))
        painter->fillRect(Weapon_sword_b, Qt::gray);
    else
        painter->fillRect(Weapon_sword_b, Qt::black);

    painter->drawImage(Weapon_1_b, weapon_HUD, Weapon_1);
    painter->drawImage(Weapon_2_b, weapon_HUD, Weapon_2);
    painter->drawImage(Weapon_3_b, weapon_HUD, Weapon_3);
    painter->restore();

    painter->save();
    if (channel->player->getWeapon() != HarvestLevel::Pickaxe)
        painter->setOpacity(0.3);
    painter->drawImage(Weapon_pickaxe_i, Weapon_pickaxe);
    painter->restore();

    painter->save();
    if (channel->player->getWeapon() != HarvestLevel::Shovel)
        painter->setOpacity(0.3);
    painter->drawImage(Weapon_shovel_i, Weapon_shovel);
    painter->restore();

    painter->save();
    if (channel->player->getWeapon() != HarvestLevel::Sword)
        painter->setOpacity(0.3);
    painter->drawImage(Weapon_sword_i, Weapon_sword);
    painter->restore();

    // paint items
    painter->save();
    for (int i = 0; i < 10; i++)
    {
        int size_i = 64, size_b = 80;
        painter->setOpacity(0.9);
        if (i == channel->player->getHolding())
        {
            size_i = 80, size_b = 100;
            painter->setOpacity(1.0);
        }
        auto item = channel->texture->getTexture(channel->player->getInventory().select(i));
        if (item)
            painter->drawImage(QRectF(channel->scene->screen.width() * (0.5 + (-490 + 100 * i + 8 - (size_i - 64) / 2) / 1960.0), channel->scene->screen.height() * 0.88 + channel->scene->screen.width() / 1960.0 * (8 - (size_i - 64) / 2), channel->scene->screen.width() / 1920.0 * size_i, channel->scene->screen.width() / 1920.0 * size_i),
                               *item);
        painter->drawImage(QRectF(channel->scene->screen.width() * (0.5 + (-490 + 100 * i - (size_b - 80) / 2) / 1960.0), channel->scene->screen.height() * 0.88 - channel->scene->screen.width() / 1960.0 * (size_b - 80) / 2, channel->scene->screen.width() / 1920.0 * size_b, channel->scene->screen.width() / 1920.0 * size_b), item_HUD);
    }

    painter->restore();

    // paint FPS
    painter->save();
    painter->setFont(FPSf);
    painter->setPen(Qt::blue);
    painter->drawText(20, 40, "FPS: " + QString::number(FPS));
    painter->restore();

    //    painter->endNativePainting();
}

bool m = false;
void Osd::update()
{
    getFPS();
    last_hp = hp;
    hp = channel->player->getHP();

    if (hp != last_hp)
    {
        hurt_process = 450;
        pre_hp = last_hp;
    }
    else if (hurt_process > 0)
        hurt_process -= channel->dt;
    else
    {
        hurt_process = 0;
        pre_hp = hp;
    }
}

void Osd::getFPS()
{
    if (sT >= 1000)
    {
        FPS = sF;
        sT = 0;
        sF = 1;
    }
    else
    {
        sT += channel->dt;
        sF++;
    }
}

QRectF Osd::getWeaponRect(const HarvestLevel &weapon) const
{
    if (weapon == HarvestLevel::Pickaxe)
        return Weapon_1_b;
    else if (weapon == HarvestLevel::Shovel)
        return Weapon_2_b;
    else if (weapon == HarvestLevel::Sword)
        return Weapon_3_b;

    return Weapon_3_b;
}
