#include "iostream"
#include <future>
#include <time.h>
#include "SFML/Graphics.hpp"
#include "PerlinNoise/PerlinNoise.hpp"

double MAX_CIRCLE_SIZE = 2;
double MIN_CIRCLE_SIZE = 2;

struct Noise{
  sf::Vector2f pos;
  double val;
  sf::Color color;
  Noise (sf::Vector2f pos, double val){
    this->pos = pos;
    this->val = val;
    color = sf::Color::White * sf::Color{(unsigned char)val, (unsigned char)val, (unsigned char)val};
  }
};

struct Circle{
  sf::Vector2f pos;
  double radius;
  sf::CircleShape circle_shape;
  Circle(sf::Vector2f pos, double radius){
    this->pos = pos;
    this->radius = radius;
    circle_shape.setRadius(radius*10);
    circle_shape.setPosition(pos*10.f);
    circle_shape.setFillColor(sf::Color::Cyan);
  }
  double dist_sq(sf::Vector2f position){
    return pow(position.x - this->pos.x, 2) + pow(position.y - this->pos.y, 2);
  }
};

std::vector<Noise> generate(float x_offset, float y_offset){
  std::vector<Noise> noise;
  float scale = 0.005;
  static siv::PerlinNoise::seed_type seed = rand();

  static const siv::PerlinNoise perlin{ seed };
  for (float y=0; y<80; y++){
    for (float x=0; x<80; x++){
      noise.push_back({{x, y}, perlin.octave2D_01((x + x_offset) * scale, (y + y_offset) * scale, 8) * 255});
    }
  }
  return noise;
}

int main(){
  sf::RenderWindow win{{800, 800}, "RPG"};
  win.setFramerateLimit(60);

  srand(time(NULL));
  
  float x_offset, y_offset;
  const float step = 100;

  std::vector<Noise> noise = generate(x_offset, y_offset);

  std::vector<Circle> circles;
  int index = 0;
  while (!noise.empty() && index < 100){
    index++;
    auto pos = noise[rand() * 1.0 / RAND_MAX * noise.size()].pos;

    auto max_size = MAX_CIRCLE_SIZE;

    for (auto circle : circles){
      auto dist = circle.dist_sq(pos);
      if (dist < pow(MAX_CIRCLE_SIZE, 2)){
        max_size = sqrt(dist);
      }
    }

    double radius = rand() * 1.0 / RAND_MAX * max_size - MIN_CIRCLE_SIZE;
    circles.push_back({pos, radius});

    for (int i=0; i<noise.size(); i++){
      if (circles.back().dist_sq(noise[i].pos) <= pow(radius + MIN_CIRCLE_SIZE, 2)){
        noise.erase(noise.begin() + i);
      }
    }
    std::cout << noise.size() << std::endl;
  }
  while (win.isOpen()){
    sf::Event e;
    while (win.pollEvent(e)){
      if (e.type == sf::Event::Closed){
        win.close();
      }
    }

    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)){
    //   noise = generate(x_offset, y_offset);
    // }
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
    //   y_offset -= step;
    //   noise = generate(x_offset, y_offset);
    // }
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
    //   y_offset += step;
    //   noise = generate(x_offset, y_offset);
    // }
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
    //   x_offset += step;
    //   noise = generate(x_offset, y_offset);
    // }
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
    //   x_offset -= step;
    //   noise = generate(x_offset, y_offset);
    // }

    win.clear();
    for (auto& circle : circles){
      win.draw(circle.circle_shape);
    }
    win.display();
  }
}
