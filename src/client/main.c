#include <gtk/gtk.h>
#include "list.h"
#include "player.h"
#include "main.h"
#include "client.h"
#include "search.h"


GtkWidget *window;
GtkWidget *lview , *sview;  //list view and song view
GtkListStore *lview_store;
GtkTreeSelection * lview_selec;
GtkWidget * statusbar;
Songlist lists[50];
gint listssum = 0;
GtkWidget *Mode1 , *Mode2 , *Mode3;

/* CSS support. */
static void apply_css (GtkWidget *widget, GtkStyleProvider *provider)
{
	gtk_style_context_add_provider (gtk_widget_get_style_context (widget), provider, G_MAXUINT);
	if (GTK_IS_CONTAINER (widget))
		gtk_container_forall (GTK_CONTAINER (widget), (GtkCallback) apply_css, provider);
}

GtkWidget *do_css_shadows (GtkWidget *do_widget)
{
	if (window) {

		GtkStyleProvider *provider;
		GBytes *bytes;

		provider = GTK_STYLE_PROVIDER (gtk_css_provider_new ());
		gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), "./free-music.css", NULL);
		apply_css (window, provider);
	}

	return window;
}

/* GTK Builder
 * 2016.2.3
 */

GtkBuilder *builder;

int main ( int argc , char *argv[] )
{

	gtk_init( &argc , &argv );

	builder = gtk_builder_new();
	gtk_builder_add_from_file( builder , "../../resources/free-music.ui" , NULL );
	window = GTK_WIDGET( gtk_builder_get_object( builder , "window" ) );
	gtk_window_set_default_size(GTK_WINDOW(window), 680, 405);
	gtk_builder_connect_signals( builder , NULL );
	do_css_shadows(window);

	/* init player,songs,tree models and so on. */
	get_object_from_builder();
	search_stack_init();
	server = client_init();
	memset(lists, 0, 50 * sizeof(Songlist));
	init_music_lib();
	init_player();

	gtk_widget_show_all( window );
	g_object_unref( G_OBJECT( builder ) );
	gtk_main();

	return 0;
}

void get_object_from_builder( void )
{
	lview = GTK_WIDGET(gtk_builder_get_object(builder, "lview" ) );
	sview = GTK_WIDGET(gtk_builder_get_object(builder, "sview" ) );
	statusbar = GTK_WIDGET( gtk_builder_get_object(builder, "statusbar" ) );
	lview_selec = GTK_TREE_SELECTION( gtk_builder_get_object(builder, "lview_selec" ) );

	search_entry = GTK_WIDGET(gtk_builder_get_object(builder, "searchentry"));
	search_view = GTK_WIDGET(gtk_builder_get_object(builder, "searchview"));
	search_select = GTK_WIDGET(gtk_builder_get_object(builder, "searchselect"));

}

void init_lview(void)
{
	GtkCellRenderer *renderer;

	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(
		GTK_TREE_VIEW(lview),
		0,
		"Lists",
		renderer,
		"text",
		0,
		NULL);
}

void init_sview(void)
{
	GtkCellRenderer *renderer;

	/* renderer = gtk_cell_renderer_text_new(); */
	/* gtk_tree_view_insert_column_with_attributes( */
	/* 	GTK_TREE_VIEW(sview), */
	/* 	0, */
	/* 	"id", */
	/* 	renderer, */
	/* 	"text", 0, */
	/* 	NULL); */
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(
		GTK_TREE_VIEW(sview),
		1,
		"Name",
		renderer,
		"text", 0,
		NULL);
}

int init_music_lib(void)
{

	/* init_a_tree_view(lview , "Lists"); */
	/* init_a_tree_view(sview , "Songs"); */
	init_lview();
	init_sview();

	lview_store = gtk_list_store_new(1 , G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(lview) , GTK_TREE_MODEL(lview_store));
	lview_selec = gtk_tree_view_get_selection(GTK_TREE_VIEW(lview));
	/* gtk_tree_view_set_model(GTK_TREE_VIEW(sview) , GTK_TREE_MODEL(lists[0].store)); */
	return 1;
}


void CloseApp (GtkWidget *widget , gpointer data)
{
	clicked_stop(NULL , NULL);
	/* write_list_to_cfg(); */
	gtk_main_quit();
}


gboolean DeleteEvent (GtkWidget *widget , gpointer data)
{
      GtkWidget *dialog;
      int result;
      gboolean ret;

      dialog = gtk_message_dialog_new(GTK_WINDOW(widget), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                  GTK_MESSAGE_INFO, GTK_BUTTONS_OK_CANCEL, "真的要退出吗？");

      result = gtk_dialog_run (GTK_DIALOG(dialog));

      switch (result)
      {
          case GTK_RESPONSE_OK:
              ret = FALSE;
              break;
          case GTK_RESPONSE_CANCEL:
              ret = TRUE;
              break;
          default:
              {
                  printf("Error!");
                  ret = FALSE;
                  break;
              }
      }
      gtk_widget_destroy(dialog);
      return ret;
}
