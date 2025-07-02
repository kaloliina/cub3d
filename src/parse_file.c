#include "cub3D.h"

static bool	check_is_digit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0')
		return (false);
	while (str[i] != '\0')
	{
		if (!(str[i] >= 48 && str[i] <= 57))
			return (false);
		i++;
	}
	return (true);
}

/*
--4--
This function expects either floor or ceiling array of strings. We do the following here:
- We check if the string contains digits, if not, we encounter an error. (- or + signs is also considered as an error)
- We convert the string to integer.
- We check if the value is outside range 0-255, if yes, we encounter an error.
- We check if the array contains 3 strings, if not, we encounter an error.
As a result, we are saving the rgb info to int array both for ceiling and floor.
*/
static void	extract_rgb_info(t_game *game, char **textures, int *rgb)
{
	int		i;

	i = 0;
	while (textures[i] != NULL)
	{
		if (check_is_digit(textures[i]) == false)
		{
			free_array(textures, 1);
			cleanup_and_exit(game, ERRRGBFORMAT, 0);
		}
		rgb[i] = ft_atoi(textures[i]);
		if (rgb[i] < 0 || rgb[i] > 255)
		{
			free_array(textures, 1);
			cleanup_and_exit(game, ERRRGB, 0);
		}
		printf("Rgb: %d\n", rgb[i]);
		i++;
	}
	free_array(textures, 1);
	if (i != 3)
		cleanup_and_exit(game, ERRTHREE, 0);
}

static char	*skip_leading_spaces(t_game *game, char *info, int i)
{
	int	j;

	j = 0;
	if (i < 4)
		j = 2;
	else
		j = 1;
	if (info[j] != ' ')
	{
		free (info);
		cleanup_and_exit(game, ERRFORMAT, 0);
	}
	while (info[j] != '\0' && info[j] == ' ')
		j++;
	return (&info[j]);
}

/*
--3--
Here we are extracting the actual line from the file by creating a substring that ends in a newline.
For the direction information, we duplicate the string but we also skip the leading characters (NO, SO, etc...) as well as the spaces.
The information (NO, SO, etc...) has to be followed at least with one space, otherwise we encounter an error.
For the textures, we also skip the leading characters, we split the string using ',' and we proceed to extract the info.
*/
static void	extract_line(t_game *game, char *temp, int i)
{
	char	*line;
	char	**textures;

	line = ft_substr(temp, 0, ft_strlen(temp)
			- ft_strlen(ft_strchr(temp, '\n')));
	if (line == NULL)
		cleanup_and_exit(game, ERRMEM, 0);
	if (i < 4)
	{
		game->asset_paths[i] = ft_strdup(skip_leading_spaces(game, line, i));
		free (line);
		if (game->asset_paths[i] == NULL)
			cleanup_and_exit(game, ERRMEM, 0);
		printf("Path: %s\n", game->asset_paths[i]);
		return ;
	}
	textures = ft_split(skip_leading_spaces(game, line, i), ',');
	free (line);
	if (textures == NULL)
		cleanup_and_exit(game, ERRMEM, 0);
	if (temp[0] == 'F')
		extract_rgb_info(game, textures, game->floor_rgb);
	if (temp[0] == 'C')
		extract_rgb_info(game, textures, game->ceiling_rgb);
}

static const char	**get_required_properties(void)
{
	static const char	*information[FILE_INFO_COUNT];

	information[0] = "NO";
	information[1] = "SO";
	information[2] = "WE";
	information[3] = "EA";
	information[4] = "F";
	information[5] = "C";
	return (information);
}

/*
--2--
Here we are searching for the information (NO, SO, WE, EA, F, C) in the buffer.
If we don't find the info (file is lacking specific info or file is even empty), we exit.
We are also tracking index so we are able to return the remainder of the buffer which is the map.
The reason we are tracking it this way is because the information (except the map) can be in any
order in the beginning of the file so we want to cut the buffer so it starts from the last info
found on the file.
After this, we then proceed to extract the actual line.
Lastly, we trim the remaining buffer which is the map by cutting newlines from beginning
and end and we check if the buffer contains two newlines, meaning there's an empty line in the map file.
*/
char	*parse_file(t_game *game, char *buffer)
{
	int			i;
	int			index;
	char		*temp;
	const char	**information;

	information = get_required_properties();
	i = -1;
	index = -1;
	while (++i < 6)
	{
		temp = ft_strnstr(buffer, information[i], MAX_BUFFER_SIZE);
		if (temp == NULL)
			cleanup_and_exit(game, ERRMISSINFO, 0);
		if (index == -1 || index > (int)ft_strlen(temp))
			index = ft_strlen(ft_strchr(temp, '\n'));
		extract_line(game, temp, i);
	}
	temp = ft_strtrim(&buffer[ft_strlen(buffer) - index], "\n");
	if (temp == NULL)
		cleanup_and_exit(game, ERRMEM, 0);
	printf("The actual map:\n%s\n", temp);
	if (ft_strnstr(temp, "\n\n", MAX_BUFFER_SIZE))
	{
		free (temp);
		cleanup_and_exit(game, ERRMAPGAP, 0);
	}
	return (temp);
}
