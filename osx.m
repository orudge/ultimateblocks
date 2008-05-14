/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#import <Foundation/Foundation.h>
#include <string.h>

#include "b4dirs.h"

static NSString *user_dir;
static NSString *game_dir;
static BOOL init_path = NO;

/*! \brief Returns the full path for this file
 *
 * This function first checks if the file can be found in the user's
 * directory. If it can not, it checks the relevant game directory
 * (data, music, lib, etc)
 *
 * \param base The first part of the string, assuming the file can't be
 * found in user_dir (eg. "/usr/local/share/kq")
 * \param subdir The second part of the string (eg. "maps")
 * \param file The filename
 * \returns the combined path
 */
static NSString* get_resource_file_path_osx(NSString* base, NSString* subdir, NSString* file, char userdir)
{
	NSFileManager* fm = [NSFileManager defaultManager];
	NSString* fullpath;

	if (file == nil)
	{
		if (subdir == nil)
			fullpath = [NSString pathWithComponents: [NSArray arrayWithObjects: user_dir, nil]];
		else
			fullpath = [NSString pathWithComponents: [NSArray arrayWithObjects: user_dir, subdir, nil]];

		if (userdir == 1)
		{
			mkdir(fullpath, 0755);
			return fullpath;
		}

		if (![fm fileExistsAtPath: fullpath])
		{
			if (subdir == nil)
				fullpath = [NSString pathWithComponents: [NSArray arrayWithObjects: base, nil]];
			else
				fullpath = [NSString pathWithComponents: [NSArray arrayWithObjects: base, subdir, nil]];

			mkdir(fullpath, 0755);
		}
	}
	else
	{
		if (subdir == nil)
		{
			fullpath = [NSString pathWithComponents: [NSArray arrayWithObjects: user_dir, file, nil]];

			if (userdir == 1)
				return(fullpath);

			if (![fm fileExistsAtPath: fullpath])
				fullpath = [NSString pathWithComponents: [NSArray arrayWithObjects: base, file, nil]];
		}
		else
		{
			fullpath = [NSString pathWithComponents: [NSArray arrayWithObjects: user_dir, subdir, file, nil]];

			if (userdir == 1)
				return(fullpath);

			if (![fm fileExistsAtPath: fullpath])
				fullpath = [NSString pathWithComponents: [NSArray arrayWithObjects: base, subdir, file, nil]];
		}
	}

	return fullpath;			
}

/*! \brief Return the name of 'significant' directories.
 *
 * \param dir Enumerated constant for directory type  \sa DATA_DIR et al.
 * \param file File name below that directory.
 * \returns The combined path
 */
const char *find_resource_file (int dir, const char *file)
{
	static char ans[PATH_MAX];
	NSString* found;
	NSString* nsfile = [NSString stringWithCString: file];

	if (init_path == NO)
	{
		/* Get home directory */

		NSArray* arr = NSSearchPathForDirectoriesInDomains (NSLibraryDirectory, NSUserDomainMask, YES);
		user_dir =[[arr objectAtIndex: 0] stringByAppendingPathComponent:@"Ultimate Blocks"];
		[user_dir retain];
		[[NSFileManager defaultManager] createDirectoryAtPath: user_dir attributes:nil];

		/* Now the data directory */
		game_dir =[[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent: @"Contents/Resources"];
		[game_dir retain];
		/*    NSLog(@"Putting user data in %@, game data in %@\n", user_dir, game_dir);*/
		init_path = YES;
	}

	switch (dir)
	{
		case APP_DIR:
			found = get_resource_file_path(game_dir, nil, nsfile, 0);
			break;

		case GRAPHICS_DIR:
			found = get_resource_file_path(game_dir, @"graphics", nsfile, 0);
			break;

		case MUSIC_DIR:
			found = get_resource_file_path(game_dir, @"music", nsfile, 0);
			break;

		case MAP_DIR:
			found = get_resource_file_path(game_dir, @"maps", nsfile, 0);
			break;
   
		case SAVE_DIR:
		case SETTINGS_DIR:
			found = get_resource_file_path(user_dir, nil, nsfile, 1);
			break;

		default:
			found = nil;
	}

	return found == nil ? NULL : strncpy (ans,[found cString], sizeof(ans));
}
