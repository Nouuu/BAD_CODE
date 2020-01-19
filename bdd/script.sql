create table user
(
    id         integer,
    email      text,
    first_name text,
    last_name  text,
    photo      blob,
    photo_name text,
    birthdate  text,
    constraint user_pk
        primary key (id autoincrement)
);

create table sanction
(
    id          integer,
    name        text,
    description text,
    user_fk     integer,
    constraint sanction_pk
        primary key (id autoincrement),
    foreign key (user_fk) references user
);

create table class
(
    id             integer,
    name           text,
    year           integer,
    apprenticeship int,
    major          text,
    user_fk        integer,
    sanction_fk    integer,
    constraint class_pk
        primary key (id autoincrement),
    foreign key (user_fk) references user,
    foreign key (sanction_fk) references sanction
);

create table student
(
    id         integer,
    first_name text,
    last_name  int,
    photo      blob,
    photo_name text,
    email      text,
    nb_bottles integer,
    class_fk   integer,
    constraint student_pk
        primary key (id autoincrement),
    foreign key (class_fk) references class
);

create table deliverable
(
    id                    integer,
    due_date              text,
    subject               text,
    audio_record          blob,
    audio_record_name     text,
    video_record          blob,
    video_record_name     text,
    bad_code              blob,
    bad_code_name         text,
    deliverable_file      blob,
    deliverable_file_name text,
    status                text,
    student_fk            integer,
    constraint deliverable_pk
        primary key (id autoincrement),
    foreign key (student_fk) references student
);


