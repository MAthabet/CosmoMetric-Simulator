#include "CelestialObject.h"

b2Vec2 calcGForce(const b2Body& body1, const b2Body& body2);
sf::Color randColor();

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Solar System");
    window.setFramerateLimit(60);

    srand(time(0));
    // Creating world
    b2World world(b2Vec2(0, 0));

    CelestialObject star(&world, 0, 0, StarMass, StarRadius);
    star.setGraphics(sf::Color::Yellow);
    star.updateGrapicsPos();

    std::vector<CelestialObject*> planets;
    int size = 0;

    bool applayForceBetweenPlanets = true;
    // Run the program as long as the window is open
    while (window.isOpen())
    {
        world.Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
        // Check for all window events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mousPos = sf::Mouse::getPosition(window);
                // random radius causes errors in box2d body (inf dens)
                // so used this to make radius more realistic
                float radius = 0;
                while (radius < StarRadius-10) //no reason for this 10 i just feel it :D
                {
                    float mass = rand() % int(((MAX_MASS - MIN_MASS + 1) + MIN_MASS)) * 1e24;
                    float radius = sqrt((AVG_PLANET_DENS / mass) / PI);
                }
                
                CelestialObject* planet = new CelestialObject(&world, mousPos.x - WINDOW_W/2, WINDOW_H / 2 - mousPos.y , mass, radius);
                planet->setOrbitalVelocity(star.getMass());
                planet->setGraphics(randColor());
                planets.push_back(planet);
                size++; //to fasten the proccess a bit
            }
        }
        
        for (int i = 0; i < size; i++)
        {
            b2Vec2 force;
            CelestialObject* temp = planets[i];
            force = calcGForce(*temp->body, *star.body);
            if(applayForceBetweenPlanets)
            for (int j = 0; j < size; j++)
            {
                if (planets[j] == temp)
                    continue;
                force += calcGForce(*temp->body, *planets[j]->body);
            }
            temp->applyForce(force);
            temp->updateGrapicsPos();
        }
                
        window.clear();
        //orbit trail
        for (int i = 0; i < size; i++)
        {
            CelestialObject* temp = planets[i];
            temp->orbitPoints.push_back(temp->graphics.getPosition());
            sf::VertexArray orbit(sf::LineStrip, temp->orbitPoints.size());
            for (int j = 0; j < temp->orbitPoints.size(); j++)
            {
                orbit[j].position = temp->orbitPoints[j];
                sf::Color trailColor = temp->graphics.getFillColor();
                orbit[j].color = sf::Color(trailColor.r, trailColor.g, trailColor.b, 100);
            }
            window.draw(orbit);
        }
        for (int i = 0; i < size; i++)
        {
            window.draw(planets[i]->graphics);
        }

        window.draw(star.graphics);
        window.display();
    }

    return 0;
}

b2Vec2 calcGForce(const b2Body& body1, const b2Body& body2)
{
    b2Vec2 pos1 = body1.GetPosition();
    b2Vec2 pos2 = body2.GetPosition();
    float dx = (pos2.x - pos1.x);
    float dy = (pos2.y - pos1.y);

    float distance = std::sqrt(dx * dx + dy * dy);

    float forceMag = 0;
    if (distance != 0)
        forceMag = G * body1.GetMass() / distance * body2.GetMass() / distance;

    b2Vec2 force = forceMag * b2Vec2(dx / distance, dy / distance);

    return force;
};
sf::Color randColor()
{
    return sf::Color(rand()%255, rand() % 255, rand() % 255);
}