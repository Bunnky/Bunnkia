
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_image.lib")

#include "main.h"

#pragma warning(disable:4018)
#pragma warning(disable:4244)

void CreateOwnInfo();

SOCKET Socket;

struct sockaddr_in Server_Address;
struct sockaddr_in ownAddr;


//------------------------------------------------------------
PlayerDatabase				*pldatabase = NULL;
MonsterDatabase				*mondatabase = NULL;
NPCDatabase					*npcdatabase = NULL;

MonsterTypesStruct			MonsterTypes[m_MonsterTypes];
NPCTypesStruct				NPCTypes[m_NPCTypes];
//------------------------------------------------------------


//------------------------------------------------------------
ItemClass					iBackpack[MaxBackpackItems];
ItemClass					iBank[MaxBankItems];
ItemClass					iWeared[MaxWearedItems];
ItemClass					iGround[MaxItems];
//------------------------------------------------------------


//------------------------------------------------------------
SDLClass					sdl; 
ScreenClass					Screen;
//------------------------------------------------------------


//------------------------------------------------------------
Container					Backpack; 
Container					Bank;

Container					v_Body;
int							v_Body_Open;
BodyDatabase				*bodydatabase = NULL;
//------------------------------------------------------------


//------------------------------------------------------------
GraphsClass					G; 
//------------------------------------------------------------


//------------------------------------------------------------
ClientClass					Client; 
LoginClass					Login;
OwnInfo						P;
//------------------------------------------------------------


//------------------------------------------------------------
DrawClass					Draw; 
//------------------------------------------------------------


//------------------------------------------------------------
MapClass					Map; 
MapAddClass					MapInfo; 
TileClass					Tiles[m_Tiles];
//------------------------------------------------------------


//------------------------------------------------------------
DraggingClass				Dragging; 
//------------------------------------------------------------


//------------------------------------------------------------
MessageBoxClass				MB; 
CombatTextClass				CT;
//------------------------------------------------------------


//------------------------------------------------------------
WritingClass				Writing;
//------------------------------------------------------------


//------------------------------------------------------------
TargetClass					Target;
//------------------------------------------------------------


//------------------------------------------------------------
PACKET_ITEMMOVE				ItemMove;
PACKET_MiscPicClass			NewPic;
PACKET_NPC					NewNPC;
PACKET_BODY					NewBody;
PACKET_NEWDECAY				NewDecay;
PACKET_NEWAMOUNT			NewAmount;
PACKET_WEIGHTANDAC			NewWeight;
PACKET_STATS				NewStats;
PACKET_SHOPITEM				NewShopItem;
PACKET_STATS_BASE			NewStats_base;
PACKET_BODYINFOSTRUCT		NewBodyInfo;
PACKET_ITEM					New;
PACKET_NEWRESISTANCE		NewRes;

NewCombatTextClass			NewCT;
//------------------------------------------------------------


//------------------------------------------------------------
MiscPicClass				TmpPics[m_TmpPics];
//------------------------------------------------------------


//------------------------------------------------------------
UseableItemStruct			Useables[m_Items_per_type];
MiscItemStruct				Collectables[m_Items_per_type];
ArmorInfo					Armors[m_Item_Types][m_Items_per_type];
WeaponInfo					Weapons[m_Item_Types][m_Items_per_type];
//------------------------------------------------------------



//------------------------------------------------------------
shopclass					shop;
//------------------------------------------------------------


//------------------------------------------------------------
OnlineListClass				*GMTool = NULL;
//------------------------------------------------------------


//------------------------------------------------------------
DeedClass					*Clan = NULL;
//------------------------------------------------------------


//------------------------------------------------------------
QuickSlotClass				QK;
//------------------------------------------------------------

cStringTable				*cStrings = NULL;

SecureTradeClass			*st;

//------------------------------------------------------------
MapObjects					*Objects[256][256];
MapObjects					*obj_List = NULL;
ObjectsStruct				ObjectTypes[ m_Object_Types ];
//------------------------------------------------------------

//------------------------------------------------------------
BlackSmithingClass			*BS;
skillsclass					skill;
//------------------------------------------------------------

identifydatabase			*idatabase = NULL;

InfoMsgClass				*Info = NULL;

MiniMap						*Minimap = NULL;

RaceInfo					Races[6];
RaceClass					*Race = NULL;


SDL_Event event;

unsigned char GMMode = 0;
unsigned char Invisibility = 0;
unsigned char ListenNormal = 0;
unsigned char ListenWhisper = 0;
unsigned char ListenGuild = 0;

int MX, MY;

short autotX, autotY;
unsigned long autotTime;

bool UP = false, DOWN = false, LEFT = false, RIGHT = false;
bool Shift = false, Alt = false; //Tab = false;
bool Identify = false;
bool ListenToServerChat = true;
bool BrowseSkills = false;
bool IsPre = false;
bool IsGold = false;


char *buffer;
char *bigbuffer;


bool First_Packet_Received = false;

unsigned long arg;

unsigned long LstTimeOutMsg;

int GameState = 0; // Title screen

bool Resume21 = false;


unsigned long LastMessageTime; 
unsigned long LastDotTime;
unsigned char AmountofDots;

unsigned long WaterFrameTime;


extern unsigned long ExpTables[MaxLevel];
extern unsigned long SkillExpTables[MaxLevel];


extern unsigned char WaterposFrames[4][4];


bool BrowseHelp = false;
bool BrowseOpt = false;
bool isKicked = false;
bool isBanned = false;
bool isRanked = false;


extern bool ExitClick;
bool Use = false;
extern unsigned long ExitClickTime;
short UseType;
short UseTarget;


bool TexturedMouse = false;
bool IdentifySomeone = false;

short SafeZones[512][512];

int main(int argv, char *argc[])
//int WINAPI WinMain(HINSTANCE Instance, HINSTANCE hPrevInstance, LPSTR lpStr, int nShowCommand)
{
	Minimap = new MiniMap();

	if (!InitAll())
	{
		printf("An error has occurred, closing client.\r\n");
	}

	CreateDirectoryA("Screenshots", NULL);

	fprintf(stderr, "test\n");
	buffer = new char[256];
	bigbuffer = new char[2470];

	pldatabase = new PlayerDatabase;
	pldatabase->own = NULL;
	pldatabase->players = NULL;

	mondatabase = new MonsterDatabase;
	mondatabase->monsters = NULL;

	npcdatabase = new NPCDatabase;
	npcdatabase->npc = NULL;

	bodydatabase = new BodyDatabase;
	bodydatabase->body = NULL;

	BS = new BlackSmithingClass;
	BS->items = NULL;
	BS->on = false;
//	BS->init();

	st = new SecureTradeClass;

	idatabase = new identifydatabase;
	idatabase->list = NULL;


	Clan = new DeedClass;

	G.load_fonts();

	GMTool = new OnlineListClass;


	Race = new RaceClass();

	memset(&P, 0, sizeof(P));

	shop.init();

	Clan->Selected = -1;

	cStrings = new cStringTable();
	cStrings->loadBin("data\\data10.dat");
	
	GMTool->Selected = NULL;
	GMTool->Scroll = 0;

	Info = new InfoMsgClass();

	LoadMap();

	CreateTables();

	CreateBlueprints();
	
	atexit(saveSettings);

	LastMessageTime = SDL_GetTicks();
	LstTimeOutMsg = SDL_GetTicks();

	bool Done = false;

	Login.Prepare();

	GameState = 1;


	// This is weird.. When I started coding Vorlia, I thought I'd make menusystem to it's own loop.
	// Reason was simply: I could easily create build-in autoupdater.
	// Check Cyldan Client if you want to see integrated menusystem into gameloop. Maybe import whole
	// system here.


	while (!Done)
	{
		switch (GameState)
		{
		case 0: // Title screen
			bool InTitle;
			InTitle = true;

			unsigned long TextTime;
				TextTime = SDL_GetTicks();

			unsigned char TextMode;
				TextMode = 0;
			short tmpcol;

			while (InTitle)
			{
				while (SDL_PollEvent(&event))
				{				
					if (event.type == SDL_KEYUP || event.type == SDL_MOUSEBUTTONUP)
					{
                        InTitle = false;					
					}
					if (event.type == SDL_QUIT) exit(1);
				}

				if (SDL_GetTicks() - TextTime > 800)
				{
					if (TextMode == 0) TextMode = 1;
					else TextMode = 0;
					TextTime = SDL_GetTicks();
				}

				SDL_BlitSurface(Login.bGround, NULL, Screen.screen, NULL);

				if (TextMode == 0)
					tmpcol = col_gray;
				if (TextMode == 1)
					tmpcol = col_white;

				sdl.text("Press any key.", 400 - BFont_TextWidth("Press any key.")/2, 578, tmpcol);
				
				Screen.Flush();
                
				SDL_Delay(5);
			}

			GameState = 1;
			break;
		case 1: // Main menu
			bool InMain;
			InMain = true;

			short mx, my;

			while (InMain)
			{
				while (SDL_PollEvent(&event))
				{
					mx = event.motion.x;
					my = event.motion.y;
					if (event.type == SDL_MOUSEBUTTONUP)
					{
						if (mx >= 368 && mx <= 435 && my >= 291 && my <= 304)
						{
							GameState = 2;
							InMain = false;
						}
						if (mx >= 311 && mx <= 495 && my >= 320 && my <= 333)
						{
							GameState = 6;
							InMain = false;
						}
						if (mx >= 375 && mx <= 421 && my >= 354 && my <= 367)
						{
							exit(1);
						}

					}
					if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
					{ 
						GameState = 2;
						InMain = false;
					}

					if (event.type == SDL_QUIT) exit(1);

/*					if (event.key.keysym.sym == SDLK_t && event.type == SDL_KEYUP)
					{
						ofstream filetti("levelit.txt", ios::app);
						filetti << "Level, Exp" << endl;
						int i;
						for (i = 0; i < 201; i++)
						{
							filetti << "level: " << i << " === exp: " << ExpTables[i] << endl;														
						}
					}*/
				}


				SDL_BlitSurface(Login.bGround, NULL, Screen.screen, NULL);

				sdl.dst.x = 250;
				sdl.dst.y = 150;
				sdl.dst.h = 300;
				sdl.dst.w = 300;

				SDL_BlitSurface(Login.mainmenu, NULL, Screen.screen, &sdl.dst);

				Screen.Flush();
                
				SDL_Delay(5);
			}

			break;
		case 2: // Login

			bool InLogin;
			InLogin = true;


			bool RealError;
			RealError = false;

			while (InLogin)
			{
				while (SDL_PollEvent(&event))
				{				
					if (event.type == SDL_KEYDOWN)
					{
						if (event.key.keysym.sym == SDLK_BACKSPACE)
						{
							Writing.BP_on = true;
							Writing.BP_time = SDL_GetTicks();
		
							Writing.BP();
						}
						else if (event.key.keysym.sym != SDLK_RETURN)
						{
							Writing.KeyboardButtonUp(event.key.keysym.sym, event.key.keysym.unicode);

						}
						if (event.key.keysym.sym == SDLK_RETURN && !RealError)
						{
							if (Writing.Mode == 10) Writing.Mode = 11;
							else if (Writing.Mode == 11) 
							{
								Connect_To_Server();
		
								InLogin = false;
	
								p_Loginpacket pLogin;

								memset(&pLogin, 0, sizeof(pLogin));
								strcpy(pLogin.Name, Login.AccName);
								strcpy(pLogin.Pass, Login.AccPass);
								pLogin.Version = _VERSION;

								buffer[0] = 2;
								memcpy(&buffer[1], &pLogin, sizeof(pLogin));
								send(Socket, buffer, 1 + sizeof(pLogin), 0);

								RealError = true;
							}
						}
					}

					if (event.type == SDL_KEYUP)
					{
						if (event.key.keysym.sym == SDLK_BACKSPACE)
							Writing.BP_on = false;
					
					}

					if (event.type == SDL_MOUSEBUTTONDOWN)
					{
						int x,y;
						x = event.motion.x;
						y = event.motion.y;
						//------------------------------------------------------------------------------
						//	Account Box
						//------------------------------------------------------------------------------
						if (x >= 394 && x <= 542 && y >= 257 && y <= 276)
						{
							Writing.Mode = 10;
						}
						//------------------------------------------------------------------------------
						//	Password Box
						//------------------------------------------------------------------------------
						if (x >= 394 && x <= 542 && y >= 289 && y <= 308)
						{
							Writing.Mode = 11;						
						}
						//------------------------------------------------------------------------------
						//	Back Button
						//------------------------------------------------------------------------------
						if (x >= 453 && x <= 500 && y >= 403 && y <= 420)
						{
							GameState = 1;
							InLogin = false;
						}
						//------------------------------------------------------------------------------
						//	Login Button
						//------------------------------------------------------------------------------
						if (x >= 290 && x <= 360 && y >= 403 && y <= 420 && !RealError)
						{

							Connect_To_Server();

							InLogin = false;

							p_Loginpacket pLogin;

							memset(&pLogin, 0, sizeof(pLogin));
							strcpy(pLogin.Name, Login.AccName);
							strcpy(pLogin.Pass, Login.AccPass);
							pLogin.Version = _VERSION;

							buffer[0] = 2;
							memcpy(&buffer[1], &pLogin, sizeof(pLogin));
							send(Socket, buffer, 1 + sizeof(pLogin), 0);
							
							RealError = true;
							GameState = 21;
						}
					
					}

				}

				if (SDL_GetTicks() - Writing.BP_time > 75 && Writing.BP_on)
					Writing.BP();

				Login.DrawPics();
				Login.DrawTexts();

				Screen.Flush();
                
				SDL_Delay(5);
			}

			break;
		case 21: // Waiting reply for Login packet

			Resume21 = false;

			printf("Waiting reply for loginpacket...\n");

			LastDotTime = 0;
			AmountofDots = 0;

			int DotCounter;

			while (!Resume21)
			{
				if (SDL_GetTicks() - LstTimeOutMsg > TimeOutTime)
				{
					buffer[0] = 98;
					send(Socket, buffer, 1, 0);
					LstTimeOutMsg = SDL_GetTicks();
				}

				HandleMessage();

				while (SDL_PollEvent(&event))
				{
					// "Cancel" button.



					////				
				}
				SDL_Delay(50);
                
				if (SDL_GetTicks() - LastDotTime > 1000)
				{
					AmountofDots++;

					Login.DrawPics();
					Login.DrawTexts();
				
					sdl.text("Connecting", 5, 572, col_white);

					for (DotCounter = 0; DotCounter < AmountofDots; DotCounter++)
					{
						sdl.text(".", 5 + BFont_TextWidth("Connecting") + DotCounter *3, 572, col_white);
					}

					if (DotCounter >= 10)
					{
						sdl.text("Connecting", 5, 572, col_white);

						for (DotCounter = 0; DotCounter < AmountofDots; DotCounter++)
						{
							sdl.text(".", 5 + BFont_TextWidth("Connecting") + DotCounter *3, 572, col_white);
						}

						sdl.text("Unable to connect to server.", 5 + BFont_TextWidth("Connecting") + DotCounter * 3, 572, col_white);

						Screen.Flush();
		
						SDL_Delay(2000);
                        Resume21 = true;
						GameState = 2;						
					}


						
					Screen.Flush();


                    
					////


					LastDotTime = SDL_GetTicks();
				}
			}

           
			break;
		case 3: // Charchoose

			bool Rrrr; Rrrr = false;

			while (!Rrrr)
			{
				if (SDL_GetTicks() - LstTimeOutMsg > TimeOutTime)
				{
					buffer[0] = 98;
					send(Socket, buffer, 1, 0);
					LstTimeOutMsg = SDL_GetTicks();
				}
				while (SDL_PollEvent(&event))
				{
					if (event.type == SDL_KEYDOWN)
					{
						if (event.key.keysym.sym == SDLK_RETURN)
						{
							if (Login.CharChooseInfo.Used[Login.CurrentChoice] == 1)
							{
								// Existing char is chosen.

								buffer[0] = 3;
								buffer[1] = Login.CurrentChoice;

								send(Socket, buffer, 2, 0);
								GameState = 31;
								Rrrr = true;
							}
							else
							{
								// Char creation is needed.
								GameState = 4;
								Rrrr = true;
							}
						}					
					}
					if (event.type == SDL_MOUSEBUTTONUP)
					{
                        int x,y;
						x = event.motion.x;
						y = event.motion.y;

						if (x >= 177 && x <= 345 && y >= 186 && y <= 334)
						{
//							if (CharChooseInfo.Used[0] == 1)
							Login.CurrentChoice = 0;
						}

						if (x >= 455 && x <= 623 && y >= 186 && y <= 334)
						{
//							if (CharChooseInfo.Used[1] == 1)
							Login.CurrentChoice = 1;
						}

                        if (x >= 290 && x <= 361 && y >= 468 && y <= 485)
						{
							// Char chosen

							if (Login.CharChooseInfo.Used[Login.CurrentChoice] == 1)
							{
								// Existing char is chosen.

								buffer[0] = 3;
								buffer[1] = Login.CurrentChoice;

								send(Socket, buffer, 2, 0);
								GameState = 31;
								Rrrr = true;
							}
							else
							{
								// Char creation is needed.
								GameState = 4;
								Rrrr = true;
							}

                            						
						}
                        if (x >= 453 && x <= 504 && y >= 468 && y <= 485)
						{
							// Exit

							buffer[0] = 120;
							send(Socket, buffer, 1, 0);
                            
							exit(1);
						}
					}				
				}

				Login.DrawCharChoosePics();
				Login.DrawCharChooseTexts();


				Screen.Flush();

				SDL_Delay(50);
			}
			

			break;

		case 31: // Waiting own info packet from server so we can run mainloop.

			bool Waiting;
			Waiting = true;

			while (Waiting)
			{
				if (SDL_GetTicks() - LstTimeOutMsg > TimeOutTime)
				{
					buffer[0] = 98;
					send(Socket, buffer, 1, 0);
					LstTimeOutMsg = SDL_GetTicks();
				}
				while (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT) exit(1);
				}

				SDL_Delay(2);

				HandleMessage();
				if (GameState == 5) Waiting = false;
			}

			break;

		case 4: // Char creation
			bool InCharCreation;
			InCharCreation = true;

			Writing.Writing = true;
			Writing.Mode = 12;
			memset(&Login.CharName, 0, sizeof(Login.CharName));
			char tmp[120];

			while (InCharCreation)
			{
				if (SDL_GetTicks() - LstTimeOutMsg > TimeOutTime)
				{
					buffer[0] = 98;
					send(Socket, buffer, 1, 0);
					LstTimeOutMsg = SDL_GetTicks();
				}
				while (SDL_PollEvent(&event))
				{				
					if (event.type == SDL_KEYDOWN)
					{
						if (event.key.keysym.sym == SDLK_BACKSPACE)
						{
							Writing.BP_on = true;
							Writing.BP_time = SDL_GetTicks();
		
							Writing.BP();
						}
						else
						{
							Writing.KeyboardButtonUp(event.key.keysym.sym, event.key.keysym.unicode);

						}
					}

					if (event.type == SDL_KEYUP)
					{
						if (event.key.keysym.sym == SDLK_BACKSPACE)
							Writing.BP_on = false;
					
					}
					if (event.type == SDL_MOUSEBUTTONUP)
					{
                        short mx, my;
						mx = event.motion.x;
						my = event.motion.y;
                        
						Race->mClick();

						if (mx >= 276 && mx <= 355 && my >= 466 && my <= 500 && Writing.LoginHandler[2] > 2)
						{
							p_CCreatepacket pCCreate;
                            
							pCCreate.Avatar = Race->chosenAvatar;
							strcpy(pCCreate.Name, Login.CharName);
							pCCreate.Race = Race->chosenRace;
							

							buffer[0] = 4;
							memcpy(&buffer[1], &pCCreate, sizeof(pCCreate));

							send(Socket, buffer, 1 + sizeof(pCCreate), 0);

							GameState = 2;
                            InCharCreation = false;
							//exit(1);// this is in here cause there is probably an error in the server while changing from character creation screen to going into the world
						}
					}
					if (event.type == SDL_QUIT) exit(1);
				}

				if (SDL_GetTicks() - Writing.BP_time > 75 && Writing.BP_on)
					Writing.BP();

				SDL_BlitSurface(Login.bGround, NULL, Screen.screen, NULL);

				sdl.dst.x = 150;
				sdl.dst.y = 100;
				sdl.dst.h = 400;
				sdl.dst.w = 500;

				SDL_BlitSurface(Login.charCreation, NULL, Screen.screen, &sdl.dst);


				Race->dRaces();
				
				
				sprintf(tmp, "%sÉ|", Login.CharName);
				sdl.text(tmp, 262, 198, col_grey);
//				BFont_PrintStringFont(Screen.screen, F.white, 262, 198, "%s|", Login.CharName);

				Screen.Flush();
                
				SDL_Delay(5);
			}

			break;
		case 6: // Account creation
			bool InAccCreation;
			InAccCreation = true;

			Resume21 = false;

			Login.PrepareAccCreation();

			memset(&Login.AccName, 0, sizeof(Login.AccName));
			memset(&Login.AccPass, 0, sizeof(Login.AccPass));

			while (InAccCreation)
			{
				while (SDL_PollEvent(&event))
				{				
					if (event.type == SDL_KEYDOWN)
					{
						if (Login.textedits->focus != NULL)
							Login.textedits->focus->keypress(event.key.keysym.unicode);
					}

					if (event.type == SDL_KEYUP)
					{
						if (Login.textedits->focus != NULL)
							Login.textedits->focus->keypress(event.key.keysym.unicode);
//						if (event.key.keysym.sym == SDLK_BACKSPACE)
//							Writing.BP_on = false;
					
					}

					if (event.type == SDL_MOUSEBUTTONDOWN)
					{
						int x,y;
						x = event.motion.x;
						y = event.motion.y;

						Login.textedits->mouseclick(x, y);
						
						if (x >= 290 && x <= 360 && y >= 353 && y <= 370 && GameState != 3)
						{
							// Create button

							Connect_To_Server();

							GameState = 3;

							p_ACreationpacket pACreate;
							memset(&pACreate, 0, sizeof(pACreate));

							textedit *tmp;
							tmp = Login.textedits->getbyid(1000001);
							strcpy(pACreate.Name, tmp->txt);

							tmp = Login.textedits->getbyid(1000002);
							strcpy(pACreate.Pass, tmp->txt);

							//tmp = Login.textedits->getbyid(1000003);
							//strcpy(pACreate.Email, tmp->txt);
							pACreate.Version = _VERSION;
							
							buffer[0] = 1;
							memcpy(&buffer[1], &pACreate, sizeof(pACreate));
							send(Socket, buffer, 1 + sizeof(pACreate), 0);
						}

						if (x >= 453 && x <= 500 && y >= 403 && y <= 420)
						{
							GameState = 1;
							InAccCreation = false;
						}
					
					}

				}
				if (GameState == 3)
				{
					HandleMessage();
				}

				if (Resume21 == true) InAccCreation = false;

				if (SDL_GetTicks() - Writing.BP_time > 75 && Writing.BP_on)
					Writing.BP();

				SDL_BlitSurface(Login.bGround, NULL, Screen.screen, NULL);

				sdl.dst.x = 250;
				sdl.dst.y = 150;
				sdl.dst.h = 300;
				sdl.dst.w = 300;

				SDL_BlitSurface(Login.accCreation, NULL, Screen.screen, &sdl.dst);

				Login.textedits->draw();

				Screen.Flush();
                
				SDL_Delay(5);
			}

			Login.CleanupAccCreation();

			break;
		case 5: // Game

			Game();

			Done = true;
			break;		
		}

	}
    
	return 1;
}

bool Done2 = false;
extern short UseTarget;

int Game()
{
	atexit(CleanUp);

	Writing.ChattingMode = 1;
	Writing.BP_on = false;
	Writing.BP_time = SDL_GetTicks();

	//####################################################################################
	// Here will be all "requests", for example MOTD, other players on screen, items, ...
	//####################################################################################


//	TexturedMouse = false;
//	SDL_ShowCursor( SDL_ENABLE );

	TexturedMouse = true;
	SDL_ShowCursor( SDL_DISABLE );

	int t1;
	t1 = 0;

	UseTarget = -1;

	unsigned long frame_time;
	frame_time = SDL_GetTicks();

	long frame_count, frames;
	frame_count = 0;
	frames = 0;


	skill.chosen = NULL;
	skill.l_list = NULL;


	Writing.Writing = false;
	Writing.Stop();
	IdentifySomeone = false;

	LstTimeOutMsg = SDL_GetTicks() + 4000;

	buffer[0] = 5;
	send(Socket, buffer, 1, 0);

	skill.c_skill_list();

	unsigned long testi = 0;

	while (!Done2)
	{
		if (SDL_GetTicks() - LstTimeOutMsg > TimeOutTime)
		{
			buffer[0] = 98;
			send(Socket, buffer, 1, 0);
			LstTimeOutMsg = SDL_GetTicks();
		}


		Client.UpdateScreen++;
//		Client.tmp++;

		frame_count++;

		// Networking stuffs

		for (t1 = 0; t1 < 16; t1++)
		{
			HandleMessage();
		}


		if (First_Packet_Received)
			{
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_MOUSEBUTTONDOWN:
					
					Client.mbut_down();

					break;

				case SDL_MOUSEBUTTONUP:

					Client.mbut_up();

					break;
                		
				case SDL_KEYDOWN: // Keydown and Keyup could be in own function too.. just like mousebuttonup and mousebuttondown

					if (event.key.keysym.sym == SDLK_UP)
						UP = true;
					if (event.key.keysym.sym == SDLK_w && !Writing.Writing && Client.WASD)
						UP = true;
					if (event.key.keysym.sym == SDLK_DOWN)
						DOWN = true;
					if (event.key.keysym.sym == SDLK_s && !Writing.Writing && Client.WASD)
						DOWN = true;
					if (event.key.keysym.sym == SDLK_LEFT)
						LEFT = true;
					if (event.key.keysym.sym == SDLK_a && !Writing.Writing && Client.WASD)
						LEFT = true;
					if (event.key.keysym.sym == SDLK_RIGHT)
						RIGHT = true;
					if (event.key.keysym.sym == SDLK_d && !Writing.Writing && Client.WASD)
						RIGHT = true;

/*					if (event.key.keysym.sym == SDLK_TAB)
						Tab = true;		*/	

					if (event.key.keysym.sym == SDLK_RSHIFT)
						Shift = true;

					if (event.key.keysym.sym == SDLK_LSHIFT)
						Shift = true;

					if (event.key.keysym.sym == SDLK_RCTRL)
						Identify = true;

					if (event.key.keysym.sym == SDLK_LCTRL)
						Identify = true;

					if (event.key.keysym.sym == SDLK_RALT)
						Alt = true;

					if (event.key.keysym.sym == SDLK_LALT)
						Alt = true;

					if (event.key.keysym.sym == SDLK_q && !Writing.Writing)
					{
						if (Client.AttackMode == ATTACK)
						{
							Client.AttackMode = PEACE;
						}
						else
						{
							Client.AttackMode = ATTACK;
						}					
					}

					if (event.key.keysym.sym == SDLK_F1 && !Writing.Writing) 
					{
						if (BrowseHelp)
							BrowseHelp = false;
						else
							BrowseHelp = true;
					}

					if (event.key.keysym.sym == SDLK_ESCAPE && !Writing.Writing) 
					{
						if (BrowseOpt)
							BrowseOpt = false;
						else
							BrowseOpt = true;
					}

					if (event.key.keysym.sym == SDLK_F3 && !Writing.Writing)
					{
						if (Client.TargetProtectionMode)
						{
							sprintf(MB.NewMsg, "Target protection mode OFF");
							MB.NewType = 7;
							MB.Update();
							Client.TargetProtectionMode = false;
						}
						else
						{
							sprintf(MB.NewMsg, "Target protection mode ON");
							MB.NewType = 7;
							MB.Update();
							Client.TargetProtectionMode = true;
						}
					}

					if (event.key.keysym.sym == SDLK_TAB && Writing.Writing && Writing.Mode == 1)
					{
						Writing.CheckTab();
					}
	
					if (event.key.keysym.sym == SDLK_BACKSPACE && (Writing.Writing || (shop.pending && shop.pending_mode == shopmode_pendingamount)))
					{
						Writing.BP_on = true;
						Writing.BP_time = SDL_GetTicks();
	
						Writing.BP();
					}

					if (Writing.Writing || (shop.pending && shop.pending_mode == shopmode_pendingamount))
						Writing.KeyboardButtonUp(event.key.keysym.sym, event.key.keysym.unicode);

					if (!Writing.Writing && !shop.on)
					{
						if (event.key.keysym.sym == SDLK_1 ||
							event.key.keysym.sym == SDLK_2 ||
							event.key.keysym.sym == SDLK_3 ||
							event.key.keysym.sym == SDLK_4 ||
							event.key.keysym.sym == SDLK_5 ||
							event.key.keysym.sym == SDLK_6 ||
							event.key.keysym.sym == SDLK_7 ||
							event.key.keysym.sym == SDLK_8 ||
							event.key.keysym.sym == SDLK_9 ||
							event.key.keysym.sym == SDLK_0)
						QK.CheckQuickSlots(event.key.keysym.sym);
					}


					if (Writing.Writing == false && !shop.pending && !shop.on)
					{
						switch (event.key.keysym.sym)
						{
						case SDLK_e:

							//MB.Expanded = true;
							
							break;

						case SDLK_t:

							Writing.Writing = true;
							Writing.Mode = 1;
							memset(&Writing.Txt, 0, sizeof(Writing.Txt));
							Writing.KeyHandler = 0;
							Writing.WritingError = false;

							break;
						case SDLK_k:

							skill.v_skills = !skill.v_skills;

							break;

						case SDLK_l:
							if (GMMode <= 1) break;

							if (GMTool->On)
							{
								GMTool->On = false;							
								GMTool->bBackpackView = false;
								GMTool->bBankView = false;	
							} else {
								GMTool->On = true;
//								GMTool->GetListView();
							}
							break;
							
						case SDLK_F4:

							if (st->on) break;

							if (SDL_GetTicks() - st->petime < 5000)
							{
								st->informserver_accepttrade();
								MB.NewMessage("You accept the trade offer.", 2);
								break;
							}

							UseType = 10;							
							Use = true;

							MB.NewMessage("Who do you wish to trade with?", 4);

							break;

						case SDLK_i:

							/*
							if (GMMode > 1)
							{
								buffer[0] = 60;
								buffer[1] = 2;
								send(Socket, buffer, 2, 0);
							}
							*/

							break;

						case SDLK_c:

							sprintf(MB.NewMsg, "Mouse at: [%d [%d.", MX, MY);
							MB.NewType = 7;
							MB.Update();

							break;
						case SDLK_F10:

							Screen.SwitchFullscreen();

							break;

						case SDLK_F12:

							Screen.shot();

							break;
						case SDLK_g:

							if (P.Guild > 0)
							{
								if (Clan->On)
								{
									Clan->On = false;
									Use = false;
									Clan->Wait_Action = false;
									Clan->SelectedAction = 0;
								}
								else
									Clan->On = true;   
							}

							break;
						case SDLK_F5:


							if (Client.CombatTextOn)
							{
								Client.CombatTextOn = false;

								strcpy(MB.NewMsg, "Combat Text: OFF");
								MB.NewType = 7;
								MB.Update();
							}
							else
							{
								Client.CombatTextOn = true;

								strcpy(MB.NewMsg, "Combat Text: ON");
								MB.NewType = 7;
								MB.Update();
							}

							break;
						case SDLK_F6:
/*
							if (TexturedMouse)
							{
								TexturedMouse = false;
								SDL_ShowCursor( SDL_ENABLE );
							}
							else
							{
								TexturedMouse = true;
								SDL_ShowCursor( SDL_DISABLE );
							}
*/
							break;
						case SDLK_m:

							Minimap->On = !Minimap->On;

							break;
						case SDLK_F11:

							Minimap->Size = !Minimap->Size;

							break;
						case SDLK_SPACE:

							OpenOwnBody();

							break;
						}
					}

					break;
				case SDL_KEYUP:

					if (event.key.keysym.sym == SDLK_UP)
						UP = false;
					if (event.key.keysym.sym == SDLK_w && Client.WASD)
						UP = false;
					if (event.key.keysym.sym == SDLK_DOWN)
						DOWN = false;
					if (event.key.keysym.sym == SDLK_s && Client.WASD)
						DOWN = false;
					if (event.key.keysym.sym == SDLK_LEFT)
						LEFT = false;
					if (event.key.keysym.sym == SDLK_a && Client.WASD)
						LEFT = false;
					if (event.key.keysym.sym == SDLK_RIGHT)
						RIGHT = false;
					if (event.key.keysym.sym == SDLK_d && Client.WASD)
						RIGHT = false;
	
					if (event.key.keysym.sym == SDLK_BACKSPACE)
						Writing.BP_on = false;

					//if (event.key.keysym.sym == SDLK_TAB)
					//	Tab = false;

					if (event.key.keysym.sym == SDLK_RSHIFT)
						Shift = false;

					if (event.key.keysym.sym == SDLK_LSHIFT)
						Shift = false;

					if (event.key.keysym.sym == SDLK_RCTRL)
						Identify = false;

					if (event.key.keysym.sym == SDLK_LCTRL)
						Identify = false;

					if (event.key.keysym.sym == SDLK_RALT)
						Alt = false;

					if (event.key.keysym.sym == SDLK_LALT)
						Alt = false;

					if (event.key.keysym.sym == SDLK_e)
						//MB.Expanded = false;

					break;
				}		
			}
	
			if (SDL_GetTicks() - CT.lstupdate > CTUpdateTime)
				CT.TimeUpdate();			
			
			if (SDL_GetTicks() - Writing.BP_time > 75 && Writing.Writing && Writing.BP_on)
				Writing.BP();
	

			if (SDL_GetTicks() - ExitClickTime > 4000)
			{
				ExitClick = false;
				ExitClickTime = 0;
			}

			if (GMMode > 1 && Client.NoLos == false)
				Client.NoLos = true;

			Draw.DrawBG();
	
			Draw.DrawWorld();
			Draw.DrawStats();

			//if (QK.ShowQuickSlots)
			//	QK.Draw();

			if (Client.CombatTextOn)
				CT.Print();

	        Draw.DrawFPS();

			Draw.d_items();

			bodydatabase->drawbody();

			npcdatabase->drawnpc();

			mondatabase->drawmon();

			pldatabase->drawpl();
			pldatabase->drawnames();

			Info->draw();

			MB.Print();
			
			if (Bank.On)
				Draw.DrawBank();
			
			if (Target.DrawTarget)
				Draw.DrawTarget();

			if (Writing.Writing == true && Writing.Mode == 1)
				Draw.DrawChatBox();
						
			if (Minimap->On)
				Minimap->draw();

			if (Dragging.On == true && Dragging.Mode == ITEM)
				Draw.DrawDraggingItem();

			if (Dragging.Split && Writing.Writing && Writing.Mode == 2)
				Draw.DrawSplit();

			Draw.DrawTmpPics();

			idatabase->draw();

			if (SDL_GetTicks() - WaterFrameTime > 400 - rand() % 300)
			{
				int i;
				for (i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						WaterposFrames[i][j] = rand() % 4;
					}
				}
				WaterFrameTime = SDL_GetTicks();
			}

			if (skill.v_skills && !shop.on)
				skill.d_skillwindow();

			shop.draw();

			if (st->on)
				st->draw();


			if (Clan->On)
			{
				Clan->Deed();
				Clan->D_Names();
				//if (Clan->IsLeader)
					Clan->Actions();
			}

			if (BS->on)
				BS->draw();

			if (GMTool->On)
				GMTool->Draw_bGround();

			if (GMTool->dDMenu)
				GMTool->dDDMenu();		

			if (GMTool->On)
			{			
				if (GMTool->bBackpackView)
					GMTool->dBackpackView();
				
				if (GMTool->bBankView)
					GMTool->dBankView();
			}

			if (v_Body.On)
				Draw.DrawOpenBody();

			MouseOver();

			if (isKicked)
				Draw.Kicked();
			if (isBanned)
				Draw.Banned();
			if (isRanked)
				Draw.Ranked();
			if (BrowseHelp)
				Draw.Help();
			if (BrowseOpt)
				Draw.Opt();
			if (Minimap->Size)
				Minimap->drawFull();
			//if (MB.Expanded)
			//	MB.Print();

			if (TexturedMouse)
				DrawCursor();
				
			Client.UpdateScreen = 0;
			Client.Update = true;

            CheckFrame();
		}

		if (SDL_GetTicks() - Client.LastMove > 200)
		{
			if (UP || DOWN || LEFT || RIGHT)
			{
				Client.KeyboardMove();
				Client.LastMove = SDL_GetTicks();
			}
			else if	(Client.MouseMoving)
			{
				Client.Mousemove();
				Client.LastMove = SDL_GetTicks();
			}
		}

		if (SDL_GetTicks() - testi > 15)
		{
			Screen.Flush();
			testi = SDL_GetTicks();
		}
        SDL_Delay(1);

		if (SDL_GetTicks() - frame_time >= 1000)
		{
			Client.FPS = frame_count;
		    frame_count = 0;
			frame_time = SDL_GetTicks();
			Client.Update = true;
		}
		
		if (SDL_GetTicks() - LastMessageTime > 25000)
		{
			printf("Timeout, closing.\n");
			CleanUp();
			exit(1);
		}
	}
	return 1;
}

void DrawCursor()
{
	if (Client.AttackMode == PEACE)
		sdl.Cursor = 0;
	if (Client.AttackMode == ATTACK)
		sdl.Cursor = 1;
	if (Identify)
		sdl.Cursor = 2;

	if (Use)
	{
		if (SDL_GetTicks() - sdl.UseCursorTime > 250)
		{
			sdl.UseCursorState++;
			if (sdl.UseCursorState == 4) sdl.UseCursorState = 0;
			sdl.UseCursorTime = SDL_GetTicks();
		}

		sdl.Cursor = 3 + sdl.UseCursorState;

		sdl.dst.x = MX - sdl.cursors[sdl.Cursor].w/2;
		sdl.dst.y = MY - sdl.cursors[sdl.Cursor].h/2;
	}
	else
	{
		sdl.dst.x = MX;
		sdl.dst.y = MY;
	}
	
	sdl.dst.h = sdl.cursors[sdl.Cursor].h;
	sdl.dst.w = sdl.cursors[sdl.Cursor].w;

	SDL_BlitSurface(G.Cursors, &sdl.cursors[sdl.Cursor], Screen.screen, &sdl.dst);



}


void CheckDragging()
{
	switch (Dragging.Mode)
	{
	case BACKPACK: // Backpack

		if (!Backpack.Docked)
		{
			Backpack.X = MX - Dragging.x;
			Backpack.Y = MY - Dragging.y;

			if (Backpack.X <= 0)
				Backpack.X = 0;
			if (Backpack.Y <= 0)
				Backpack.Y = 0;

			if (Backpack.X + Backpack_w > 800) Backpack.X = 800 - Backpack_w;
			if (Backpack.Y + Backpack_h > 600) Backpack.Y = 600 - Backpack_w;
		}

		break;
	case BANK: // Bank

		if (!Bank.Docked)
		{
			Bank.X = MX - Dragging.x;
			Bank.Y = MY - Dragging.y;

			if (Bank.X <= 0)
				Bank.X = 0;
			if (Bank.Y <= 0)
				Bank.Y = 0;

			if (Bank.X + Bank_w > 800) Bank.X = 800 - Bank_w;
			if (Bank.Y + Bank_h > 600) Bank.Y = 600 - Bank_h;
		}

		break;
	case MINIMAP: // Minimap

		if (!Minimap->Docked)
		{
			Minimap->X = MX - Dragging.x;
			Minimap->Y = MY - Dragging.y;

			if (Minimap->X <= 0)
				Minimap->X = 0;
			if (Minimap->Y <= 0)
				Minimap->Y = 0;

			if (Minimap->X + Minimap_w > 800) Minimap->X = 800 - Minimap_w;
			if (Minimap->Y + Minimap_h > 600) Minimap->Y = 600 - Minimap_w;
//			if (Minimap->Size)
//			{
//				if (Minimap->X + Minimap_w_BIG > 800) Minimap->X = 800 - Minimap_w_BIG;
//				if (Minimap->Y + Minimap_h_BIG > 600) Minimap->Y = 600 - Minimap_h_BIG;
//			}
		}

		break;
	case IDENTIFY: // Identify

		Dragging.iddrag->x = MX - Dragging.x;
		Dragging.iddrag->y = MY - Dragging.y;

		if (Dragging.iddrag->x <= 0)
			Dragging.iddrag->x = 0;
		if (Dragging.iddrag->y <= 0)
			Dragging.iddrag->y = 0;

		if (Dragging.iddrag->x + 119 > 800) Dragging.iddrag->x = 800 - 119;
		if (Dragging.iddrag->y + 150 > 600) Dragging.iddrag->y = 600 - 150;

		break;
	}


}

void CreateOwnInfo()
{
	PlayerNode *tmp;
	tmp = new PlayerNode;

	tmp->avatar = P.Avatar;
	tmp->hpleft = P.HPLeft;
	tmp->hpmax = P.HPMax;
	tmp->id = P.ID;
	strcpy(tmp->name, P.Name);
	tmp->reputation = P.Reputation;
	tmp->x = P.X;
	tmp->y = P.Y;
	tmp->wanted = P.Wanted;
	tmp->race = P.Race;

	tmp->database = pldatabase;
	tmp->addto();

	calc_LOS();
}


int FilterEvents(const SDL_Event *event)
{
	// Exit if exited.
	if ( event->type == SDL_QUIT)
	{
		exit(1);
 	}

		// Have some special use/checks for moving mouse.
	if ( event->type == SDL_MOUSEMOTION) 
	{
		MX = event->motion.x;
		MY = event->motion.y;

		if (Dragging.On)
			CheckDragging();

		if (Shift && Client.AttackMode == ATTACK)
		{
			if (MX >= 7 && MX <= 427 && MY >= 7 && MY <= 427)
			{
				if (autotX != (MX-7)/20 || autotY != (MY-7) /20 || SDL_GetTicks() - autotTime > 50)
					CheckAutoTarget();
			}
		}

        return 0;
    }

	return 1;
}

void CheckAutoTarget()
{
	autotX = (MX - 7) / 20;
	autotY = (MY - 7) / 20;
	autotTime = SDL_GetTicks();

	MonsterClass *mon;
	mon = mondatabase->getlast();

	while (mon)
	{
		if (mon->x == autotX + pldatabase->own->x - 10 && 
			mon->y == autotY + pldatabase->own->y - 10 && 
			mon->knowid != Target.TarID) 
		{
			Target.DrawTarget = true;
			Target.MonTarget = mon;
			Target.PlTarget = NULL;
			Target.NPCTarget = NULL;
			Target.Type = MONSTER;
		
			Target.Frame = 0;
			Target.FramePlus = 1;
			Target.FrameTime = SDL_GetTicks();
			Target.TarID = mon->knowid;
			
			buffer[0] = 30;
			buffer[1] = 1;
			memcpy(&buffer[2], &Target.TarID, sizeof(Target.TarID));

			send(Socket, buffer, 2 + sizeof(Target.TarID), 0);

			mon = NULL;
		}
		
		if (mon != NULL) mon = mon->prev;
	}

	PlayerNode *pl;
	pl = pldatabase->players;

	short tx = autotX;
	short ty = autotY;

		while (pl)
		{			
			if (pl->x == autotX + pldatabase->own->x - 10 && pl->y == autotY + pldatabase->own->y - 10 && pl != pldatabase->own && 
				Client.AttackMode == ATTACK && !Identify && MapInfo.Visible[-pldatabase->own->x + pl->x + 10][-pldatabase->own->y + pl->y + 10] > 3)
			{
				if (Client.TargetProtectionMode && !pl->wanted)
				{
					strcpy(MB.NewMsg, "Target protection mode prevents you from targeting this Player.");
					MB.NewType = 7;
					MB.Update();
				}
				else
				{
					Target.PlTarget = pl;
					Target.MonTarget = NULL;
					Target.NPCTarget = NULL;
					Target.Type = PLAYER;

					Target.DrawTarget = true;
	
					Target.Frame = 0;
					Target.FramePlus = 1;
					Target.FrameTime = SDL_GetTicks();
					Target.TarID = pl->id;
			
					buffer[0] = 30;
					buffer[1] = 2;
					memcpy(&buffer[2], &Target.TarID, sizeof(Target.TarID));
		
					send(Socket, buffer, 2 + sizeof(Target.TarID), 0);
				}
                    
				pl = NULL;

			}


		if (pl != NULL) pl = pl->next;
	}
}

void CleanUp()
{
	if (GameState >= 3)
	{
		buffer[0] = 60;
		buffer[1] = 37;
		send(Socket, buffer, 2, 0);

		buffer[0] = 120;
		
		if (SafeZones[ pldatabase->own->x >> 4 ][ pldatabase->own->y >> 4 ] > 0)
			buffer[1] = 1;
		else
			buffer[1] = 0;
		send(Socket, buffer, 2, 0);
	}

	Done2 = true;

	SaveFogOfWar();

	if (closesocket(Socket) == SOCKET_ERROR) printf("Failed to close the socket.\n");

	WSACleanup();

	saveSettings();

	exit(1);
}


void MakeOwnInfo(p_OwnCharInfo OwnNode)
{
	P.Avatar			= OwnNode.Avatar;
	P.Con				= OwnNode.Con;
	P.DevMode			= OwnNode.devMode;
	P.Dex				= OwnNode.Dex;
	P.Exp				= OwnNode.Exp;
	P.HPLeft			= OwnNode.hp;
	P.ID				= OwnNode.ID;
	P.Int				= OwnNode.Int;
	P.KilledMonsters	= OwnNode.KilledMonsters;
	P.CrimCount			= OwnNode.crimCount;
	P.Level				= OwnNode.Level;
	P.Reputation		= OwnNode.Reputation;
	P.StatPoints		= OwnNode.Statpoints;
	P.Str				= OwnNode.Str;
	P.Race				= OwnNode.Race;
	P.Wanted			= OwnNode.Wanted;
	P.X					= OwnNode.X;
	P.Y					= OwnNode.Y;	

	strcpy(P.Name,		OwnNode.Name);
	memcpy(&P.Skills,	&OwnNode.Skills, sizeof(OwnNode.Skills));

	CreateOwnInfo();
	Minimap->createBack();
}


void HandleMessage()
{
	fd_set input_set;
	int s, nfds;

	timeval timeout; 

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	
    FD_ZERO(&input_set);

	FD_SET(Socket, &input_set);

    nfds = 1;

	s = select(nfds,&input_set,NULL,NULL,&timeout);
	
	if (s > 0)
	{	
		if (FD_ISSET(Socket, &input_set))
			GetMessage();
	}
}


void GetMessage()
{
	recv(Socket, buffer, 2, 0);

	if (buffer[0] != 0) LastMessageTime = SDL_GetTicks();

//	fprintf(stderr, "Buf[0] = %d, Buf[1] = %d\n", buffer[0], buffer[1]);

	switch (buffer[0])
	{
	//==============================================================================
	//
	//	All (and only) login network traffic goes under buffer[0] = 1;
	//
	//==============================================================================
	case 1:
		switch (buffer[1])
		{
		//------------------------------------------------------------------------------
		//
		//	Receive own infopacket.
		//
		//------------------------------------------------------------------------------
		case 1:


			p_OwnCharInfo pOwnInfo;

			memset(&P, 0, sizeof(P));
			recv(Socket, bigbuffer, sizeof(pOwnInfo), 0);
		
			memset(&pOwnInfo, 0, sizeof(pOwnInfo));
			memcpy(&pOwnInfo, bigbuffer, sizeof(pOwnInfo));

			MakeOwnInfo(pOwnInfo);

			First_Packet_Received = true;

			GameState = 5;
			break;
		//------------------------------------------------------------------------------
		//
		//	Old version.
		//
		//------------------------------------------------------------------------------
		case 2:

			sdl.text("You are running old version. Please update the client at www.eldoriaonline.com", 5, 572, col_grey);
			Screen.Flush();
			SDL_Delay(4000);

			exit(1);	
			break;
		//------------------------------------------------------------------------------
		//
		//	Receive character choose information.
		//
		//------------------------------------------------------------------------------
		case 3:

			sdl.text("Connecting", 5, 572, col_white);

			int DotCounter;
			DotCounter = 0;

			for (DotCounter = 0; DotCounter < AmountofDots; DotCounter++)
			{
				sdl.text(".", 10 + BFont_TextWidth("Connecting") + DotCounter*5, 572, col_white);
			}

			sdl.text("connected.", 10 + BFont_TextWidth("Connecting.") + DotCounter * 5, 572, col_white);
	
			Screen.Flush();
	
			memset(&Login.CharChooseInfo, 0, sizeof(Login.CharChooseInfo));
	
			recv(Socket, buffer, sizeof(Login.CharChooseInfo), 0);
	                        
			memcpy(&Login.CharChooseInfo, buffer, sizeof(Login.CharChooseInfo));
		
			Resume21 = true;
			GameState = 3;
			break;
		//------------------------------------------------------------------------------
		//
		//	Account doesn't exist.
		//
		//------------------------------------------------------------------------------
		case 4:

			sdl.text("Connecting", 5, 572, col_white);

			for (DotCounter = 0; DotCounter < AmountofDots; DotCounter++)
				sdl.text(".", 10 + BFont_TextWidth("Connecting") + DotCounter*5, 572, col_white);

			sdl.text("Invalid account information..", 10 + BFont_TextWidth("Connecting.") + DotCounter * 5, 572, col_white);
			Screen.Flush();
			SDL_Delay(2000);

			Resume21 = true;
			GameState = 2;

			break;
		//------------------------------------------------------------------------------
		//
		//	Incorrect password
		//
		//------------------------------------------------------------------------------
		case 5:
			sdl.text("Connecting", 5, 572, col_white);

			for (DotCounter = 0; DotCounter < AmountofDots; DotCounter++)
				sdl.text(".", 10 + BFont_TextWidth("Connecting") + DotCounter*5, 572, col_white);

			sdl.text("Invalid account information.", 10 + BFont_TextWidth("Connecting.") + DotCounter * 5, 572, col_white);
			Screen.Flush();
			SDL_Delay(2000);

			Resume21 = true;
			GameState = 2;
		
			break;
		//------------------------------------------------------------------------------
		//
		//	Banned.
		//
		//------------------------------------------------------------------------------
		case 6:
			sdl.text("Connecting", 5, 572, col_white);

			for (DotCounter = 0; DotCounter < AmountofDots; DotCounter++)
				sdl.text(".", 10 + BFont_TextWidth("Connecting") + DotCounter*5, 572, col_white);

			sdl.text("You are banned!", 10 + BFont_TextWidth("Connecting.") + DotCounter * 5, 572, col_white);
			Screen.Flush();
			SDL_Delay(2000);

			Resume21 = true;
			GameState = 2;

			break;
		//------------------------------------------------------------------------------
		//
		//	Account already logged in.
		//
		//------------------------------------------------------------------------------
		case 7:
			sdl.text("Connecting", 5, 572, col_white);

			for (DotCounter = 0; DotCounter < AmountofDots; DotCounter++)
				sdl.text(".", 10 + BFont_TextWidth("Connecting") + DotCounter*5, 572, col_white);

			sdl.text("This account is already in use.", 10 + BFont_TextWidth("Connecting.") + DotCounter * 5, 572, col_white);
			Screen.Flush();
			SDL_Delay(2000);

			Resume21 = true;
			GameState = 2;

			break;
		//------------------------------------------------------------------------------
		//
		//	Connection confirmed.
		//
		//------------------------------------------------------------------------------
		case 8: 

			break;
		//------------------------------------------------------------------------------
		//
		//	Account created.
		//
		//------------------------------------------------------------------------------
		case 9:
	
			memset(&Login.CharChooseInfo, 0, sizeof(Login.CharChooseInfo));
		    Resume21 = true;
			GameState = 3;
			
			break;
		//------------------------------------------------------------------------------
		//
		//	Creating account.
		//
		//------------------------------------------------------------------------------
		case 10:

			sdl.text("Trying to create account", 5, 572, col_white);

			for (DotCounter = 0; DotCounter < AmountofDots; DotCounter++)
				sdl.text(".", 10 + BFont_TextWidth("Trying to create account") + DotCounter*5, 572, col_white);

			sdl.text("This account name is taken.", 10 + BFont_TextWidth("Trying to create account") + DotCounter * 5, 572, col_white);
			Screen.Flush();
			SDL_Delay(2000);

			exit(1);
			break;

		case 11:

			sdl.text("Trying to create account", 5, 572, col_white);

			for (DotCounter = 0; DotCounter < AmountofDots; DotCounter++)
				sdl.text(".", 10 + BFont_TextWidth("Trying to create account") + DotCounter*5, 572, col_white);

			sdl.text("This Email has already been registered.", 10 + BFont_TextWidth("Trying to create account") + DotCounter * 5, 572, col_white);
			Screen.Flush();
			SDL_Delay(2000);
			break;

		}

		break;

	case 2:

		unsigned long tmptmp;
		recv(Socket, buffer, sizeof(tmptmp), 0);
		memcpy(&tmptmp, buffer, sizeof(tmptmp));

		pldatabase->own->id = tmptmp;
		P.ID = tmptmp;

		break;

	//==============================================================================
	//
	//	Chatting traffic.
	//
	//==============================================================================
	case 9: 

		PACKET_CHATMESSAGE NewPacket;
		memset(&NewPacket, 0, sizeof(NewPacket));
		
		recv(Socket, buffer, sizeof(NewPacket), 0);

		memcpy(&NewPacket, buffer, sizeof(NewPacket));

		strcpy(MB.NewMsg, NewPacket.Message);

		bool er;
		er = false;
		switch (NewPacket.MsgType)
		{
		case 1: // Normal
			
			if (!Client.SayOn) er = true;
			MB.NewType = 4;
			break;
		case 2: // Whisper
			
			if (!Client.WhisOn) er = true;
			MB.NewType = 3;
			break;
		case 3: // Global
			
			if (!Client.GlobOn) er = true;
			MB.NewType = 11;
			break;
		case 4: // Guild

			MB.NewType = 5;
			break;
		case 5: // GM

			MB.NewType = 2;
			break;
		case 6: // EWC

			MB.NewType = 9;
			break;
		case 7: // Serverwide chat

			if (!ListenToServerChat) er = true;
			MB.NewType = 10;
			break;
		case 10: 

			MB.NewType = 6;
			break;
		case 11: // GM Whisper

			MB.NewType = 11;
			break;
		default:
			MB.NewType = 7;
			break;
		}

		if (!er)
		MB.Update();


		break;
	// Moving around (== New coordinates for self or other players on screen.)
	case 10: 

		switch (buffer[1])
		{
		case 1: // They were for self.

			recv(Socket, buffer, sizeof(PACKET_COORDINATES), 0);

			PACKET_COORDINATES NewCoordinates;
			memset(&NewCoordinates, 0, sizeof(NewCoordinates));
			memcpy(&NewCoordinates, buffer, sizeof(NewCoordinates));

			short x,y;

			x = pldatabase->own->x;
			y = pldatabase->own->y;
			
			pldatabase->own->x = NewCoordinates.X;
			pldatabase->own->y = NewCoordinates.Y;

			if (x >> 4 != pldatabase->own->x >> 4 ||
				y >> 4 != pldatabase->own->y >> 4)
				Minimap->createBack();

			x = pldatabase->own->x;
			y = pldatabase->own->y;


			MapInfo.MapVisibility[ (x - 12)	>> 4 ][ (y - 12)	>> 4] = true;
			MapInfo.MapVisibility[ (x - 12)	>> 4 ][ (y)			>> 4] = true;
			MapInfo.MapVisibility[ (x - 12)	>> 4 ][ (y + 12)	>> 4] = true;

			MapInfo.MapVisibility[ (x)		>> 4 ][ (y - 12)	>> 4] = true;
			MapInfo.MapVisibility[ (x)		>> 4 ][ (y)			>> 4] = true;
			MapInfo.MapVisibility[ (x)		>> 4 ][ (y + 12)	>> 4] = true;

			MapInfo.MapVisibility[ (x + 12)	>> 4 ][ (y - 12)	>> 4] = true;
			MapInfo.MapVisibility[ (x + 12)	>> 4 ][ (y)			>> 4] = true;
			MapInfo.MapVisibility[ (x + 12)	>> 4 ][ (y + 12)	>> 4] = true;
			
			calc_LOS();

			break;
		case 2: // They were for other player.

			recv(Socket, buffer, sizeof(PACKET_COORDINATES), 0);

			memset(&NewCoordinates, 0, sizeof(NewCoordinates));

			memcpy(&NewCoordinates, buffer, sizeof(NewCoordinates));

			PlayerNode *tmp;
			tmp = pldatabase->getbyid(NewCoordinates.ID);

			if (tmp == NULL) break;
			if (tmp == pldatabase->own) break;

			tmp->x = NewCoordinates.X;
			tmp->y = NewCoordinates.Y;			

			break;		
		case 3: // Player we don't know came to screen.

			PACKET_PLAYERINFO np;

			memset(&np, 0, sizeof(np));

			recv(Socket, buffer, sizeof(np), 0);

            memcpy(&np, buffer, sizeof(np));

			PlayerNode *nn;

			nn = pldatabase->getbyid(np.ID);

			if (nn != NULL) 
			{
				// Player is already in the database, refresh the info
				nn->x = np.X;
				nn->y = np.Y;
				nn->hpleft = np.HPLeft;
				nn->hpmax = np.HPMax;
				nn->wanted = np.Wanted;
				nn->reputation = np.Reputation;			
			}
			else
			{
				// New player.
				nn = new PlayerNode;

				nn->avatar = np.Avatar;
				nn->hpleft = np.HPLeft;
				nn->hpmax = np.HPMax;
				nn->id = np.ID;
				nn->isinguild = np.IsInGuild;
				nn->reputation = np.Reputation;
				nn->wanted = np.Wanted;
				nn->race = np.Race;
				nn->x = np.X;
				nn->y = np.Y;
				strcpy(nn->name, np.Name);
				strcpy(nn->tag, np.Tag);

				nn->database = pldatabase;

				nn->addto();
			}

			break;
		}


        break;        

	case 8: // New information about players

		switch (buffer[1])
		{
		case 1: // New Alignment for self

			unsigned short tmprepu;
			recv(Socket, buffer, sizeof(tmprepu), 0);
			memcpy(&tmprepu, buffer, sizeof(tmprepu));

			P.Reputation = tmprepu;
			pldatabase->own->reputation = tmprepu;

			break;
		case 2: // New Alignment for other

			char col;

			short tmpid;

			recv(Socket, buffer, 1, 0);
			col = buffer[0];
			recv(Socket, buffer, sizeof(tmpid), 0);
			memcpy(&tmpid, buffer, sizeof(tmpid));

			PlayerNode *tmp;
			tmp = pldatabase->getbyid(tmpid);
			
			if (tmp == NULL) break;
			else tmp->reputation = col;
			
			break;
		case 3: // repupoints report

			long repupoints;
			recv(Socket, buffer, sizeof(repupoints), 0);
			memcpy(&repupoints, buffer, sizeof(repupoints));

			P.KilledMonsters = repupoints;

			break;
		case 4:

			p_Guildinfo nGuild;
			recv(Socket, buffer, sizeof(nGuild), 0);
			memcpy(&nGuild, buffer, sizeof(nGuild));

			P.Guild = nGuild.guild;
			strcpy(pldatabase->own->tag, nGuild.tag);

			break;
		}

		break;
	case 11:

		switch (buffer[1])
		{

		//---------------------------------------------------------------------
		// Someone needs to be removed from playerlist (disconnected or died).
		//---------------------------------------------------------------------
		case 1: 

			printf("Removing someone from the Playerlist.\n");

			long TmpID;
			TmpID = 0;

			recv(Socket, buffer, sizeof(TmpID), 0);
			memcpy(&TmpID, buffer, sizeof(TmpID));

			PlayerNode *tmp;
			tmp = pldatabase->getbyid(TmpID);
			if (tmp == NULL) break;
			else tmp->removefrom();

			break;

		//--------------------------------------------
		// New information about own character comes.
		//--------------------------------------------

		case 2:  // New amount of exp.

            unsigned long NewAmount;
			NewAmount = 0;

			recv(Socket, buffer, sizeof(NewAmount), 0);
			memcpy(&NewAmount, buffer, sizeof(NewAmount));

			P.Exp = NewAmount;
            
			break;

		case 3: // New Skill

			PACKET_SKILL NewSkill;
			memset(&NewSkill, 0, sizeof(NewSkill));

			recv(Socket, buffer, sizeof(NewSkill), 0);

			memcpy(&NewSkill, buffer, sizeof(NewSkill));

			P.Skills[NewSkill.This].Exp = NewSkill.SkillExp;
			P.Skills[NewSkill.This].Level = NewSkill.SkillLevel;

			break;
		case 4: 

			recv(Socket, buffer, sizeof(TmpID), 0);
			memcpy(&TmpID, buffer, sizeof(TmpID));
			
			GMTool->removeByID(TmpID);

			break;
		}

		break;

	//-------------
	// Item things
	//-------------
		
	case 12:

		switch (buffer[1])
		{
		case 1: // Item appeared to ground.

			recv(Socket, buffer, sizeof(New), 0);

			memset(&New, 0, sizeof(New));

			memcpy(&New, buffer, sizeof(New));

			ItemClass New2;
			memset(&New2, 0, sizeof(New2));

			New2.Used = true;
			New2.Family = New.Family;
			New2.ID = New.ID;
			New2.KnowID = New.KnowID;
			New2.Quantity = New.Quantity;
			New2.Type = New.Type;
			New2.x = New.x;
			New2.y = New.y;
			New2.Durability = New.Durability;


			Item_Add(New2, ItemGround);

			break;

		case 2: // Item appeared to backpack.

			recv(Socket, buffer, sizeof(New), 0);

			memset(&New, 0, sizeof(New));

			memcpy(&New, buffer, sizeof(New));

			memset(&New2, 0, sizeof(New2));

			New2.Used = true;
			New2.Family = New.Family;
			New2.ID = New.ID;
			New2.KnowID = New.KnowID;
			New2.Quantity = New.Quantity;
			New2.Type = New.Type;
			New2.x = New.x;
			New2.y = New.y;
			New2.Durability = New.Durability;

			Item_Add(New2, ItemBackpack);

			break;

		case 3: // Item appeared to equipment.

			recv(Socket, buffer, sizeof(New), 0);

			memset(&New, 0, sizeof(New));

			memcpy(&New, buffer, sizeof(New));

//			memcpy(&New2, buffer, sizeof(New));
			

			memset(&New2, 0, sizeof(New2));

			New2.Used = true;
			New2.Family = New.Family;
			New2.ID = New.ID;
			New2.KnowID = New.KnowID;
			New2.Quantity = New.Quantity;
			New2.Type = New.Type;
			New2.x = New.x;
			New2.y = New.y;
			New2.Durability = New.Durability;

			
			Item_Add(New2, ItemWeared);

			break;

		case 4: // Item appeared to bank.

			recv(Socket, buffer, sizeof(New), 0);

			memset(&New, 0, sizeof(New));

			memcpy(&New, buffer, sizeof(New));

			memset(&New2, 0, sizeof(New2));

			New2.Used = true;
			New2.Family = New.Family;
			New2.ID = New.ID;
			New2.KnowID = New.KnowID;
			New2.Quantity = New.Quantity;
			New2.Type = New.Type;
			New2.x = New.x;
			New2.y = New.y;
			New2.Durability = New.Durability;

			Item_Add(New2, ItemBank);

			break;

		case 11: // Item disappeared from ground.

			recv(Socket, buffer, sizeof(short), 0);

			short tmpID;
			memcpy(&tmpID, buffer, sizeof(tmpID));

			int i;
			for (i = 0; i < MaxItems; i++)
			{
				if (iGround[i].Used)
				{
					if (iGround[i].KnowID == tmpID)
					{
						Item_Remove( i, ItemGround );

						if (Dragging.TargetItem == &iGround[i])
						{
							Dragging.TargetItem = NULL;
							Dragging.On = false;
						}
					}
				}
			}

			break;
		case 12: // Item disappeared from Backpack.

			recv(Socket, buffer, sizeof(short), 0);

			memcpy(&tmpID, buffer, sizeof(tmpID));


			for (i = 0; i < MaxBackpackItems; i++)
			{
				if (iBackpack[i].Used)
					if (iBackpack[i].KnowID == tmpID)
						Item_Remove( i, ItemBackpack );
			}

			break;
		case 13: // Item disappeared from Equipment.

			recv(Socket, buffer, sizeof(short), 0);

			memcpy(&tmpID, buffer, sizeof(tmpID));

			fprintf(stderr, "Item from equipment removed\n");

			for (i = 0; i < MaxWearedItems; i++)
			{
				if (iWeared[i].Used)
					if (iWeared[i].KnowID == tmpID)
						Item_Remove( i, ItemWeared );
			}

			break;
		case 14: // Item disappeared from Bank.
		
			recv(Socket, buffer, sizeof(short), 0);

			memcpy(&tmpID, buffer, sizeof(tmpID));


			for (i = 0; i < MaxBankItems; i++)
			{
				if (iBank[i].Used)
					if (iBank[i].KnowID == tmpID)
						Item_Remove( i, ItemBank );
			}

			break;

		case 15: // Item on ground gets new coords.

			recv(Socket, buffer, sizeof(PACKET_COORDINATES), 0);

			PACKET_COORDINATES NewCoords;
			memset(&NewCoords, 0, sizeof(NewCoords));

			memcpy(&NewCoords, buffer, sizeof(NewCoords));

            for (i = 0; i < MaxItems; i++)
			{
				if (iGround[i].Used)
				{
					if (iGround[i].KnowID == NewCoords.ID)
					{
						iGround[i].x = NewCoords.X;
						iGround[i].y = NewCoords.Y;	
					}
				}
			}

			break;
		case 16: // Item in bank gets new coords.

			recv(Socket, buffer, sizeof(PACKET_COORDINATES), 0);

			memset(&NewCoords, 0, sizeof(NewCoords));

			memcpy(&NewCoords, buffer, sizeof(NewCoords));

            for (i = 0; i < MaxBankItems; i++)
			{
				if (iBank[i].Used)
				{
					if (iBank[i].KnowID == NewCoords.ID)
					{
						iBank[i].x = NewCoords.X;
						iBank[i].y = NewCoords.Y;	
					}
				}
			}


			break;
		case 17: // Item in Backpack gets new coords.

			recv(Socket, buffer, sizeof(PACKET_COORDINATES), 0);

			memset(&NewCoords, 0, sizeof(NewCoords));

			memcpy(&NewCoords, buffer, sizeof(NewCoords));

            for (i = 0; i < MaxBackpackItems; i++)
			{
				if (iBackpack[i].Used)
				{
					if (iBackpack[i].KnowID == NewCoords.ID)
					{
						iBackpack[i].x = NewCoords.X;
						iBackpack[i].y = NewCoords.Y;
					}
				}
			}

			break;
		case 18:

			memset(&iBackpack, 0, sizeof(iBackpack));
//			memset(&iWeared, 0, sizeof(iWeared));

			sprintf(MB.NewMsg, "You have dropped your backpack!");
			MB.NewType = 7;
			MB.Update();

			break;

		case 20: // Getting someone's backpackitem

			recv(Socket, buffer, sizeof(New), 0);

			memset(&New, 0, sizeof(New));
			memcpy(&New, buffer, sizeof(New));
			memset(&New2, 0, sizeof(New2));

			New2.Used = true;
			New2.Family = New.Family;
			New2.ID = New.ID;
			New2.KnowID = New.KnowID;
			New2.Quantity = New.Quantity;
			New2.Type = New.Type;
			New2.x = New.x;
			New2.y = New.y;
			New2.Durability = New.Durability;

			memcpy(&GMTool->iBackpackItems[New.KnowID], &New2, sizeof(New2));
			GMTool->iBackpackItems[New.KnowID].Used = true;

			break;

		case 21: // Getting someone's bankitem

			recv(Socket,	buffer, sizeof(New), 0);

			memset(&New,	0,		sizeof(New));
			memcpy(&New,	buffer, sizeof(New));
			memset(&New2,	0,		sizeof(New2));

			New2.Used		= true;
			New2.Family		= New.Family;
			New2.ID			= New.ID;
			New2.KnowID		= New.KnowID;
			New2.Quantity	= New.Quantity;
			New2.Type		= New.Type;
			New2.x			= New.x;
			New2.y			= New.y;
			New2.Durability = New.Durability;

			memcpy(&GMTool->iBankItems[New.KnowID], &New2, sizeof(New2));
			GMTool->iBankItems[New.KnowID].Used = true;

			break;
		case 22: // Item request info

			recv(Socket,buffer, sizeof(GMTool->cIdOwnerName), 0);
			memcpy(&GMTool->cIdOwnerName,	buffer, sizeof(GMTool->cIdOwnerName));

			memset(&GMTool->iBackpackItems, 0,		sizeof(GMTool->iBackpackItems));

			break;
		case 23: // Bank request info

			recv(Socket,buffer, sizeof(GMTool->cIdOwnerName), 0);
			memcpy(&GMTool->cIdOwnerName,	buffer, sizeof(GMTool->cIdOwnerName));

			memset(&GMTool->iBankItems,		0,		sizeof(GMTool->iBankItems));
			break;
			
		}

		break;

	//---------------------------------------------------
	// Item gets new quantity/durability/other things...
	//---------------------------------------------------

	case 13: 

		switch (buffer[1])
		{
		case 1: // New Amount

			memset(&NewAmount, 0, sizeof(NewAmount));

			recv(Socket, buffer, sizeof(NewAmount), 0);

			memcpy(&NewAmount, buffer, sizeof(NewAmount));

			ItemClass *This;

			This = GetItemByID(NewAmount.InList, NewAmount.KnowID);
			
			if (This != NULL)
				This->Quantity = NewAmount.Amount;
            
			break;
		}

		break;

	//----------------
	// Monster things
	//----------------

	case 20:

		switch (buffer[1])
		{
		case 1: // New monster came to screen.

			PACKET_MONSTER pm;

			memset(&pm, 0, sizeof(pm)); //Set the memmory for the Monster Packet
			recv(Socket, buffer, sizeof(pm), 0); //Receive the Monster Packet
            memcpy(&pm, buffer, sizeof(pm)); //Copy the information from the buffer to the Monster Packet
			
			MonsterClass *nm; //Create an instance of the Monster Class
			
			nm = mondatabase->getbyid(pm.KnowID); //Look for the Monster in the database

			if (nm != NULL) //If the monster already exist in the database
			{
				//Monster is already in the database refresh the info
				nm->x = pm.X;
				nm->y = pm.Y;
				nm->hpleft = pm.HPLeft;
				nm->type = pm.Type;			
			}
			else //If the monster does not exist in the database
			{
				//New monster
				nm = new MonsterClass;
				nm->x = pm.X;
				nm->y = pm.Y;
				nm->hpleft = pm.HPLeft;
				nm->type = pm.Type;
				nm->knowid = pm.KnowID;
				nm->anim_start();
				nm->database = mondatabase;
				nm->addto(); //Add the monster to the database
			}
			break;

		case 2: // Monster died, so it has to be removed. (by KnowID)

			short TmpID;
			TmpID = 0;

			recv(Socket, buffer, sizeof(TmpID), 0);
			memcpy(&TmpID, buffer, sizeof(TmpID));


			nm = mondatabase->getbyid(TmpID);
			if (nm == NULL) break;

			if (Target.MonTarget == nm)
			{
				Target.Type = 0;
				Target.MonTarget = NULL;
				Target.TarID = -1;
			}

			nm->removefrom();
			break;

		case 3: // Some monster/Player lost/gained HP

			NewHPClass NewPacket2; //Create a new NewHP Packet

			memset(&NewPacket2, 0, sizeof(NewPacket2)); //Set memory for the NewHP Packet
            recv(Socket, buffer, sizeof(NewPacket2), 0); //Receive the NewHP Packet
			memcpy(&NewPacket2, buffer, sizeof(NewPacket2)); //Copy the information from buffer to NewHP Packet

			if (NewPacket2.Type == 0) //The Packet is for a Players HP
			{
				//Player lost/gained HP
				PlayerNode *tmp;
				tmp = pldatabase->getbyid(NewPacket2.KnowID); //Look for the player in the database
				if (tmp == NULL) break; //if that player does not exist in the database then exit
			
				if (tmp == pldatabase->own)
					P.HPLeft = NewPacket2.NewHP;

				tmp->hpleft = NewPacket2.NewHP;	
			}
			if (NewPacket2.Type == 1) //The Packet is for a Monsters HP
			{
				//Monster lost/gained HP
				nm = mondatabase->getbyid(NewPacket2.KnowID); //Look for the monster in the database
				if (nm == NULL) break; //if that monster does not exist in the database then exit

				nm->hpleft = NewPacket2.NewHP; //set the new HP to the monster
			}
			break;

		case 4:  //Some monster got new coords

			PACKET_COORDINATES NewCoords2;
			memset(&NewCoords2, 0, sizeof(NewCoords2));

			recv(Socket, buffer, sizeof(NewCoords2), 0);
			memcpy(&NewCoords2, buffer, sizeof(NewCoords2));


			nm = mondatabase->getbyid(NewCoords2.ID);
			if (nm == NULL) break;

			nm->x = NewCoords2.X;
			nm->y = NewCoords2.Y;
			break;

		case 5: // NPC came to screen

			recv(Socket, buffer, sizeof(NewNPC), 0);
			memset(&NewNPC, 0, sizeof(NewNPC));
			memcpy(&NewNPC, buffer, sizeof(NewNPC));


			NPCClass *nnp;

			nnp = npcdatabase->getbyid(NewNPC.ID);
			
			if (nnp != NULL)
			{
				nnp->x = NewNPC.X;
				nnp->y = NewNPC.Y;
				nnp->conv_id = NewNPC.conv_id;
				nnp->hp = NewNPC.HP;
				nnp->type = NewNPC.type;
			}
			else
			{
				nnp = new NPCClass;
				nnp->x = NewNPC.X;
				nnp->y = NewNPC.Y;
				nnp->conv_id = NewNPC.conv_id;
				nnp->hp = NewNPC.HP;
				nnp->type = NewNPC.type;
				nnp->id = NewNPC.ID;

				nnp->anim_start();
				nnp->database = npcdatabase;
				nnp->addto();			
			}
			break;

		case 6:

			memset(&NewCoords2, 0, sizeof(NewCoords2));
			recv(Socket, buffer, sizeof(NewCoords2), 0);
			memcpy(&NewCoords2, buffer, sizeof(NewCoords2));

			nnp = npcdatabase->getbyid(NewCoords2.ID);
			if (nnp == NULL) break;

			nnp->x = NewCoords2.X;
			nnp->y = NewCoords2.Y;

			break;

		case 7: // NPC died

            short _i;
			recv (Socket, buffer, sizeof(_i), 0);
			memcpy(&_i, buffer, sizeof(_i));


			nnp = npcdatabase->getbyid(_i);
			if (nnp == NULL) break;


			if (Target.NPCTarget == nnp)
			{
				Target.Type = 0;
				Target.NPCTarget = NULL;
				Target.TarID = -1;
			}

			nnp->removefrom();
			
			break;
		}

		break;

	//----------------------------
	// Misc pics appear on screen
	//----------------------------

	case 21: 

		switch (buffer[1])
		{
		case 1: // Hit pic

			memset(&NewPic, 0, sizeof(NewPic));
			
			recv(Socket, buffer, sizeof(NewPic), 0);

			memcpy(&NewPic, buffer, sizeof(NewPic));

			int i3;
			for (i3 = 0; i3 < 10; i3++)
			{
				if (TmpPics[i3].Used == 0)
				{
					TmpPics[i3].Used = 1;
					TmpPics[i3].FrameTime = SDL_GetTicks();
					TmpPics[i3].X = NewPic.X;
					TmpPics[i3].Y = NewPic.Y;
					TmpPics[i3].Pic = NewPic.Pic;
					i3 = 10;
				}
			}

			break;
		}


		break;

	case 22: // Bodies

		switch (buffer[1])
		{
		case 1: // New Body appeared to screen
	
			memset(&NewBody, 0, sizeof(NewBody));
			recv(Socket, buffer, sizeof(NewBody), 0);
            
			memcpy(&NewBody, buffer, sizeof(NewBody));


			BodyClass *body;
			body = bodydatabase->getbyid(NewBody.ID);

			if (body != NULL) break;

			body						= new BodyClass;
			body->id					= NewBody.ID;
			body->decaystate			= NewBody.DecayState;
			body->looted				= NewBody.Looted;
			body->x						= NewBody.X;
			body->y						= NewBody.Y;
			body->realid				= NewBody.RealID;
			strcpy(body->lootername,	NewBody.LooterName);
			strcpy(body->ownername,		NewBody.OwnerName);

			memset(body->Items, 0, sizeof(body->Items));

			body->database = bodydatabase;
			body->addto();
			break;

		case 2: // Body becomes looted

			recv(Socket, buffer, sizeof(NewBodyInfo), 0);
			memcpy(&NewBodyInfo, buffer, sizeof(NewBodyInfo));

			body = bodydatabase->getbyid(NewBodyInfo.BodyID);
			if (body == NULL) break;

			strcpy(body->lootername, NewBodyInfo.Name);
			body->looted = NewBodyInfo.Looted;

			break;

		case 3: // Item appeared to body

			recv(Socket, buffer, sizeof(PACKET_ITEM), 0);

			if (!v_Body.On)
				break;

			body = bodydatabase->getbyid(v_Body_Open);
			if (body == NULL) break;

			PACKET_ITEM New;
			memset(&New, 0, sizeof(New));
			memcpy(&New, buffer, sizeof(New));

			body->Items[New.KnowID].KnowID = New.KnowID;

			body->Items[New.KnowID].Used		= true;
			body->Items[New.KnowID].Family		= New.Family;
			body->Items[New.KnowID].ID			= New.ID;
			body->Items[New.KnowID].KnowID		= New.KnowID;
			body->Items[New.KnowID].Quantity	= New.Quantity;
			body->Items[New.KnowID].Type		= New.Type;
			body->Items[New.KnowID].x			= New.x;
			body->Items[New.KnowID].y			= New.y;
			body->Items[New.KnowID].Durability	= New.Durability;

			break;

		case 4: // Item from body disappeared

			short Tmp;
			recv(Socket, buffer, sizeof(Tmp), 0);

			if (!v_Body.On) break;

			body = bodydatabase->getbyid(v_Body_Open);
			if (body == NULL) break;

			memcpy(&Tmp, buffer, sizeof(Tmp));
			memset(&body->Items[Tmp], 0, sizeof(body->Items[Tmp]));

			break;
		case 5: // Body's decaystate changed

			recv(Socket, buffer, sizeof(NewDecay), 0);

			memset(&NewDecay, 0, sizeof(NewDecay));
			memcpy(&NewDecay, buffer, sizeof(NewDecay));

			body = bodydatabase->getbyid(NewDecay.ID);
			if (body == NULL) break;

			body->decaystate = NewDecay.NewDecay;
			if (body->decaystate >= 5)
				body->removefrom();

			break;

		case 6: // Own body looted

			memset(&NewBodyInfo, 0, sizeof(NewBodyInfo));
			recv(Socket, buffer, sizeof(NewBodyInfo), 0);
			memcpy(&NewBodyInfo, buffer, sizeof(NewBodyInfo));

			sprintf(MB.NewMsg, "Spirit of darkness tells you that æ%sÅ is looting your body.", NewBodyInfo.Name);
			MB.NewType = 7;
			MB.Update();

            break;		
		}

		break;

	case 25: // Identify

		PACKET_IDENTIFYPACKET idp;
		memset(&idp, 0, sizeof(idp));

		recv(Socket, buffer, sizeof(idp), 0);

		memcpy(&idp, buffer, sizeof(idp));

		identify *id;

		id = idatabase->getbyid(idp.id);

		if (id)
		{
			id->items->clearlist();
			for (int i = 0; i < 20; i++)
			{
				if (idp.Family[i] != 0)
				{
					ItemClass *ni;
					ni = new ItemClass;				
					ni->ID = idp.ID[i];
					ni->Type = idp.Type[i];
					ni->Family = idp.Family[i];
					ni->database = id->items;
					ni->addto();
				}
			}

			strcpy(id->name, idp.Name);
			strcpy(id->tag, idp.Tag);
			id->reputation = idp.reputation;

			if (id->tag[0] != 0)
			{
				sprintf(MB.NewMsg, "You see ô%sÅ of the ï%sÅ Clan.", id->name, idp.Guild);
				MB.NewType = 7;
				MB.Update(); 
			}
			else
			{
				sprintf(MB.NewMsg, "You see ô%sÅ.", id->name);
				MB.NewType = 7;
				MB.Update(); 
			}		
		
			break;
		}

		id					= new identify;
		id->database		= idatabase;
		id->items			= new ItemDatabase;
		id->items->items	= NULL;

		int i;
		for (i = 0; i < 20; i++)
		{
			if (idp.Family[i] != 0)
			{
				ItemClass *ni;
				ni				= new ItemClass;				
				ni->ID			= idp.ID[i];
				ni->Type		= idp.Type[i];
				ni->Family		= idp.Family[i];
				ni->database	= id->items;
				ni->addto();
			}
		}

		strcpy(id->name,	idp.Name);
		strcpy(id->tag,		idp.Tag);
		id->reputation		= idp.reputation;

		id->id	= idp.id;
		id->x	= rand() % 650;
		id->y	= rand() % 400;
		
		id->addto();

		if (id->tag[0] != 0)
		{
			sprintf(MB.NewMsg, "You see ô%sÅ of the ï%sÅ Clan.", id->name, idp.Guild);
			MB.NewType = 7;
			MB.Update(); 
		}
		else
		{
			sprintf(MB.NewMsg, "You see ô%sÅ.", id->name);
			MB.NewType = 7;
			MB.Update(); 
		}		
		
		break;

	case 26:

		switch (buffer[1])
		{
		case 1: 
            
			recv(Socket, buffer, sizeof(NewPacket), 0);

			memcpy(&NewPacket, buffer, sizeof(NewPacket));

			sprintf(MB.NewMsg, "%s", NewPacket.Message);
			MB.NewType = 4;
			MB.Update();

			break;
		case 2:
			
			sprintf(MB.NewMsg, "You already know your items.");
			MB.NewType = 7;
			MB.Update();

            break;		
		}

		break;

	//---------------------
	// New AC, Weight, ...
	//---------------------

	case 30:

		switch (buffer[1])
		{
		case 1: // New Weight.

			memset(&NewWeight, 0, sizeof(NewWeight));
			recv(Socket, buffer, sizeof(NewWeight), 0);
			memcpy(&NewWeight, buffer, sizeof(NewWeight));

			P.WeightMax	= NewWeight.WeightMax;
			P.Weight	= NewWeight.Weight;
			P.AC		= NewWeight.AC;

			break;
		case 2: // New stats & damage
			memset(&NewStats, 0, sizeof(NewStats));

			recv(Socket, buffer, sizeof(NewStats), 0);

			memcpy(&NewStats, buffer, sizeof(NewStats));

			P.Str		= NewStats.Str;
			P.Con		= NewStats.Con;
			P.Int		= NewStats.Int;
			P.Dex		= NewStats.Dex;
			P.DamMin	= NewStats.DamMin;
			P.DamMax	= NewStats.DamMax;

//			Draw.DrawStats();

			break;
		case 3: // New level and stat points.

			PACKET_LEVELUP NewLevel;

			memset(&NewLevel, 0, sizeof(NewLevel));

			recv(Socket, buffer, sizeof(NewLevel), 0);

			memcpy(&NewLevel, buffer, sizeof(NewLevel));

			P.Level			= NewLevel.Level;
			P.StatPoints	= NewLevel.Statpoints;
			P.HPMax			= NewLevel.HPMax;

			sprintf(MB.NewMsg, "You have reached next level! Exp required to level Ä%dÅ is Ä%luÅ.", P.Level+1, ExpTables[P.Level+1]);
			MB.NewType = 2;
			MB.Update();

			break;

		case 4: // New skill level

			PACKET_SKILLUP NewSkill;

			memset(&NewSkill, 0, sizeof(NewSkill));

			recv(Socket, buffer, sizeof(NewSkill), 0);
			memcpy(&NewSkill, buffer, sizeof(NewSkill));

			P.Skills[NewSkill.SkillID].Level = NewSkill.NewSkill;

			sprintf(MB.NewMsg, "You feel that your ï%sÅ skill is raising. Exp required to skill level Ä%dÅ is Ä%luÅ.", skills[NewSkill.SkillID].name, P.Skills[NewSkill.SkillID].Level + 1, SkillExpTables[P.Skills[NewSkill.SkillID].Level + 1]);
			MB.NewType = 2;
			MB.Update();

			break;
		case 5: // New base stats.

			PACKET_STATS_BASE nnn;

			recv(Socket, buffer, sizeof(nnn), 0);
			memcpy(&nnn, buffer, sizeof(nnn));

			P.StatPoints = nnn.StatPoints;
			
			break;
		case 6: // NewHP max

			short NewHP;

			recv(Socket, buffer, sizeof(NewHP), 0);

			memcpy(&NewHP, buffer, sizeof(NewHP));

			P.HPMax = NewHP;

			break;
		case 7: // New crimcount amount

			long NewCounts;
			recv(Socket, buffer, sizeof(NewCounts), 0);

			memcpy(&NewCounts, buffer, sizeof(NewCounts));

			P.CrimCount = NewCounts;

			break;

		case 8: // New resistances

			recv(Socket, buffer, sizeof(NewRes), 0);
			memcpy(&NewRes, buffer, sizeof(NewRes));

			P.fire	= NewRes.fire;
			P.air	= NewRes.air;
			P.earth = NewRes.earth;
			P.water = NewRes.water;

//			Draw.DrawStats();
			break;
		}

		break;

	case 31: // Shopping system.

		switch (buffer[1])
		{
		case 1: // New item to Shopping list.

			memset(&NewShopItem, 0, sizeof(NewShopItem));

			recv(Socket, buffer, sizeof(NewShopItem), 0);

			fprintf(stderr, "shop item received\n");

			memcpy(&NewShopItem, buffer, sizeof(NewShopItem));

			shop.items[NewShopItem.RealID].id		= NewShopItem.ID;
			shop.items[NewShopItem.RealID].type		= NewShopItem.Type;
			shop.items[NewShopItem.RealID].family	= NewShopItem.Family;
			shop.items[NewShopItem.RealID].in_use	= true;
			shop.items[NewShopItem.RealID].realid	= NewShopItem.RealID;

			shop.on = true;

			break;

		case 2: // Item disappears from the list (it was bought by self or someone else).


            break;			

		case 3: // Abort trade

			shop.uninit();

			strcpy(MB.NewMsg, "You do not have enough money to buy these!");
			MB.NewType = 6;
			MB.Update();			

			break;
		case 4: // Trade succeed, remove lists.

			shop.uninit();

			strcpy(MB.NewMsg, "Trade finished!");
			MB.NewType = 7;
			MB.Update();			

			break;
		case 5:
			shop.init();
			break;
		case 6: 
			shop.createlist();
			break;
		case 7: 

			shop.uninit();

			strcpy(MB.NewMsg, "Shopkeeper is too far away!");
			MB.NewType = 6;
			MB.Update();			

			break;
		}
		break;

	case 32: // more NPC things..

		switch (buffer[1])
		{
		case 1: // Banker.

			NPCClass *np;
			np = npcdatabase->npc;
			short tx, ty, ix, iy;

			tx = pldatabase->own->x;
			ty = pldatabase->own->y;

			while (np)
			{
				ix = np->x;
				iy = np->y;

				if (tx - ix > -10 && tx - ix < 10 && ty - iy > -10 && ty - iy < 10)
				{	
					if (np->type == 16) 
					{
						Bank.On = true;;

						sprintf(MB.NewMsg, "%s: %s", NPCTypes[np->type].Name, npc_conv[ np->conv_id ] );
						MB.NewType = 7;
						MB.Update();

						np = NULL;
					}
				}
				
				if (np) np = np->next;
			}
//			Bank.On = true;

			break;
		case 2: // Guild buy

//			Shop.GuildBuy = true;
//			Shop.On = true;
			shop.pending		= true;
			shop.pending_mode	= shopmode_pendingguildbuy;

			break;
		case 3: // Guild buy, not enough level

			sprintf(MB.NewMsg, "Your level is not high enough.");
			MB.NewType = 7;
			MB.Update();
            
			break;
		case 4: // guild buy, not enough money

			sprintf(MB.NewMsg, "You don't have enough money. (500,000gp needs to be in the same pile.)");
			MB.NewType = 7;
			MB.Update();

			break;
		case 5: // guild buy, you are already in a guild

			sprintf(MB.NewMsg, "You are already in a guild.");
			MB.NewType = 7;
			MB.Update();

			break;
		case 6: // No place for more guilds

			sprintf(MB.NewMsg, "There's no place for more guilds in the realm of Eldoria. [12 max reached]");
			MB.NewType = 7;
			MB.Update();

			break;
		case 7: // You have started new guild

			sprintf(MB.NewMsg, "You have started a new guild!");
			MB.NewType = 7;
			MB.Update();

			break;
		case 8: // Confimation for buying guild

//			Shop.BuyGuild = true;

			break;

		case 9: // Guilds off

			sprintf(MB.NewMsg, "Guilds are currently disabled!");
			MB.NewType = 7;
			MB.Update();

			break;

		}

		break;
	case 33: 

		short _id;

		bool wanted;
		_id = buffer[1];
		if (_id == 1) wanted = false;
		else if (_id == 2) wanted = true;
		else break;

		recv(Socket, buffer, sizeof(_id), 0);
		memcpy(&_id, buffer, sizeof(_id));

		PlayerNode *tmp;
		tmp = pldatabase->getbyid(_id);

		if (tmp == NULL) break;

		tmp->wanted = wanted;
		if (tmp == pldatabase->own) P.Wanted = wanted;		

		break;

	// GUILDS
	case 40:

		switch (buffer[1])
		{
		case 1: // New guild member.

			PACKET_NEWGUILDMEMBER NewMember;
			recv(Socket, buffer, sizeof(NewMember), 0);
			memcpy(&NewMember, buffer, sizeof(NewMember));

			for (i = 0; i < 50; i++)
			{
				if (!Clan->UsedSlot[i])
				{			
					strcpy(Clan->Names[i],	NewMember.Name[i]);
					Clan->UsedSlot[i]		= true;
					Clan->IDs[i]			= NewMember.ID[i];
					Clan->Status[i]			= NewMember.Status[i];
					Clan->ranks[i]			= NewMember.rank[i];
                    
					i = 50;
				}			
			}			

			break;
		case 2: // Someone was removed from guild.

			short _ID;
			_ID = -1;

            recv(Socket, buffer, sizeof(_ID), 0);

			memcpy(&_ID, buffer, sizeof(_ID));

			if (_ID >= 0)
			{
			//	if (Clan->IsLeader)
			//	{
					for (i = 0; i < 50; i++)
					{
						if (Clan->IDs[i] == _ID)
						{				
							Clan->IDs[i]			= 0;
							Clan->UsedSlot[i]		= 0;
							memset(&Clan->Names[i], 0, sizeof(Clan->Names[i]));;

							if (Clan->Selected == i) 
								Clan->Selected = -1;
							i = 50;
						}
					}
				//}
			}

			break;

		case 3: // Confirmation of leadership.

			recv(Socket, buffer, sizeof(short), 0);
			memcpy(&_ID, buffer, sizeof(short));
			
			Clan->IsLeader = true;
			Clan->own_rank = _ID;

			break;
		
		case 4: // New tag

			PACKET_NEWGUILDTAG NewTag;
			recv(Socket, buffer, sizeof(NewTag), 0);
			memcpy(&NewTag, buffer, sizeof(NewTag));

			strcpy(pldatabase->own->tag, NewTag.Tag);

			//if (Clan->IsLeader)
				strcpy(Clan->Tag, NewTag.Tag);

			sprintf(MB.NewMsg, "Your guild tag has changed!");
			MB.NewType = 7;
			MB.Update();

			break;

		case 5: // New name

			PACKET_NEWGUILDNAME NewName;
			recv(Socket, buffer, sizeof(NewName), 0);
			memcpy(&NewName, buffer, sizeof(NewName));

			//if (Clan->IsLeader)
				strcpy(Clan->Name, NewName.Name);

			sprintf(MB.NewMsg, "Your guild name has changed!");
			MB.NewType = 7;
			MB.Update();

            break;		

		case 6: // Self was added to guild

			pldatabase->own->isinguild = true;

			sprintf(MB.NewMsg, "You were added to a guild!");
			MB.NewType = 7;
			MB.Update();

			break;
		case 7: // self was removed from guild

			Clan->IsLeader = false;
			Clan->On = false;
			Use = false;
			Clan->Wait_Action = false;
			Clan->SelectedAction = 0;
			pldatabase->own->isinguild = false;
			memset(&pldatabase->own->tag, 0, sizeof(pldatabase->own->tag));

			sprintf(MB.NewMsg, "You were booted from the guild!");
			MB.NewType = 7;
			MB.Update();

			break;

		case 8: // disband

			Clan->IsLeader = false;
			Clan->On = false;
			Use = false;
			Clan->Wait_Action = false;
			Clan->SelectedAction = 0;
			pldatabase->own->isinguild = false;	
			memset(&pldatabase->own->tag, 0, sizeof(pldatabase->own->tag));

			sprintf(MB.NewMsg, "Your guild was disbanded!");
			MB.NewType = 7;
			MB.Update();
			break;

		case 9: // Tag taken

			sprintf(MB.NewMsg, "Guild Tag already taken");
			MB.NewType = 7;
			MB.Update();
			break;

		case 10: // Promoted

			sprintf(MB.NewMsg, "You have been promoted!");
			MB.NewType = 7;
			MB.Update();
			break;

		case 11: // Demoted

			sprintf(MB.NewMsg, "You have been demoted!");
			MB.NewType = 7;
			MB.Update();
			break;

		case 12: // No Room

			sprintf(MB.NewMsg, "The guild is at its max player limit!");
			MB.NewType = 7;
			MB.Update();
			break;

		case 13: // self left guild

			Clan->IsLeader = false;
			Clan->On = false;
			Use = false;
			Clan->Wait_Action = false;
			Clan->SelectedAction = 0;
			pldatabase->own->isinguild = false;
			memset(&pldatabase->own->tag, 0, sizeof(pldatabase->own->tag));

			sprintf(MB.NewMsg, "You left the guild!");
			MB.NewType = 7;
			MB.Update();

			break;

		case 14: // Transfer deed

			sprintf(MB.NewMsg, "The guild leader has changed!");
			MB.NewType = 7;
			MB.Update();

			break;

		case 15: // New online member.

			memset(&NewMember, 0, sizeof(NewMember));
			recv(Socket, buffer, sizeof(NewMember), 0);
			memcpy(&NewMember, buffer, sizeof(NewMember));

			// Clear out current clan member data			
			memset(&Clan->UsedSlot, 0, sizeof(Clan->UsedSlot));
			memset(&Clan->IDs, 0, sizeof(Clan->IDs));
			memset(&Clan->Names, 0, sizeof(Clan->Names));
			memset(&Clan->Status, 0, sizeof(Clan->Status));
			memset(&Clan->ranks, 0, sizeof(Clan->ranks));

				for (i = 0; i < 50; i++)
				{
					if (!Clan->UsedSlot[i])
					{			
						if (NewMember.Name[i])
						{
							strcpy(Clan->Names[i],	NewMember.Name[i]);
							Clan->UsedSlot[i]		= true;
							Clan->IDs[i]			= NewMember.ID[i];
							Clan->Status[i]			= NewMember.Status[i];
							Clan->ranks[i]			= NewMember.rank[i];
						}
					}
				}

				Clan->Deed();
				Clan->D_Names();
				Clan->Actions();
			
			break;
		
		case 17: // Tag Taken

			sprintf(MB.NewMsg, "That guild tag is taken!");
			MB.NewType = 7;
			MB.Update();
		
			break;
		
		case 18: // Name Taken

			sprintf(MB.NewMsg, "That guild name is taken!");
			MB.NewType = 7;
			MB.Update();

			break;

		case 19: // Confirm tag

			recv(Socket, buffer, sizeof(NewTag), 0);
			memcpy(&NewTag, buffer, sizeof(NewTag));

			strcpy(pldatabase->own->tag, NewTag.Tag);
			strcpy(Clan->Tag, NewTag.Tag);

			break;

		case 20: // Confirm name

			recv(Socket, buffer, sizeof(NewName), 0);
			memcpy(&NewName, buffer, sizeof(NewName));

			strcpy(Clan->Name, NewName.Name);

            break;	
		
		case 21: // Name to long

			sprintf(MB.NewMsg, "Guild name must be between 1 and 43 characters!");
			MB.NewType = 7;
			MB.Update();

			break;
		
		case 22: // Tag to long

			sprintf(MB.NewMsg, "Guild tag must be between 1 and 4 characters!");
			MB.NewType = 7;
			MB.Update();

			break;
		}

		break;

	case 53:  // door things.

		short x,y, dir;
		dir = buffer[1];

		recv(Socket, buffer, sizeof(x), 0);
		memcpy(&x, buffer, sizeof(x));
		recv(Socket, buffer, sizeof(y), 0);
		memcpy(&y, buffer, sizeof(y));

		obj_List = Objects[x / 32][y / 32];

		while (obj_List != NULL)
		{
			if (obj_List->X == x && obj_List->Y == y)
			{
				switch (dir)
				{
				case 1: obj_List->On = false; 
						calc_LOS();
					break;
				case 2: obj_List->On = true;
						calc_LOS();
					break;
				}

				obj_List = NULL;
			}
				
			if (obj_List != NULL)
				obj_List = obj_List->next;
		}

		break;


	case 54: // Secure trade

		switch (buffer[1])
		{
		case 1: // Own lock changed.
			
			recv(Socket, buffer, 1, 0);

			if (buffer[0] != 0)
				st->locks[0] = true;
			else
				st->locks[0] = false;
			
			break;
		case 2: // His lock changed.

			recv(Socket, buffer, 1, 0);

			if (buffer[0] != 0)
				st->locks[1] = true;
			else
				st->locks[1] = false;
			
			break;

		case 3: // Both trades unlocked.

			memset(&st->locks, 0, sizeof(st->locks));

			break;

		case 4: // Item appeared to own tradelist
			
			short k;
			recv(Socket, buffer, sizeof(k), 0);
			memcpy(&k, buffer, sizeof(k));
			st->add_ownoffer(k);

			break;

		case 5: // Item appeared to his tradelist

			short f, t, i, ki;
			unsigned long a;

			recv(Socket,	buffer, sizeof(f),	0);
			memcpy(&f,		buffer, sizeof(f));
			recv(Socket,	buffer, sizeof(t),	0);
			memcpy(&t,		buffer, sizeof(t));
			recv(Socket,	buffer, sizeof(i),	0);
			memcpy(&i,		buffer, sizeof(i));
			recv(Socket,	buffer, sizeof(a),	0);
			memcpy(&a,		buffer, sizeof(a));
			recv(Socket,	buffer, sizeof(ki), 0);
			memcpy(&ki,		buffer, sizeof(ki));

			st->add_hisoffer(f, t, i, a, ki);

			break;

		case 6: // Item removed from own tradelist

			recv(Socket, buffer, sizeof(k), 0);
			memcpy(&k, buffer, sizeof(k));
			st->remove_ownoffer(k);

			break;

		case 7: // Item removed from his tradelist

			recv(Socket, buffer, sizeof(f), 0);
			memcpy(&f, buffer, sizeof(f));

			st->remove_hisoffer(f);

			break;

		case 8: // Abort trade

			st->clear();

			MB.NewMessage("Trade finished.", 2);

			break;

		case 10: // Wanna trade message
			
			char Stringi[25];
			memset(&Stringi, 0, sizeof(Stringi));

			recv(Socket, buffer, sizeof(Stringi), 0);
			memcpy(&Stringi, buffer, sizeof(Stringi));
						
			sprintf(MB.NewMsg, "%s offers you a trade.(Press f4 to accept)", Stringi);
			MB.NewType = 2;
			MB.Update();

			st->petime = SDL_GetTicks();

			break;

		case 11: // Start trade

			st->init();

			memset(&Stringi, 0, sizeof(Stringi));

			recv(Socket, buffer, 1, 0);
			int siz;
			siz = buffer[0];
						
			recv(Socket, buffer, siz, 0);
			memcpy(&Stringi, buffer, siz);
						
			strcpy(st->tradersname, Stringi);

			break;

		case 20: // Trade aborted/failed

			st->clear();

			MB.NewMessage("Trade failed!", 6);

			break;

		case 21: // Aborted

			st->clear();

			MB.NewMessage("Trade aborted.", 6);

			break;
		}

		break;

	//-----------
	// GM Things
	//-----------

	case 60:

		switch (buffer[1])
		{
		case 1: // GM Mode activated.

			recv(Socket, buffer, 1, 0);

			GMMode = buffer[0];

			//Info->newInfoMsg("Moderating game");
			
		//	MB.NewType = 2;
		//	MB.Update();

			break;
		case 2: // Invisibility on/off

			recv(Socket, buffer, 1, 0);

			Invisibility = buffer[0];

			if (Invisibility == 1)	
				Info->newInfoMsg((char*)"You are now invisible");
			if (Invisibility == 0)
				Info->newInfoMsg((char*)"You are now visible");

			break;		
		case 4: // ListenNormal on/off

			recv(Socket, buffer, 1, 0);

			ListenNormal = buffer[0];

			if (ListenNormal == 1)			
				Info->newInfoMsg((char*)"Listening worldwide normal chat");
			if (ListenNormal == 0)
				Info->newInfoMsg((char*)"Not listening worldwide normal chat anymore");

			break;		
		case 49: // ListenGuild on/off

			recv(Socket, buffer, 1, 0);

			ListenGuild = buffer[0];

			if (ListenGuild == 1)			
				strcpy(MB.NewMsg, "ListenGuild: ON.");
			if (ListenGuild == 0)
				strcpy(MB.NewMsg, "ListenGuild: OFF.");
	
			MB.NewType = 2;
			MB.Update();

			break;		
		case 5: // ListenWhisper on/off

			recv(Socket, buffer, 1, 0);

			ListenWhisper = buffer[0];

			if (ListenWhisper == 1)			
				strcpy(MB.NewMsg, "ListenWhisper: ON.");
			if (ListenWhisper == 0)
				strcpy(MB.NewMsg, "ListenWhisper: OFF.");
			MB.NewType = 2;
			MB.Update();

			break;		
		case 6: // Infopacket

			PACKET_INFOSTATS A;
			memset(&A, 0, sizeof(A));
	
			recv(Socket, buffer, sizeof(A), 0);
			memcpy(&A, buffer, sizeof(A));

			sprintf(MB.NewMsg, "%s: [Lvl: %d] [HP: %d/%d] [Str: %d] [Dex: %d] [Con: %d] [Int: %d] [Cha: %d] [Lck: %d]", A.Name, A.Level, A.HPLeft, A.HPMax, A.Str, A.Dex, A.Con, A.Int, A.Cha, A.Lck);
			MB.NewType = 7;
			MB.Update();			

			sprintf(MB.NewMsg, "%s: [Account name: %s] [Exp: %lu]", A.Name, A.AccName, A.Exp);
			MB.NewType = 7;
			MB.Update();			

			break;

		case 7:

			short Client_;
			recv(Socket, buffer, sizeof(Client_), 0);
			memcpy(&Client_, buffer, sizeof(Client_));

			GMTool->onlineAmount = Client_;

			break;

		case 8: // BackTo packet

			PACKET_BACK_TELEPORTTOOTHER B;
			memset(&B, 0, sizeof(B));
	
			recv(Socket, buffer, sizeof(B), 0);
			memcpy(&B, buffer, sizeof(B));

			PACKET_TELEPORTSELF Teleport;

			Teleport.X = B.X;
			Teleport.Y = B.Y;

			buffer[0] = 60;
			buffer[1] = 10;
			memcpy(&buffer[2], &Teleport, sizeof(Teleport));

			send(Socket, buffer, 2 + sizeof(Teleport), 0);	

			break;

		case 50:

			GMMode = 111;

			break;

		case 60: // New Player info for OnlineList

			PACKET_NEWONLINEPLAYER NewPlayer;

			memset(&NewPlayer, 0, sizeof(NewPlayer));
			recv(Socket, buffer, sizeof(NewPlayer), 0);
			memcpy(&NewPlayer, buffer, sizeof(NewPlayer));


			GMTool->addPlayer(NewPlayer);
			
			break;
		}

		break;

	case 62: // Premium account things

		switch( buffer[1])
		{
			case 1: // Premium mode activated

				IsPre = true;

				break;

			case 2: // Remove black from map

				memset(&MapInfo.MapVisibility, 1, sizeof(MapInfo.MapVisibility));
					SaveFogOfWar();

				break;
		}

		break;

	case 63: // Golden account things

		switch( buffer[1])
		{
			case 1: // Golden mode activated

				IsGold = true;

				break;

			case 2: // Remove black from map

				memset(&MapInfo.MapVisibility, 1, sizeof(MapInfo.MapVisibility));
					SaveFogOfWar();

				break;

		}

		break;

	case 64: // Change Map state	
		switch( buffer[1])
		{
			case 1: // Reset Map to black

				memset(&MapInfo.MapVisibility, 0, sizeof(MapInfo.MapVisibility));
					SaveFogOfWar();

				break;

			case 2: // Clear black from map

				memset(&MapInfo.MapVisibility, 1, sizeof(MapInfo.MapVisibility));
					SaveFogOfWar();

				break;

		}

		break;

	//-----------------
	// Combat messages
	//-----------------

	case 97: 

		memset(&NewCT, 0, sizeof(NewCT));
		recv(Socket, buffer, sizeof(NewCT), 0);
		memcpy(&NewCT, buffer, sizeof(NewCT));

			short NewHP;
		switch (NewCT.Type)
		{
		case CT_HEALED:
			
			NewHP = P.HPLeft + NewCT.Amount;
			if (NewHP >= P.HPMax)
			{
				P.HPLeft = P.HPMax;
			} else {
				P.HPLeft = NewHP;
			}

			sprintf(CT.newmsg, "%s + %d", NPCTypes[NewCT.FromTo].Name, NewCT.Amount);
			CT.newmsgtype = 6;
			CT.Update();

			break;
		case CT_REGEN:
			
				P.HPLeft = NewCT.Amount;

			break;
		case CT_DAMAGETAKEN:

			NewHP = P.HPLeft - NewCT.Amount;
			if (NewHP >= 0)
				P.HPLeft = NewHP;

			sprintf(CT.newmsg, "%s: %d", MonsterTypes[NewCT.FromTo].Name, NewCT.Amount);

			CT.newmsgtype = 5;

			CT.Update();
			break;

		case CT_DAMAGEDEALT:

			sprintf(CT.newmsg, "%s: %d", MonsterTypes[NewCT.FromTo].Name, NewCT.Amount);

			CT.newmsgtype = 2;

			CT.Update();
			break;
		case CT_EXPRECEIVED:

			sprintf(CT.newmsg, "%s: %lu", skills[NewCT.SkillType].name, NewCT.Amount);

			P.Skills[NewCT.SkillType].Exp += NewCT.Amount;

			CT.newmsgtype = 1;

			CT.Update();

			break;
		case CT_EXPRECEIVED_MAX:

			sprintf(CT.newmsg, "%lu", NewCT.Amount);

			CT.newmsgtype = 7;

			CT.Update();

			break;
		case 20:

			sprintf(CT.newmsg, "%s: %d", NPCTypes[NewCT.FromTo].Name, NewCT.Amount);

			CT.newmsgtype = 2;

			CT.Update();

			break;
		case 21:

			PlayerNode *tmp;

			tmp = pldatabase->getbyid(NewCT.FromTo);

			if (tmp == NULL) break;
			
			NewHP = NewCT.Amount;
			if (NewHP >= 0)
				P.HPLeft = NewHP;

			sprintf(CT.newmsg, "%s: %d", tmp->name, NewCT.Amount);
			CT.newmsgtype = 5;
			CT.Update();

			break;
		case 22:
			
			NewHP = NewCT.Amount;
			if (NewHP > 0)
				P.HPLeft = NewHP;

			sprintf(CT.newmsg, "%s: %d", NPCTypes[NewCT.FromTo].Name, NewCT.Amount);
			CT.newmsgtype = 5;
			CT.Update();

			break;
		case 23:

			tmp = pldatabase->getbyid(NewCT.FromTo);
			if (tmp == NULL) break;

			sprintf(CT.newmsg, "%s: %d", tmp->name, NewCT.Amount);
			CT.newmsgtype = 2;
			CT.Update();

			break;
		case 24:

			tmp = pldatabase->getbyid(NewCT.FromTo);
			if (tmp == NULL) break;

			sprintf(CT.newmsg, "%s: %d", tmp->name, NewCT.Amount);
			CT.newmsgtype = 5;
			CT.Update();

			break;

		case 25: // GM Kill		

			P.HPLeft = 0;

			sprintf(CT.newmsg, "GM: %d", NewCT.Amount);
			CT.newmsgtype = 5;
			CT.Update();

			sprintf(MB.NewMsg, "You have been killed by a GM!");
			MB.NewType = 6;
			MB.Update();

			break;
		case 26: // GM Heal

			NewHP = NewCT.Amount;
			if (NewHP >= P.HPMax)
			{
				P.HPLeft = P.HPMax;
			} else {
				P.HPLeft = NewHP;
			}

			sprintf(CT.newmsg, "GM: %d", NewCT.Amount);
			CT.newmsgtype = 2;
			CT.Update();

			sprintf(MB.NewMsg, "You have been healed by a GM!");
			MB.NewType = 2;
			MB.Update();

			break;
		}

		break;

	//------------------
	// Timeout & Timein
	//------------------

	case 98: 

		LastMessageTime = SDL_GetTicks(); 

		break;
	// All Messages and texts from server into this case.
	case 99: 
		switch (buffer[1])
		{			
		// MOTD
		case 1: 

			break;

		case 2: //

			sprintf(MB.NewMsg, "You are overburdened!");
			MB.NewType = 7;
			MB.Update();
			break;

		case 3: // Kick

			isKicked = true;	

			break;

		case 4: // ban

			isBanned = true;	

			break;

		case 5: // You have died by monster

			short TmpType;

			recv(Socket, buffer, sizeof(TmpType), 0);

			memcpy(&TmpType, buffer, sizeof(TmpType));

			P.HPLeft = P.HPMax * 0.75;

			sprintf(MB.NewMsg, "You have been slain by ï%sÅ.", MonsterTypes[TmpType].Name);
			MB.NewType = 6;
			MB.Update();

			break;

		case 6: // You have died by NPC

			recv(Socket, buffer, sizeof(TmpType), 0);

			memcpy(&TmpType, buffer, sizeof(TmpType));

			P.HPLeft = P.HPMax * 0.75;

			sprintf(MB.NewMsg, "You have been slain by ï%sÅ.", NPCTypes[TmpType].Name);
			MB.NewType = 6;
			MB.Update();

			break;

		case 7: // Done

			sprintf(MB.NewMsg, "Done.");
			MB.NewType = 2;
			MB.Update();

			break;
		case 8: // Crim count ++

			sprintf(MB.NewMsg, "Jailtime have been added to you for breaking the Eldoria rules.");
			MB.NewType = 2;
			MB.Update();

			break;

		case 10: // You have been healed by %d points.

			TmpType = 0;

			recv(Socket, buffer, sizeof(TmpType), 0);

			memcpy(&TmpType, buffer, sizeof(TmpType));

			sprintf(MB.NewMsg, "You have been healed by Ä%dÅ points.", TmpType);
			MB.NewType = 7;
			MB.Update();

			break;

		case 11: // You do not need to be healed.

			sprintf(MB.NewMsg, "You do not require healing.");
			MB.NewType = 7;
			MB.Update();

			break;

		case 12: //  You are not strong enough to wear

			sprintf(MB.NewMsg, "You are not strong enough to wear that item.");
			MB.NewType = 7;
			MB.Update();

			break;
		case 13: // 

			short Tmp;
            
			recv(Socket, buffer, sizeof(Tmp), 0);

			memcpy(&Tmp, buffer, sizeof(Tmp));

			if (iWeared[Tmp].Family == FAMILY_WEAPON)
			{
				sprintf(MB.NewMsg, "You feel that your item ï%sÅ is weakening.");
				MB.NewType = 7;
				MB.Update();
			}
			if (iWeared[Tmp].Family == FAMILY_ARMOR)
			{
				sprintf(MB.NewMsg, "You feel that your item ï%sÅ is weakening.");
				MB.NewType = 7;
				MB.Update();
			}

			break;

		case 14:

			sprintf(MB.NewMsg, "You must wait before using another item.");
			MB.NewType = 6;
			MB.Update();

			break;

		case 15:

			sprintf(MB.NewMsg, "You have been jailed!");
			MB.NewType = 6;
			MB.Update();

			break;

		case 16:

			sprintf(MB.NewMsg, "You are not high enough rank for that!");
			MB.NewType = 6;
			MB.Update();

			break;

		case 20: 

			unsigned long amount;
			recv(Socket, buffer, sizeof(amount), 0);

			memcpy(&amount, buffer, sizeof(amount));

			sprintf(MB.NewMsg, "You lost Ä%luÅ exp!", amount);
			MB.NewType = 7;
			MB.Update();

			break;

		case 30:

			Info->newInfoMsg((char*)"Character name is already taken, can't change!");

			break;

		case 31:

			Info->newInfoMsg((char*)"Character name changed!");

			break;

		case 54: 

			short _tmp;
			_tmp = -1;
			recv(Socket, buffer, sizeof(_tmp), 0);

			if (_tmp >= 0)
			{
				for (i = 0; i < MaxBackpackItems; i++)
				{
					if (iBackpack[i].KnowID == _tmp)
					{
						sprintf(MB.NewMsg, "You broke your ï%sÅ.", Useables[iBackpack[i].ID].Name);
						MB.NewType = 6;
						MB.Update();
                        i = MaxBackpackItems;
					}				
				}				
			}		

			break;
		case 55: // Mining

			sprintf(MB.NewMsg, "Unable to mine here.");
			MB.NewType = 6;
			MB.Update();

			break;
		case 56: //

			sprintf(MB.NewMsg, "Nothing to mine here.");
			MB.NewType = 6;
			MB.Update();

			break;
		case 57:

			short _tm;
			_tm = -1;
			recv(Socket, buffer, sizeof(_tm), 0);

			memcpy(&_tm, buffer, sizeof(_tm));

			if (_tm > 0)
			{
				sprintf(MB.NewMsg, "You have mined Ä%dÅ ore.", _tm);
				MB.NewType = 2;
				MB.Update();
			}

			break;
		case 58: 

			sprintf(MB.NewMsg, "You failed to mine anything.");
			MB.NewType = 6;
			MB.Update();

			break;
		case 59: 

			sprintf(MB.NewMsg, "You begin to mine...");
			MB.NewType = 4;
			MB.Update();
			break;

		case 60: 

			sprintf(MB.NewMsg, "Info you requested is not available. (Player doesn't excist / not online / ...)");
			MB.NewType = 7;
			MB.Update();

			break;

		case 61: 

			sprintf(MB.NewMsg, "You have set your spawn.");
			MB.NewType = 7;
			MB.Update();

			break;

		case 62:

			sprintf(MB.NewMsg, "That player is busy right now, try again!");
			MB.NewType = 7;
			MB.Update();

			break;

		case 70:

			sprintf(MB.NewMsg, "You destroyed all your ore.");
			MB.NewType = 6;
			MB.Update();

			break;

		case 71:
            
			_tm = -1;
			recv(Socket, buffer, sizeof(_tm), 0);

			memcpy(&_tm, buffer, sizeof(_tm));

			if (_tm > 0)
			{
				sprintf(MB.NewMsg, "You successfully smelted Ä%dÅ ore to ingots.", _tm);
				MB.NewType = 2;
				MB.Update();
			}
			break;
		case 72:
            
			_tm = -1;
			recv(Socket, buffer, sizeof(_tm), 0);

			memcpy(&_tm, buffer, sizeof(_tm));

			if (_tm > 0)
			{
				sprintf(MB.NewMsg, "You destroyed Ä%dÅ ore.", _tm);
				MB.NewType = 6;
				MB.Update();
			}
			break;

		case 73: 

			sprintf(MB.NewMsg, "You begin to smelt...");
			MB.NewType = 4;
			MB.Update();

			break;

		case 74: 

			sprintf(MB.NewMsg, "You begin to forge...");
			MB.NewType = 4;
			MB.Update();

			break;
		case 75: 

			sprintf(MB.NewMsg, "You failed to forge the item.");
			MB.NewType = 6;
			MB.Update();

			break;
		case 76: 

			sprintf(MB.NewMsg, "You successfully forged the item!");
			MB.NewType = 2;
			MB.Update();

			break;
		case 77: 

			sprintf(MB.NewMsg, "You cannot PM yourself!");
			MB.NewType = 7;
			MB.Update();

			break;		
		case 78: // Kicked for rank change

			isRanked = true;	

			break;
		case 93: 

			sprintf(MB.NewMsg, "You have been muted by a GM!");
			MB.NewType = 6;
			MB.Update();

			break;
		case 94: 

			sprintf(MB.NewMsg, "You are muted!");
			MB.NewType = 6;
			MB.Update();

			break;
		case 95: 

			sprintf(MB.NewMsg, "You been unmuted by a GM!");
			MB.NewType = 2;
			MB.Update();

			break;
		case 99:

			sprintf(MB.NewMsg, "Server is saving, please wait.");
			MB.NewType = 6;
			MB.Update();

			LastMessageTime = SDL_GetTicks();

			break;

		case 100:
			sprintf(MB.NewMsg, "Server save completed.");
			MB.NewType = 2;
			MB.Update();

			LastMessageTime = SDL_GetTicks();
			break;

		}
		break;

	case 100:

		PACKET_NEWINFOMSG NewInfo;
		memset(&NewInfo, 0, sizeof(NewInfo));
		recv(Socket, buffer, sizeof(NewInfo), 0);

		memcpy(&NewInfo, buffer, sizeof(NewInfo));

		Info->newInfoMsg(NewInfo.Msg);
		

		break;

	default:

		break;
	
	}


}



ItemClass *GetItemByID( short InList, short ID)
{
	int i;
	switch (InList)
	{
	case ItemBackpack:

		for (i = 0; i < MaxBackpackItems; i++)
		{
			if (iBackpack[i].Used)
			{
				if (iBackpack[i].KnowID == ID)
					return &iBackpack[i];
			}
		}

		break;
	case ItemBank:

		for (i = 0; i < MaxBankItems; i++)
		{
			if (iBank[i].Used)
			{
				if (iBank[i].KnowID == ID)
					return &iBank[i];
			}
		}

		break;
	case ItemWeared:

		break;
	case ItemGround:

		for (i = 0; i < MaxItems; i++)
		{
			if (iGround[i].Used)
			{
				if (iGround[i].KnowID == ID)
					return &iGround[i];
			}
		}

		break;
	
	}
	return NULL;
}






//--------------------------------------------
// Get direction where player is mousemoving.
//--------------------------------------------

int GetDir()
{
	short ax, ay, dx, dy;

	ax			= MX;
	ay			= MY;
	ax			-= 7;
	ay			-= 7;

	ax			-= 210;
	ay			-= 210;

	if (ax < 0)	dx = ax * -1;
	else		dx = ax;
	if (ay < 0) dy = ay * -1;
	else		dy = ay;

	if (ax < 0 && ay < 0)
	{
		if (dx >= (dy / 2))
		{
			if (dy >= (dx / 2))
				return 7;
			else
				return 4;
		}
		else
			return 8;	
	}
	else if (ax >= 0 && ay < 0)
	{
		if (dx >= (dy / 2))
		{
			if (dy >= (dx / 2))
				return 9;
			else
				return 6;		
		}
		else
			return 8;
	}
	else if (ax < 0 && ay >= 0)
	{
		if (dx >= (dy / 2))
		{
			if (dy >= (dx / 2))
				return 1;
			else
				return 4;
		}
		else 
			return 2;
	}
	else if (ax >= 0 && ay >= 0)
	{
		if (dx >= (dy / 2))
		{
			if (dy >= (dx / 2))
				return 3;
			else
				return 6;
		}
		else 
			return 2;
	}
	return 0;
}








bool ClientClass::mbut_down()
{	
	if (event.button.button == 1) mbut1 = true;
	if (event.button.button == 1 && !skill.v_skills && !shop.on && !Clan->On && !GMTool->On)
	{
		MouseButtonDown();
	}
	if (event.button.button == 3) mbut2 = true;
	if (event.button.button == 3 && !skill.v_skills && !shop.on && !Clan->On && !GMTool->On)
	{
		MouseRightButtonDown();
	}
	return true;
}

bool ClientClass::mbut_up()
{
	if (event.button.button == 4) // Scroll up
	{
		if (GMTool->On)
		{
			GMTool->Scroll -= 3;
	
			if (GMTool->Scroll < 0) 
				GMTool->Scroll = 0;
		}
	}
	if (event.button.button == 5) // Scroll down
	{
		if (GMTool->On)
			GMTool->Scroll += 3;
	}

	if (event.button.button == 1)
	{
		mbut1 = false;

		if (GMTool->On)
		{
			GMTool->Mouseclick();
			return true;
		}

		if (BS->on)
		{
			BS->mouseclick();
			return true;
		}

		if (Clan->On)
		{
			Clan->MouseClick();
			return true;
		}

		if (st->on)
		{
			st->mouseclick();
			return true;
		}

		if (shop.on || shop.pending)
		{
			shop.mouseclick();
			return true;
		}

		if (skill.v_skills)
		{
			skill.mouseclick();
			return true;
		}

		MouseButtonUp();

		return true;
	}

	if (event.button.button == 3)
	{		
		mbut2 = false;


		if (idatabase->rightclick())
		{
			if (Client.MouseMoving) Client.MouseMoving = false;
//			if (Minimap.Size) Minimap.Size = false;
			return true;
		}


		if (!skill.v_skills && !shop.on && !GMTool->On && !Clan->On && !BS->on)
			MouseRightButtonUp();
		
		return true;
	}

	if (Use && UseTarget == -1) Use = false;

	return true;
}





void ItemClass::addto()
{
	if (database->items == NULL)
	{
		database->items			= this;
		prev					= NULL;
		next					= NULL;
	}
	else
	{
		next					= database->items;
		prev					= NULL;
		database->items->prev	= this;
		database->items			= this;	
	}
}

void ItemClass::remove()
{
	if (database->items == this)
	{
		database->items = database->items->next;
		if (database->items != NULL)
			database->items->prev = NULL;
	}
	else
	{
		if (prev != NULL)
			prev->next = next;
		if (next != NULL)
			next->prev = prev;
	}
	delete this;
}

void ItemDatabase::clearlist()
{
	ItemClass *tmp;
	while (items)
	{
		tmp = items->next;
		delete items;
		items = tmp;
	}
	items = NULL;
}



void LoginClass::PrepareAccCreation()
{
	textedits = new texteditdatabase();

	textedits->NewTextedit((char*)"", 395, 208, 150, 16, false, false, 20, false, false, 1000001, 1000002);
	textedits->NewTextedit((char*)"", 395, 240, 150, 16, false, false, 20, false, false, 1000002, 1000001);
	//textedits->NewTextedit("Your e-mail", 400, 240, 150, 16, false, false, 65, false, false, 1000003, 1000001);

	textedit *tmp;
	tmp = textedits->list;
	tmp->target(tchar, (void *)AccName);
	tmp = tmp->next;
	tmp->target(tchar, (void *)AccPass);
	//tmp = tmp->next;
	//tmp->target(tchar, (void *)EMail);
}

void LoginClass::CleanupAccCreation()
{
	delete textedits;
}