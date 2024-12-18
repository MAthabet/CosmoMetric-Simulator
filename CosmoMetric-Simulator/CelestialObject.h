#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "AppCofij.h"

class CelestialObject
{
private:

	b2World* world;
	b2CircleShape circle;
public:

	b2BodyDef bodyDef;
	b2FixtureDef fixture;
	sf::CircleShape graphics;
	b2Body* body;
	std::vector<sf::Vector2f> orbitPoints;

	void setGraphics(sf::Color color);
	void updateGrapicsPos();
	void applyForce(const b2Vec2& force);
	float getMass();
	void setOrbitalVelocity(float starMass, float starX = 0, float starY = 0);
	b2Vec2 getVelocity();
	float distanceBetween(CelestialObject* other);
	CelestialObject(b2World* world, float x, float y, double mass, double radius);
	~CelestialObject();

};

