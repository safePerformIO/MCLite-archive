#include "Game.hpp"
#include "ui_Game.h"
#include "QTimer"
#include "View.hpp"
#include <QDesktopWidget>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QGuiApplication>
#include <QScreen>
#include <random>
#include <ctime>

Game::Game(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Game),
      menuBkgnd(":/backGround/res/menuBGI.jpg")
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    ui->setupUi(this);
    this->setGeometry(QGuiApplication::primaryScreen()->geometry().width()/2 - QGuiApplication::primaryScreen()->geometry().width() / 1920.0 * 600 / 2 , QGuiApplication::primaryScreen()->geometry().height()/2 - QGuiApplication::primaryScreen()->geometry().width() / 1920.0 * 800 / 2, QGuiApplication::primaryScreen()->geometry().width() / 1920.0 * 600, QGuiApplication::primaryScreen()->geometry().width() / 1920.0 * 800);
    QPixmap bkgnd2 = menuBkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd2);
    this->setPalette(palette);

    ui->label->setStyleSheet("font: 88pt \"Comic Sans MS\";"
                             "color:red;");
}

void Game::on_btn_start_game_clicked()
{
    bool ok = false;
    // pre select a random seed
    std::mt19937 rng;
    rng.seed(time(NULL));
    std::uniform_int_distribution<std::mt19937::result_type> dist(-32767, 32767);
    // ask player to input seed for map
    int seed = QInputDialog::getInt(this, tr("Input seed"), tr("seed"), dist(rng), -32767, 32767, 1, &ok);
    if (ok)
    {
        start_game(seed);
    }
}

void Game::on_btn_load_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Open Save File"), "", tr("Game Save Files (*.txt)"));
    if (fileName.isEmpty())
    {
        QMessageBox::information(this, tr("Path"), tr("You didn't select any files."));
        return;
    }
    else
    {
        start_game(fileName.toStdString());
    }
}

void Game::on_btn_exit_clicked()
{
    QApplication::exit(0);
}

void Game::start_game(int seed)
{
    QRect screen = QGuiApplication::primaryScreen()->geometry();
    scene = new Scene(screen);
    scene->setSceneRect(0, 0, screen.width(), screen.height());
    View *view = new View(scene, this, seed);
    QTimer::singleShot(500, this, SLOT(showFullScreen()));
    setWindowFlags(Qt::Window);

    this->setCentralWidget(view);
}

void Game::start_game(std::string path)
{
    QRect screen = QGuiApplication::primaryScreen()->geometry();
    scene = new Scene(screen);
    scene->setSceneRect(0, 0, screen.width(), screen.height());
    View *view = new View(scene, this, path);
    QTimer::singleShot(500, this, SLOT(showFullScreen()));
    setWindowFlags(Qt::Window);

    this->setCentralWidget(view);
}

Game::~Game()
{
    delete ui;
}
