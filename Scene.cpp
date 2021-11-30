#include "Scene.hpp"
#include <math.h>

Scene::Scene(QRectF screen) : screen(screen) {}

Scene::~Scene() {}

void Scene::updateMouse(const QPointF &mouse)
{
    this->mouse = mouse;
}

QPointF Scene::getMouse() const
{
    return mouse;
}

float Scene::getAngle() const
{
    return -atan2(mouse.x() - screen.width() / 2.0, mouse.y() - screen.height() / 2.0) * 180.0 / 3.141592654;
}
