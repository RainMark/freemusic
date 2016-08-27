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

        def register(self):
                dialog_builder = Gtk.Builder()
                dialog_builder.add_from_file("resources/register.ui")

                dialog = dialog_builder.get_object("dialog")
                window = self.builder.get_object("window")
                dialog.set_transient_for(window)

                dialog.add_button("Cancel", 0)
                dialog.add_button("Submit", 1)
                dialog.show_all()

                email = ""
                nick = ""
                password = ""
                res = dialog.run()
                while 1 == res:
                        password_entry = dialog_builder.get_object("password")
                        email_entry = dialog_builder.get_object("email")
                        nick_entry = dialog_builder.get_object("nick")
                        email = email_entry.get_text()
                        nick = nick_entry.get_text()
                        password = password_entry.get_text()

                        if "" != email and "" != password:
                                break
                        else:
                                print("Email and password can not be empty.")
                        res = dialog.run()

                print("Email:{}\nNick:{}\nPassword:{}".format(email, nick, password))

                dialog.destroy()

        def main(self):
                Gtk.main()

if __name__ == "__main__":
        message = """
        app = UI()
        app.bind_hanlder(hanler()) # hanlder class.
        app.main()
        """
        print(message)
