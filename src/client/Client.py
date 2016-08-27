#!/usr/bin/env python3

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk
from UI import UI

class hanlder:
        def hanlder_exit(self, *args):
                Gtk.main_quit(*args)

        def hanlder_login(self, button):
                print("You clicked ", self.hanlder_login.__name__)
                # Todo

        def hanlder_logout(self, button):
                print("You clicked ", self.hanlder_logout.__name__)
                # Todo

        def hanlder_register(self, button):
                app.register()
                # Todo

        def hanlder_user_list_refresh(self, button):
                print("You clicked ", self.hanlder_user_list_refresh.__name__)
                # Todo

        def hanlder_user_list_new(self, button):
                print("You clicked ", self.hanlder_user_list_new.__name__)
                # Todo

        def hanlder_user_list_remove(self, button):
                print("You clicked ", self.hanlder_user_list_remove.__name__)
                # Todo

        def hanlder_list_song_insert(self, button):
                print("You clicked ", self.hanlder_list_song_insert.__name__)
                # Todo

        def hanlder_list_song_remove(self, button):
                print("You clicked ", self.hanlder_list_song_remove.__name__)
                # Todo

        def hanlder_list_song_double_clicked(self, view):
                print("You double clicked ", self.hanlder_list_song_double_clicked.__name__)
                # Todo

        def hanlder_list_change(self, selection):
                print("You clicked ", self.hanlder_list_change.__name__)
                # Todo

        def hanlder_search_song(self, button):
                print("You clicked ", self.hanlder_search_song.__name__)
                # Todo

        def hanlder_play_pause(self, button):
                print("You clicked ", self.hanlder_play_pause.__name__)
                # Todo

        def hanlder_play_next(self, button):
                print("You clicked ", self.hanlder_play_next.__name__)
                # Todo

        def hanlder_play_last(self, button):
                print("You clicked ", self.hanlder_play_last.__name__)
                # Todo

if __name__ == "__main__":
        app = UI()
        app.bind_hanlder(hanlder())
        app.main()