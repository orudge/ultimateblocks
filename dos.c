/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/
#if defined(DJGPP)

#include <allegro.h>
#include "blocks3.h"

/*! \brief Return the name of 'significant' directories.
 *
 * \param   dir Enumerated constant for directory type  \sa DATA_DIR et al.
 * \param   file File name below that directory.
 * \returns the combined path
 */
const char *find_resource_file (int dir, const char *file)
{
	static char ans[MAX_PATH];

	switch (dir)
	{
		case APP_DIR:
			if (file == NULL)
				strcpy(ans, ".");
			else
				strcpy(ans, file);

			break;

		case GRAPHICS_DIR:
			if (file == NULL)
				strcpy(ans, "graphics");
			else
				sprintf(ans, "graphics/%s", file);

			break;

		case MUSIC_DIR:
			if (file == NULL)
				strcpy(ans, "music");
			else
				sprintf(ans, "music/%s", file);

			break;

		case MAP_DIR:
			if (file == NULL)
				strcpy(ans, "maps");
			else
				sprintf(ans, "maps/%s", file);

			break;
   
		case SAVE_DIR:
		case SETTINGS_DIR:
			if (file == NULL)
				strcpy(ans, ".");
			else
				strcpy(ans, file);

			break;

		default:
			return NULL;
   }

   return fix_filename_slashes (ans);
}

#endif
