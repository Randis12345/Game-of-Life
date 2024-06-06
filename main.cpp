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
	void operator+=(const sf::Vector2i& vec){
		x += vec.x;
		y += vec.y;
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

void createnewpoint(){
	float x = (mousepos.x - camera.x)/zoom;
	float y = (mousepos.y - camera.y)/zoom;
	if (x < 0) x-=1;
	if (y < 0) y-=1;
	point newpoint({int(x),int(y)});
	points.insert(newpoint);
	//std::cout << newpoint.x << " " << newpoint.y << "\n";

};


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
			
		if (Clock.getElapsedTime().asSeconds() > 0.2 && !paused){
			update();
			Clock.restart();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && paused){
			createnewpoint();
		}
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
			if (dragging){
				camera+= (mousepos-lastmousepos);
			}
			lastmousepos = mousepos;
			dragging = true;
		}else{
			dragging = false;
		}
        window.clear();
		for (auto p = points.begin(); p != points.end(); p++){
			pixel.setPosition((*p).x*zoom + camera.x,(*p).y*zoom + camera.y);
			window.draw(pixel);
		}
        window.display();
    }
    return 0;
}