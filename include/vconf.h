#ifndef VOLKANO_VCONF_H
#define VOLKANO_VCONF_H

#ifdef VLK_BUILD_LIB
#define VLK_API __declspec(dllexport)
#else
#define VLK_API extern
#endif

#endif // VOLKANO_VCONF_H
