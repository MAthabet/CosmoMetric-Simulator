#include "CelestialObject.h"
#include <iomanip>
#include <sstream>

b2Vec2 calcGForce(const b2Body& body1, const b2Body& body2);
sf::Color randColor();

int main()
{
    srand(time(0));
    std::ostringstream oss;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Solar System", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    sf::View view(sf::FloatRect(0.f, 0.f, VIEW_W, VIEW_H));
    view.zoom(0.05);
    window.setView(view);

    // Creating world with gravity 0
    b2World world(b2Vec2(0, 0));
    // Star init
    CelestialObject star(&world, 0, 0, StarMass, StarRadius);
    star.setGraphics(sf::Color::Yellow);
    star.updateGrapicsPos();
    b2Fixture* fixture = star.body->GetFixtureList();
    fixture->SetSensor(true);
    std::vector<CelestialObject*> planets;
    int size = 0;

    bool applayForceBetweenPlanets = false;
    
    sf::Font font;
    if (!font.loadFromFile("./resources/rfont.ttf"))
    {
        printf("can not load font");
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);
    text.setPosition(0, 0);

    float radius = 0;
    float mass = 0;
    bool drag = false;
    // Run the program as long as the window is open
    while (window.isOpen())
    {
        // Check for all window events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                window.setView(view);
                sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                // random radius causes errors in box2d body (inf dens)
                // so used this to make radius more realistic
                do 
                {
                    mass = rand() % int(((MAX_MASS - MIN_MASS + 1) + MIN_MASS));
                    radius = sqrt((AVG_PLANET_DENS / mass) / PI) * R_SCALE;
                } while (radius > StarRadius - 10); //no reason for this 10 i just feel it :D
                
                CelestialObject* planet = new CelestialObject(&world, pos.x - VIEW_W/2, VIEW_H/2 - pos.y , mass * 1e24, radius);
                planet->setOrbitalVelocity(star.getMass());
                planet->setGraphics(randColor());
                planets.push_back(planet);
                size++; //to fasten the proccess a bit
            }
            else if (event.type == sf::Event::MouseWheelMoved) {
                if (event.mouseWheel.delta == 1)
                    view.zoom(0.9f);
                else
                    view.zoom(1.1f);
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    view.move(VIEW_delta, 0);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    view.move(-VIEW_delta, 0);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    view.move(0 , -VIEW_delta);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    view.move(0 , VIEW_delta);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                {
                    view.zoom(0.05);
                }

            }
        }
        b2Vec2 a;
        if(size)
         a = planets[0]->body->GetLinearVelocity();
        world.Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
        for (int i = 0; i < size; i++)
        {
            CelestialObject* temp = planets[i]; 
            b2Vec2 force = calcGForce(*temp->body, *star.body);
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
        if (size)
        a -= planets[0]->body->GetLinearVelocity();
        printf("x = %f\n y= %f\n----------\n",a.x,a.y);
        window.clear();
        window.setView(view);
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
        // detect if swallowed by sun
        for (int i = 0; i < size; i++)
        {
            float r = star.graphics.getRadius() - planets[i]->graphics.getRadius();
            if (planets[i]->distanceBetween(&star) < r)
            {
                delete planets[i];
                planets.erase(planets.begin() + i);
                size--;
                i--;
                printf("Planet Vanished\n");
            }
        }
        for (int i = 0; i < size; i++)
        {
            window.draw(planets[i]->graphics);
        }
        //printing data
        window.draw(star.graphics);
        window.setView(window.getDefaultView());
        if (size)
            {
                // to make texts more redable
                oss << std::fixed << std::setprecision(2) << planets[size - 1]->getVelocity().x;
                std::string vX = oss.str();
                oss.str("");
                oss.clear();
                oss << std::fixed << std::setprecision(2) << planets[size - 1]->getVelocity().y;
                std::string vY = oss.str();
                oss.str("");
                oss.clear();
                oss << std::fixed << std::setprecision(2) << radius;
                std::string r = oss.str();
                oss.str("");
                oss.clear();

                text.setString("Orbital Velocity: " + vX + " , " + vY
                   + "\n Mass: " + std::to_string(int(planets[size - 1]->getMass()/1e24)) + "e24 kg"
                   + "\n Radius: " + r + "e7 m");
                window.draw(text);
            }
        window.display();
    }

    return 0;
}

b2Vec2 calcGForce(const b2Body& body1, const b2Body& body2)
{
    b2Vec2 pos1 = body1.GetPosition();
    b2Vec2 pos2 = body2.GetPosition();
    float dx = SCALE * (pos2.x - pos1.x);
    float dy = SCALE * (pos2.y - pos1.y);

    float distance = std::sqrt(dx * dx + dy * dy);

    float forceMag = 0;
    
    if (distance != 0)
        forceMag = G * (body1.GetMass()/ distance) * (body2.GetMass() / distance);

    b2Vec2 force = forceMag * b2Vec2(dx / distance, dy / distance);

    return force;
};
sf::Color randColor()
{
    return sf::Color(rand()%255, rand() % 255, rand() % 255);
}