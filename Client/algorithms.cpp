#include "main.h"



//Algorithms file, contains line of sight and other stuff. /* Coded by MDF */


char sgn (int a) 
{
  if (a > 0) return +1;
  else if (a < 0) return -1;
  else return 0;
}

void visible(int x1, int y1, int x2, int y2) 
{
  long u, s, v, d1x, d1y, d2x, d2y, m, n;
  int  i;
  u   = x2-x1;
  v   = y2-y1;
  d1x = sgn(u);
  d1y = sgn(v);
  d2x = sgn(u);
  d2y = 0;
  m   = abs(u);
  n   = abs(v);

  if (m <= n) {
    d2x = 0;
    d2y = sgn(v);
    m   = abs(v);
    n   = abs(u);
  }

  s = (int)(m / 2);

  for (i = 0; i < m; i++) {
    s += n;
    if (s >= m) {
      s -= m;
      x1 += d1x;
      y1 += d1y;
    } else {
      x1 += d2x;
      y1 += d2y;
    }
    MapInfo.Visible[x1 - pldatabase->own->x + 10][y1 - pldatabase->own->y + 10] = 1;

	bool r;
	r = false;

	if (Tiles[Map.Chunks[0][Map.ChunkMap[x1 >> 4][y1 >> 4]][x1 % 16][y1 % 16]].Visibility == 1) r = true;
		 
	if (Tiles[Map.Chunks[1][Map.ChunkMap[x1 >> 4][y1 >> 4]][x1 % 16][y1 % 16]].Visibility == 1 &&
		Map.Chunks[1][Map.ChunkMap[x1 >> 4][y1 >> 4]][x1 % 16][y1 % 16] != 0) r = true;

	if (MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - x1) ][ -1 * ((pldatabase->own->y-10) - y1) ]) r = true;

	if (Tiles[Map.Chunks[1][Map.ChunkMap[x1 >> 4][y1 >> 4]][x1 % 16][y1 % 16]].Visibility == 0 &&
		Map.Chunks[1][Map.ChunkMap[x1 >> 4][y1 >> 4]][x1 % 16][y1 % 16] != 0 &&
		its(pldatabase->own->x, x1) <= 1 && its(pldatabase->own->y, y1) <= 1) r = false;

//	if (MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - x1) ][ -1 * ((pldatabase->own->y-10) - y1) ] && its(pldatabase->own->x, x1) <= 1 && its(pldatabase->own->y, y1) <= 1) r = false;

	if (r) return;
  }
}


/*
void getvisible()
{
	if (NoLos) 
	{
		memset(&MapInfo.Visible, 1, sizeof(MapInfo.Visible));
		return;
	}

	memset(&MapInfo.Visible, 0, sizeof(MapInfo.Visible));
	memset(&MapInfo.VisBlocktable, 0, sizeof(MapInfo.VisBlocktable));

	GetMapVisBlocktable();

	int x1, y1;

	x1 = own->x - 10;
	while (x1 <= own->x + 10) 
	{
		visible (own->x, own->y, x1, own->y - 10);
		visible (own->x, own->y, x1, own->y + 10);
		x1++;
	};
	
	y1 = own->y - 10;
	while (y1 <= own->y + 10) 
	{
	    visible (own->x, own->y, own->x - 10, y1);
	    visible (own->x, own->y, own->x + 10, y1);
	    y1++;
	};
	
	MapInfo.Visible[10][10] = true;
	return;
}
*/
void GetMapVisBlocktable()
{
	memset(&MapInfo.VisBlocktable, 0, sizeof(MapInfo.VisBlocktable));
	short hshx[9], hshy[9];	hshx[0] = pldatabase->own->x / 32;	hshy[0] = pldatabase->own->y / 32;	hshx[1] = hshx[0] -1;	hshy[1] = hshy[0] -1; 	hshx[2] = hshx[0] -1;	hshy[2] = hshy[0]; 	hshx[3] = hshx[0] -1;	hshy[3] = hshy[0] +1; 	hshx[4] = hshx[0];	hshy[4] = hshy[0] -1; 	hshx[5] = hshx[0];	hshy[5] = hshy[0] +1; 	hshx[6] = hshx[0] +1;	hshy[6] = hshy[0] -1; 	hshx[7] = hshx[0] +1;	hshy[7] = hshy[0]; 	hshx[8] = hshx[0] +1;	hshy[8] = hshy[0] +1;

	short ox, oy;

	int i;
	for (i = 0; i < 9; i++)
	{
		if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 256 && hshy[i] < 256)
		{
			obj_List = Objects[hshx[i]][hshy[i]];

			while (obj_List != NULL)
			{
				if (IsInRange(pldatabase->own->x, pldatabase->own->y, obj_List->X, obj_List->Y))
				{
					ox = obj_List->X;
					oy = obj_List->Y;
					if ( obj_List->On)
					{
						if (ObjectTypes[obj_List->Type].VisBlock)
							MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - ox) ][ -1 * ((pldatabase->own->y-10) - oy) ] = 1;
						else
							if (!itsi(pldatabase->own->x, pldatabase->own->y, ox, oy))
								MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - ox) ][ -1 * ((pldatabase->own->y-10) - oy) ] = 1;
					}
				}

				if (obj_List != NULL)
					obj_List = obj_List->next;	
			}
		}			
	}
	bool r;
	short x1,y1,x2,y2,x3,y3;
	for (i = 0; i < 21; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			r = false;

			x1 = i + pldatabase->own->x - 10;
			y1 = j + pldatabase->own->y - 10;
			x2 = x1 >> 4;
			y2 = y1 >> 4;
			x3 = x1 % 16;
			y3 = y1 % 16;

			if (Tiles[Map.Chunks[0][Map.ChunkMap[ x2 ][ y2 ]][ x3 ][ y3 ]].Visibility == 1) r = true;
				 
			if (Tiles[Map.Chunks[1][Map.ChunkMap[ x2 ][ y2 ]][ x3 ][ y3 ]].Visibility == 1 &&
				Map.Chunks[1][Map.ChunkMap[ x2 ][ y2 ]][ x3 ][ y3 ] != 0) r = true;
		
			if (MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - x1) ][ -1 * ((pldatabase->own->y-10) - y1) ] > 0) r = true;
		
			if (Tiles[Map.Chunks[1][Map.ChunkMap[ x2 ][ y2 ]][ x3 ][ y3 ]].Visibility == 0 &&
				Map.Chunks[1][Map.ChunkMap[ x2 ][ y2 ]][ x3 ][ y3 ] != 0 &&
				itsi(pldatabase->own->x, pldatabase->own->y, x1, y1)) r = false;

			if (r) MapInfo.VisBlocktable[-1 * ((pldatabase->own->x - 10) - x1) ][-1 * ((pldatabase->own->y - 10) - y1) ] = 1;
			else MapInfo.VisBlocktable[-1 * ((pldatabase->own->x - 10) - x1) ][-1 * ((pldatabase->own->y - 10) - y1) ] = 0;
		}
	}
}

bool itsi(short x1, short y1, short x2, short y2)
{
	short itsix, itsiy;
	itsix = its(x1, x2);
	itsiy = its(y1, y2);
	short itsiplus;
	itsiplus = itsix + itsiy;
	if (itsiplus > 1) return false;
	else return true;
}


void calclos(char type)
{
	if (Client.NoLos)
	{
		memset(&MapInfo.Visible, 5, sizeof(MapInfo.Visible));
		return;
	}
  if (type == 0)
  {
	  memset(&MapInfo.Visible, 0, sizeof(MapInfo.Visible));
	  GetMapVisBlocktable();

    for (int y = 0; y <= 20; y++)
    {
      for (int x = 0; x <= 20; x++)
   	  {
        if (MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))])
          MapInfo.Visible[x][y] = 2;
      }
    }
  }

  for (int y = 0; y <= 20; y++)
  {
 	  for (int x = 0; x <= 20; x++)
 	  {
 	  	if (x > 0)
 	  	{
 	  		if (y > 0)
 	  		{
 	  			if (type == 0)
 	  			{
  	 	  		if (MapInfo.Visible[x - 1][y - 1] == 0 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 1;
  	 	    }
  	 	    else if (type == 1)
  	 	    {
            if (MapInfo.Visible[x - 1][y - 1] == 4 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 5;
          }
 	  		}

        if (type == 0)
        {
 	  		  if (MapInfo.Visible[x - 1][y] == 0 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 1;
 	  		}
 	  		else if (type == 1)
 	  		{
 	  			if (MapInfo.Visible[x - 1][y] == 4 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 5;
 	  		}

        if (y < 20)
        {
        	if (type == 0)
        	{
          	if (MapInfo.Visible[x - 1][y + 1] == 0 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 1;
          }
          else if (type == 1)
          {
          	if (MapInfo.Visible[x - 1][y + 1] == 4 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 5;
          }
        }
 	  	}

 	  	if (x < 20)
 	  	{
 	  		if (y > 0)
 	  		{
 	  			if (type == 0)
 	  			{
	 	  		  if (MapInfo.Visible[x + 1][y - 1] == 0 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 1;
	 	  		}
				else if (type == 1)
				{
          			if (MapInfo.Visible[x + 1][y - 1] == 4 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 5;
				}
 	  		}
	        if (type == 0)
		    {
			  if (MapInfo.Visible[x + 1][y] == 0 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 1;
	        }	
		    else if (type == 1)
			{
        		if (MapInfo.Visible[x + 1][y] == 4 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 5;
			}

	        if (y < 20)
		    {
				if (type == 0)
				{
				if (MapInfo.Visible[x + 1][y + 1] == 0 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 1;
          }
          else if (type == 1)
          {
          	if (MapInfo.Visible[x + 1][y + 1] == 4 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 5;
          }
        }
 	  	}

 	  	if (x >= 0 || x <= 20)
 	  	{
 	  		if (y > 0)
 	  		{
 	  			if (type == 0)
 	  			{
	 	  		  if (MapInfo.Visible[x][y - 1] == 0 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 1;
	 	  		}
          else if (type == 1)
          {
          	if (MapInfo.Visible[x][y - 1] == 4 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 5;
          }
 	  		}

        if (y < 20)
        {
        	if (type == 0)
        	{
        	  if (MapInfo.Visible[x][y + 1] == 0 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 1;
        	}
          else if (type == 1)
          {
          	if (MapInfo.Visible[x][y + 1] == 4 && MapInfo.VisBlocktable[ -1 * ((pldatabase->own->x-10) - (pldatabase->own->x-10 + x))][ -1 * ((pldatabase->own->y-10) - (pldatabase->own->y-10 + y))]) MapInfo.Visible[x][y] = 5;
          }
        }
 	  	}
 	  }
  }

  if (type == 1)
  {
	  MapInfo.Visible[9][9] = 5;
	  MapInfo.Visible[10][9] = 5;
	  MapInfo.Visible[11][9] = 5;
	  MapInfo.Visible[9][10] = 5;
	  MapInfo.Visible[11][10] = 5;
	  MapInfo.Visible[9][11] = 5;
	  MapInfo.Visible[10][11] = 5;
	  MapInfo.Visible[11][11] = 5;

  }
}

void recurfill(int x, int y)
{
  if (MapInfo.Visible[x][y] != 1 && MapInfo.Visible[x][y] != 2)
  {
  	MapInfo.Visible[x][y] = 4;
    if (x > 0 && MapInfo.Visible[x - 1][y] != 4) recurfill (x - 1, y);
    if (x < 20 && MapInfo.Visible[x + 1][y] != 4) recurfill (x + 1, y);
    if (y > 0 && MapInfo.Visible[x][y - 1] != 4) recurfill (x, y - 1);
    if (y < 20 && MapInfo.Visible[x][y + 1] != 4) recurfill (x, y + 1);
    if (x > 0 && y < 20 && MapInfo.Visible[x - 1][y + 1] != 4) recurfill (x - 1, y + 1);
    if (x < 20 && y > 0 && MapInfo.Visible[x + 1][y - 1] != 4) recurfill (x + 1, y - 1);
    if (x < 20 && y < 20 && MapInfo.Visible[x + 1][y + 1] != 4) recurfill (x + 1, y + 1);
    if (x > 0 && y > 0 && MapInfo.Visible[x - 1][y - 1] != 4) recurfill (x - 1, y - 1);
  }
}



bool IsInRange(short int plx, short int ply, short int tmpx, short int tmpy)
{
	if ( plx - tmpx >= -11 && plx - tmpx <= 11 &&
		 ply - tmpy >= -11 && ply - tmpy <= 11)
		 return true;
	return false;
}

bool IsInRange2(short int plx, short int ply, short int tmpx, short int tmpy)
{
	if ( plx - tmpx >= -1 && plx - tmpx <= 1 &&
		 ply - tmpy >= -1 && ply - tmpy <= 1)
		 return true;
	return false;
}
short its(short n1, short n2)
{
	if (n1 - n2 < 0) return -1 * (n1 - n2);
	if (n1 - n2 >= 0) return n1 - n2;
	return 0;
}

void calc_LOS()
{
	calclos(0); MapInfo.Visible[10][10] = 0; recurfill(10,10); calclos(1);
}
