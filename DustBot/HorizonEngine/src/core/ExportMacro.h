#pragma once

#ifdef HORIZON_BUILD
#define HORIZON_API __declspec(dllexport)
#else
#define HORIZON_API __declspec(dllimport)
#endif //!HORIZON_BUILD