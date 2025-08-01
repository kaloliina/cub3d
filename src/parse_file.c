#include "../include/cub3D.h"

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

static void	extract_rgb_info(t_game *game, char **textures, int *rgb)
{
	int		i;

	i = 0;
	while (textures[i] != NULL)
	{
		if (check_is_digit(textures[i]) == false)
		{
			free_array(textures, 1);
			cleanup_and_exit(game, ERRRGBFORMAT, 0, 0);
		}
		rgb[i] = ft_atoi(textures[i]);
		if (rgb[i] < 0 || rgb[i] > 255)
		{
			free_array(textures, 1);
			cleanup_and_exit(game, ERRRGB, 0, 0);
		}
		printf("Rgb: %d\n", rgb[i]);
		i++;
	}
	free_array(textures, 1);
	if (i != 3)
		cleanup_and_exit(game, ERRTHREE, 0, 0);
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
		cleanup_and_exit(game, ERRFORMAT, 0, 0);
	}
	while (info[j] != '\0' && info[j] == ' ')
		j++;
	return (&info[j]);
}

static void	extract_line(t_game *game, char *temp, int i)
{
	char	*line;
	char	**textures;

	line = ft_substr(temp, 0, ft_strlen(temp)
			- ft_strlen(ft_strchr(temp, '\n')));
	if (line == NULL)
		cleanup_and_exit(game, ERRMEM, 0, 0);
	if (i < 4)
	{
		game->asset_paths[i] = ft_strdup(skip_leading_spaces(game, line, i));
		free (line);
		if (game->asset_paths[i] == NULL)
			cleanup_and_exit(game, ERRMEM, 0, 0);
		printf("Path: %s\n", game->asset_paths[i]);
		return ;
	}
	textures = ft_split(skip_leading_spaces(game, line, i), ',');
	free (line);
	if (textures == NULL)
		cleanup_and_exit(game, ERRMEM, 0, 0);
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

char	*parse_file(t_game *game, char *buffer)
{
	int			i;
	int			index;
	char		*temp;
	const char	**information;

	information = get_required_properties();
	i = -1;
	while (++i < 6)
	{
		temp = ft_strnstr(buffer, information[i], MAX_BUFFER_SIZE);
		if (temp == NULL)
			cleanup_and_exit(game, ERRMISSINFO, 0, 0);
		if (i == 0 || index > (int)ft_strlen(temp))
			index = ft_strlen(ft_strchr(temp, '\n'));
		extract_line(game, temp, i);
	}
	temp = ft_strtrim(&buffer[ft_strlen(buffer) - index], "\n");
	if (temp == NULL)
		cleanup_and_exit(game, ERRMEM, 0, 0);
	printf("The actual map:\n%s\n", temp);
	if (ft_strnstr(temp, "\n\n", MAX_BUFFER_SIZE))
	{
		free (temp);
		cleanup_and_exit(game, ERRMAPGAP, 0, 0);
	}
	return (temp);
}
