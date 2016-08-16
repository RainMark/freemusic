-- Test Script For Free Music.

use free_music;
-- user table

insert into `user` (user_id,user_passwd,user_nick) values ('0000000001','rain','rain');

-- list table
insert into list (list_id,list_name,owner_id) values ('0000000001','我喜欢','0000000001');

-- singer table
insert into singer (singer_id,singer_name,singer_national,singer_age,singer_height,singer_weight) values
('0000000001','邓紫棋','中国香港',NULL,NULL,NULL);
insert into singer (singer_id,singer_name,singer_national,singer_age,singer_height,singer_weight) values
('0000000002','张靓颖','中国',NULL,NULL,NULL);
insert into singer (singer_id,singer_name,singer_national,singer_age,singer_height,singer_weight) values
('0000000003','Avril Lavigne','加拿大',NULL,NULL,NULL);

-- song table
insert into song (song_id,song_name,song_path,song_ftype,song_style,song_singer) values
('0000000001','红蔷薇白玫瑰',NULL,'mp3','流行','0000000001');

insert into song (song_id,song_name,song_path,song_ftype,song_style,song_singer) values
('0000000002','心电感应808',NULL,'mp3','流行','0000000002');

insert into song (song_id,song_name,song_path,song_ftype,song_style,song_singer) values
('0000000003','Game Over',NULL,'mp3','流行','0000000001');

insert into song (song_id,song_name,song_path,song_ftype,song_style,song_singer) values
('0000000004','Complicated',NULL,'mp3','流行','0000000003');

insert into song (song_id,song_name,song_path,song_ftype,song_style,song_singer) values
('0000000005','偶尔',NULL,'mp3','流行','0000000001');

-- songs_of_lists
insert into songs_of_lists (list_id,song_id) values ('0000000001','0000000001');
insert into songs_of_lists (list_id,song_id) values ('0000000001','0000000002');
insert into songs_of_lists (list_id,song_id) values ('0000000001','0000000003');
insert into songs_of_lists (list_id,song_id) values ('0000000001','0000000004');
