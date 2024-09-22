/* See LICENSE file for copyright and license details. */

#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"
#define SESSION_FILE "/tmp/dwm-session"

/* appearance */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 0;        /* 2 is the default spacing around the bar's font */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 9;       /* snap pixel */
static const unsigned int gappih    = 12;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 12;       /* vert inner gap between windows */
static const unsigned int gappoh    = 21;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 21;       /* vert outer gap between windows and screen edge */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const char *fonts[]          = { "sans:size=10:antialias=true:autohint=true",
					"Noto Color Emoji:pixelsize=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "sans:size=10:antialias=true:autohint=true";
static const char fgcolor[]         = "#f0f0f0";
static const char bgcolor[]         = "#232323";
static const char bordercolor[]     = "#636363";
static const char col_borderbar[]   = "#636363";
static const char selfgcolor[]      = "#232323";
static const char selbgcolor[]      = "#9871ff";
static const char *colors[][3]      = {
	/*                    fg                bg              border      */
	[SchemeNorm] 	  = { fgcolor,		bgcolor,	bordercolor },
	[SchemeSel]  	  = { selfgcolor,	selbgcolor,	selbgcolor  },
	[SchemeStatus]    = { fgcolor,		bgcolor,	"#000000"   },
	[SchemeTagsSel]   = { selfgcolor,	selbgcolor,	"#000000"   },
	[SchemeTagsNorm]  = { fgcolor,		bgcolor,	"#000000"   },
	[SchemeInfoSel]   = { fgcolor,		bgcolor,	"#000000"   },
	[SchemeInfoNorm]  = { fgcolor,		bgcolor,	"#000000"   },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[]    = {"st", "-n", "spvim", "-g", "120x34", "-e", "nvim", NULL };
const char *spcmd2[]    = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd3[]    = {"keepassxc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spvim",      spcmd1},
	{"spranger",    spcmd2},
	{"keepassxc",   spcmd3},
};

/* tagging */
static const char *tags[] = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           	0,         0,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           	1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           	0,         0,          1,           0,        -1 },
	{ "Gcolor3", NULL,     NULL,           	0,         1,          0,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 	0,         0,          0,           1,        -1 }, /* xev */
	{ NULL,      "spvim", NULL,		SPTAG(0),  1,          1,           1,        -1 }, 
	{ NULL,      "spfm",   NULL, 		SPTAG(1),  1,          1,           1,        -1 }, 
};

/* layout(s) */
static const float mfact        = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	{ "[1]", bstackhoriz },
	{ "[2]", bstack      },
	{ "[3]", tile        },
	{ "[4]", spiral      },
	{ "[5]", grid        },
	{ "[F]", NULL        },
	{  NULL, NULL        },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(CHAIN,KEY,TAG) \
	{ MODKEY,                       CHAIN,    KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           CHAIN,    KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             CHAIN,    KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, CHAIN,    KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2]       = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", bgcolor, "-nf", fgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

#include "shiftview.c"
#include "movestack.c"

static const Key keys[] = {
	/* Modifier	      ChainKey	Function	Argument	Comment	   */
	{ MODKEY,		-1,	XK_j,			focusstack,	{.i = +1 } },
	{ MODKEY,		-1,	XK_k,			focusstack,	{.i = -1 } },
	{ MODKEY,		-1,	XK_bracketleft,		setmfact,	{.f = -0.05} },
	{ MODKEY,		-1,	XK_bracketright,	setmfact,	{.f = +0.05} },
	{ MODKEY,		-1,	XK_backslash,		resetmfact,	{0} },
	{ MODKEY,		-1,	XK_Up,			cyclelayout,	{ .i =  1 } },
	{ MODKEY,		-1,	XK_Down,		cyclelayout,	{ .i = -1 } },
	{ MODKEY,		-1,	XK_Right,		shiftview,	{ .i =  1 } },
	{ MODKEY,		-1,	XK_Left,		shiftview,	{ .i = -1 } },
	{ MODKEY|ShiftMask,	-1,	XK_j,			movestack,	{.i = +1 } },
	{ MODKEY|ShiftMask,	-1,	XK_k,			movestack,	{.i = -1 } },
	{ MODKEY,		-1,	XK_comma,		focusmon,	{.i = -1 } },
	{ MODKEY,		-1,	XK_period,		focusmon,	{.i = +1 } },
	{ MODKEY|ShiftMask,	-1,	XK_comma,		tagmon,		{.i = -1 } },
	{ MODKEY|ShiftMask,	-1,	XK_period,		tagmon,		{.i = +1 } },
	{ MODKEY,		-1,	XK_f,			togglefullscr,	{0} },
	{ MODKEY,		-1,	XK_s,			togglesticky,	{0} },
	{ MODKEY,		-1,	XK_q,			killclient,	{0} },
	{ MODKEY|ControlMask,	-1,	XK_r,			quit,		{1} },
	{ MODKEY|ShiftMask,	-1,	XK_q,			quit,		{0} },
	{ MODKEY,		-1,	XK_Return,		zoom,		{0} },
     	{ MODKEY,		-1,	XK_i,			incnmaster,	{.i = +1 } },
     	{ MODKEY,		-1,	XK_u,			incnmaster,	{.i = -1 } },

        /* Tags */
	TAGKEYS(		-1,	XK_1,		0)
	TAGKEYS(		-1,	XK_2,		1)
	TAGKEYS(		-1,	XK_3,		2)
	TAGKEYS(		-1,	XK_4,		3)
	TAGKEYS(		-1,	XK_5,		4)
	TAGKEYS(		-1,	XK_6,		5)
	TAGKEYS(		-1,	XK_7,		6)
	TAGKEYS(		-1,	XK_8,		7)
	TAGKEYS(		-1,	XK_9,		8)
	TAGKEYS(		-1,	XK_0,		9)

        /* Layouts */
	{ MODKEY,		XK_l,	XK_1,		setlayout,	{.v = &layouts[0]} },
	{ MODKEY,		XK_l,	XK_2,		setlayout,	{.v = &layouts[1]} },
	{ MODKEY,		XK_l,	XK_3,		setlayout,	{.v = &layouts[2]} },
	{ MODKEY,		XK_l,	XK_4,		setlayout,	{.v = &layouts[3]} },
	{ MODKEY,		XK_l,	XK_5,		setlayout,	{.v = &layouts[4]} },
	{ MODKEY,		XK_l,	XK_6,		setlayout,	{.v = &layouts[5]} },
	{ MODKEY,		XK_l,	XK_7,		setlayout,	{.v = &layouts[6]} },
	{ MODKEY,		XK_l,	XK_8,		setlayout,	{.v = &layouts[7]} },
	{ MODKEY,		XK_l,	XK_9,		setlayout,	{.v = &layouts[8]} },
	{ MODKEY,		XK_l,	XK_0,		setlayout,	{.v = &layouts[9]} },
	{ MODKEY,		XK_l,	XK_f,		togglefloating,	{0} },

        /* Run */
        { MODKEY,		-1,	XK_slash,	spawn,		{.v = dmenucmd } },
	{ MODKEY,		XK_p,	XK_1,		togglescratch,	{.ui = 0 } },
	{ MODKEY,		XK_p,	XK_2,		togglescratch,	{.ui = 1 } },
	{ MODKEY,		XK_r,	XK_t,		spawn,		{.v = termcmd } },
	{ MODKEY,		XK_r,	XK_b,		spawn,		{.v = (const char*[]){ BROWSER, NULL } } },
	{ MODKEY,		XK_r,	XK_f,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "ranger", NULL } } },
	{ MODKEY,		XK_r,	XK_e,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "vim", NULL } } },
	{ MODKEY,		XK_r,	XK_s,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
	{ MODKEY,		XK_r,	XK_a,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "pulsemixer", NULL } } },
	{ MODKEY,		XK_r,	XK_o,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "calcurse", NULL } } },
	{ MODKEY,		XK_r,	XK_g,		spawn,		{.v = (const char*[]){ "ggimp", NULL } } },

	/* Volume Controls */
        { MODKEY,		-1,	XK_minus,	spawn,		SHCMD("pactl set-sink-mute 0 false ; pactl set-sink-volume 0 -1%; kill -38 $(pidof dwmblocks)") },
     	{ MODKEY,		-1,	XK_equal,	spawn,		SHCMD("pactl set-sink-mute 0 false ; pactl set-sink-volume 0 +1%; kill -38 $(pidof dwmblocks)" ) },
     	{ MODKEY,		-1,	XK_m,		spawn,		SHCMD("pactl set-sink-mute 0 toggle; kill -38 $(pidof dwmblocks)") },

	/* Print Screen */
	{ 0,			-1,	XK_Print,	spawn,		SHCMD("maim -m 10 | xclip -selection clipboard -t image/png") },
	{ MODKEY,		-1,	XK_Print,	spawn,		SHCMD("maim -m 10 ~/Pictures/Screenshots/Screenshot-$(date +%y-%m-%d-%H-%M-%S).png") },
};

/* Mouse Buttons */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        cyclelayout,    {.i = +1 } },
	{ ClkLtSymbol,          0,              Button3,        cyclelayout,    {.i = -1 } },
	{ ClkLtSymbol,          0,              Button4,        cyclelayout,    {.i = +1 } },
	{ ClkLtSymbol,          0,              Button5,        cyclelayout,    {.i = -1 } },
	{ ClkWinTitle,          0,              Button1,        spawn,          {.v = dmenucmd } },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            0,              Button4,        shiftview,      { .i =  1 } },
	{ ClkTagBar,            0,              Button5,        shiftview,      { .i = -1 } },
};
