#!/usr/bin/env python3

import mysql.connector as mariadb

class database:
        'Free music database interface.'

        db = mariadb.connect(user="root", password="root", database="free_music")
        cursor = db.cursor()

        def __init__(self):
                pass
                # Todo

        def close(self):
            self.db.close()

        def check_sql_code(self, sql):
                print(sql)

        def get_password_by_email(self, email):
                'Return password string if email in database. else return None.'

                sql = "select user_passwd from user where user_email=\'{}\'".format(email)
                self.check_sql_code(sql)
                self.cursor.execute(sql)
                data = self.cursor.fetchone()

                if None == data:
                        return None
                else:
                        return data[0]

        def insert_new_user(self, email, password, nick):
                'Return 0 if insert successfull.'

                if None == nick:
                        nick = "your_own_nick"

                sql = """insert into user(user_email, user_passwd, user_nick)
                        values(\'{}\', \'{}\', \'{}\')""".format(email, password, nick)

                self.check_sql_code(sql)

                try:
                        self.cursor.execute(sql)
                except mariadb.Error as error:
                        print("Insert Error: {}".format(error))
                        return False

                self.db.commit()
                return True

        def get_list_by_email(self, email):
                'Return a list of tuples. When no rows are available, it returns an empty list.'

                sql = "select list_id,list_name from list where owner_email=\'{}\'".format(email)
                self.check_sql_code(sql)
                self.cursor.execute(sql)

                res = []
                data = self.cursor.fetchmany()
                while len(data):
                        res.append(data[0])
                        data = self.cursor.fetchmany()

                return res

        # Todo

if __name__ == "__main__":
        db = database()
        #db.get_password_by_email("rain.by.zhou@gmail.com")
        #db.insert_new_user("rain_of_mem@163.com", "mark_pass", "mark")
        db.get_list_by_email("rain.by.zhou@gmail.com")
        db.db.close()
