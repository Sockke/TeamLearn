create table user_tb (
  user_id varchar(64) not null,
  user_status int default 1,
  user_createtime timestamp default current_timestamp,
  user_deletetime timestamp default '0000-00-00 00:00:00',
  user_name varchar(64) not null,
  user_gender enum('0', '1') not null,
  user_password varchar(128) not null,
  user_icon blob default null,
  user_email varchar(128) default '',
  user_focus int unsigned default 0,
  user_fans int unsigned default 0,
  primary key(user_id),
  index user_index1(user_id, user_status),
  index user_index2(user_id, user_password, user_status),
);

create table question_tb(
  question_id int unsigned primary key auto_increment,
  question_status int default 1,
  question_createtime timestamp default current_timestamp,
  question_deletetime timestamp default '0000-00-00 00:00:00',
  user_id varchar(64) not null,
  question_head varchar(1024) not null,
  question_info blob not null,
  question_views int unsigned default 0,
  question_like int unsigned default 0,
  question_unlike int unsigned default 0
);

create table answer_tb(
  answer_id int unsigned primary key auto_increment,
  answer_status int default 1,
  answer_createtime timestamp default current_timestamp,
  answer_deletetime timestamp default '0000-00-00 00:00:00',
  question_id int unsigned not null,
  user_id varchar(64) not null,
  answer_info blob not null,
  answer_like int unsigned default 0,
  answer_unlike int unsigned default 0,
  index answer_index1(question_id, answer_status)
);

create table share_tb (
  share_id int unsigned primary key auto_increment,
  share_status int default 1,
  share_createtime timestamp default current_timestamp,
  share_deletetime timestamp default '0000-00-00 00:00:00',
  user_id varchar(64) not null,
  share_head varchar(1024) not null,
  share_info blob not null,
  share_read int unsigned default 0,
  share_like int unsigned default 0,
  share_unlike int unsigned default 0,
  index share_index1(share_status),
  index share_index2(share_id, share_status),
);

create table remark_tb (
  remark_id int unsigned primary key auto_increment,
  remark_status int not null default 1,
  remark_createtime timestamp default current_timestamp,
  remark_deletetime timestamp default '0000-00-00 00:00:00',
  remark_type int not null, # remark or reply
  dynamic_id int unsigned not null,
  dynamic_type int not null, # qa or share 
  user_id varchar(64) not null,
  reply_remark_id int unsigned default 0,
  remark_info blob not null,
  index remark_index1(dynamic_id, dynamic_type),
  index remark_index2(dynamic_id, dynamic_type, remark_type, remark_status),
  index remark_index3(remark_id, user_id)
);

create table favorite_tb (
  favorite_id int unsigned primary key auto_increment,
  favorite_status int default 1,
  favorite_createtime timestamp default current_timestamp,
  favorite_deletetime timestamp default '0000-00-00 00:00:00',
  user_id varchar(64) not null,
  dynamic_id int unsigned not null,
  dynamic_type int not null,
  index favorite_index1(dynamic_id, dynamic_type)
);

create table team_tb (
  team_id int unsigned primary key auto_increment,
  team_status int default 1,
  team_createtime timestamp default current_timestamp,
  team_deletetime timestamp default '0000-00-00 00:00:00',
  user_id varchar(64) not null, # creator
  team_name varchar(64) not null,
  team_info varchar(1024) not null,
  team_usernum int unsigned default 1
);

create table task_tb (
  task_id int unsigned primary key auto_increment,
  task_status int default 1,
  task_createtime timestamp default current_timestamp,
  task_deletetime timestamp default '0000-00-00 00:00:00',
  task_info varchar(1024) not null, 
  task_annex binary
);

create table task_submit_tb (
  task_submit_id int unsigned primary key auto_increment,
  task_submit_createtime timestamp default current_timestamp,
  task_submit_modifytime timestamp default current_timestamp,
  task_id int unsigned not null,
  user_id varchar(64) not null,
  task_submit_info varchar(1024),
  task_submit_annex binary,
  task_submit_score int unsigned default 0,
  index task_submit_index1(task_id)
);
