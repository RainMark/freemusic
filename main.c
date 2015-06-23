/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2015 
 * 
 * free-music is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * free-music is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include <gtk/gtk.h>
#include "list.h"
#include "player.h"
#include "scan-mp3.h"
#include "main.h"

GtkWidget *window;
GtkWidget *lview , *sview;  //list view and song view
GtkListStore *lview_store;
GtkTreeSelection * lview_selec;
GtkWidget * statusbar;
Songlist lists[50];
gint listssum = 0;
GtkWidget *Mode1 , *Mode2 , *Mode3;

int main(int argc , char *argv[])
{
    //Create MainWidgets
    
	GtkWidget *Container;
    GtkWidget *Frame;

    GtkWidget *AddL , *Rm;
    GtkWidget *Remove , *AddS , *RmAll;
    GtkWidget *Previous , *Play , *Pause , *Next , *Stop;

    GtkWidget *Help;

    GtkWidget *Mode;

    //Create ImageWidgeets

    GtkWidget *AddLI , *RmI;
    GtkWidget *RemoveI , *AddSI , *RmAllI;
    GtkWidget *PreviousI , *PlayI , *PauseI , *NextI;

    GtkWidget *scrolll;
	GtkWidget *scrolls;

    //Start
	{
    
		gtk_init(&argc , &argv);
		gst_init(&argc , &argv);
		chdir("/opt/free-music");
	}

	lview = gtk_tree_view_new();
	sview = gtk_tree_view_new();
	
	scrolll     = gtk_scrolled_window_new(NULL , NULL);
	scrolls     = gtk_scrolled_window_new(NULL , NULL);
	
	gtk_container_add(GTK_CONTAINER(scrolll) , lview);
	gtk_container_add(GTK_CONTAINER(scrolls) , sview);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolll)
			, GTK_POLICY_AUTOMATIC
			, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolls)
			, GTK_POLICY_AUTOMATIC
			, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_size_request(scrolll , 100 , 200);
	gtk_widget_set_size_request(scrolls , 300 , 200);

	//New statusbar
	statusbar = gtk_statusbar_new();

    //Set Winndows Properties
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(window) , 400 , 300);
    gtk_window_set_title (GTK_WINDOW (window) , "Free-Music");
    gtk_window_set_position(GTK_WINDOW(window) , GTK_WIN_POS_CENTER);
    gtk_window_set_icon(GTK_WINDOW(window) , GDK_PIXBUF(create_pixbuf("Music.png")));

    //Add&Design Tables
    Frame = gtk_vbox_new(FALSE , 2);
    Container = gtk_table_new (64 , 48 , FALSE);
    gtk_table_set_row_spacings(GTK_TABLE(Container) , 2);
    gtk_table_set_col_spacings(GTK_TABLE(Container) , 2);

    //Add RadioButton
    Mode = gtk_vbox_new (TRUE , 5);
    Mode1 = gtk_radio_button_new_with_label (NULL , "顺序");
    Mode2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(Mode1) , "单曲");
    Mode3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(Mode1) , "随机");
    gtk_box_pack_start (GTK_BOX(Mode) , Mode1 , TRUE , TRUE , 1);
    gtk_box_pack_start (GTK_BOX(Mode) , Mode2 , TRUE , TRUE , 1);
    gtk_box_pack_start (GTK_BOX(Mode) , Mode3 , TRUE , TRUE , 1);

    //Add Images
    AddL = create_button_with_image_over("Add.png" , "添加列表");
    Rm = create_button_with_image_over("Del.png" , "删除列表");
    Remove = create_button_with_image_left("Del.png" , "删除歌曲");
    RmAll = create_button_with_image_left("RemoveAll.png" , "删除所有");
    AddS = create_button_with_image_left("Add.png" , "添加歌曲");
    Previous = create_button_with_image_none("Pre.png");
    Play = create_button_with_image_none("Play.png");
    Pause = create_button_with_image_none("Pause.png");
    Next = create_button_with_image_none("Next.png");
    Help = create_button_with_image_none("Help.png");
    Stop = create_button_with_image_none("Stop.png");


    //Design The Table
    gtk_table_attach_defaults (GTK_TABLE(Container) , Remove , 0 , 13 , 40 , 42);
    gtk_table_attach_defaults (GTK_TABLE(Container) , AddS , 13 , 43 , 40 , 42);
    gtk_table_attach_defaults (GTK_TABLE(Container) , RmAll , 43 , 56 , 40 , 42);
    gtk_table_attach_defaults (GTK_TABLE(Container) , Help , 0 , 6 , 42 , 60);
    gtk_table_attach_defaults (GTK_TABLE(Container) , Previous , 6 , 13 , 42 , 60);
    gtk_table_attach_defaults (GTK_TABLE(Container) , Play , 13 , 23 , 42 , 60);
    gtk_table_attach_defaults (GTK_TABLE(Container) , Pause , 23 , 33 , 42 , 60);
    gtk_table_attach_defaults (GTK_TABLE(Container) , Stop , 33 , 43 , 42 , 60);
    gtk_table_attach_defaults (GTK_TABLE(Container) , Next , 43 , 50 , 42 , 60);
    gtk_table_attach_defaults (GTK_TABLE(Container) , AddL , 58 , 61 , 40 , 60);
    gtk_table_attach_defaults (GTK_TABLE(Container) , Rm , 61 , 64 , 40 , 60);
    gtk_table_attach_defaults (GTK_TABLE(Container) , Mode , 50 , 56 , 42 , 60);
    //gtk_table_attach_defaults (GTK_TABLE(Container) ,  , 0 , 56 , 0 ,2);
    gtk_table_attach_defaults (GTK_TABLE(Container) , scrolls , 0 , 56 , 0 , 40);
    gtk_table_attach_defaults (GTK_TABLE(Container) , scrolll , 58 , 64 , 0 , 40);

    //Design The Window
    gtk_box_pack_start(GTK_BOX(Frame) , statusbar , FALSE , FALSE , 2);
    gtk_box_pack_start(GTK_BOX(Frame) , Container , FALSE , FALSE , 2);

    gtk_container_add (GTK_CONTAINER(window) , Frame);

//Do somethings
	init_music_lib();
	read_list_from_cfg();
	init_player();
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(sview) , GTK_TREE_MODEL(lists[0].store));

	gtk_widget_show_all(window);

	//Signal Connect
    g_signal_connect (GTK_OBJECT(Help) , "clicked" ,
			GTK_SIGNAL_FUNC(HelpDialog) , window);
    
	g_signal_connect (GTK_OBJECT(window) , "destroy" ,
			GTK_SIGNAL_FUNC(CloseApp) , window);
    
	g_signal_connect (GTK_OBJECT (window) , "delete_event" ,
			GTK_SIGNAL_FUNC(DeleteEvent) , NULL);

	g_signal_connect(lview_selec , "changed" ,
			GTK_SIGNAL_FUNC(on_change_list) , NULL);
	
	g_signal_connect(GTK_OBJECT(Remove) , "clicked" ,
			GTK_SIGNAL_FUNC(clicked_remove_song_from_list) , NULL);
	
	g_signal_connect(GTK_OBJECT(RmAll) , "clicked" ,
			GTK_SIGNAL_FUNC(clicked_remove_all_of_list) , NULL);
	
	g_signal_connect(GTK_OBJECT(Rm) , "clicked" ,
			GTK_SIGNAL_FUNC(clicked_del_a_list) , NULL);
	
	g_signal_connect(GTK_OBJECT(AddL) , "clicked" ,
			GTK_SIGNAL_FUNC(clicked_add_a_list) , NULL);

	g_signal_connect(GTK_OBJECT(AddS) , "clicked" ,
			GTK_SIGNAL_FUNC(clicked_add_song_to_list) , NULL);
	
	g_signal_connect(GTK_OBJECT(Play) , "clicked" ,
			GTK_SIGNAL_FUNC(clicked_play) , NULL);
	
	g_signal_connect(GTK_OBJECT(Pause) , "clicked" ,
			GTK_SIGNAL_FUNC(clicked_pause) , NULL);
	
	g_signal_connect(GTK_OBJECT(Next) , "clicked" ,
			GTK_SIGNAL_FUNC(clicked_next) , NULL);
	
	g_signal_connect(GTK_OBJECT(Previous) , "clicked" ,
			GTK_SIGNAL_FUNC(clicked_previous) , NULL);
	
	g_signal_connect(GTK_OBJECT(Stop) , "clicked" ,
			GTK_SIGNAL_FUNC(clicked_stop) , NULL);
	
	g_signal_connect(GTK_OBJECT(sview), "row-activated", 
			GTK_SIGNAL_FUNC(Double_clicked), NULL);

	g_signal_connect(GTK_OBJECT(Mode1), "clicked", 
			GTK_SIGNAL_FUNC(clicked_radio_buttons), NULL);
	
	g_signal_connect(GTK_OBJECT(Mode2), "clicked", 
			GTK_SIGNAL_FUNC(clicked_radio_buttons), NULL);
	
	g_signal_connect(GTK_OBJECT(Mode3), "clicked", 
			GTK_SIGNAL_FUNC(clicked_radio_buttons), NULL);

	gtk_main();


	return 0;
}


/////init-music-lib
int init_a_tree_view(GtkWidget * view , const gchar * title)
{
	GtkCellRenderer *renderer;

	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(
		GTK_TREE_VIEW(view) , 
		0 , 
		title , 
		renderer , 
		"text" , 
		0 , 
		NULL);
	return 1;
}

int init_music_lib(void)
{

	init_a_tree_view(lview , "歌曲列表");
	init_a_tree_view(sview , "当前列表曲目");

	lview_store = gtk_list_store_new(1 , G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(lview) , GTK_TREE_MODEL(lview_store));
	lview_selec = gtk_tree_view_get_selection(GTK_TREE_VIEW(lview));
	gtk_tree_view_set_model(GTK_TREE_VIEW(sview) , GTK_TREE_MODEL(lists[0].store));
	return 1;
}

int get_songs_by_scan_dir(void)
{
	gint i;

	scan_dir("/home");
	add_a_list("本地文件");
	for(i = 0;i < music_number ; i++)
		add_a_song_to_a_list(0 , music_lib[i].name , music_lib[i].path);
	return 1;
}


////////////////////UI functions//////////////////////////////////

void CloseApp (GtkWidget *widget , gpointer data)
{
	clicked_stop(NULL , NULL);
	write_list_to_cfg();
    gtk_main_quit();
}

gboolean DeleteEvent (GtkWidget *widget , gpointer data)
{
      GtkWidget *dialog;
      int result;
      gboolean ret;

      dialog = gtk_message_dialog_new (GTK_WINDOW(widget) , GTK_DIALOG_DESTROY_WITH_PARENT ,
                  GTK_MESSAGE_WARNING , GTK_BUTTONS_YES_NO , "真的要退出吗？");

      result = gtk_dialog_run (GTK_DIALOG(dialog));

      switch (result)
      {
          case GTK_RESPONSE_YES:
              ret = FALSE; 
              break;
          case GTK_RESPONSE_NO:
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

//Connect To Help Button
void HelpDialog (GtkWidget *widget , gpointer data)
{
    GtkWidget *Dialog;

    Dialog = gtk_message_dialog_new(GTK_WINDOW(data) , GTK_DIALOG_DESTROY_WITH_PARENT ,
                GTK_MESSAGE_INFO , GTK_BUTTONS_OK ,
                "  Free-Music V1.2\nBy Eggroll && Rain\n        Copyright\n            2015");
    gtk_dialog_run (GTK_DIALOG(Dialog));
    gtk_widget_destroy(Dialog);

}


//To Creat Buttons
GtkWidget * create_pixbuf(const gchar *filename)
{
    GdkPixbuf *Buf;

    Buf = gdk_pixbuf_new_from_file(filename , NULL);
}

GtkWidget * create_button_with_image_none(const gchar *filename)
{
    GdkPixbuf *Buf , *BufEnter;
    GtkWidget *Image;
    GtkWidget *Button;

    Buf = gdk_pixbuf_new_from_file (filename , NULL);
    BufEnter = gdk_pixbuf_scale_simple(Buf , 32 , 32 , GDK_INTERP_HYPER);
    Image = gtk_image_new_from_pixbuf(BufEnter);
    Button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(Button) , Image);

    return Button;
}

GtkWidget * create_button_with_image_left(const gchar *filename , const gchar *label)
{
    GdkPixbuf *Buf , *BufEnter;
    GtkWidget *Image;
    GtkWidget *Button;
    GtkWidget *Box;
    GtkWidget *Label;
    GtkWidget *Table;

    Label = gtk_label_new(label);
    Buf = gdk_pixbuf_new_from_file (filename , NULL);
    BufEnter = gdk_pixbuf_scale_simple(Buf , 32 , 32 , GDK_INTERP_HYPER);
    Image = gtk_image_new_from_pixbuf(BufEnter);

    Box = gtk_hbox_new(FALSE , 0);
    gtk_box_pack_start(GTK_BOX(Box) , Image , FALSE , FALSE , 2);
    gtk_box_pack_start(GTK_BOX(Box) , Label , FALSE , FALSE , 2);

    Table = gtk_table_new(1 , 1 , FALSE);
    gtk_table_attach_defaults (GTK_TABLE(Table) , Box , 0 , 1 , 0 , 1);

    Button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(Button) , Table);

    return Button;
}

GtkWidget * create_button_with_image_over(const gchar *filename , const gchar *label)
{
    GdkPixbuf *Buf , *BufEnter;
    GtkWidget *Image;
    GtkWidget *Button;
    GtkWidget *Box;
    GtkWidget *Label;

    Label = gtk_label_new(label);
    Buf = gdk_pixbuf_new_from_file (filename , NULL);
    BufEnter = gdk_pixbuf_scale_simple(Buf , 32 , 32 , GDK_INTERP_HYPER);
    Image = gtk_image_new_from_pixbuf(BufEnter);

    Box = gtk_vbox_new(FALSE , 0);
    gtk_box_pack_start(GTK_BOX(Box) , Image , FALSE , FALSE , 2);
    gtk_box_pack_start(GTK_BOX(Box) , Label , FALSE , FALSE , 2);

    Button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(Button) , Box);

    return Button;
}


