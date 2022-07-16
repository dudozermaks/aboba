#include "SFML/Graphics.hpp"
#include "FastNoise/FastNoise.h"

int main(){
  sf::RenderWindow win{{800, 800}, "RPG"};
  win.setFramerateLimit(60);
  

  std::vector<> noise(800 * 800);
  auto fn_perlin = FastNoise::New<FastNoise::Perlin>(); 
  while (win.isOpen()){
    sf::Event e;
    while (win.pollEvent(e)){
      if (e.type == sf::Event::Closed){
        win.close();
      }
    }
    win.clear();
    win.display();
  }
}
