#if HAVE_PANGOCAIRO
typedef struct _SimpleBoardColor SimpleBoardColor;

/** \brief struct to hold the colors for points, dice, checkers, etc.*/
struct _SimpleBoardColor {
	float fill[3];
	float stroke[3];
	float text[3];
};

typedef struct _SimpleBoard SimpleBoard;
struct _SimpleBoard {
	SimpleBoardColor color_checker[2];
	SimpleBoardColor color_point[2];
	SimpleBoardColor color_cube;
	matchstate ms;
	gchar *annotation;
	gchar *header;
	gint text_size;
	gdouble size;
	gdouble surface_x;
	gdouble surface_y;
	cairo_t *cr;
};

extern gint simple_board_draw(SimpleBoard * board);

extern SimpleBoard *simple_board_new(matchstate * ms, cairo_t * cr);

extern SimpleBoard *simple_board_new_from_ids(gchar * position_id, gchar * matchid, cairo_t * cr);

extern void simple_board_destroy(SimpleBoard * board);
#endif
