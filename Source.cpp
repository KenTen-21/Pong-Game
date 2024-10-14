#include <SFML/Graphics.hpp> // Include SFML graphics library
#include <SFML/Audio.hpp> // Include SFML audio library
#include <iostream> // Include input/output stream library
#include <fstream> // Include file stream library
#include <string>
using namespace std;

enum GameState { MENU, SINGLE_PLAYER_MENU, GAME, PAUSE, MULTIPLAYER };
enum Difficulty { EASY, MEDIUM, HARD };

int readHighScore() {
    ifstream file("highscore.txt");
    int highScore = 0;
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    return highScore;
}

void writeHighScore(int highScore) {
    ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Pong Game"); // Create window.
    GameState gameState = MENU; // Default game state.
    Difficulty difficulty; // Difficulty level.

    sf::RectangleShape paddle1(sf::Vector2f(20, 150)); // Player 1 paddle.
    sf::RectangleShape paddle2(sf::Vector2f(20, 150)); // Player 2 or AI paddle.
    sf::CircleShape ball(15);

    paddle1.setPosition(50, 375); // Set player 1 paddle position.
    paddle2.setPosition(1530, 375); // Set player 2 or AI paddle position.
    ball.setPosition(800, 450); // Set ball position.

    const float paddleSpeed = 2.0f; // Default paddle speed (player).
    float aiSpeed = 1.0f; // Default paddle speed (AI).
    sf::Vector2f ballVelocity(0.5f, 0.5f); // Default ball velocity (speed).

    int score1 = 0; // Player 1 score.
    int score2 = 0; // Player 2 or AI score.
    int highScore = readHighScore(); // Read the highest score from file.

    sf::Font font; // Load font.
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        // Handle error
        cout << "Error loading font" << endl; // Output error message.
    }

    sf::Text scoreText1; // Player 1 score text.
    scoreText1.setFont(font);
    scoreText1.setCharacterSize(24);
    scoreText1.setFillColor(sf::Color::White);
    scoreText1.setPosition(30, 20);

    sf::Text scoreText2; // Player 2 or AI score text.
    scoreText2.setFont(font);
    scoreText2.setCharacterSize(24);
    scoreText2.setFillColor(sf::Color::White);
    scoreText2.setPosition(1450, 20);

    sf::Text highScoreText; // High score text.
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color::White);
    highScoreText.setPosition(750, 20);
    highScoreText.setString("High Score: " + std::to_string(highScore));

    sf::Text menuText; // Menu text.
    menuText.setFont(font);
    menuText.setCharacterSize(48);
    menuText.setFillColor(sf::Color::White);
    menuText.setString("Select Mode:\n1. Single Player\n2. Multiplayer\n3. Exit Game");
    menuText.setPosition(400, 200);

    sf::Text singlePlayerMenuText; // Single Player Menu text.
    singlePlayerMenuText.setFont(font);
    singlePlayerMenuText.setCharacterSize(48);
    singlePlayerMenuText.setFillColor(sf::Color::White);
    singlePlayerMenuText.setString("Select Difficulty:\n1. Easy\n2. Medium\n3. Hard\n4. Back to Main Menu");
    singlePlayerMenuText.setPosition(400, 200);

    sf::Text pauseText; // Pause text.
    pauseText.setFont(font);
    pauseText.setCharacterSize(48);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setString("Game Paused\nPress P to Resume\nPress M for Main Menu");
    pauseText.setPosition(400, 200);

    // Load sound buffers and create sound objects
    sf::SoundBuffer paddleHitBuffer;
    if (!paddleHitBuffer.loadFromFile("paddle_hit.wav")) {
        cout << "Error loading paddle_hit.wav" << endl;
    }
    sf::Sound paddleHitSound;
    paddleHitSound.setBuffer(paddleHitBuffer);

    sf::SoundBuffer scoreBuffer;
    if (!scoreBuffer.loadFromFile("score.wav")) {
        cout << "Error loading score.wav" << endl;
    }
    sf::Sound scoreSound;
    scoreSound.setBuffer(scoreBuffer);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (gameState == MENU) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Num1) {
                        gameState = SINGLE_PLAYER_MENU;
                    }
                    else if (event.key.code == sf::Keyboard::Num2) {
                        gameState = MULTIPLAYER;
                        score1 = 0; // Reset scores
                        score2 = 0;
                    }
                    else if (event.key.code == sf::Keyboard::Num3) {
                        window.close();
                    }
                }
            }
            else if (gameState == SINGLE_PLAYER_MENU) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Num1) {
                        difficulty = EASY;
                        aiSpeed = 1.0f; // Easy AI speed
                        ballVelocity = sf::Vector2f(0.5f, 0.5f); // Easy ball speed
                        gameState = GAME;
                        score1 = 0; // Reset scores
                        score2 = 0;
                    }
                    else if (event.key.code == sf::Keyboard::Num2) {
                        difficulty = MEDIUM;
                        aiSpeed = 1.5f; // Medium AI speed
                        ballVelocity = sf::Vector2f(0.7f, 0.7f); // Medium ball speed
                        gameState = GAME;
                        score1 = 0; // Reset scores
                        score2 = 0;
                    }
                    else if (event.key.code == sf::Keyboard::Num3) {
                        difficulty = HARD;
                        aiSpeed = 2.0f; // Hard AI speed
                        ballVelocity = sf::Vector2f(1.0f, 1.0f); // Hard ball speed
                        gameState = GAME;
                        score1 = 0; // Reset scores
                        score2 = 0;
                    }
                    else if (event.key.code == sf::Keyboard::Num4) {
                        gameState = MENU;
                    }
                }
            }
            else if (gameState == GAME || gameState == MULTIPLAYER) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                    gameState = PAUSE;
                }
            }
            else if (gameState == PAUSE) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                    gameState = (gameState == MULTIPLAYER) ? MULTIPLAYER : GAME;
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M) {
                    gameState = MENU;
                }
            }
        }

        // Game logic
        if (gameState == GAME || gameState == MULTIPLAYER) {
            // Player 1 paddle movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && paddle1.getPosition().y > 0)
                paddle1.move(0, -paddleSpeed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && paddle1.getPosition().y < window.getSize().y - paddle1.getSize().y)
                paddle1.move(0, paddleSpeed);

            // Player 2 paddle movement (Multiplayer)
            if (gameState == MULTIPLAYER) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && paddle2.getPosition().y > 0)
                    paddle2.move(0, -paddleSpeed);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && paddle2.getPosition().y < window.getSize().y - paddle2.getSize().y)
                    paddle2.move(0, paddleSpeed);
            }
            // AI paddle movement (Single Player)
            else {
                if (ball.getPosition().y > paddle2.getPosition().y + paddle2.getSize().y / 2 && paddle2.getPosition().y < window.getSize().y - paddle2.getSize().y)
                    paddle2.move(0, aiSpeed);
                if (ball.getPosition().y < paddle2.getPosition().y + paddle2.getSize().y / 2 && paddle2.getPosition().y > 0)
                    paddle2.move(0, -aiSpeed);
            }

            // Ball movement
            ball.move(ballVelocity);

            // Ball collision
            if (ball.getPosition().y <= 0 || ball.getPosition().y >= window.getSize().y - ball.getRadius() * 2) {
                ballVelocity.y = -ballVelocity.y;
            }

            if (ball.getPosition().x <= 0) {
                score2++; // Player 2 or AI scores
                ball.setPosition(800, 450); // Reset ball position
                ballVelocity.x = -ballVelocity.x; // Reverse direction but keep the same speed
                scoreSound.play();
            }

            if (ball.getPosition().x >= window.getSize().x - ball.getRadius() * 2) {
                score1++; // Player 1 scores
                ball.setPosition(800, 450); // Reset ball position
                ballVelocity.x = -ballVelocity.x; // Reverse direction but keep the same speed
                scoreSound.play();
            }

            // Player 1 paddle collision
            if (ball.getGlobalBounds().intersects(paddle1.getGlobalBounds())) {
                ball.setPosition(paddle1.getPosition().x + paddle1.getSize().x + ball.getRadius(), ball.getPosition().y);
                ballVelocity.x = -ballVelocity.x;
                paddleHitSound.play();
            }

            // Player 2 or AI paddle collision
            if (ball.getGlobalBounds().intersects(paddle2.getGlobalBounds())) {
                ball.setPosition(paddle2.getPosition().x - ball.getRadius() * 2, ball.getPosition().y);
                ballVelocity.x = -ballVelocity.x;
                paddleHitSound.play();
            }

            // Update score texts
            scoreText1.setString("Player 1: " + std::to_string(score1));
            scoreText2.setString((gameState == MULTIPLAYER ? "Player 2: " : "AI: ") + std::to_string(score2));

            // Update high score if necessary
            if (score1 > highScore) {
                highScore = score1;
                writeHighScore(highScore);
                highScoreText.setString("High Score: " + std::to_string(highScore));
            }

            // Check for game over
            window.clear();
            window.draw(paddle1);
            window.draw(paddle2);
            window.draw(ball);
            window.draw(scoreText1);
            window.draw(scoreText2);
            window.draw(highScoreText);
        }
        // Draw menu text
        else if (gameState == MENU) {
            window.clear();
            window.draw(menuText);
            window.draw(highScoreText);
        }
        // Draw single player menu text
        else if (gameState == SINGLE_PLAYER_MENU) {
            window.clear();
            window.draw(singlePlayerMenuText);
        }
        // Draw pause text
        else if (gameState == PAUSE) {
            window.clear();
            window.draw(pauseText);
        }
        window.display();
    }

    return 0;
}