/**
 * @file View.hpp
 * @author H14
 * @brief the main view, recieve user input, initialize all game components
 * @version 0.1
 * @date 2021-11-30
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <QGraphicsView>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>
#include "Game.hpp"
#include "Channel.hpp"

/**
 * @brief the view shows everything player can see on the screen, and also collectes inputs from player
 *
 *  The view will be used by the game
 *  it will render things in camera
 *  and pass user input to world
 */
class View : public QGraphicsView
{
    Q_OBJECT
private:
    Channel channel; // the view owns the channel
    Game *game;
    QOpenGLWidget *Viewport;
    QTime clock;
    QTimer *timer;
    long long lastTime;
    // world save for autosave
    QString save;

    void getDt();

    // helper function to init the view
    void init(Scene *scene, Game *game);
    // start the whole game
    void start();

    /**
     * @brief create a new world with user input seed
     *
     * @param seed
     */
    void setupWorld(int seed);

    /**
     * @brief load saved game from selected file
     *
     * @param path
     */
    void setupWorld(std::string path);

    // auto save the world
    void autoSave();

public:
    // constructor from path to save
    View(Scene *scene, Game *game, std::string path);
    // constructor from map generation seed
    View(Scene *scene, Game *game, int seed);
    ~View();

public slots:
    /**
     * @brief will be called once per frame
     *
     */
    void update();

protected:
    /**
     * @brief functions to handle user input
     *
     * @param event
     */
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};
