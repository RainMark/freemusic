#ifndef __SEARCH_H

#include <gtk/gtk.h>


extern GtkWidget *search_entry;
extern GtkWidget *search_view;
extern GtkWidget *search_select;

void search_stack_init(void);
int clicked_search(GtkWidget *, gpointer );
void search_song_clear(void);
void search_song_show(struct song_st *, int );


#endif /* _SEARCH_H */
