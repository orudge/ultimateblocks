#include <allegro.h>
#include <math.h>

#include "blocks3.h"

int counter;

 particle *list = NULL;  // make sure it starts by being NULL!!!

struct particle *create_snow_flake(struct particle *parent);
struct particle *clean_particles(BITMAP *bmp, struct particle *t,int all);
void draw_particles(BITMAP *bmp, struct particle *t);
void update_particles(struct particle *t) ;
void remove_particle_drawing(BITMAP *bmp, struct particle *t);

void Add_Part(int x, int y, int dx, int dy, int col)
{
 part[part_count].x = x;
 part[part_count].y = y;
 part[part_count].dx = dx;
 part[part_count].dy = dy;
 part[part_count].col = col;

 part_count++;
}

void Del_Part(int p_no)
{
 int i;

 for (i = p_no; i < part_count; i++)
 {
  part[i].x = part[i + 1].x;
  part[i].y = part[i + 1].y;
  part[i].dx = part[i + 1].dx;
  part[i].dy = part[i + 1].dy;
  part[i].col = part[i + 1].col;
 }

 part_count--;
}

void Particle(void)
{
#if 0
 if (part_count < MAX_PARTICLES)
 {
  if (rand()%2 == 1) Add_Part(320 + rand()%16, 200 + rand()%32, (rand()%50 + 20), -(rand()%10 + 5), rand()%255);
  else  Add_Part(320 + rand()%16, 200 + rand()%32, -(rand()%50 + 20), -(rand()%10 + 5), rand()%255);
 }
#else
	//list=create_snow_flake(list);
#endif

 Particle_Move();
}

void Particle_Move(void)
{
#if 0
 int i;

 counter++;

 if (counter > 50) counter = 0;

 for (i = 0; i < part_count; i++)
 {
  putpixel(screen, part[i].x, part[i].y, getpixel(temp, part[i].x, part[i].y));

  if (!(counter%part[i].dx)) part[i].x += part[i].dx/abs(part[i].dx);
  if (!(counter%part[i].dy)) part[i].y += part[i].dy/abs(part[i].dy);

  part[i].col++;

  if (part[i].col == 256)
  {
   Del_Part(i);
   i--;
  }
  else
  {
   putpixel(screen, part[i].x, part[i].y, makecol8(255, 255 - part[i].col, 255 - part[i].col));

   if ((part[i].col > 100) && (part_count < 900))
   {
    Add_Part(part[i].x, part[i].y + 1, part[i].dx, part[i].dy, part[i].col);
   }
  }
 }
#else
	if (!_particles)
		return;

	blit(screen, temp_particle, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

	list=clean_particles(temp_particle, list,FALSE);

	if (part_count < 25)
		list=create_snow_flake(list);

	update_particles(list);
	draw_particles(temp_particle,list);
	blit(temp_particle, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
#endif
}


void reset_particles()
{
	part_count = 0; // old

	list=clean_particles(NULL, list,FALSE);

}

struct particle *create_particle(struct particle *parent)
{
	struct particle *tmp;
	tmp = (struct particle *)malloc(sizeof(struct particle));
	tmp->next = parent;

	part_count++;
	return tmp;
}

struct particle *create_rain_drop(struct particle *parent,double direction)
{
	struct particle *tmp;
	double r;

	tmp = create_particle (parent);  // We allocate the memory for the particle here.

	// place the rain in a random place on the screen

	tmp->x = (double) (rand()%(SCREEN_W+40))-20;
	tmp->y = (double) (rand()%SCREEN_H)-60;

	// decide speed of the particle

	r = ((double)(rand()%30))/10+20;  // good rain needs to be fast! ;)

	// set the speed & direction

	tmp->dx = r*sin(direction);
	tmp->dy = -r*cos(direction);

	// rx & ry should be 0 since we don't want the rain to wobble.

	tmp->rx = 0;
	tmp->ry = 0;

	// don't forget to add life to the particle.

	tmp->life = rand()%30+60;

	// I like my rain grey coloured ;)

	tmp->colour = makecol(220,220,220);

	// and finally set the drawing type

	tmp->type = RAIN_PARTICLE;

	tmp->first_update = 1;

	return tmp;
}

struct particle *create_snow_flake(struct particle *parent)
{
	struct particle *tmp;
	double r,direction;
	int c;

	tmp = create_particle (parent);   // once again start by creating the particle

	// place the snow in a random place on the screen

	tmp->x = (double) (rand()%(SCREEN_W+20))-20;
	tmp->y = (double) (rand()%(SCREEN_H+20))-20;

	// decide speed and direction of this particle of snow

	r = (double) (rand()%15)/15+1;  // notice the snow is much slower than the rain was
	direction=rand()%15; // 15; //rand()%64+64;

	// set the speed & direction

	tmp->dx = r*sin(direction);
	tmp->dy = -r*cos(direction);

	// snow is very light so it tends to be blown about, so we add some randomness
	// to its movement.

	tmp->rx = 1;
	tmp->ry = 1;

	// don't forget to add life to the particle.

	tmp->life = 1; //rand()%30+60;

	// snow should be all white.. but I made it a random shade of grey to make it look
	// more... interesting.

	c = rand()%128+128;
	tmp->colour=makecol(c,c,c);

	// and finally set the drawing type
	// randomly set it to either of the two flake types.

	tmp->type = rand()%2+SNOW_PARTICLE1;

	tmp->first_update = 1;

	return tmp;
}

struct particle *clean_particles(BITMAP *bmp, struct particle *t,int all) 
{
	struct particle *tmp;
	struct particle *top;

	top=t;
	tmp=NULL;

	while(t)   // basically we are looping through all the particles in the list.
	{    
   
	if(t->life<=0||all) { // is the particle is dead or if we are deleting all particles
	 if(tmp) {          // then delete it!

	 if (bmp != NULL)
		 remove_particle_drawing(bmp, t);
	 tmp->next=t->next;
	free(t);
	t=tmp->next;

	part_count--;
	 }
	 else {

	if (bmp != NULL)
		remove_particle_drawing(bmp, t);

	top=t->next;
	free(t);
	t=top;

	part_count--;
	 }
	}
	else {
	 tmp=t;
	 t=t->next;
	}
	}
	return top; // return the list back, possibly empty :)
}

void remove_particle_drawing(BITMAP *bmp, struct particle *t)
{
	switch(t->type)
	{
		case SNOW_PARTICLE1: // just draw a single pixel for a small flake of snow
			
			// put whatever was there before back
			if ((t->tx > -1) && (t->ty > -1) && (t->first_update == 0))
			{
				if ((t->tx != t->x) && (t->ty != t->y))
				{
					putpixel(bmp, (int)t->tx, (int)t->ty, t->oldcol[0]);
				}
			}
			break;

		case SNOW_PARTICLE2:
			// put whatever was there before back

			if ((t->tx > -1) && (t->ty > -1) && (t->first_update == 0))
			{
				if ((t->tx != t->x) && (t->ty != t->y))
				{
					putpixel(bmp, (int)t->tx, (int)t->ty, t->oldcol[0]); 
					putpixel(bmp, (int)t->tx-1, (int)t->ty, t->oldcol[1]); 
					putpixel(bmp, (int)t->tx+1, (int)t->ty, t->oldcol[2]);
					putpixel(bmp, (int)t->tx, (int)t->ty-1, t->oldcol[3]);
					putpixel(bmp, (int)t->tx, (int)t->ty+1, t->oldcol[4]);
				}
			}
			break;

	}
}

void draw_particle(BITMAP *bmp, struct particle *t) 
{
	switch(t->type)
	{
		case RAIN_PARTICLE:  // for rain just draw a simple line
			line(bmp, (int)t->x, (int)t->y, (int)t->tx, (int)t->ty, t->colour);  
			break;

		case SNOW_PARTICLE1: // just draw a single pixel for a small flake of snow
			
			// put whatever was there before back
			if ((t->tx > -1) && (t->ty > -1) && (t->first_update == 0))
			{
				if ((t->tx != t->x) && (t->ty != t->y))
					putpixel(bmp, (int)t->tx, (int)t->ty, t->oldcol[0]);
			}

			t->oldcol[0] = getpixel(bmp, (int)t->x, (int)t->y);
			t->first_update = 0;

			putpixel(bmp, (int)t->x, (int)t->y, t->colour); 
			break;

		case SNOW_PARTICLE2:
			// put whatever was there before back

			if ((t->tx > -1) && (t->ty > -1) && (t->first_update == 0))
			{
				if ((t->tx != t->x) && (t->ty != t->y))
				{
					putpixel(bmp, (int)t->tx, (int)t->ty, t->oldcol[0]); 
					putpixel(bmp, (int)t->tx-1, (int)t->ty, t->oldcol[1]); 
					putpixel(bmp, (int)t->tx+1, (int)t->ty, t->oldcol[2]);
					putpixel(bmp, (int)t->tx, (int)t->ty-1, t->oldcol[3]);
					putpixel(bmp, (int)t->tx, (int)t->ty+1, t->oldcol[4]);
				}
			}

			t->oldcol[0] = getpixel(bmp, (int)t->x, (int)t->y);
			t->oldcol[1] = getpixel(bmp, (int)t->x-1, (int)t->y);
			t->oldcol[2] = getpixel(bmp, (int)t->x+1, (int)t->y);
			t->oldcol[3] = getpixel(bmp, (int)t->x, (int)t->y-1);
			t->oldcol[4] = getpixel(bmp, (int)t->x, (int)t->y+1);
			t->first_update = 0;

			putpixel(bmp, (int)t->x, (int)t->y, t->colour); 
			putpixel(bmp, (int)t->x-1, (int)t->y, t->colour); 
			putpixel(bmp, (int)t->x+1, (int)t->y, t->colour);
			putpixel(bmp, (int)t->x, (int)t->y-1, t->colour);
			putpixel(bmp, (int)t->x, (int)t->y+1, t->colour);
			break;
	}
}

void draw_particles(BITMAP *bmp, struct particle *t) 
{
	while(t) {    // once again we are just looping through all the particles
		draw_particle(bmp,t);
		t=t->next;
	}
}


void update_particle(struct particle *t)
{
	int rnd;

	// first store the old positon as the end of the trail

	t->tx = t->x;  // first things first! store the current position of the particle
	t->ty = t->y;

	// then update the position using its speed & direction

	t->x += t->dx; 
	t->y += t->dy; 

	// now add any randomness

	if(t->rx>0)
		t->x += (rand() % (int) (t->rx*2)) - t->rx;

	if(t->ry>0)
	{
		rnd = (rand() % (int) (t->ry*2)) - t->ry;

		if (rnd < 0)
			rnd = 0;

		t->y += rnd;
	}

	// subtract one from the particles life.

	// test
	if ((t->x >= SCREEN_W) || (t->y >= SCREEN_H) || (t->x <= 0) || (t->y <= 0))
		t->life = 0;

	//t->life--;
}

void update_particles(struct particle *t) 
{
	while(t) {    // and once again we are just looping through all the particles
	update_particle(t);
	t=t->next;
	}
}
