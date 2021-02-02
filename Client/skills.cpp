#include "main.h"
#pragma warning(disable:4244)


skillinfo skills[20] = 
{
	{ true,		"Unarmed",			skilltype_combat	}, // 0
	{ true,		"Swords",			skilltype_combat	}, // 1
	{ true,		"Axes",				skilltype_combat	}, // 2
	{ true,		"Blunts",			skilltype_combat	}, // 3
	{ true,		"Polearms",			skilltype_combat	}, // 4
	{ false,	"Bows",				skilltype_combat	}, // 5
	{ false,	"Identifying",		skilltype_knowledge	}, // 6
	{ false,	"Combat knowledge",	skilltype_knowledge	}, // 7
	{ false,	"",					0				 },
	{ false,	"",					0				 },
	{ false,	"",					0				 }, // 10
	{ false,	"",					0				 },
	{ false,	"",					0				 },
	{ false,	"",					0				 },
	{ false,	"",					0				 },
	{ true,		"Mining",			skilltype_trade		}, // 15
	{ true,		"Blacksmithing",	skilltype_trade		}, // 16
	{ false,	"",					0				 },
	{ false,	"",					0				 },
	{ false,	"",					0				 },
};



void skillsclass::c_skill_list()
{
	for (int i = 1; i < 5; i++) // 4 skill types... Let's make sorting later, now just sort by basic type.
	{
		for (int j = 0; j < 20; j++)
		{
			if (P.Skills[j].Exp >= 0 && skills[j].skilltype == i && skills[j].used)
			{
				skill_list *snew = new skill_list;
				snew->next = l_list;
				snew->skill = j;
				l_list = snew;
			}
		}
	}
}

void skillsclass::d_skillwindow()
{
	sdl.dst.x = 400;
	sdl.dst.y = 0;
	sdl.dst.h = 600;
	sdl.dst.w = 400;

	SDL_BlitSurface(G.skills, NULL, Screen.screen, &sdl.dst);

	d_skillnames();
	if (chosen)
		d_statusbar();
}


void skillsclass::d_statusbar()
{
	sprintf(tmp, "%s: ô%d (%lu)", skills[chosen->skill].name, P.Skills[chosen->skill].Level, P.Skills[chosen->skill].Exp);
	sdl.text(tmp, 440, 500, col_grey);


//	BFont_PrintStringFont(Screen.screen, F.lightblue, 414, 540, "%s:  %d", Skills[ChosenSkill].Name, P.SkillLevels[ChosenSkill]);

	int i;

	int HowMany;

	HowMany = 350 * (((double)P.Skills[chosen->skill].Exp - (double)SkillExpTables[P.Skills[chosen->skill].Level]) / ((double)SkillExpTables[P.Skills[chosen->skill].Level + 1]-(double)SkillExpTables[P.Skills[chosen->skill].Level]));
		
	PutPixel(Screen.screen, 414, 556, SDL_MapRGB(Screen.screen->format, 220,220,220));
	PutPixel(Screen.screen, 414, 557, SDL_MapRGB(Screen.screen->format, 220,220,220));
	PutPixel(Screen.screen, 414, 558, SDL_MapRGB(Screen.screen->format, 220,220,220));
	PutPixel(Screen.screen, 414, 559, SDL_MapRGB(Screen.screen->format, 220,220,220));

	PutPixel(Screen.screen, 765, 556, SDL_MapRGB(Screen.screen->format, 220,220,220));
	PutPixel(Screen.screen, 765, 557, SDL_MapRGB(Screen.screen->format, 220,220,220));
	PutPixel(Screen.screen, 765, 558, SDL_MapRGB(Screen.screen->format, 220,220,220));
	PutPixel(Screen.screen, 765, 559, SDL_MapRGB(Screen.screen->format, 220,220,220));

	for (i = 0; i < 351; i++)
	{
		PutPixel(Screen.screen, 414+i, 555, SDL_MapRGB(Screen.screen->format, 220,220,220));
		PutPixel(Screen.screen, 414+i, 560, SDL_MapRGB(Screen.screen->format, 220,220,220));

		if (i < HowMany)
		{
			PutPixel(Screen.screen, 415+i, 556, SDL_MapRGB(Screen.screen->format, 190,0,0));
			PutPixel(Screen.screen, 415+i, 557, SDL_MapRGB(Screen.screen->format, 210,0,0));		
			PutPixel(Screen.screen, 415+i, 558, SDL_MapRGB(Screen.screen->format, 190,0,0));
			PutPixel(Screen.screen, 415+i, 559, SDL_MapRGB(Screen.screen->format, 170,0,0));		
		}
		else
		{
			PutPixel(Screen.screen, 415+i, 556, SDL_MapRGB(Screen.screen->format, 0,0,0));
			PutPixel(Screen.screen, 415+i, 557, SDL_MapRGB(Screen.screen->format, 0,0,0));		
			PutPixel(Screen.screen, 415+i, 558, SDL_MapRGB(Screen.screen->format, 0,0,0));
			PutPixel(Screen.screen, 415+i, 559, SDL_MapRGB(Screen.screen->format, 0,0,0));		
		}
	}
	sprintf(tmp, "%d / %d", P.Skills[chosen->skill].Exp, SkillExpTables[P.Skills[chosen->skill].Level + 1]);
	sdl.text(tmp, 600 - BFont_TextWidth(tmp), 565, col_white);
}


void DrawClass::DrawSkills()
{
/*	if (P.SkillExps[ChosenSkill] > 0)
	{
		// Prinformation of this skill.

		BFont_PrintStringFont(Screen.screen, F.lightblue, 414, 540, "%s:  %d", Skills[ChosenSkill].Name, P.SkillLevels[ChosenSkill]);

		int i;

		int HowMany;

		HowMany = 350 * (((double)P.SkillExps[ChosenSkill]-(double)SkillExpTables[P.SkillLevels[ChosenSkill]]) / ((double)SkillExpTables[P.SkillLevels[ChosenSkill]+1]-(double)SkillExpTables[P.SkillLevels[ChosenSkill]]));
		
		PutPixel(Screen.screen, 414, 556, SDL_MapRGB(Screen.screen->format, 220,220,220));
		PutPixel(Screen.screen, 414, 557, SDL_MapRGB(Screen.screen->format, 220,220,220));
		PutPixel(Screen.screen, 414, 558, SDL_MapRGB(Screen.screen->format, 220,220,220));
		PutPixel(Screen.screen, 414, 559, SDL_MapRGB(Screen.screen->format, 220,220,220));

		PutPixel(Screen.screen, 764, 556, SDL_MapRGB(Screen.screen->format, 220,220,220));
		PutPixel(Screen.screen, 764, 557, SDL_MapRGB(Screen.screen->format, 220,220,220));
		PutPixel(Screen.screen, 764, 558, SDL_MapRGB(Screen.screen->format, 220,220,220));
		PutPixel(Screen.screen, 764, 559, SDL_MapRGB(Screen.screen->format, 220,220,220));

		for (i = 0; i < 350; i++)
		{
			PutPixel(Screen.screen, 414+i, 555, SDL_MapRGB(Screen.screen->format, 220,220,220));
			PutPixel(Screen.screen, 414+i, 560, SDL_MapRGB(Screen.screen->format, 220,220,220));

			if (i < HowMany)
			{
				PutPixel(Screen.screen, 415+i, 556, SDL_MapRGB(Screen.screen->format, 190,0,0));
				PutPixel(Screen.screen, 415+i, 557, SDL_MapRGB(Screen.screen->format, 210,0,0));		
				PutPixel(Screen.screen, 415+i, 558, SDL_MapRGB(Screen.screen->format, 190,0,0));
				PutPixel(Screen.screen, 415+i, 559, SDL_MapRGB(Screen.screen->format, 170,0,0));		
			}
			else
			{
				PutPixel(Screen.screen, 415+i, 556, SDL_MapRGB(Screen.screen->format, 0,0,0));
				PutPixel(Screen.screen, 415+i, 557, SDL_MapRGB(Screen.screen->format, 0,0,0));		
				PutPixel(Screen.screen, 415+i, 558, SDL_MapRGB(Screen.screen->format, 0,0,0));
				PutPixel(Screen.screen, 415+i, 559, SDL_MapRGB(Screen.screen->format, 0,0,0));		
			}
		}
		char x1[64];
		sprintf(x1, "%lu / %lu", P.SkillExps[ChosenSkill], SkillExpTables[P.SkillLevels[ChosenSkill]+1]);

		BFont_PrintStringFont(Screen.screen, F.red, 600 - BFont_TextWidth(x1), 565, "%d / %d", P.SkillExps[ChosenSkill], SkillExpTables[P.SkillLevels[ChosenSkill]+1]); 
	}*/
}






void skillsclass::d_skillnames()
{
	short mx = MX, my = MY, counter = 0;
	skill_list *tmpl;
	for (int i = 1; i < 5; i++)
	{
		tmpl = l_list;

		if (expanded[i])
		{
			if (mx >= 410 && mx <= 750 && my >= 80 + counter * 12 && my <= 90 + counter * 12)
			sprintf(tmp, "==%s skills== É(showing) <->", skilltypes[i]);
			else
			sprintf(tmp, "==%s skills== É(showing)", skilltypes[i]);

//			BFont_PrintStringFont(Screen.screen, F.grey, 410, 80 + counter * 12, tmp);			
			sdl.text(tmp, 410, 80 + counter * 12, col_grey);
			counter++;
			while (tmpl)
			{
				if (skills[tmpl->skill].skilltype == i)
				{
					if (chosen == tmpl)
					{
						if (mx >= 425 && mx <= 425 + BFont_TextWidth(skills[tmpl->skill].name) + BFont_TextWidth("* ") && my >= 80 + counter * 12 && my <= 90 + counter * 12)
							 col = col_grey;
						else col = col_gray;
						sprintf(tmp, "* %s", skills[tmpl->skill].name);
					}
					else
					{
						if (mx >= 425 && mx <= 425 + BFont_TextWidth(skills[tmpl->skill].name) && my >= 80 + counter * 12 && my <= 90 + counter * 12)
							 col = col_grey;
						else col = col_gray;
						sprintf(tmp, "%s", skills[tmpl->skill].name);
					}

					sdl.text(tmp, 425, 80 + counter * 12, col);
					sprintf(tmp, "%d", P.Skills[tmpl->skill].Level);
					sdl.text(tmp, 750, 80 + counter * 12, col_white);
					counter++;
				}
				tmpl = tmpl->next;
			}
		}
		else
		{
			if (mx >= 410 && mx <= 750 && my >= 80 + counter * 12 && my <= 90 + counter * 12)
			sprintf(tmp, "==%s skills== É(hidden) <->", skilltypes[i]);
			else
			sprintf(tmp, "==%s skills== É(hidden)", skilltypes[i]);
			sdl.text(tmp, 410, 80 + counter * 12, col_grey);
		}
		counter++;
	}
}

void skillsclass::mouseclick()
{
	short mx = MX, my = MY, counter = 0;
	skill_list *tmpl;
	for (int i = 1; i < 5; i++)
	{
		tmpl = l_list;

		if (mx >= 410 && mx <= 750 && my >= 80 + counter * 12 && my <= 90 + counter * 12)
		{
			expanded[i] = !expanded[i];
			i = 5;
		}

		if (expanded[i])
		{
			counter++;
			while (tmpl)
			{
				if (skills[tmpl->skill].skilltype == i)
				{
					if (mx >= 425 && mx <= 425 + BFont_TextWidth(skills[tmpl->skill].name) && my >= 80 + counter * 12 && my <= 90 + counter * 12 && chosen != tmpl)
					{
						chosen = tmpl;
						tmpl = NULL;
						i = 5;
					}

					counter++;
				}
				if (tmpl != NULL)
				tmpl = tmpl->next;
			}
		}
		counter++;
	}
}

void MouseButtonUpSkills()
{

}