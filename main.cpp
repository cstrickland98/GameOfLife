#include <SFML/Graphics.hpp>
#include <vector>

void updateGrid(std::vector<std::vector<bool>>& grid);
void renderGrid(sf::RenderWindow& window, const std::vector<std::vector<bool>>& grid, int cellSize, int gridWidth, int gridHeight);
void centerText(sf::Text& text, sf::RectangleShape& shape);
void drawGridLines(sf::RenderWindow& window, int windowWidth, int windowHeight, int cellSize);

int main() {
    int gridWidth = 50, gridHeight = 50;
    const int cellSize = 10;
    const int windowWidth = gridWidth * cellSize;
    const int windowHeight = gridHeight * cellSize;

    std::vector<std::vector<bool>> grid(gridWidth, std::vector<bool>(gridHeight, false));

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Conway's Game of Life");


    int frameCounter = 0;
    int updateFrequency = 5; // Update the grid every 5 frames by default
    bool simulationRunning = false;

    // Define Play/Pause button
    sf::RectangleShape playPauseButton(sf::Vector2f(cellSize * 8, cellSize * 3));
    playPauseButton.setPosition(sf::Vector2f(cellSize, windowHeight - 3 * cellSize));
    playPauseButton.setFillColor(sf::Color::Green);

    // Define Stop button
    sf::RectangleShape stopButton(sf::Vector2f(cellSize * 5, cellSize * 3));
    stopButton.setPosition(sf::Vector2f(windowWidth - 5 * cellSize, windowHeight - 3 * cellSize));
    stopButton.setFillColor(sf::Color::Red);

    // Define Randomize button
    sf::RectangleShape randomizeButton(sf::Vector2f(cellSize * 8, cellSize * 3));
    randomizeButton.setFillColor(sf::Color::Blue);


    // Define button labels
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Error handling
    }
    sf::Text playPauseText("Play/Pause", font, 16);
    sf::Text stopText("Stop", font, 16);
    sf::Text randomizeText("Randomize", font, 16);

    playPauseText.setPosition(playPauseButton.getPosition().x + playPauseButton.getSize().x / 2, playPauseButton.getPosition().y + playPauseButton.getSize().y / 2);
    stopText.setPosition(stopButton.getPosition().x + stopButton.getSize().x / 2, stopButton.getPosition().y + stopButton.getSize().y / 2);
    playPauseText.setFillColor(sf::Color::Black);

    // Position the Randomize button in the bottom center of the window
    randomizeButton.setPosition(sf::Vector2f(windowWidth / 2 - randomizeButton.getSize().x / 2, windowHeight - 3 * cellSize));
    randomizeText.setPosition(randomizeButton.getPosition().x + randomizeButton.getSize().x / 2, randomizeButton.getPosition().y + randomizeButton.getSize().y / 2);


    // Center text
    centerText(playPauseText, playPauseButton);
    centerText(stopText, stopButton);
    centerText(randomizeText, randomizeButton);


    // Slider initialization

    sf::RectangleShape sliderBackground(sf::Vector2f(windowWidth * 0.3f, 10));
    sliderBackground.setPosition(playPauseButton.getPosition().x, playPauseButton.getPosition().y - 50);
    sliderBackground.setFillColor(sf::Color(200, 200, 200));

    sf::RectangleShape sliderHandle(sf::Vector2f(20, 30));
    sliderHandle.setPosition(sliderBackground.getPosition().x, sliderBackground.getPosition().y - 10);
    sliderHandle.setFillColor(sf::Color(100, 100, 100));

    bool sliderDrag = false;

    sf::Text sliderText("Update Frequency: " + std::to_string(updateFrequency), font, 16);
    sliderText.setPosition(sliderBackground.getPosition().x, sliderBackground.getPosition().y - 30);

    

    // Main loop
    while (window.isOpen()) {
        // Event handling
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
                    simulationRunning = !simulationRunning; // Play/Pause
                }

                if (event.key.code == sf::Keyboard::Add && updateFrequency > 1) {
                    updateFrequency--;
                }
                if (event.key.code == sf::Keyboard::Subtract) {
                    updateFrequency++;
                }
                if (event.key.code == sf::Keyboard::S) {
                    simulationRunning = false; // Stop
                    grid = std::vector<std::vector<bool>>(gridWidth, std::vector<bool>(gridHeight, false)); // Reset grid
                }
            }


            // Slider interaction
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left &&
                sliderHandle.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                sliderDrag = true;
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                sliderDrag = false;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x / cellSize;
                int y = event.mouseButton.y / cellSize;

                // Play/Pause button
                if (playPauseButton.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                    simulationRunning = !simulationRunning;
                }

                // Stop button
                else if (stopButton.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                    simulationRunning = false;
                    grid = std::vector<std::vector<bool>>(gridWidth, std::vector<bool>(gridHeight, false)); // Reset grid
                }

                // Randomize button
                else if (randomizeButton.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                    int numberOfAliveCells = rand() % (gridWidth * gridHeight); // Adjust as needed
                    for (int i = 0; i < numberOfAliveCells; ++i) {
                        int x = rand() % gridWidth;
                        int y = rand() % gridHeight;
                        grid[x][y] = true;
                    }
                }
            }

            if (event.type == sf::Event::Resized) {
                // Calculate the new grid dimensions
                int newGridWidth = event.size.width / cellSize;
                int newGridHeight = event.size.height / cellSize;

                // Create a new grid with the new dimensions
                std::vector<std::vector<bool>> newGrid(newGridWidth, std::vector<bool>(newGridHeight, false));

                // Copy the existing cell values to the new grid
                for (int x = 0; x < std::min(gridWidth, newGridWidth); ++x) {
                    for (int y = 0; y < std::min(gridHeight, newGridHeight); ++y) {
                        newGrid[x][y] = grid[x][y];
                    }
                }

                // Update the position of the Play/Pause button
                playPauseButton.setPosition(sf::Vector2f(cellSize, event.size.height - 3 * cellSize));
                playPauseText.setPosition(playPauseButton.getPosition().x + playPauseButton.getSize().x / 2, playPauseButton.getPosition().y + playPauseButton.getSize().y / 2);
                centerText(playPauseText, playPauseButton);

                // Update the position of the Stop button
                stopButton.setPosition(sf::Vector2f(event.size.width - 5 * cellSize, event.size.height - 3 * cellSize));
                stopText.setPosition(stopButton.getPosition().x + stopButton.getSize().x / 2, stopButton.getPosition().y + stopButton.getSize().y / 2);
                centerText(stopText, stopButton);


                // Update slider position
                sliderBackground.setPosition(playPauseButton.getPosition().x, playPauseButton.getPosition().y - 50);
                sliderHandle.setPosition(sliderBackground.getPosition().x + (updateFrequency - 1) / 499.0f * sliderBackground.getSize().x, sliderBackground.getPosition().y - 10);
                sliderText.setPosition(sliderBackground.getPosition().x, sliderBackground.getPosition().y - 30);
                // Update the position of the Randomize button
                randomizeButton.setPosition(sf::Vector2f(event.size.width / 2 - randomizeButton.getSize().x / 2, event.size.height - 3 * cellSize));
                randomizeText.setPosition(randomizeButton.getPosition().x + randomizeButton.getSize().x / 2, randomizeButton.getPosition().y + randomizeButton.getSize().y / 2);
                centerText(randomizeText, randomizeButton);

                // Update the grid and its dimensions
                grid = newGrid;
                gridWidth = newGridWidth;
                gridHeight = newGridHeight;


                // Update slider position
                sliderBackground.setPosition(playPauseButton.getPosition().x, playPauseButton.getPosition().y - 50);
                sliderHandle.setPosition(sliderBackground.getPosition().x + (updateFrequency - 1) / 499.0f * sliderBackground.getSize().x, sliderBackground.getPosition().y - 10);
                sliderText.setPosition(sliderBackground.getPosition().x, sliderBackground.getPosition().y - 30);
                // Update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        // Get mouse position
        int x = sf::Mouse::getPosition(window).x / cellSize;
        int y = sf::Mouse::getPosition(window).y / cellSize;

        // Left mouse button (draw alive cells)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
            grid[x][y] = true;
        }

        // Right mouse button (draw dead cells)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
            grid[x][y] = false;
        }



        // If slider is being dragged, update its position and the updateFrequency
        if (sliderDrag) {
            float mouseX = sf::Mouse::getPosition(window).x;
            float sliderStart = sliderBackground.getPosition().x;
            float sliderEnd = sliderStart + sliderBackground.getSize().x - sliderHandle.getSize().x;

            if (mouseX < sliderStart) {
                mouseX = sliderStart;
            }
            else if (mouseX > sliderEnd) {
                mouseX = sliderEnd;
            }

            sliderHandle.setPosition(mouseX, sliderHandle.getPosition().y);

            // Map slider position to updateFrequency (1 to 500)
            float ratio = (mouseX - sliderStart) / (sliderEnd - sliderStart);
            updateFrequency = static_cast<int>(1 + ratio * 499);  // 1 to 500

            sliderText.setString("Update Frequency: " + std::to_string(updateFrequency));
        }
        if (simulationRunning) {
            frameCounter++;
            if (frameCounter % updateFrequency == 0) {

                updateGrid(grid);
            }

        }
        renderGrid(window, grid, cellSize, gridWidth, gridHeight);

        // Draw buttons and their labels
        window.draw(playPauseButton);
        window.draw(stopButton);
        window.draw(playPauseText);
        window.draw(stopText);
        window.draw(randomizeButton);
        window.draw(randomizeText);


        // Draw slider and text
        window.draw(sliderBackground);
        window.draw(sliderHandle);
        window.draw(sliderText);
        window.display();
    }

    return 0;
}

void updateGrid(std::vector<std::vector<bool>>& grid) {
    int width = grid.size();
    int height = grid[0].size();

    std::vector<std::vector<bool>> newGrid = grid;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            int aliveNeighbors = 0;

            // Check the 8 neighboring cells
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue; // Skip the current cell

                    int nx = (x + dx + width) % width;
                    int ny = (y + dy + height) % height;

                    aliveNeighbors += grid[nx][ny] ? 1 : 0;
                }
            }

            if (grid[x][y] && (aliveNeighbors < 2 || aliveNeighbors > 3)) {
                newGrid[x][y] = false;
            }
            else if (!grid[x][y] && aliveNeighbors == 3) {
                newGrid[x][y] = true;
            }
        }
    }

    grid = newGrid;
}

void renderGrid(sf::RenderWindow& window, const std::vector<std::vector<bool>>& grid, int cellSize, int gridWidth, int gridHeight) {
    window.clear();

    // draw grid display
    drawGridLines(window, (gridWidth* cellSize), (gridHeight* cellSize), cellSize);

    for (int x = 0; x < grid.size(); ++x) {
        for (int y = 0; y < grid[0].size(); ++y) {
            if (grid[x][y]) {
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                cell.setPosition(x * cellSize, y * cellSize);
                cell.setFillColor(sf::Color::White);
                window.draw(cell);
            }
        }
    }
}

// Define a helper function for positioning text
void centerText(sf::Text& text, sf::RectangleShape& shape) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f); // Center text origin
    text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f, shape.getPosition().y + shape.getSize().y / 2.0f); // Center text position
}

void drawGridLines(sf::RenderWindow& window, int windowWidth, int windowHeight, int cellSize) {
    // Draw grid lines
    for (int x = 0; x <= windowWidth; x += cellSize) {
        sf::RectangleShape verticalLine(sf::Vector2f(1, windowHeight));
        verticalLine.setPosition(x, 0);
        verticalLine.setFillColor(sf::Color(150, 150, 150));
        window.draw(verticalLine);
    }

    for (int y = 0; y <= windowHeight; y += cellSize) {
        sf::RectangleShape horizontalLine(sf::Vector2f(windowWidth, 1));
        horizontalLine.setPosition(0, y);
        horizontalLine.setFillColor(sf::Color(150, 150, 150));
        window.draw(horizontalLine);
    }
}