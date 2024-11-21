#include "CelestialObject.h"

void CelestialObject::setGraphics(sf::Color color)
{
    graphics.setFillColor(color);
    graphics.setOrigin(circle.m_radius, circle.m_radius);
}

void CelestialObject::updateGrapicsPos()
{
    graphics.setPosition(WINDOW_W / 2 + body->GetPosition().x, WINDOW_H / 2 - body->GetPosition().y);
}

void CelestialObject::applyForce(const b2Vec2& force)
{
    body->ApplyForceToCenter(force, true);
}

float CelestialObject::getMass()
{
    return body->GetMass();
}

b2Vec2 CelestialObject::getVelocity()
{
    return body->GetLinearVelocity();
}

float CelestialObject::distanceBetween(CelestialObject* other)
{
    b2Vec2 dis = this->body->GetPosition() - other->body->GetPosition();
    return std::sqrt(dis.x * dis.x + dis.y * dis.y);
}

void CelestialObject::setOrbitalVelocity(float starMass, float starX, float starY)
{

    double x = body->GetPosition().x - starX;
    double y = body->GetPosition().y - starY;

    double r = std::sqrt(x * x + y * y);

    float orbitalSpeed = std::sqrt(G * starMass / r);

    // Set linear velocity in the tangential direction to the orbit
    body->SetLinearVelocity(b2Vec2(-y / r * orbitalSpeed, x / r * orbitalSpeed));
}


CelestialObject::CelestialObject(b2World* world, float x, float y, double mass, double radius)
{
    this->world = world;

    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    body = world->CreateBody(&bodyDef);



    circle.m_radius = radius;
    circle.m_p.Set(0, 0);
    fixture.shape = &circle;

    double area = PI * radius * radius;
    fixture.density = float(mass / area);

    body->CreateFixture(&fixture);
    body->ResetMassData();
    graphics.setRadius(radius);
}

CelestialObject::~CelestialObject()
{
    world->DestroyBody(body);
};
