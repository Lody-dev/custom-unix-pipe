#include "libft.h"
char	*ft_tstrjoin(const char *s1, const char *s2, const char *s3)
{
	char	*new;
	int	s1_len;
	int	s2_len;
	int	s3_len;
	int	fn_len;

	if (!s1 || !s2 || !s3)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	s3_len = ft_strlen(s3);
	fn_len = s1_len + s2_len + s3_len;
	new = (char *)malloc(fn_len + 1);
	if (!new)
		return (NULL);
	ft_memcpy(new, s1, s1_len);
	ft_memcpy(new + s1_len, s2, s2_len);
	ft_memcpy(new + s1_len + s2_len, s3, s3_len);
	new[fn_len] = '\0';
	return (new);
}
