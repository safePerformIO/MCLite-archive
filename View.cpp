#include "View.hpp"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QApplication>
#include <memory>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>

#include "Map/MapGen.hpp"
#include "Map/WorldLoader.hpp"

void View::init(Scene *scene, Game *_game)
{
    setScene(scene);
    channel.scene = scene;
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    /* OpenGL */
   Viewport = new QOpenGLWidget(this);
   Viewport->setUpdateBehavior(QOpenGLWidget::UpdateBehavior::PartialUpdate);
   Viewport->setWindowFlag(Qt::WindowType::Widget);
   Viewport->setWindowState(Qt::WindowState::WindowFullScreen);
   this->setViewport(Viewport);
   this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    /* OpenGL */
    setMouseTracking(true);
    game = _game;
}

View::~View()
{
   delete Viewport;
    delete timer;
}

void View::start()
{
    timer = new QTimer(this);
    this->setCacheMode(CacheModeFlag::CacheBackground);
    update();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);
    clock.start();
    lastTime = clock.elapsed();
}

void View::setupWorld(int seed)
{
    std::unique_ptr<MapGen> mapgen(std::make_unique<MapGen>(seed));
    channel.map = std::make_unique<Map>(mapgen->generate());
    channel.world = std::make_unique<World>(&channel);
    channel.player = std::make_unique<Player>(&channel);
    channel.osd = std::make_unique<Osd>(&channel);
    channel.texture = std::make_shared<Texture>();
}

void View::setupWorld(std::string path)
{

    std::ifstream input;
    input.open(path);

    // load player
    int px, py, hp, ay, dx, dy;
    input >> px;
    input >> py;
    input >> hp;
    input >> ay;
    input >> dx;
    input >> dy;

    // load map
    channel.map = std::make_unique<Map>();
    channel.map->setChannel(&channel);
    std::stringstream fmt;
    fmt << input.rdbuf();
    channel.map->load(fmt);
    // finish io
    input.close();
    // setup channel
    channel.world = std::make_unique<World>(&channel);
    channel.player = std::make_unique<Player>(&channel);
    channel.player->load(hp, QPointF(px, py), dx, dy, ay);
    channel.osd = std::make_unique<Osd>(&channel);
    channel.texture = std::make_shared<Texture>();
}

View::View(Scene *scene, Game *game, std::string path)
{
    init(scene, game);
    setupWorld(path);
    save = QString::fromStdString(path);
    start();
}

View::View(Scene *scene, Game *game, int seed)
{
    init(scene, game);
    setupWorld(seed);
    save = QString();
    start();
}

void View::getDt()
{
    channel.dt = clock.elapsed() - lastTime;
    lastTime = clock.elapsed();
}

void View::autoSave()
{
    if (!save.isEmpty())
    {
        WorldLoader::saveWorld(channel, save.toStdString());
    }
}

void View::mousePressEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
        channel.player->attack();
        if (channel.osd->getWeaponRect(HarvestLevel::Pickaxe).contains(event->pos()))
        {
            if (channel.player->getWeapon() == HarvestLevel::Pickaxe)
                channel.player->setWeapon(HarvestLevel::Hand);
            else
                channel.player->setWeapon(HarvestLevel::Pickaxe);
        }
        else if (channel.osd->getWeaponRect(HarvestLevel::Shovel).contains(event->pos()))
        {
            if (channel.player->getWeapon() == HarvestLevel::Shovel)
                channel.player->setWeapon(HarvestLevel::Hand);
            else
                channel.player->setWeapon(HarvestLevel::Shovel);
        }
        else if (channel.osd->getWeaponRect(HarvestLevel::Sword).contains(event->pos()))
        {
            if (channel.player->getWeapon() == HarvestLevel::Sword)
                channel.player->setWeapon(HarvestLevel::Hand);
            else
                channel.player->setWeapon(HarvestLevel::Sword);
        }
        break;
    case Qt::RightButton:
        channel.player->place();
    default:
        break;
    }
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
        channel.player->attack(false);
        break;
    case Qt::RightButton:
        channel.player->place(false);
    default:
        break;
    }
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    channel.scene->updateMouse(event->pos());
    channel.player->setPunch((event->pos()));
}

void View::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key::Key_Escape:
    {
        QApplication::exit(0);
        autoSave();
        break;
    }
    case Qt::Key_F1:
    {
        QString file = QFileDialog::getSaveFileName(this, tr("Save Game"), "", tr("Game Save Files (*.txt)"));
        WorldLoader::saveWorld(channel, file.toStdString());
        break;
    }
    case Qt::Key_F2:
        channel.player->respawn();
        break;
    case Qt::Key::Key_W:
        //() << "up";
        channel.player->setDY(+1);
        break;
    case Qt::Key::Key_A:
        //() << "left";
        channel.player->setDX(-1);
        break;
    case Qt::Key::Key_S:
        //() << "down";
        channel.player->setDY(-1);
        break;
    case Qt::Key::Key_D:
        //() << "right";
        channel.player->setDX(+1);
        break;
    case Qt::Key_Space:
        channel.player->jump();
        break;
    case Qt::Key_1:
        channel.player->setHolding(0);
        break;
    case Qt::Key_2:
        channel.player->setHolding(1);
        break;
    case Qt::Key_3:
        channel.player->setHolding(2);
        break;
    case Qt::Key_4:
        channel.player->setHolding(3);
        break;
    case Qt::Key_5:
        channel.player->setHolding(4);
        break;
    case Qt::Key_6:
        channel.player->setHolding(5);
        break;
    case Qt::Key_7:
        channel.player->setHolding(6);
        break;
    case Qt::Key_8:
    {
        if (channel.player->getWeapon() == HarvestLevel::Pickaxe)
            channel.player->setWeapon((HarvestLevel::Hand));
        else
            channel.player->setWeapon(HarvestLevel::Pickaxe);
        break;
    }
    case Qt::Key_9:
    {
        if (channel.player->getWeapon() == HarvestLevel::Shovel)
            channel.player->setWeapon((HarvestLevel::Hand));
        else
            channel.player->setWeapon(HarvestLevel::Shovel);
        break;
    }
    case Qt::Key_0:
    {
        if (channel.player->getWeapon() == HarvestLevel::Sword)
            channel.player->setWeapon((HarvestLevel::Hand));
        else
            channel.player->setWeapon(HarvestLevel::Sword);
        break;
    }
    default:
        break;
    }
}

void View::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key::Key_W:
        if (channel.player->getDY() != 0)
            channel.player->setDY(0);
        break;
    case Qt::Key::Key_A:
        if (channel.player->getDX() != 0)
            channel.player->setDX(0);
        break;
    case Qt::Key::Key_S:
        if (channel.player->getDY() != 0)
            channel.player->setDY(0);
        break;
    case Qt::Key::Key_D:
        if (channel.player->getDX() != 0)
            channel.player->setDX(0);
        break;
    case Qt::Key::Key_Space:
        channel.player->jump(false);
        break;
    default:
        break;
    }
}

void View::update()
{
    getDt();
    channel.scene->update();
}
