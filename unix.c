/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/
#if defined(UNIX)

#include <allegro.h>
#include "blocks3.h"

static int init_path = 0;
static char user_dir[MAX_PATH];
static char data_dir[MAX_PATH];

#define UBLOCKS_BIN		"/usr/local/bin/blocks4"
#define UBLOCKS_DATA		"/usr/share/games/blocks4"

/*! \brief Returns the full path for this file
 *
 * This function first checks if the file can be found in the user's
 * directory. If it can not, it checks the relavent game directory
 * (data, music, lib, etc)
 *
 * \param str1 The first part of the string, assuming the file can't be
 * found in user_dir (eg. "C:\Program Files\Ultimate Blocks\")
 * \param str2 The second part of the string (eg. "music")
 * \param file The filename
 * \returns the combined path
 */
const char *get_resource_file_path (const char * str1, const char * str2, const char * file, char userdir)
{
	static char ans[MAX_PATH];

	// we're dealing with a directory
	if (file == NULL)
	{
		// check if userdir/str2 exists, or just userdir if str2 not passed
		if (str2 == NULL)
			strcpy(ans, user_dir);
		else
			sprintf(ans, "%s/%s", user_dir, str2);

		fix_filename_slashes(ans);

		// if we force userdir, return that
		if (userdir == 1)
		{
			mkdir(ans, 0755);
			return ans;
		}

		// check to see if the directory exists
		if (!file_exists(ans, FA_DIREC, NULL))
		{
			// it doesn't, so try str1
			if (str2 == NULL)
				strcpy(ans, str1);
			else
				sprintf(ans, "%s/%s", str1, str2);

			fix_filename_slashes(ans);
			mkdir(ans, 0755);
		}
	}
	else
	{
		if (str2 == NULL)
		{
			sprintf (ans, "%s/%s", user_dir, file);
			fix_filename_slashes(ans);

			if (userdir == 1)
				return ans;

			if (!exists(ans))
				sprintf (ans, "%s/%s", str1, file);
		}
		else
		{
			sprintf (ans, "%s/%s/%s", user_dir, str2, file);
			fix_filename_slashes(ans);

			if (userdir == 1)
				return ans;

			if (!exists(ans))
				sprintf (ans, "%s/%s/%s", str1, str2, file);
		}
	}

	fix_filename_slashes(ans);
	return ans;
}

/*! \brief Return the name of 'significant' directories.
 *
 * \param   dir Enumerated constant for directory type  \sa DATA_DIR et al.
 * \param   file File name below that directory.
 * \returns the combined path
 */
const char *find_resource_file (int dir, const char *file)
{
	HINSTANCE SHFolder;
	SHGETFOLDERPATH SHGetFolderPath;
	char *home;
	static char ans[MAX_PATH];

	if (!init_path)
	{
		/* Get home directory; this bit originally written by SH */
		struct passwd *pwd;
		char *home = getenv ("HOME");

		if (home == NULL)
		{
			/* Try looking in password file for home dir. */
			if ((pwd = getpwuid (getuid ())))
				home = pwd->pw_dir;
		}

		/* Do not get fooled by a corrupted $HOME */
		if (home != NULL && strlen (home) < PATH_MAX)
		{
			sprintf (user_dir, "%s/.blocks4", home);
			/* Always try to make the directory, just to be sure. */
			mkdir (user_dir, 0755);
		}
		else
		{
			strcpy (user_dir, ".");
		}

		/* Now the data directory */
		get_executable_name (exe, sizeof (exe));

		if (strcmp (exe, UBLOCKS_BIN) == 0)
		{
			/* It's in its proper installed location */
			strcpy (data_dir, UBLOCKS_DATA);
		}
		else
		{
			/* Not installed, development version */
			strcpy (data_dir, ".");
		}

		init_path = 1;
	}

	switch (dir)
	{
		case APP_DIR:
			return get_resource_file_path(data_dir, NULL, file, 0);
			break;

		case GRAPHICS_DIR:
			return get_resource_file_path(data_dir, "graphics", file, 0);
			break;

		case MUSIC_DIR:
			return get_resource_file_path(data_dir, "music", file, 0);
			break;

		case MAP_DIR:
			return get_resource_file_path(data_dir, "maps", file, 0);
			break;
   
		case SAVE_DIR:
		case SETTINGS_DIR:
			return get_resource_file_path(user_dir, NULL, file, 1);
			break;

		default:
			return NULL;
	}

	return fix_filename_slashes (ans);
}

#endif
