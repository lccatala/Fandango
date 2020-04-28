#pragma once

/* Mouse buttons */
#define FNDG_MOUSE_BUTTON_1         0
#define FNDG_MOUSE_BUTTON_2         1
#define FNDG_MOUSE_BUTTON_3         2
#define FNDG_MOUSE_BUTTON_4         3
#define FNDG_MOUSE_BUTTON_5         4
#define FNDG_MOUSE_BUTTON_6         5
#define FNDG_MOUSE_BUTTON_7         6
#define FNDG_MOUSE_BUTTON_8         7
#define FNDG_MOUSE_BUTTON_LAST      FNDG_MOUSE_BUTTON_8
#define FNDG_MOUSE_BUTTON_LEFT      FNDG_MOUSE_BUTTON_1
#define FNDG_MOUSE_BUTTON_RIGHT     FNDG_MOUSE_BUTTON_2
#define FNDG_MOUSE_BUTTON_MIDDLE    FNDG_MOUSE_BUTTON_3

/* Printable keys */
#define FNDG_KEY_SPACE              32
#define FNDG_KEY_APOSTROPHE         39  /* ' */
#define FNDG_KEY_COMMA              44  /* , */
#define FNDG_KEY_MINUS              45  /* - */
#define FNDG_KEY_PERIOD             46  /* . */
#define FNDG_KEY_SLASH              47  /* / */
#define FNDG_KEY_0                  48
#define FNDG_KEY_1                  49
#define FNDG_KEY_2                  50
#define FNDG_KEY_3                  51
#define FNDG_KEY_4                  52
#define FNDG_KEY_5                  53
#define FNDG_KEY_6                  54
#define FNDG_KEY_7                  55
#define FNDG_KEY_8                  56
#define FNDG_KEY_9                  57
#define FNDG_KEY_SEMICOLON          59  /* ; */
#define FNDG_KEY_EQUAL              61  /* = */
#define FNDG_KEY_A                  65
#define FNDG_KEY_B                  66
#define FNDG_KEY_C                  67
#define FNDG_KEY_D                  68
#define FNDG_KEY_E                  69
#define FNDG_KEY_F                  70
#define FNDG_KEY_G                  71
#define FNDG_KEY_H                  72
#define FNDG_KEY_I                  73
#define FNDG_KEY_J                  74
#define FNDG_KEY_K                  75
#define FNDG_KEY_L                  76
#define FNDG_KEY_M                  77
#define FNDG_KEY_N                  78
#define FNDG_KEY_O                  79
#define FNDG_KEY_P                  80
#define FNDG_KEY_Q                  81
#define FNDG_KEY_R                  82
#define FNDG_KEY_S                  83
#define FNDG_KEY_T                  84
#define FNDG_KEY_U                  85
#define FNDG_KEY_V                  86
#define FNDG_KEY_W                  87
#define FNDG_KEY_X                  88
#define FNDG_KEY_Y                  89
#define FNDG_KEY_Z                  90
#define FNDG_KEY_LEFT_BRACKET       91  /* [ */
#define FNDG_KEY_BACKSLASH          92  /* \ */
#define FNDG_KEY_RIGHT_BRACKET      93  /* ] */
#define FNDG_KEY_GRAVE_ACCENT       96  /* ` */
#define FNDG_KEY_WORLD_1            161 /* non-US #1 */
#define FNDG_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define FNDG_KEY_ESCAPE             256
#define FNDG_KEY_ENTER              257
#define FNDG_KEY_TAB                258
#define FNDG_KEY_BACKSPACE          259
#define FNDG_KEY_INSERT             260
#define FNDG_KEY_DELETE             261
#define FNDG_KEY_RIGHT              262
#define FNDG_KEY_LEFT               263
#define FNDG_KEY_DOWN               264
#define FNDG_KEY_UP                 265
#define FNDG_KEY_PAGE_UP            266
#define FNDG_KEY_PAGE_DOWN          267
#define FNDG_KEY_HOME               268
#define FNDG_KEY_END                269
#define FNDG_KEY_CAPS_LOCK          280
#define FNDG_KEY_SCROLL_LOCK        281
#define FNDG_KEY_NUM_LOCK           282
#define FNDG_KEY_PRINT_SCREEN       283
#define FNDG_KEY_PAUSE              284
#define FNDG_KEY_F1                 290
#define FNDG_KEY_F2                 291
#define FNDG_KEY_F3                 292
#define FNDG_KEY_F4                 293
#define FNDG_KEY_F5                 294
#define FNDG_KEY_F6                 295
#define FNDG_KEY_F7                 296
#define FNDG_KEY_F8                 297
#define FNDG_KEY_F9                 298
#define FNDG_KEY_F10                299
#define FNDG_KEY_F11                300
#define FNDG_KEY_F12                301
#define FNDG_KEY_F13                302
#define FNDG_KEY_F14                303
#define FNDG_KEY_F15                304
#define FNDG_KEY_F16                305
#define FNDG_KEY_F17                306
#define FNDG_KEY_F18                307
#define FNDG_KEY_F19                308
#define FNDG_KEY_F20                309
#define FNDG_KEY_F21                310
#define FNDG_KEY_F22                311
#define FNDG_KEY_F23                312
#define FNDG_KEY_F24                313
#define FNDG_KEY_F25                314
#define FNDG_KEY_KP_0               320
#define FNDG_KEY_KP_1               321
#define FNDG_KEY_KP_2               322
#define FNDG_KEY_KP_3               323
#define FNDG_KEY_KP_4               324
#define FNDG_KEY_KP_5               325
#define FNDG_KEY_KP_6               326
#define FNDG_KEY_KP_7               327
#define FNDG_KEY_KP_8               328
#define FNDG_KEY_KP_9               329
#define FNDG_KEY_KP_DECIMAL         330
#define FNDG_KEY_KP_DIVIDE          331
#define FNDG_KEY_KP_MULTIPLY        332
#define FNDG_KEY_KP_SUBTRACT        333
#define FNDG_KEY_KP_ADD             334
#define FNDG_KEY_KP_ENTER           335
#define FNDG_KEY_KP_EQUAL           336
#define FNDG_KEY_LEFT_SHIFT         340
#define FNDG_KEY_LEFT_CONTROL       341
#define FNDG_KEY_LEFT_ALT           342
#define FNDG_KEY_LEFT_SUPER         343
#define FNDG_KEY_RIGHT_SHIFT        344
#define FNDG_KEY_RIGHT_CONTROL      345
#define FNDG_KEY_RIGHT_ALT          346
#define FNDG_KEY_RIGHT_SUPER        347
#define FNDG_KEY_MENU               348
