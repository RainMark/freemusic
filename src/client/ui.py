#!/usr/bin/env python3

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

class hanlder:
        def CloseApp(self, *args):
                Gtk.main_quit(*args)

class ui:
        'Client UI class.'
        builder = Gtk.Builder()

        def __init__(self):
                self.builder.add_from_file("resources/free-music.ui")
                self.builder.connect_signals(hanlder())
                window = self.builder.get_object("window")
                window.show_all()
        
        def main(self):
                Gtk.main()

if __name__ == "__main__":
        ui = ui()
        ui.main()
