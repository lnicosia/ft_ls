#include "libft.h"
#include <stdarg.h>

#define LSPRINTF_BUFFER_SIZE 16384

int			ft_lsprintf(int flush, const char *restrict format, ...)
{
	static char		buffer[LSPRINTF_BUFFER_SIZE] = {0};
	static size_t	index = 0;
	int				ret;
	va_list			ap;

	va_start(ap, format);
	if (flush == 1)
	{
		index = 0;
		ret = ft_printf("%s", buffer);
		ret += ft_vprintf(format, ap);
		va_end(ap);
		return (ret);
	}
	ret = ft_novprintf(format, ap);
	va_end(ap);
	va_start(ap, format);
	if (ret >= LSPRINTF_BUFFER_SIZE)
	{
		ret = ft_printf("%s", buffer);
		index = 0;
		buffer[0] = '\0';
		ret += ft_vprintf(format, ap);
	}
	else if ((size_t)ret + index >= LSPRINTF_BUFFER_SIZE)
	{
		ft_printf("%s", buffer);
		ret = ft_vsprintf(buffer, format, ap);
		index = (size_t)ret;
	}
	else
	{
		ret = ft_vsprintf(buffer + index, format, ap);
		index += (size_t)ret;
	}
	va_end(ap);
	return (ret);
}
