#!/usr/bin/env python3

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

class UI:
        'Client UI class.'
        builder = Gtk.Builder()

        def __init__(self):
                self.builder.add_from_file("resources/free-music.ui")

                window = self.builder.get_object("window")
                window.show_all()

        def bind_hanlder(self, hanlder):
                self.builder.connect_signals(hanlder)

        def main(self):
                Gtk.main()

if __name__ == "__main__":
        message = """
        app = UI()
        app.bind_hanlder(hanler()) # hanlder class.
        app.main()
        """
        print(message)
