#include "iostream"
#include <future>
#include <time.h>
#include "SFML/Graphics.hpp"
#include "PerlinNoise/PerlinNoise.hpp"

const double MAX_CIRCLE_SIZE = 2;
const double MIN_CIRCLE_SIZE = 1;
const double DIST_BETWEEN = 0;

struct Noise{
  sf::Vector2f pos;
  double val;
  sf::Color color;
  Noise (sf::Vector2f pos, double val){
    this->pos = pos;
    this->val = val;
    val *= 255;
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
    circle_shape.setOrigin({(float)radius*10.f, (float)radius*10.f});
    circle_shape.setRadius(radius*10);
    circle_shape.setPosition(pos*10.f);
    circle_shape.setFillColor(sf::Color::Cyan);
  }
  double dist_sq(sf::Vector2f position){
    return pow(position.x - this->pos.x, 2) + pow(position.y - this->pos.y, 2);
  }
  bool is_inside(sf::Vector2f position){
    return dist_sq(position) - pow(radius, 2) <= 0;
  }
};

std::vector<Noise> generate(float x_offset, float y_offset){
  std::vector<Noise> noise;
  float scale = 0.1;
  static siv::PerlinNoise::seed_type seed = rand();

  static const siv::PerlinNoise perlin{ seed };
  for (float y=0; y<80; y++){
    for (float x=0; x<80; x++){
      noise.push_back({{x, y}, perlin.octave2D_01((x + x_offset) * scale, (y + y_offset) * scale, 8)});
      if (noise.back().val > 0.5) {noise.pop_back();}
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
    // index++;
    Noise n = noise[rand() * 1.0 / RAND_MAX * (noise.size() - 1)];

    double max_size = MAX_CIRCLE_SIZE;

    for (auto circle : circles){
      double dist = circle.dist_sq(n.pos) - pow(circle.radius, 2);
      if (dist < pow(max_size, 2)){
        max_size = sqrt(dist);
      }
    }
    double radius = rand() * 1.0 / RAND_MAX * (max_size - MIN_CIRCLE_SIZE) + MIN_CIRCLE_SIZE;
    circles.push_back({n.pos, radius});

    Circle delete_radius = circles.back();
    delete_radius.radius += MIN_CIRCLE_SIZE + DIST_BETWEEN;
    for (int i=0; i<noise.size(); i++){
      if (delete_radius.is_inside(noise[i].pos)){
        noise.erase(noise.begin() + i);
        i--;
      }
    }
    win.clear();
    for (auto& circle : circles){
      win.draw(circle.circle_shape);
    }
    for (auto& n : noise){
      sf::Vertex point(n.pos * 10.f, sf::Color::White);
      win.draw(&point, 1, sf::Points);
    }
    win.display();
    bool u_pressed = false;
    while (!u_pressed){
      sf::Event e;
      while (win.pollEvent(e)){
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::U){
          u_pressed = true;
        }
      }
    }
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
