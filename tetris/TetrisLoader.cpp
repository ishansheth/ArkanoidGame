#include "TetrisLoader.h"

TetrisLoader::TetrisLoader(sf::RenderWindow& win):tetris_window(win)
{}


void TetrisLoader::start()
{
    using namespace sf;

    Texture t1,t2,t3;

    t1.loadFromFile(STRINGIZE_VALUE_OF(TETRIS_IMAGES_PATH) + std::string("/tiles.png"));
    t2.loadFromFile(STRINGIZE_VALUE_OF(TETRIS_IMAGES_PATH) + std::string("/background.png"));
    t3.loadFromFile(STRINGIZE_VALUE_OF(TETRIS_IMAGES_PATH) + std::string("/frame.png"));
  
    Sprite s(t1), background(t2), frame(t3);
  
    int dx=0; bool rotate=0; int colorNum=1;
    float timer=0,delay=0.3; 
  
    Clock clock;
  
    while (tetris_window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;
      
        Event e;
        while (tetris_window.pollEvent(e))
        {
	        if (e.type == Event::Closed)
	            tetris_window.close();
	  
	        if (e.type == Event::KeyPressed)
	            if (e.key.code==Keyboard::Up) 
                    rotate=true;
	            else if (e.key.code==Keyboard::Left) 
                    dx=-1;
	        else if (e.key.code==Keyboard::Right) 
                dx=1;
	    }
      
        if (Keyboard::isKeyPressed(Keyboard::Down)) 
            delay=0.05;
      
        //// <- Move -> ///
        for (int i=0;i<4;i++)  
        { 
            b[i]=a[i]; 
            a[i].x+=dx; 
        }

        if (!check()) 
            for (int i=0;i<4;i++) 
                a[i]=b[i];
      
        //////Rotate//////
        if (rotate)
	    {
	        Point p = a[1]; //center of rotation
	        for (int i=0;i<4;i++)
	        {
	            int x = a[i].y-p.y;
	            int y = a[i].x-p.x;
	            a[i].x = p.x - x;
	            a[i].y = p.y + y;
	        }
    	    if (!check()) 
                for (int i=0;i<4;i++) 
                    a[i]=b[i];
	    }
      
        ///////Tick//////
        if (timer>delay)
	    {
	        for (int i=0;i<4;i++) 
            { 
                b[i]=a[i]; 
                a[i].y+=1; 
            }
	  
	        if (!check())
	        {
	            for (int i=0;i<4;i++) 
                    field[b[i].y][b[i].x]=colorNum;
	      
	            colorNum=1+rand()%7;
	            int n=rand()%7;
	            for (int i=0;i<4;i++)
		        {
		            a[i].x = figures[n][i] % 2;
		            a[i].y = figures[n][i] / 2;
		        }
	        }
	  
	        timer=0;
	    }
      
        ///////check lines//////////
        int k=M-1;
        for (int i=M-1;i>0;i--)
	    {
	        int count=0;
	        for (int j=0;j<N;j++)
	        {
	            if (field[i][j]) 
                    count++;
	            field[k][j]=field[i][j];
	        }
    	    if (count<N) 
                k--;
	    }
      
        dx=0; 
        rotate=0; 
        delay=0.3;
      
        /////////draw//////////
        tetris_window.clear(Color::White);	
        tetris_window.draw(background);
      
        for (int i=0;i<M;i++)
	        for (int j=0;j<N;j++)
	        {
	            if (field[i][j]==0) 
                    continue;
	            s.setTextureRect(IntRect(field[i][j]*18,0,18,18));
	            s.setPosition(j*18,i*18);
	            s.move(28,31); //offset
	            tetris_window.draw(s);
	        }
      
        for (int i=0;i<4;i++)
	    {
	        s.setTextureRect(IntRect(colorNum*18,0,18,18));
	        s.setPosition(a[i].x*18,a[i].y*18);
	        s.move(28,31); //offset
	        tetris_window.draw(s);
	    }
      
        tetris_window.draw(frame);
        tetris_window.display();
    }
}
