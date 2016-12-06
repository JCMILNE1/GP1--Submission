/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();
	// this is a comment
	cout << "this is a comment";
	// Set filename
	theFile.setFileName("Data/usermap.dat");
	// Check file is available
	if (!theFile.openFile(ios::in)) //open file for input output
	{
		cout << "Could not open specified file '" << theFile.getFileName() << "'. Error " << SDL_GetError() << endl;
		fileAvailable = false;
	}
	else
	{
		cout << "File '" << theFile.getFileName() << "' opened for input!" << endl;
		fileAvailable = true;
	}

	// Store the textures
	textureName = { "asteroid1", "asteroid2", "asteroid3", "asteroid4", "bullet","theRocket","theBackground"};
	texturesToUse = { "Images\\asteroid1.png", "Images\\asteroid2.png", "Images\\asteroid2.png", "Images\\asteroid2.png", "Images\\bullet.png", "Images\\rocketSprite.png", "Images\\starscape1024x768.png"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	gameTextList = { "Defender", "Use The arrow keys to move", "Stop the asteroids from hitting the bottom of the screen", "Press space to continue", "Game over, Your Score was:", "Highscore:", "Press space to return to main menu"};
	
	theTextureMgr->addTexture("Title", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Tutorial", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));   //creates the text for the game
	theTextureMgr->addTexture("Tutorial line 2", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Tutorial line 3", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Game over line 1", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[4], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Game over line 2", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[5], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Game over line 3", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[6], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));

	// Load game sounds
	soundList = { "theme","menutheme", "shot", "explosion" };
	soundTypes = { MUSIC, MUSIC, SFX, SFX };
	soundsToUse = { "Audio/who10Edit.wav", "Audio/level_3_music.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}



	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	theRocket.setSpritePos({ 500, 600 });
	theRocket.setTexture(theTextureMgr->getTexture("theRocket"));
	theRocket.setSpriteDimensions(theTextureMgr->getTexture("theRocket")->getTWidth(), theTextureMgr->getTexture("theRocket")->getTHeight());
	theRocket.setRocketVelocity({ 0, 0 });

	// Create vector array of textures
	int yposition = 0;
	for (int astro = 0; astro < 2; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos({(rand() % 950 ), yposition});
		theAsteroids[astro]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theTextureMgr->getTexture(textureName[randAsteroid]));
		theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randAsteroid])->getTWidth(), theTextureMgr->getTexture(textureName[randAsteroid])->getTHeight());
		theAsteroids[astro]->setAsteroidVelocity({ 0.0f, 30.0f });
		theAsteroids[astro]->setActive(true);
		yposition = yposition - 600;
	}

	gamestate = MENU;        //makes the game start on the menu screen
	cout << theFile.readDataFromFile(); 
	highscorestring = theFile.readDataFromFile();        //takes the high score from the external file and converts it to an integer
	highscore = atoi(highscorestring.c_str());
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (gamestate)
	{
	case MENU:   //renders the title, tutorial and background in the menu screen,
	{


		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		cTexture* tempTextTexture2 = theTextureMgr->getTexture("Tutorial");
		SDL_Rect pos2 = { 10, 200, tempTextTexture2->getTextureRect().w, tempTextTexture2->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		tempTextTexture2->renderTexture(theRenderer, tempTextTexture2->getTexture(), &tempTextTexture2->getTextureRect(), &pos2, scale2);

		cTexture* tempTextTexture3 = theTextureMgr->getTexture("Tutorial line 2");
		SDL_Rect pos3 = { 10, 250, tempTextTexture3->getTextureRect().w, tempTextTexture3->getTextureRect().h };
		FPoint scale3 = { 1, 1 };
		tempTextTexture3->renderTexture(theRenderer, tempTextTexture3->getTexture(), &tempTextTexture3->getTextureRect(), &pos3, scale3);

		cTexture* tempTextTexture4 = theTextureMgr->getTexture("Tutorial line 3");
		SDL_Rect pos4= { 10, 300, tempTextTexture4->getTextureRect().w, tempTextTexture4->getTextureRect().h };
		FPoint scale4 = { 1, 1 };
		tempTextTexture4->renderTexture(theRenderer, tempTextTexture4->getTexture(), &tempTextTexture4->getTextureRect(), &pos4, scale4);
	}
	break;

	case game:     //renders tha asteroids, player, score and background in the game screen
	{
		if (music != 1) //plays the music
		{
			theSoundMgr->getSnd("theme")->play(1);
			music = 1;                      //stops the music from restarting instiantly
		}
		SDL_RenderClear(theRenderer);
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render each asteroid in the vector array
		for (int draw = 0; draw < theAsteroids.size(); draw++)
		{
			theAsteroids[draw]->render(theRenderer, &theAsteroids[draw]->getSpriteDimensions(), &theAsteroids[draw]->getSpritePos(), theAsteroids[draw]->getSpriteRotAngle(), &theAsteroids[draw]->getSpriteCentre(), theAsteroids[draw]->getSpriteScale());
		}
		// render the rocket
		theRocket.render(theRenderer, &theRocket.getSpriteDimensions(), &theRocket.getSpritePos(), theRocket.getSpriteRotAngle(), &theRocket.getSpriteCentre(), theRocket.getSpriteScale());

		//render the score

		if (scoreprocessing == 0);
		{
			scorevector = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "90", "91", "92", "93", "94", "95", "96", "97", "98", "99"};
			theTextureMgr->addTexture("Scoretext", theFontMgr->getFont("digital")->createTextTexture(theRenderer, scorevector[score], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
		
			cTexture* tempTextTexture5 = theTextureMgr->getTexture("Scoretext");
			SDL_Rect pos5 = { 10, 100, tempTextTexture5->getTextureRect().w, tempTextTexture5->getTextureRect().h };
			FPoint scale5 = { 1, 1 };
			tempTextTexture5->renderTexture(theRenderer, tempTextTexture5->getTexture(), &tempTextTexture5->getTextureRect(), &pos5, scale5);

			scoreprocessing = 1;        //stops the score texture from rendering when the score is not updating
		}
	}
	break;

	case gameover:     //renders the game over text, you score and the highscore
	{
		SDL_RenderClear(theRenderer);
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		theTextureMgr->addTexture("Highscoretext", theFontMgr->getFont("digital")->createTextTexture(theRenderer, scorevector[highscore], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));

		cTexture* tempTextTexture = theTextureMgr->getTexture("Game over line 1");
		SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		cTexture* tempTextTexture5 = theTextureMgr->getTexture("Scoretext");
		SDL_Rect pos5 = { 420, 10, tempTextTexture5->getTextureRect().w, tempTextTexture5->getTextureRect().h };
		FPoint scale5 = { 1, 1 };
		tempTextTexture5->renderTexture(theRenderer, tempTextTexture5->getTexture(), &tempTextTexture5->getTextureRect(), &pos5, scale5);

		cTexture* tempTextTexture3 = theTextureMgr->getTexture("Game over line 2");
		SDL_Rect pos3 = { 10, 40, tempTextTexture3->getTextureRect().w, tempTextTexture3->getTextureRect().h };
		FPoint scale3 = { 1, 1 };
		tempTextTexture3->renderTexture(theRenderer, tempTextTexture3->getTexture(), &tempTextTexture3->getTextureRect(), &pos3, scale3);

		cTexture* tempTextTexture2 = theTextureMgr->getTexture("Game over line 3");
		SDL_Rect pos2 = { 10, 70, tempTextTexture2->getTextureRect().w, tempTextTexture2->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		tempTextTexture2->renderTexture(theRenderer, tempTextTexture2->getTexture(), &tempTextTexture2->getTextureRect(), &pos2, scale2);

		cTexture* tempTextTexture4 = theTextureMgr->getTexture("Highscoretext");
		SDL_Rect pos4 = { 200, 40, tempTextTexture4->getTextureRect().w, tempTextTexture4->getTextureRect().h };
		FPoint scale4 = { 1, 1 };
		tempTextTexture4->renderTexture(theRenderer, tempTextTexture4->getTexture(), &tempTextTexture4->getTextureRect(), &pos4, scale4);
	}
	break;

	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	if (gamestate == game)
	{
		// Update the visibility and position of each asteriod
		vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
		while (asteroidIterator != theAsteroids.end())
		{
			if ((*asteroidIterator)->isActive() == false)
			{
				asteroidIterator = theAsteroids.erase(asteroidIterator);
			}
			else
			{
				(*asteroidIterator)->update(deltaTime);
				++asteroidIterator;
			}
		}
		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/


		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
		{
			if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &theRocket.getBoundingRect()))

			{
				// if a collision respawn the asteroid and increase the score

				(*asteroidIterator)->setSpritePos({ rand() % 700, -600 });       //resets the asteroids position
				score++;                                                       //increses the score
				theTextureMgr->deleteTexture("Scoretext");              //deletes the score texture so it can update
				scoreprocessing = 0;                                  //lets the score update
				theSoundMgr->getSnd("explosion")->play(0);

				cout << "scored";
			}

			if (((*asteroidIterator)->getSpritePos().y) > 700)  //ends the game if the player lets and asteroid reach the bottom of the screen
			{
				(*asteroidIterator)->setSpritePos({ rand() % 700, -600 });
				
				if (score > highscore)         //if the player beat the high score it is saved as the new highscore
				{
					highscore = score;
					highscorestring = (to_string(highscore).c_str());
					theFile.writeDataToFile(highscorestring);
				}
				else
				{
					highscore = highscore;
				}
				gamestate = gameover;
			}
		}


		// Update the Rockets position
		theRocket.update(deltaTime);
	}

}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				break;

				case SDLK_UP:
				break;
				case SDLK_RIGHT:            //moves the player right
				{
					if (theRocket.getSpritePos().x < 950)    //stops the player from going off screen
					{
						theRocket.setSpritePos({ (theRocket.getSpritePos().x + 10), 600 });
					}
				}
				break;

				case SDLK_LEFT:          //moves the player left
				{
					if (theRocket.getSpritePos().x > 0)                 //stops the player from going off screen
					{
						theRocket.setSpritePos({ (theRocket.getSpritePos().x - 10), 600 });
					}
				}
				break;
				case SDLK_SPACE:
				{
					if (gamestate == MENU)
					{
						gamestate = game;
					}

					if (gamestate == gameover)
					{
						gamestate = MENU;        //takes the playe back to the menu
						score = 0;                    //re-sets the score
						scoreprocessing = 0;
						theTextureMgr->deleteTexture("Scoretext");              //deletes the score textures so they can update
						theTextureMgr->deleteTexture("Highscoretext");
						music = 0;                                     //lets the music play again
					}
				}
				break;
				default:
					break;
				}
				
			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

