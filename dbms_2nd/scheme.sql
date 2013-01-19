create table if not exists rooms (
    id int primary key,
    location varchar(500)
);

create table if not exists places (
    id int,
    room_id int references rooms,

    primary key (room_id, id)
);

create table if not exists units (
    id int primary key,
    name varchar(300) unique not null
);

create table if not exists items (
    id int primary key,
    name varchar(300) unique not null,

    unit_id int not null references units,
    amount int not null default 0 check (amount >= 0)
);

create table if not exists placings (
    room_id int references rooms,
    item_id int references items,

    primary key (item_id, room_id),

    amount int default 0 not null check (amount >= 0)
);

create index on placings (room_id, item_id);

create table if not exists departments (
    id int primary key,
    name varchar(300) not null unique
);
create index on departments using hash (name);

create table if not exists groups (
    id int primary key,
    name varchar(5) not null unique,
    department_id int not null references departments
);
create index on groups using hash(name);

create table if not exists students (
    id          int             primary key,
    first_name  varchar(300)    not null,
    last_name   varchar(300)    not null,
    middle_name varchar(300),

    group_id    int             not null references groups
);
create index on students using hash (last_name);

create table if not exists time_units (
    id      int             primary key,
    name    varchar(300)    not null unique,

    lasting interval        not null
);

create table if not exists pricings (
    id              int         primary key,

    unit_id         int         not null references time_units,
    start_price     money       not null check (start_price >= '0.0')
);

create table if not exists price_changes (
    id          int         not null,
    ts          timestamp   not null default now(),
    pricing_id  int         not null references pricings,
    primary key (pricing_id, id),
    unique (pricing_id, ts),

    price       money       not null check (price >= '0.0')
);

create table if not exists contracts (
    id          int         primary key,

    student_id  int         unique not null references students,
    room_id     int         not null,
    place_id    int         not null,

    foreign key (room_id, place_id) references places,
    unique      (room_id, place_id),

    signed      date        not null,
    lasting     int         not null check (lasting > 0),
    pricing_id  int         not null references pricings
);
create index on contracts (student_id);
create index on contracts (room_id);
create index on contracts (signed);

create table if not exists payments (
    id              int     primary key,
    contract_id     int     not null references contracts,

    amount          money   not null check (amount >= '0.0')
);
create index on payments (contract_id);
