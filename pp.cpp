#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <math.h>
#include <iostream>

float getDistance(float x1, float y1, float x2, float y2);

struct balls
{
    sf::CircleShape ball;
    float px, py;
    float vx, vy;
    float ax, ay;
    float radius;
    float mass = 10.0f;
    float speedB = 10.0f;  // Constant speed for simplicity
    //Color
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;
    sf::Color ballColor = sf::Color(r,g,b);
    int id;

    balls(float iRad, float posX, float posY, float velX, float velY, int ballId)
    {
        ball.setRadius(iRad);
        ball.setFillColor(ballColor);
        px = posX;
        py = posY;
        vx = velX;
        vy = velY;
        radius = iRad;
        id = ballId;
    }

    void Update(float time)
    {
        // Apply gravity
        ay += 20.8; // Assuming gravity is a constant (you can adjust the value)

        // Update velocities based on acceleration
        vx += ax * time;
        vy += ay * time;

        // Introduce linear damping factor
        float linearDamping = 0.0001f;  // Adjust the value as needed
        vx *= (1.0f - linearDamping);
        vy *= (1.0f - linearDamping);

        // Update positions based on velocities
        px += vx * time * speedB;
        py += vy * time * speedB;

        // Constraint to keep the ball within the visible area
        if (px - radius <= -10)
        {
            px = radius;
            vx = -vx; // Reverse velocity on collision with the left wall
        }
        else if (px + radius >= 980)
        {
            px = 980 - radius;
            vx = -vx; // Reverse velocity on collision with the right wall
        }

        if (py - radius <= 0)
        {
            py = radius;
            vy = -vy; // Reverse velocity on collision with the top wall
        }
        else if (py + radius >= 980)
        {
            py = 980 - radius;
            vy = -vy; // Reverse velocity on collision with the bottom wall
        }

        // Stop the motion if the velocity becomes very small
        if (std::abs(vx) < 0.1 && std::abs(vy) < 0.1)
        {
            vx = 0.0;
            vy = 0.0;
        }

        // Reset acceleration for the next frame
        ax = 0.0f;
        ay = 0.0f;

        // Update the ball's position
        ball.setPosition(px, py);
    }

    /*void CheckWallCols(float px, float py, float &vx, float &vy, float radius)
    {
        if (px - radius <= 0 - radius || px + radius >= 1000 - radius)
        {
            vx = -vx;
        }
        if (py - radius <= 0 - radius || py + radius >= 1000 - radius)
        {
            vy = -vy;
            // Adjust the ball's position to stay within the visible area
            if (py - radius <= 0 - radius)
            {
                py = radius;
            }
            else if (py + radius >= 1000 - radius)
            {
                py = 1000 - radius;
            }
        }
    }*/

    void CheckBallCollision(balls &other)
    {
        float distance = getDistance(px, py, other.px, other.py);
        float minDistance = radius + other.radius;

        if (distance <= minDistance)
        {
            // Balls are colliding, handle the collision
            // For example, you can reverse their velocities
            float angle = atan2(py - other.py, px - other.px);
            float overlap = minDistance - distance;

            px += cos(angle) * overlap / 2;
            py += sin(angle) * overlap / 2;
            other.px -= cos(angle) * overlap / 2;
            other.py -= sin(angle) * overlap / 2;

            float angleNormal = atan2(py - other.py, px - other.px);
            float magnitude1 = sqrt(vx * vx + vy * vy);
            float magnitude2 = sqrt(other.vx * other.vx + other.vy * other.vy);

            float newVelX1 = magnitude2 * cos(angleNormal);
            float newVelY1 = magnitude2 * sin(angleNormal);
            float newVelX2 = magnitude1 * cos(angleNormal + M_PI);
            float newVelY2 = magnitude1 * sin(angleNormal + M_PI);

            vx = newVelX1;
            vy = newVelY1;
            other.vx = newVelX2;
            other.vy = newVelY2;
        }
    }
};

float getDistance(float x1, float y1, float x2, float y2)
{
    float xDis = x2 - x1;
    float yDis = y2 - y1;
    return sqrtf(pow(xDis, 2) + pow(yDis, 2));
};

int main()
{
    srand(time(NULL));
    int ballsCount = 500;
    std::vector<balls> BallVec;

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SORT!"); // creating window
    sf::Font font;
    if (!font.loadFromFile("G:\\abdullah\\Abdullah(Dont Delete Any thing)\\FONTS\\Rubik_Mono_One\\RMOR.ttf"))
    {
        std::cout << "Can't load/find the file" << '\n';
    }
    for (int i = 0; i < ballsCount; i++)
    {
        float ranPosX = rand() % (1000 - 2 * 10) + 10; // Adjust 10 for the radius
        float ranPosY = rand() % (1000 - 2 * 10) + 10; // Adjust 10 for the radius
        BallVec.push_back(balls(10.0f, ranPosX, ranPosY, 20.0f, 2.5f, i + 1));
    }

    // Apply an initial force in the x-direction to the first ball
    float initialForceX = 100.0f; // Adjust the force value as needed
    BallVec[0].ax += initialForceX / BallVec[0].mass; // F = ma, so a = F/m

    sf::Clock clock;
    sf::Clock fpsClock;
    int fpsCounter = 0;
    
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds() * 2; // Get the elapsed time since the last frame
        fpsCounter++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0)
        {
            std::cout << "FPS: " << fpsCounter << std::endl;
            fpsCounter = 0;
            fpsClock.restart();
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                std::cout << "Clicking\n";
            }
        }
        for (auto &ball : BallVec)
        {
            //ball.CheckWallCols(ball.px, ball.py, ball.vx, ball.vy, ball.radius);
            ball.Update(deltaTime);
            for (auto &target : BallVec)
            {
                if (&ball != &target) // Avoid comparing a ball to itself
                {
                    ball.CheckBallCollision(target);
                }
            }
        }
        window.clear(); // clearing window before using

        for (auto &ball : BallVec)
        {
            window.draw(ball.ball);
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        window.display(); // displaying everything
    }

    return 0;
}