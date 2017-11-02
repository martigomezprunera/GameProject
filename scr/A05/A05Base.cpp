#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <time.h>
#include <iostream>

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int, char*[]) {

	// --- INIT ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "No es pot inicialitzar SDL subsystems";
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };//
	if (!(IMG_Init(imgFlags) & imgFlags))throw"Error_SDL_img init";//
	if (TTF_Init() != 0)throw "error ftt";
	// --- WINDOW ---
	SDL_Window *window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (window == nullptr) throw "No es pot inicialitzar SDL_Window";
	// ---TIME---
	clock_t lastTime = clock();
	float timeDown = 10;
	float deltaTime = 0;
	
	// --- RENDERER ---
	SDL_Renderer *renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (renderer == nullptr) throw "No es pot inicialitzar SDL_Renderer";

	// --- SPRITES ---
	SDL_Texture *bgTexture{ IMG_LoadTexture(renderer,"../../res/img/bgCastle.jpg") };//
	if (bgTexture == nullptr)throw "no carrega text";//
	SDL_Rect bgRect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };//

	
	/*SDL_Texture *playerTexture{ IMG_LoadTexture(renderer,"../../res/img//kintoun.png") };
	if (playerTexture == nullptr)throw "Error Personatge text";
	SDL_Rect playerRect{0, 0, 350, 189};*/
	SDL_Rect playerTarget{ 0, 0, 100, 100 };

	// --- Animated Sprite ---
	#define FPS 60
	SDL_Texture *playerTexture{ IMG_LoadTexture(renderer,"../../res/img/spCastle.png") };
	SDL_Rect playerRect, playerPosition;													//PlayerRect = RECTANGULO SPRITES, PlayerPosition = LUGAR DONDE SE ENCUENTRA EL SPRITE EN PANTALLAS
	int textwidth, textheigh, framewidth, frameheigh;
	SDL_QueryTexture(playerTexture, NULL, NULL, &textwidth, &textheigh);
	framewidth = textwidth / 12;
	frameheigh = textheigh / 8;
	playerPosition.x = 20;
	playerPosition.y = 500;
	playerRect.x = playerRect.y = 0;
	playerPosition.h = playerRect.h = framewidth;
	playerPosition.w = playerRect.w = frameheigh;
	int frameTime = 0;



	// --- TEXT --- PL1
	//TTF_Font *font{ TTF_OpenFont("../../res//ttf/saiyan.ttf",50) };
	TTF_Font *font{TTF_OpenFont("../../res//ttf/MarioLuigi2.ttf",50)};
	if (font == nullptr)throw "jhih";
	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font,"Pl 1: ", SDL_Color {0,0,0,0}) };
	if (tmpSurf == nullptr)TTF_CloseFont(font), throw "squaad";
	SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(renderer,tmpSurf) };
	SDL_Rect textRect{ 25,25,tmpSurf->w,tmpSurf->h };
	SDL_FreeSurface(tmpSurf);
	//TTF_CloseFont(font);

	// --- TEXT --- PL2
	SDL_Surface *tmpSurf2{ TTF_RenderText_Blended(font,"Pl 2: ", SDL_Color{ 0,0,0,0 }) };
	if (tmpSurf2 == nullptr)TTF_CloseFont(font), throw "squaad";
	SDL_Texture *textTexture2{ SDL_CreateTextureFromSurface(renderer,tmpSurf2) };
	SDL_Rect textRect2{25,80,tmpSurf2->w,tmpSurf2->h};
	SDL_FreeSurface(tmpSurf2);
	TTF_CloseFont(font);

	// --- AUDIO ---
	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
	if (!Mix_Init(mixFlags)&mixFlags) throw "error mp3";
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		throw"error2audio";
	}
	Mix_Music *soundtrack{Mix_LoadMUS("../../res/au/mainTheme.mp3")};
	if (!soundtrack)throw "error3audio";
	Mix_VolumeMusic(MIX_MAX_VOLUME );
	Mix_PlayMusic(soundtrack, -1);//-1=numero de repeticions, -1=infinit

	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	while (isRunning){
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {

			case SDL_MOUSEMOTION:
				playerTarget.x = event.motion.x - 50;
				playerTarget.y = event.motion.y - 50;
							
				break;
			case SDL_QUIT:		isRunning = false; break;
			case SDL_KEYDOWN:	
				if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false; 
				if (event.key.keysym.sym == SDLK_w)
				{
					playerPosition.y = playerPosition.y - 10;
				}
				
				break;
			default:;
			}
		}
		

		// UPDATE
		//playerRect.x += (playerTarget.x - playerRect.x) / 10;//
		//playerRect.y += (playerTarget.y - playerRect.y) / 10;//
		frameTime++;
		if (FPS / frameTime <= 9)
		{
			frameTime = 0;
			playerRect.x += framewidth;
			/*if (playerRect.x >= textwidth/4)			//4 personajes en foto
			{
				playerRect.x = 0;
				playerRect.y += frameheigh;
				if (playerRect.y >= textheigh/2)
				{
					playerRect.y = 0;
				}
			}*/
		}

		/*deltaTime = (clock() - lastTime);
		lastTime = clock();
		deltaTime /= CLOCKS_PER_SEC;
		timeDown -= deltaTime;
		std::cout << timeDown << std::endl;*/


		// DRAW
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);

		//DRAW LINE
		SDL_RenderDrawLine(renderer, 0, 150, 800, 150);
		
		SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
		SDL_RenderCopy(renderer, textTexture2, nullptr, &textRect2);
		SDL_RenderCopy(renderer, playerTexture, &playerRect, &playerPosition);
		
		//Background
		
		//Animated Sprite
		//SDL_RenderPresent(renderer);
		SDL_RenderPresent(renderer);
	}

	// --- DESTROY ---
	Mix_CloseAudio();
	SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// --- QUIT ---
	Mix_Quit();
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}

