/**
 * @file Game.hpp
 * @author H14
 * @brief main window of the game, will be replaced by view after game started
 * @version 0.1
 * @date 2021-11-30
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once

#include <QMainWindow>
#include <QWidget>
#include "Scene.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Game;
}
QT_END_NAMESPACE
class View;
class Game : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Game object
     *
     * @param parent
     */
    Game(QWidget *parent = nullptr);
    ~Game();

private slots:
    /**
     * @brief when start_game button is clicked, the game will be created and starts
     *
     */
    void on_btn_start_game_clicked();

    /**
     * @brief terminate the program
     *
     */
    void on_btn_exit_clicked();

    /**
     * @brief load saved game
     *
     */
    void on_btn_load_clicked();

private:
    Ui::Game *ui; // the main ui of the menu

    /**
     * @brief when game started, it will create scene and view
     *
     */
    Scene *scene = nullptr;
    View *view = nullptr;

    QPixmap menuBkgnd;                 // the image for menu background
    void start_game(int seed);         // start a new game with player input seed
    void start_game(std::string path); // load saved game
};
