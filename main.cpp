#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <unordered_map>
#include <cmath>

struct point{
	int x;
	int y;
	bool operator==(const point& other) const
	{	
		//std::cout << &other << "\n";
		return x == other.x && y == other.y;
	}
	void operator=(const sf::Vector2i& vec){
		x = vec.x;
		y = vec.y;
	}
	void operator+=(const sf::Vector2i& vec){
		x += vec.x;
		y += vec.y;
	}
};

struct pointhash {
    std::size_t operator()(const point& p) const noexcept
    {	
        return p.x + p.y*10;
    }
};


std::unordered_set<point,pointhash> points;
std::unordered_set<point,pointhash> pointsnext;

void update() {
    std::unordered_map<point, int, pointhash> neighbor_counts;
    
    // establish candidates and their neighbour counts
    for (const auto& p : points) {
        neighbor_counts[p]; 
        
        const int x = p.x;
        const int y = p.y;
        neighbor_counts[{x - 1, y - 1}]++;
        neighbor_counts[{x,     y - 1}]++;
        neighbor_counts[{x + 1, y - 1}]++;
        neighbor_counts[{x - 1, y    }]++;
        neighbor_counts[{x + 1, y    }]++;
        neighbor_counts[{x - 1, y + 1}]++;
        neighbor_counts[{x,     y + 1}]++;
        neighbor_counts[{x + 1, y + 1}]++;
    }

    // Generate next iter 
    pointsnext.clear();
    for (const auto& [p, count] : neighbor_counts) {
        bool alive = points.find(p) != points.end();
        if (alive) {
            if (count == 2 || count == 3) {
                pointsnext.insert(p);
            }
        } else {
            if (count == 3) {
                pointsnext.insert(p);
            }
        }
    }
    points = std::move(pointsnext);
}


constexpr float step_time = 0.2;

float zoom = 10;
sf::RectangleShape pixel({zoom,zoom});
point camera({0,0});
sf::Vector2i mousepos;
    
void scroll(const int deltawheel){
	float amount = deltawheel*zoom/10;
	if (1-zoom > amount) amount = 1-zoom;
	camera.x = mousepos.x - (1+amount/zoom)*(mousepos.x-camera.x);
	camera.y = mousepos.y - (1+amount/zoom)*(mousepos.y-camera.y);
	zoom += amount;
	pixel.setSize({zoom,zoom});

}

int main()
{	
	points.insert(point({0,0}));	
	points.insert(point({1,0}));
	points.insert(point({2,0}));
	sf::RenderWindow window(sf::VideoMode(600, 600), "GOL");
	sf::Clock Clock;
  sf::Vector2i lastmousepos;
  bool dragging = false;
  bool paused = true;
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
              window.close();
		  if (event.type == sf::Event::MouseWheelMoved)
		  	scroll(event.mouseWheel.delta);
		  if (event.type == sf::Event::KeyPressed && event.key.code==57)
		  	paused = !paused;
    }
	  mousepos = sf::Mouse::getPosition(window);
	  	
	  if (Clock.getElapsedTime().asSeconds() > step_time  && !paused){
	  	update();
	  	Clock.restart();
	  }
	  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && paused){
      float x = (mousepos.x - camera.x)/zoom;
      float y = (mousepos.y - camera.y)/zoom;
      point newpoint({static_cast<int>(std::floor(x)),
                      static_cast<int>(std::floor(y))});
      points.insert(newpoint);
	  }
	  
	  if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
	    if (dragging){
	  	  camera += (mousepos-lastmousepos);
	  	}
	  	lastmousepos = mousepos;
	  	dragging = true;
	  }else{
	  	dragging = false;
	  }
    
    // Draw
    window.clear();
	  for (auto p = points.begin(); p != points.end(); p++){
	  	pixel.setPosition((*p).x*zoom + camera.x,(*p).y*zoom + camera.y);
	  	window.draw(pixel);
	  }
    window.display();
  }
  return 0;
}
