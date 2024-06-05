#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <iostream>
#include <typeinfo>

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
	//bool operator!=(const point& other) const
	//{	
	//	return !operator==(other);
	//}

};

struct pointhash {
    std::size_t operator()(const point& p) const noexcept
    {	
		//std::cout << p.x << " " << p.y <<"\n";
        return p.x + p.y*10;
    }
};

const unsigned int surround_len = 8;

void gen_surround(point* out,const point& p){	
	*out = (point({p.x-1,p.y-1}));
	++out;
	*out = (point({p.x,p.y-1}));
	++out;
	*out = (point({p.x+1,p.y-1}));
	++out;
	*out = (point({p.x-1,p.y}));  
	++out;
	*out = (point({p.x+1,p.y}));  
	++out;
	*out = (point({p.x-1,p.y+1}));
	++out;
	*out = (point({p.x,p.y+1}));  
	++out;
	*out = (point({p.x+1,p.y+1}));
}

std::unordered_set<point,pointhash> points;

unsigned int cellnum(const point& p){
	point surround[surround_len];
	gen_surround(&surround[0],p);
	unsigned int num = 0;
	for (int i = 0; i < surround_len; i++){
		num += (points.find(surround[i]) != points.end());
	}	
	return num;
}

std::unordered_set<point,pointhash> pointsnext;
std::unordered_set<point,pointhash> proccessed;

void processcell(const point& p){
	if (proccessed.find(p) != points.end()) return;
	proccessed.insert(p);
	const bool alive = (points.find(p) != points.end());
	const unsigned int numaround = cellnum(p);
	if ((alive and (numaround == 2) or (numaround == 3)) or (!alive and (numaround == 3))) pointsnext.insert(p);
}

void update(){
	for (auto p = points.begin(); p != points.end(); p++){
			processcell(*p);
			point surround[surround_len];
			gen_surround(&surround[0],*p);
			for (int i = 0; i < surround_len; i++){
				processcell(surround[i]);
			}
	}
	points = pointsnext;
	pointsnext.clear();
	proccessed.clear();
}

int main()
{	
	points.insert(point({0,0}));	
	points.insert(point({1,0}));
	points.insert(point({2,0}));
	unsigned int zoom = 10;
	point camera({0,0});
	sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");
	sf::Clock Clock;
    sf::RectangleShape pixel(sf::Vector2f(zoom,zoom));
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		
		if (Clock.getElapsedTime().asSeconds() > 0.5){
			update();
			Clock.restart();
		}
		
		camera = sf::Mouse::getPosition(window);

        window.clear();
		for (auto p = points.begin(); p != points.end(); p++){
			pixel.setPosition((*p).x*zoom + camera.x,(*p).y*zoom + camera.y);
			window.draw(pixel);
		}
        window.display();
    }
    return 0;
}