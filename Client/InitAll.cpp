
#include "main.h"


extern SOCKET Socket;

extern struct sockaddr_in Server_Address;
extern struct sockaddr_in ownAddr;

extern char *buffer;

extern int GameState;


bool InitAll()
{

	bool Error = false;
	bool Error2 = true;


	loadSettings();

	Minimap->On = true;
	Backpack.On = true;

	Dragging.TargetItem = NULL;
	Client.AttackMode = PEACE;
//	Shop.TargetID = -1;
//	Shop.TargetItem = NULL;

	QK.ShowQuickSlots = true;

	memset(&QK.QuickSlotTargets, -1, sizeof(QK.QuickSlotTargets));

	Client.CombatTextOn = true;
	Error = false;

	//fprintf(stderr, "testia\n");

	fprintf(stderr, "InitSDL(); ... ");
	Error = InitSDL();
	//fprintf(stderr, "testia\n");

	v_Body.Docked = true;
	v_Body.On	  = false;
	v_Body.X      = 800 - 119 - sdl.Rect_addgf[ BACKPACK ].w;
	v_Body.Y      = 600 - sdl.Rect_addgf[ BACKPACK ].h;

	if (Error)
	{
		fprintf(stderr, "InitSDL failed.\n");
		Error2 = false;
	}
	else
	{
		fprintf(stderr, "InitSDL success.\n");
	}


	int i, j, k;
	for (i = 0; i < 21; i++)
	{
		for (j = 0; j < 21; j++)
		{
			sdl.Main[i][j].x = i * 20 +7;
			sdl.Main[i][j].y = j * 20 +7;
			sdl.Main[i][j].h = 20;
			sdl.Main[i][j].w = 20;
		}	
	}

	for (i = 0; i < 14; i++)
	{
		for (j = 0; j < (m_Tiles / 14); j++)
		{
			sdl.TileRect[i + j * 14].x = i * 20;
			sdl.TileRect[i + j * 14].y = j * 20;
			sdl.TileRect[i + j * 14].h = 20;
			sdl.TileRect[i + j * 14].w = 20;
		}
	}	

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 10; j++)
		{
			for (k = 0; k < 4; k++)
			{
				sdl.Avatars[i * 10 + j][k].x = j * 80 + k * 20;
				sdl.Avatars[i * 10 + j][k].y = i * 20;
				sdl.Avatars[i * 10 + j][k].h = 20;
				sdl.Avatars[i * 10 + j][k].w = 20;
			}
		}
	}


	return Error2;
}

void Connect_To_Server()
{
	// Networking thingies.

	fprintf(stderr, "Initializing networking...\n");

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		fprintf(stderr, "Error at WSAStartup().\n");
	}
	
	unsigned long blockmode;
	blockmode = 0;

	Socket = socket(AF_INET, SOCK_STREAM, 0);
/*	ioctlsocket(Socket, FIONBIO, &blockmode);

	const char yes = 1;

	if (setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
	{
		fprintf(stderr, "setsockopt() failed.\n");
		exit(1);
	}*/

	hostent *h;
	h = gethostbyname(Hostname);

	if (h == NULL)
		fprintf(stderr, "gethostbyname() failed\n");

	memcpy((char *) &Server_Address.sin_addr.s_addr, h->h_addr_list[0], h->h_length);

	Server_Address.sin_family = AF_INET;
	Server_Address.sin_port = htons(Port);

//	ownAddr.sin_family = AF_INET;
//	ownAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//	ownAddr.sin_port = htons(Port);
//	int nret;
/*while(true)
{
connect(Socket, (LPSOCKADDR)&Server_Address, sizeof(struct sockaddr));

}*/
	if (connect(Socket, (LPSOCKADDR)&Server_Address, sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		fprintf(stderr, "Connect() failed.\n");
	}
}


bool InitSDL()
{
	// We only need Video functions of SDL. 

	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );
	Screen.Fullscreen = false;
 
	SDL_WM_SetIcon(SDL_LoadBMP("Bunnkia.ico"), NULL);

	if (!Screen.Fullscreen)
		Screen.fscreen = SDL_SetVideoMode(800, 600, 24, SDL_HWSURFACE);
	else
		Screen.fscreen = SDL_SetVideoMode(800, 600, 24, SDL_HWSURFACE|SDL_FULLSCREEN);

	SDL_WM_SetCaption(TITLETEXT, "Bunnkia.ico");

	// This filters some SDL commands for specialized care. It makes SDL require less CPU. 
	SDL_SetEventFilter(FilterEvents);

	// Do some window stuff. 
//		Screen.screen = SDL_SetVideoMode(800, 600, 0, SDL_ANYFORMAT|SDL_SWSURFACE);

	// Enable Unicode. 
	SDL_EnableUNICODE( 1 );

	/* First of all, create backsurface tmp. */
	Uint32 rmask,gmask,bmask,amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif


	SDL_Surface *tmp;

	tmp = SDL_CreateRGBSurface(SDL_HWSURFACE, 800, 600, 24, rmask, gmask, bmask, amask);

	if (tmp == NULL)
	{
		printf("Failed to create RGB Surface");
		return 0;
	}

	Screen.screen = SDL_DisplayFormat( tmp );


	// Load graphics. 

	load_graphics();

/*	int all_size = 0;

	G.tiles = sdl.LoadImage(file_tiles_w, file_tiles_h, files_depth, all_size);
	all_size += file_tiles_h * (file_tiles_w * 3);

	G.avatars = sdl.LoadImage(file_avatars_w, file_avatars_h, files_depth, all_size);
	all_size += file_avatars_h * (file_avatars_w * 3);

	G.layout = sdl.LoadImage(file_layout_w, file_layout_h, files_depth, all_size);
	all_size += file_layout_h * (file_layout_w * 3);

	G.addgf = sdl.LoadImage(file_addgf_w, file_addgf_h, files_depth, all_size);
	all_size += file_addgf_h * (file_addgf_w * 3);

	G.items = sdl.LoadImage(file_items_w, file_items_h, files_depth, all_size);
	all_size += file_items_h * (file_items_w * 3);

	G.skills = sdl.LoadImage(file_skills_w, file_skills_h, files_depth, all_size);
	all_size += file_skills_h * (file_skills_w * 3);

	G.shop = sdl.LoadImage(file_shop_w, file_shop_h, files_depth, all_size);
	all_size += file_shop_h * (file_shop_w * 3);

	G.Identify = sdl.LoadImage(file_identify_w, file_identify_h, files_depth, all_size);
	all_size += file_identify_h * (file_identify_w * 3);

	G.List = sdl.LoadImage(file_list_w, file_list_h, files_depth, all_size);
	all_size += file_list_h * (file_list_w * 3);

	G.Cursors = sdl.LoadImage(file_cursors_w, file_cursors_h, files_depth, all_size);
	all_size += file_cursors_h * (file_cursors_w * 3);

	G.objects = sdl.LoadImage(file_objects_w, file_objects_h, files_depth, all_size);
	all_size += file_objects_h * (file_objects_w * 3);

	Login.bGround = sdl.LoadImage(file_bGround_w, file_bGround_h, files_depth, all_size);
	all_size += file_bGround_h * (file_bGround_w * 3);

	Login.fGround = sdl.LoadImage(file_fGround_w, file_fGround_h, files_depth, all_size);
	all_size += file_fGround_h * (file_fGround_w * 3);

	Login.charchoose = sdl.LoadImage(file_charchoose_w, file_charchoose_h, files_depth, all_size);
	all_size += file_charchoose_h * (file_charchoose_w * 3);

	Login.accCreation = sdl.LoadImage(file_acccreation_w, file_acccreation_h, files_depth, all_size);
	all_size += file_acccreation_h * (file_acccreation_w * 3);

	Login.mainmenu = sdl.LoadImage(file_mainmenu_w, file_mainmenu_h, files_depth, all_size);
	all_size += file_mainmenu_h * (file_mainmenu_w * 3);

	Login.charCreation = sdl.LoadImage(file_charcreation_w, file_charcreation_h, files_depth, all_size);
	all_size += file_charcreation_h * (file_charcreation_w * 3);
*/

	sdl.Rect_addgf[BACKPACK].w = Backpack_w;
	sdl.Rect_addgf[BACKPACK].h = Backpack_h;

	sdl.Rect_addgf[MINIMAP].w = Minimap_w;
	sdl.Rect_addgf[MINIMAP].h = Minimap_h;
	sdl.Rect_addgf[MINIMAP].x = Backpack_w+1;

	sdl.Rect_addgf[CHATBOX].x = 0;
	sdl.Rect_addgf[CHATBOX].y = 146;
	sdl.Rect_addgf[CHATBOX].h = 44;
	sdl.Rect_addgf[CHATBOX].w = 600;

	sdl.Rect_addgf[BUTTON_N].x = 146;
	sdl.Rect_addgf[BUTTON_N].y = 135;
	sdl.Rect_addgf[BUTTON_N].h = 11;
	sdl.Rect_addgf[BUTTON_N].w = 11;

	sdl.Rect_addgf[BUTTON_W].x = 157;
	sdl.Rect_addgf[BUTTON_W].y = 135;
	sdl.Rect_addgf[BUTTON_W].h = 11;
	sdl.Rect_addgf[BUTTON_W].w = 11;

	sdl.Rect_addgf[BUTTON_G].x = 168;
	sdl.Rect_addgf[BUTTON_G].y = 135;
	sdl.Rect_addgf[BUTTON_G].h = 11;
	sdl.Rect_addgf[BUTTON_G].w = 11;

	sdl.Rect_addgf[BUTTON_GM].x = 179;
	sdl.Rect_addgf[BUTTON_GM].y = 135;
	sdl.Rect_addgf[BUTTON_GM].h = 11;
	sdl.Rect_addgf[BUTTON_GM].w = 11;

	sdl.Rect_addgf[BUTTON_WC].x = 227;
	sdl.Rect_addgf[BUTTON_WC].y = 134;
	sdl.Rect_addgf[BUTTON_WC].h = 11;
	sdl.Rect_addgf[BUTTON_WC].w = 11;
	
	/* The blue dots
	sdl.Rect_addgf[BUTTON_SC].x = 195;
	sdl.Rect_addgf[BUTTON_SC].y = 135;
	sdl.Rect_addgf[BUTTON_SC].h = 11;
	sdl.Rect_addgf[BUTTON_SC].w = 15;
	*/

	sdl.Rect_addgf[BUTTON_SC].x = 494;
	sdl.Rect_addgf[BUTTON_SC].y = 135;
	sdl.Rect_addgf[BUTTON_SC].h = 11;
	sdl.Rect_addgf[BUTTON_SC].w = 11;
	sdl.Rect_addgf[BUTTON_SCNO].x = 505;
	sdl.Rect_addgf[BUTTON_SCNO].y = 135;
	sdl.Rect_addgf[BUTTON_SCNO].h = 11;
	sdl.Rect_addgf[BUTTON_SCNO].w = 11;

	sdl.Rect_addgf[CHARCHOOSESELECTED].x = 255;
	sdl.Rect_addgf[CHARCHOOSESELECTED].y = 0;
	sdl.Rect_addgf[CHARCHOOSESELECTED].h = 146;
	sdl.Rect_addgf[CHARCHOOSESELECTED].w = 168;

	int ind;
	for (ind = 0; ind < 4; ind++)
	{
		sdl.Rect_addgf[TARGET + ind].x = 166 + ind * 22;
		sdl.Rect_addgf[TARGET + ind].y = 109;
		sdl.Rect_addgf[TARGET + ind].h = 24;
		sdl.Rect_addgf[TARGET + ind].w = 22;
	}
	for (ind = 0; ind < 4; ind++)
	{
		sdl.Rect_addgf[ORB + ind].x = 195 + ind * 4;
		sdl.Rect_addgf[ORB + ind].y = 135;
		sdl.Rect_addgf[ORB + ind].h = 8;
		sdl.Rect_addgf[ORB + ind].w = 4;
	}

	sdl.Rect_addgf[HIT].x = 423;
	sdl.Rect_addgf[HIT].y = 0;
	sdl.Rect_addgf[HIT].h = 20;
	sdl.Rect_addgf[HIT].w = 20;
	sdl.Rect_addgf[MISS].x = 443;
	sdl.Rect_addgf[MISS].y = 0;
	sdl.Rect_addgf[MISS].h = 20;
	sdl.Rect_addgf[MISS].w = 20;

	sdl.Rect_addgf[SPLIT].x = 423;
	sdl.Rect_addgf[SPLIT].y = 20;
	sdl.Rect_addgf[SPLIT].h = 50;
	sdl.Rect_addgf[SPLIT].w = 160;

	sdl.Rect_addgf[LOS].x = 146;
	sdl.Rect_addgf[LOS].y = 109;
	sdl.Rect_addgf[LOS].h = 20;
	sdl.Rect_addgf[LOS].w = 20;

	sdl.Rect_addgf[LOCK].x = 243;
	sdl.Rect_addgf[LOCK].y = 134;
	sdl.Rect_addgf[LOCK].h = 13;
	sdl.Rect_addgf[LOCK].w = 11;

	sdl.Rect_addgf[EXIT].x = 214;
	sdl.Rect_addgf[EXIT].y = 135;
	sdl.Rect_addgf[EXIT].h = 7;
	sdl.Rect_addgf[EXIT].w = 7;

	sdl.Rect_addgf[QUICKSLOTS].x = 0;
	sdl.Rect_addgf[QUICKSLOTS].y = 190;
	sdl.Rect_addgf[QUICKSLOTS].h = 26;
	sdl.Rect_addgf[QUICKSLOTS].w = 358;

	sdl.Rect_addgf[RESPAWN].x = 463;
	sdl.Rect_addgf[RESPAWN].y = 0;
	sdl.Rect_addgf[RESPAWN].h = 20;
	sdl.Rect_addgf[RESPAWN].w = 20;

	sdl.Rect_addgf[YESNOBOX].x = 0;
	sdl.Rect_addgf[YESNOBOX].y = 216;
	sdl.Rect_addgf[YESNOBOX].h = 89;
	sdl.Rect_addgf[YESNOBOX].w = 319;

	sdl.Rect_addgf[ BODY_DECAY1 ].x = 483;
	sdl.Rect_addgf[ BODY_DECAY1 ].y = 0;
	sdl.Rect_addgf[ BODY_DECAY1 ].h = 20;
	sdl.Rect_addgf[ BODY_DECAY1 ].w = 20;

	sdl.Rect_addgf[ BODY_DECAY2 ].x = 503;
	sdl.Rect_addgf[ BODY_DECAY2 ].y = 0;
	sdl.Rect_addgf[ BODY_DECAY2 ].h = 20;
	sdl.Rect_addgf[ BODY_DECAY2 ].w = 20;

	sdl.Rect_addgf[ BODY_DECAY3 ].x = 523;
	sdl.Rect_addgf[ BODY_DECAY3 ].y = 0;
	sdl.Rect_addgf[ BODY_DECAY3 ].h = 20;
	sdl.Rect_addgf[ BODY_DECAY3 ].w = 20;

	sdl.Rect_addgf[ BODY_DECAY4 ].x = 543;
	sdl.Rect_addgf[ BODY_DECAY4 ].y = 0;
	sdl.Rect_addgf[ BODY_DECAY4 ].h = 20;
	sdl.Rect_addgf[ BODY_DECAY4 ].w = 20;

	sdl.Rect_addgf[ BODY_DECAY5 ].x = 563;
	sdl.Rect_addgf[ BODY_DECAY5 ].y = 0;
	sdl.Rect_addgf[ BODY_DECAY5 ].h = 20;
	sdl.Rect_addgf[ BODY_DECAY5 ].w = 20;


	sdl.Rect_addgf[ login_mouseover_Login ].x = 423;
	sdl.Rect_addgf[ login_mouseover_Login ].y = 132;
	sdl.Rect_addgf[ login_mouseover_Login ].h = 14;
	sdl.Rect_addgf[ login_mouseover_Login ].w = 70;

	sdl.Rect_addgf[ login_mouseover_Exit ].x = 423;
	sdl.Rect_addgf[ login_mouseover_Exit ].y = 117;
	sdl.Rect_addgf[ login_mouseover_Exit ].h = 14;
	sdl.Rect_addgf[ login_mouseover_Exit ].w = 48;

	sdl.Rect_addgf[ login_mouseover_Choose ].x = 423;
	sdl.Rect_addgf[ login_mouseover_Choose ].y = 87;
	sdl.Rect_addgf[ login_mouseover_Choose ].h = 14;
	sdl.Rect_addgf[ login_mouseover_Choose ].w = 84;

	sdl.Rect_addgf[ login_mouseover_CreateAcc ].x = 319;
	sdl.Rect_addgf[ login_mouseover_CreateAcc ].y = 216;
	sdl.Rect_addgf[ login_mouseover_CreateAcc ].h = 14;
	sdl.Rect_addgf[ login_mouseover_CreateAcc ].w = 188;

	sdl.Rect_addgf[ login_mouseover_Create ].x = 423;
	sdl.Rect_addgf[ login_mouseover_Create ].y = 102;
	sdl.Rect_addgf[ login_mouseover_Create ].h = 14;
	sdl.Rect_addgf[ login_mouseover_Create ].w = 78;

	sdl.Rect_addgf[ expo_1 ].x = 0;
	sdl.Rect_addgf[ expo_1 ].y = 306;
	sdl.Rect_addgf[ expo_1 ].h = 6;
	sdl.Rect_addgf[ expo_1 ].w = 348;

	sdl.Rect_addgf[ alig_1 ].x = 0;
	sdl.Rect_addgf[ alig_1 ].y = 318;
	sdl.Rect_addgf[ alig_1 ].h = 6;
	sdl.Rect_addgf[ alig_1 ].w = 348;

	sdl.Rect_addgf[ PIDGEON ].x = 423;
	sdl.Rect_addgf[ PIDGEON ].y = 70;
	sdl.Rect_addgf[ PIDGEON ].h = 16;
	sdl.Rect_addgf[ PIDGEON ].w = 16;

	sdl.Rect_addgf[ MINIMAP_BIG ].x = 366;
	sdl.Rect_addgf[ MINIMAP_BIG ].y = 189;
	sdl.Rect_addgf[ MINIMAP_BIG ].h = Minimap_h_BIG;
	sdl.Rect_addgf[ MINIMAP_BIG ].w = Minimap_w_BIG;

	sdl.Rect_addgf[ TRADEWINDOW ].x = 600;
	sdl.Rect_addgf[ TRADEWINDOW ].y = 0;
	sdl.Rect_addgf[ TRADEWINDOW ].h = 255;
	sdl.Rect_addgf[ TRADEWINDOW ].w = 349;

	sdl.Rect_addgf[ lock_up ].x = 622;
	sdl.Rect_addgf[ lock_up ].y = 255;
	sdl.Rect_addgf[ lock_up ].h = 22;
	sdl.Rect_addgf[ lock_up ].w = 24;
	
	sdl.Rect_addgf[ lock_down ].x = 600;
	sdl.Rect_addgf[ lock_down ].y = 255;
	sdl.Rect_addgf[ lock_down ].h = 22;
	sdl.Rect_addgf[ lock_down ].w = 24;
	
	sdl.Rect_addgf[ accept_trade ].x = 610;
	sdl.Rect_addgf[ accept_trade ].y = 74;
	sdl.Rect_addgf[ accept_trade ].h = 28;
	sdl.Rect_addgf[ accept_trade ].w = 28;
	
	sdl.Rect_addgf[ decline_trade ].x = 610;
	sdl.Rect_addgf[ decline_trade ].y = 45;
	sdl.Rect_addgf[ decline_trade ].h = 28;
	sdl.Rect_addgf[ decline_trade ].w = 28;

	sdl.Rect_addgf[ alig_pointer ].x = 0;
	sdl.Rect_addgf[ alig_pointer ].y = 324;
	sdl.Rect_addgf[ alig_pointer ].h = 8;
	sdl.Rect_addgf[ alig_pointer ].w = 3;


	sdl.Rect_addgf[ secure_bg ].x = 644;
	sdl.Rect_addgf[ secure_bg ].y = 0;
	sdl.Rect_addgf[ secure_bg ].h = 256;
	sdl.Rect_addgf[ secure_bg ].w = 373;

	sdl.Rect_addgf[ secure_offerlock ].x = 519;
	sdl.Rect_addgf[ secure_offerlock ].y = 70;
	sdl.Rect_addgf[ secure_offerlock ].h = 61;
	sdl.Rect_addgf[ secure_offerlock ].w = 125;

	sdl.Rect_addgf[ secure_sup_inactive ].x = 583;
	sdl.Rect_addgf[ secure_sup_inactive ].y = 15;
	sdl.Rect_addgf[ secure_sup_inactive ].h = 15;
	sdl.Rect_addgf[ secure_sup_inactive ].w = 15;

	sdl.Rect_addgf[ secure_sup_active ].x = 598;
	sdl.Rect_addgf[ secure_sup_active ].y = 15;
	sdl.Rect_addgf[ secure_sup_active ].h = 15;
	sdl.Rect_addgf[ secure_sup_active ].w = 15;

	sdl.Rect_addgf[ secure_sdown_inactive ].x = 583;
	sdl.Rect_addgf[ secure_sdown_inactive ].y = 0;
	sdl.Rect_addgf[ secure_sdown_inactive ].h = 15;
	sdl.Rect_addgf[ secure_sdown_inactive ].w = 15;

	sdl.Rect_addgf[ secure_sdown_active ].x = 598;
	sdl.Rect_addgf[ secure_sdown_active ].y = 0;
	sdl.Rect_addgf[ secure_sdown_active ].h = 15;
	sdl.Rect_addgf[ secure_sdown_active ].w = 15;


	sdl.Rect_addgf[ secure_bexit_inactive ].x = 613;
	sdl.Rect_addgf[ secure_bexit_inactive ].y = 0;
	sdl.Rect_addgf[ secure_bexit_inactive ].h = 15;
	sdl.Rect_addgf[ secure_bexit_inactive ].w = 15;

	sdl.Rect_addgf[ secure_bexit_active ].x = 628;
	sdl.Rect_addgf[ secure_bexit_active ].y = 0;
	sdl.Rect_addgf[ secure_bexit_active ].h = 15;
	sdl.Rect_addgf[ secure_bexit_active ].w = 15;

	sdl.Rect_addgf[ secure_lock_inactive ].x = 613;
	sdl.Rect_addgf[ secure_lock_inactive ].y = 45;
	sdl.Rect_addgf[ secure_lock_inactive ].h = 15;
	sdl.Rect_addgf[ secure_lock_inactive ].w = 15;

	sdl.Rect_addgf[ secure_lock_active ].x = 628;
	sdl.Rect_addgf[ secure_lock_active ].y = 270;
	sdl.Rect_addgf[ secure_lock_active ].h = 15;
	sdl.Rect_addgf[ secure_lock_active ].w = 15;

	sdl.Rect_addgf[ button_all_inactive ].x = 613;
	sdl.Rect_addgf[ button_all_inactive ].y = 15;
	sdl.Rect_addgf[ button_all_inactive ].h = 15;
	sdl.Rect_addgf[ button_all_inactive ].w = 30;

	sdl.Rect_addgf[ button_all_active ].x = 613;
	sdl.Rect_addgf[ button_all_active ].y = 30;
	sdl.Rect_addgf[ button_all_active ].h = 15;
	sdl.Rect_addgf[ button_all_active ].w = 30;

	sdl.Rect_addgf[ selected_frame ].x = 600;
	sdl.Rect_addgf[ selected_frame ].y = 146;
	sdl.Rect_addgf[ selected_frame ].h = 22;
	sdl.Rect_addgf[ selected_frame ].w = 22;


	sdl.Rect_addgf[ button_change ].x = 439;
	sdl.Rect_addgf[ button_change ].y = 70;
	sdl.Rect_addgf[ button_change ].w = 45;
	sdl.Rect_addgf[ button_change ].h = 15;


	sdl.Rect_addgf[ BANK ].x = 1018;
	sdl.Rect_addgf[ BANK ].y = 0;
	sdl.Rect_addgf[ BANK ].w = 273;
	sdl.Rect_addgf[ BANK ].h = 146;

	sdl.Rect_addgf[ tgprotection ].x = 484;
	sdl.Rect_addgf[ tgprotection ].y = 70;
	sdl.Rect_addgf[ tgprotection ].w = 16;
	sdl.Rect_addgf[ tgprotection ].h = 16;

	sdl.Rect_addgf[ bgframe ].x = 622;
	sdl.Rect_addgf[ bgframe ].y = 146;
	sdl.Rect_addgf[ bgframe ].w = 22;
	sdl.Rect_addgf[ bgframe ].h = 22;

	sdl.Rect_addgf[ addgf_checkbox ].x = 794;
	sdl.Rect_addgf[ addgf_checkbox ].y = 259;
	sdl.Rect_addgf[ addgf_checkbox ].w = 16;
	sdl.Rect_addgf[ addgf_checkbox ].h = 16;

	sdl.Rect_addgf[ addgf_textedit ].x = 644;
	sdl.Rect_addgf[ addgf_textedit ].y = 259;
	sdl.Rect_addgf[ addgf_textedit ].w = 150;
	sdl.Rect_addgf[ addgf_textedit ].h = 16;


	sdl.cursors[0].x = 0;
	sdl.cursors[0].y = 0;
	sdl.cursors[0].w = 12;
	sdl.cursors[0].h = 18;

	sdl.cursors[1].x = 13;
	sdl.cursors[1].y = 0;
	sdl.cursors[1].w = 12;
	sdl.cursors[1].h = 18;

	sdl.cursors[2].x = 26;
	sdl.cursors[2].y = 0;
	sdl.cursors[2].w = 13;
	sdl.cursors[2].h = 18;

	sdl.cursors[3].x = 40;
	sdl.cursors[3].y = 0;
	sdl.cursors[3].w = 15;
	sdl.cursors[3].h = 15;

	sdl.cursors[4].x = 55;
	sdl.cursors[4].y = 0;
	sdl.cursors[4].w = 13;
	sdl.cursors[4].h = 15;

	sdl.cursors[5].x = 68;
	sdl.cursors[5].y = 0;
	sdl.cursors[5].w = 11;
	sdl.cursors[5].h = 15;

	sdl.cursors[6].x = 79;
	sdl.cursors[6].y = 0;
	sdl.cursors[6].w = 9;
	sdl.cursors[6].h = 15;

	// Set colorkeys (invisible colors) for graphics. 
	SDL_SetColorKey(G.avatars, SDL_SRCCOLORKEY, SDL_MapRGB(G.avatars->format, 255, 0, 255));
	SDL_SetColorKey(G.tiles, SDL_SRCCOLORKEY, SDL_MapRGB(G.tiles->format, 255, 0, 255));
	SDL_SetColorKey(G.addgf, SDL_SRCCOLORKEY, SDL_MapRGB(G.addgf->format, 255, 0, 255));
	SDL_SetColorKey(G.items, SDL_SRCCOLORKEY, SDL_MapRGB(G.items->format, 255, 0, 255));
	SDL_SetColorKey(G.Cursors, SDL_SRCCOLORKEY, SDL_MapRGB(G.Cursors->format, 255, 0, 255));
	SDL_SetColorKey(G.objects, SDL_SRCCOLORKEY, SDL_MapRGB(G.objects->format, 255, 0, 255));

    return 0;
}

void load_graphics()
{
	sdl.loaded = 0;
	Login.accCreation = sdl.load_image(0);
	Login.bGround = sdl.load_image(0);
	Login.charchoose = sdl.load_image(0);
	Login.charCreation = sdl.load_image(0);
	Login.fGround = sdl.load_image(0);
	Login.mainmenu = sdl.load_image(0);
	
	sdl.loaded = 0;

	G.Identify = sdl.load_image(1);
	G.layout = sdl.load_image(1);
	G.List = sdl.load_image(1);
	G.Cursors = sdl.load_image(1);
	G.shop = sdl.load_image(1);
	G.skills = sdl.load_image(1);

	sdl.loaded = 0;
	G.tiles = sdl.load_image(2);
    //SDL_SaveBMP(G.tiles, "tiles.bmp");
	sdl.loaded = 0;
	G.items = sdl.load_image(3);

	sdl.loaded = 0;
	G.avatars = sdl.load_image(4);
    //SDL_SaveBMP(G.avatars, "Avatars.bmp");
	sdl.loaded = 0;
	G.addgf = sdl.load_image(5);
	
	sdl.loaded = 0;
	G.objects = sdl.load_image(6);

	sdl.loaded = 0;
	G.MBExpanded = sdl.load_image(7);
}


SDL_Surface* SDLClass::load_image(int i)
{
	/* First of all, create backsurface tmp. */
	Uint32 rmask,gmask,bmask,amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0x0000ff00;
	gmask = 0x00ff0000;
	bmask = 0xff000000;
	amask = 0x000000ff;
#else
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	amask = 0xff000000;
#endif


	SDL_Surface *tmp;


	fstream filu;

	char str[120];
	memset(&str, 0, sizeof(str));
	sprintf(str, "data/gdata0%d.vdf", i);

	filu.open (str, ios::in|ios::binary);

	if (!filu.is_open())
	{
		fprintf(stderr, "Failed to open %s!", str);
		exit(1);
	}

	if (loaded != 0)
	{
		filu.seekg(loaded, ios::beg);
	}

	long h, w, tmpi;
	filu.read((char *)&h, sizeof(h));
	filu.read((char *)&tmpi, sizeof(tmpi));
	filu.read((char *)&w, sizeof(w));
	loaded += 12;

	h = h >> 2;
	w = w >> 2;

	tmp = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 24, rmask, gmask, bmask, amask);

	if (tmp == NULL)
	{
		printf("Failed to create RGB Surface");
		return 0;
	}
	/* Done. Next thing is to load needed pixels into the surface. */


	filu.read((char *)tmp->pixels, h * (w *3));
	loaded += h * (w *3);
	filu.close();
	//printf("Loading Graphics...done.\n");

	return SDL_DisplayFormat( tmp );
}




SDL_Surface* SDLClass::LoadImage(short width, short height, short depth, long starting_point)
{
	/* First of all, create backsurface tmp. */
	Uint32 rmask,gmask,bmask,amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0x0000ff00;
	gmask = 0x00ff0000;
	bmask = 0xff000000;
	amask = 0x000000ff;
#else
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	amask = 0xff000000;
#endif

	SDL_Surface *tmp;

	tmp = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 24, rmask, gmask, bmask, amask);

	if (tmp == NULL)
	{
		printf("Failed to create RGB Surface");
		return 0;
	}
	/* Done. Next thing is to load needed pixels into the surface. */

	fstream filu;

	printf("Loading pixels..");
	
	filu.open ("data/Data02.dat", ios::in|ios::binary);


	if (filu.is_open())
	{
		if (starting_point != 0)
		{
			filu.seekg(starting_point, ios::beg);
			filu.read((char *)tmp->pixels, height * (width*3));
		}
		else
		{
			filu.read((char *)tmp->pixels, height * (width*3));	
		}
		filu.close();
		printf("...done.\n");
	}
	else
	{
		printf("..failed.\n");
	}


	return SDL_DisplayFormat( tmp );
}

void CreateBlueprints()
{
	G.chunkback = sdl.create_backsurface(16384, 64, 24);

	int curchunk;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 1024; i++)
		{
			for (int k = 0; k < 16; k++)
			{
				for (int l = 0; l < 16; l++)
				{
					curchunk = (j << 10) + i;
					if (Map.Chunks[1][ curchunk ] [k] [l] != 0)
						PutPixel(G.chunkback, (i << 4) + k, (j << 4) + l, SDL_MapRGB( G.chunkback->format,
								Tiles[ Map.Chunks[1][ curchunk ] [ k ] [ l ]].r, 
								Tiles[ Map.Chunks[1][ curchunk ] [ k ] [ l ]].g, 
								Tiles[ Map.Chunks[1][ curchunk ] [ k ] [ l ]].b ));
					else
						PutPixel(G.chunkback, (i << 4) + k, (j << 4) + l, SDL_MapRGB( G.chunkback->format,
								Tiles[ Map.Chunks[0][ curchunk ] [ k ] [ l ]].r, 
								Tiles[ Map.Chunks[0][ curchunk ] [ k ] [ l ]].g, 
								Tiles[ Map.Chunks[0][ curchunk ] [ k ] [ l ]].b ));
				}
			}
		}
	}

	SDL_UpdateRect(G.chunkback, 0, 0, 0, 0);

	G.mmChunkmap = sdl.create_backsurface(144, 144, 24);
}
