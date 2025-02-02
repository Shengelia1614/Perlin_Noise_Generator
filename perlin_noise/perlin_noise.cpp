// perlin_noise.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <random>
#include <math.h>


using namespace sf;
using namespace std;


float fade(float t) {
    return t* t* t * (t * (t * 6 - 15) + 10);
}
double dot_product(pair<float, float> gradient, float dx, float dy) {
    return gradient.first * dx + gradient.second * dy;
}

vector<vector<pair<float, float>>> perlin_grid(int size_X, int size_Y);
Image prelin_image(vector<vector<pair<float, float>>> perlin_grid, int grid_size);



int main()
{

    
    RenderWindow window(sf::VideoMode(1900, 1080), "SFML works!");

    
    srand(time(NULL));

    int grid_size = 0;
    
    
    sf::Texture texture;
    sf::Sprite sprite;





    string input_text;
    Font font;
    font.loadFromFile("resources/ARIAL.ttf");
    Text output("", font);
    Text prompt("Input grid size:", font);
    prompt.setCharacterSize(30);
    output.setCharacterSize(24);

    
    

    while (window.isOpen())
    {
        window.clear();


        
        

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    grid_size = 0;
                    input_text = "";
                }
            }
            if (event.type == Event::KeyPressed && grid_size==0) {
                switch (event.key.code)
                {
                
                case Keyboard::Num0:input_text.push_back('0'); break;
                case Keyboard::Num1:input_text.push_back('1'); break;
                case Keyboard::Num2:input_text.push_back('2'); break;
                case Keyboard::Num3:input_text.push_back('3'); break;
                case Keyboard::Num4:input_text.push_back('4'); break;
                case Keyboard::Num5:input_text.push_back('5'); break;
                case Keyboard::Num6:input_text.push_back('6'); break;
                case Keyboard::Num7:input_text.push_back('7'); break;
                case Keyboard::Num8:input_text.push_back('8'); break;
                case Keyboard::Num9:input_text.push_back('9'); break;
                case Keyboard::BackSpace: { if (input_text.size() != 0) input_text.pop_back(); }; break;
                case Keyboard::Enter:grid_size= stoi(input_text); break;
                
                default:
                    break;
                }
            }
        }


        cout << grid_size << endl;
        if (grid_size == 0) {
            
            window.clear();

            prompt.setPosition(Vector2f((window.getSize().x / 2 - (prompt.getGlobalBounds().width) / 2), ((window.getSize().y / 2) - prompt.getGlobalBounds().height * 3)));

            output.setPosition(Vector2f((window.getSize().x / 2 - (output.getGlobalBounds().width) / 2), ((window.getSize().y / 2) - output.getGlobalBounds().height / 2)));
            output.setString(input_text);
            window.draw(prompt);
            window.draw(output);
        }
        else {
            if (grid_size > 1900) grid_size = 1900;

            int size_X = window.getSize().x / grid_size;
            int size_Y = window.getSize().y / grid_size;
            texture.loadFromImage(prelin_image(perlin_grid(size_X, size_Y), grid_size));
            sprite.setTexture(texture);


            window.draw(sprite);
        }


        

        

        window.display();

    }
   
}

vector<vector<pair<float, float>>> perlin_grid(int size_X, int size_Y)
{
    vector<vector<pair<float, float>>> perlin_grid;
    for (size_t i = 0; i < size_X + 2; i++)
    {
        vector<pair<float, float>> temp;

        for (size_t k = 0; k < size_Y + 2; k++)
        {

            temp.push_back({ (float)((rand() % 400) - 200) / 80, (float)((rand() % 400) - 200) / 80 });
        }
        perlin_grid.push_back(temp);

    }
    return perlin_grid;
}

Image prelin_image(vector<vector<pair<float, float>>> perlin_grid, int grid_size)
{
    double pixel_power = 0;

    Image image;
    image.create((perlin_grid.size() - 1) * grid_size, (perlin_grid[0].size() - 1) * grid_size, sf::Color::Transparent);


    for (size_t i = 0; i < perlin_grid[0].size() - 1; i++)

    {

        for (size_t k = 0; k < perlin_grid.size() - 1; k++)

        {
            for (int mx = 0; mx < grid_size; mx++)
            {
                for (int my = 0; my < grid_size; my++)
                {



                    float dx = (float)mx / grid_size;  // x offset within the grid cell
                    float dy = (float)my / grid_size;  // y offset within the grid cell


                    float dot00 = dot_product(perlin_grid[k][i], dx, dy);
                    float dot10 = dot_product(perlin_grid[k + 1][i], dx - 1.0f, dy); // dx-1 because it's shifted
                    float dot01 = dot_product(perlin_grid[k][i + 1], dx, dy - 1.0f); // dy-1 for vertical offset
                    float dot11 = dot_product(perlin_grid[k + 1][i + 1], dx - 1.0f, dy - 1.0f); // Both dx-1, dy-1



                    // Apply the fade function to smooth the interpolation
                    float fade_x = fade(dx);
                    float fade_y = fade(dy);

                    // Interpolate in the x-direction
                    float x0 = dot00 + (dot10 - dot00) * fade_x;
                    float x1 = dot01 + (dot11 - dot01) * fade_x;

                    // Interpolate in the y-direction
                    float pixel_power = x0 + (x1 - x0) * fade_y;

                    // Scale the result to [0, 255]
                    pixel_power = (pixel_power + 1) * 128;  // Ensure range is [-1, 1] -> [0, 255]

                    // Clip to 0-255 range
                    if (pixel_power > 255) pixel_power = 255;
                    if (pixel_power < 0) pixel_power = 0;

                    // Set the pixel color based on the interpolated value
                    image.setPixel(mx + grid_size * k, my + grid_size * i, sf::Color(pixel_power, pixel_power, pixel_power, 255));

                }

            }

        }
    }

    return image;
}