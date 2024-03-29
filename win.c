/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/
#if defined(WIN32) || defined (WIN64)

#include <allegro.h>
#include <winalleg.h>

#define SECURITY_WIN32
#include <security.h>

#include "blocks3.h"

static int init_path = 0;
static char user_dir[MAX_PATH];
static char game_dir[MAX_PATH];
typedef HRESULT (WINAPI * SHGETFOLDERPATH) (HWND, int, HANDLE, DWORD, LPTSTR);

#  define CSIDL_FLAG_CREATE 0x8000
#  define CSIDL_APPDATA 0x1A
#  define SHGFP_TYPE_CURRENT 0


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
			_mkdir(ans);
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
			_mkdir(ans);
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
		home = ans;
		/* Get home directory; this bit originally written by SH */
		SHFolder = LoadLibrary ("shfolder.dll");

		if (SHFolder != NULL)
		{
			SHGetFolderPath = (void *) GetProcAddress (SHFolder, "SHGetFolderPathA");

			if (SHGetFolderPath != NULL)
			{
				/* Get the "Application Data" folder for the current user */
				if (SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, home) != S_OK)
					*home = '\0';
			}

			FreeLibrary (SHFolder);
		}

		/* Do not get fooled by a corrupted $HOME */
		if (home != NULL && strlen (home) < MAX_PATH)
		{
			sprintf (user_dir, "%s\\Ultimate Blocks", home);
			/* Always try to make the directory, just to be sure. */
			_mkdir (user_dir);
		}
		else
		{
			strcpy (user_dir, ".");
		}
		/* Now the data directory */
		strcpy (game_dir, ".");
		init_path = 1;
	}

	switch (dir)
	{
		case APP_DIR:
			return get_resource_file_path(game_dir, NULL, file, 0);
			break;

		case GRAPHICS_DIR:
			return get_resource_file_path(game_dir, "graphics", file, 0);
			break;

		case MUSIC_DIR:
			return get_resource_file_path(game_dir, "music", file, 0);
			break;

		case MAP_DIR:
			return get_resource_file_path(game_dir, "maps", file, 0);
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

typedef BOOL (WINAPI *CONVERTSIDTOSTRINGSID)(PSID, LPTSTR *);
typedef BOOL (WINAPI *OPENPROCESSTOKEN)(HANDLE, DWORD, PHANDLE);
typedef BOOL (WINAPI *GETTOKENINFORMATION)(HANDLE, TOKEN_INFORMATION_CLASS, LPVOID, DWORD, PDWORD);
typedef BOOL (WINAPI *ISVALIDSID)(PSID);

static const char *get_username_ex(EXTENDED_NAME_FORMAT format)
{
	CONVERTSIDTOSTRINGSID p_ConvertSidToStringSid;
	OPENPROCESSTOKEN p_OpenProcessToken;
	GETTOKENINFORMATION p_GetTokenInformation;
	ISVALIDSID p_IsValidSid;
	char username[MAX_PATH];
	DWORD len = MAX_PATH;
	HMODULE hModule;

	if (GetUserName(username, &len) == 0)
		strcpy(username, "Username");

	if (format == NameDisplay)
		return(strdup(username));
	else if (format == NameUniqueId)
	{
		hModule = LoadLibrary("advapi32.dll");

		if (hModule)
		{
			p_OpenProcessToken = (OPENPROCESSTOKEN) GetProcAddress(hModule, "OpenProcessToken");
			p_GetTokenInformation = (GETTOKENINFORMATION) GetProcAddress(hModule, "GetTokenInformation");
			p_ConvertSidToStringSid = (CONVERTSIDTOSTRINGSID) GetProcAddress(hModule, "ConvertSidToStringSidA");
			p_IsValidSid = (ISVALIDSID) GetProcAddress(hModule, "IsValidSid");

			if ((p_OpenProcessToken) && (p_ConvertSidToStringSid) && (p_GetTokenInformation))
			{
				HANDLE hToken = NULL;
				DWORD dwBufferSize = 0;
				PTOKEN_USER pTokenUser = NULL;
				LPSTR StringSid;

				/* Open the access token associated with the calling process. */
				if (!p_OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
				{
					FreeLibrary(hModule);
					return(strdup(username));
				}

				/* get the size of the memory buffer needed for the SID */
				(void)p_GetTokenInformation(hToken, TokenUser, NULL, 0, &dwBufferSize);

				pTokenUser = (PTOKEN_USER)malloc(dwBufferSize);
				memset(pTokenUser, 0, dwBufferSize);

				/* Retrieve the token information in a TOKEN_USER structure. */
				if (!p_GetTokenInformation(hToken, TokenUser, pTokenUser, dwBufferSize, &dwBufferSize))
				{
					FreeLibrary(hModule);
					free(pTokenUser);
					return(strdup(username));
				}

				CloseHandle(hToken);

				if (p_IsValidSid(pTokenUser->User.Sid))
				{
					if (p_ConvertSidToStringSid(pTokenUser->User.Sid, &StringSid) != 0)
					{
						memset(username, 0, sizeof(username));
						safe_strncpy(username, sizeof(username), StringSid, lstrlen(StringSid));
						LocalFree(StringSid);
					}
				}

				free(pTokenUser);
			}
		}

		FreeLibrary(hModule);
	}

	return(strdup(username));
}

const char *get_current_username()
{
	return(get_username_ex(NameUniqueId));
}

const char *get_current_user_nicename()
{
	return(get_username_ex(NameDisplay));
}

#endif
