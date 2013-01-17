create table documents (
    d_id serial primary key,
    title text not null,
    authors text not null,
    year int not null,
    url text,
    abstract text
);
insert into documents (d_id, title, authors, year) values
    (0, 'Unknown', 'Unknown', 0);

create table researchers (
    r_id serial primary key,
    fname varchar(300) not null,
    lname varchar(300) not null,
    degree varchar(300),
    title varchar(300),
    university varchar(1000),
    about text
);
insert into researchers (r_id, fname, lname) values
    (0, 'Unknown', 'Unknown');

create table research_groups (
    rg_id serial primary key,
    title varchar(500) not null,
    opened date not null,
    closed date,
    mr_id int not null
);

create table research_group_memberships (
    role text not null,
    r_id int not null references researchers (r_id) default 0,
    rg_id int not null references research_groups (rg_id),
    primary key (r_id, rg_id)
);
    
alter table research_groups add foreign key(rg_id, mr_id) 
    references research_group_memberships (rg_id, r_id) deferrable;

create table research_lines(
    rl_id serial unique,
    title varchar(200) not null,
    opened date not null,
    closed date,
    rg_id int not null, 
    mr_id int not null,
    
    primary key (rl_id, rg_id),
    foreign key (rg_id) references research_groups (rg_id)
);

create table research_line_memberships (
    role text not null,
    r_id int not null,
    rg_id int not null,
    rl_id int not null,

    primary key (rl_id, r_id, rg_id),
    foreign key (rl_id, rg_id) references research_lines (rl_id, rg_id),
    foreign key (r_id, rg_id) references 
        research_group_memberships (r_id, rg_id)
);
alter table research_lines add foreign key (rl_id, mr_id, rg_id) 
    references research_line_memberships (rl_id, r_id, rg_id) deferrable;

create type priority as enum ('low', 'high', 'normal');

create table research_tasks (
    rt_id serial unique,
    rl_id int not null, 
    rg_id int not null,
    task text not null,
    title varchar(200) not null,
    prio priority not null default 'normal',
    resolved date,

    primary key (rt_id, rl_id, rg_id),

    foreign key (rl_id, rg_id) references research_lines (rl_id, rg_id),
    unique (rt_id, rl_id)
);

create table research_asignees (
    resolution text,
    resolved date,

    rt_id int not null,
    rl_id int not null,
    rg_id int not null,
    r_id int not null,

    primary key (rt_id, r_id),
    foreign key (rt_id, rl_id) references 
        research_tasks (rt_id, rl_id),
    foreign key (rl_id, r_id, rg_id) references 
        research_line_memberships (rl_id, r_id, rg_id),

    check (resolution is null = resolved is null)
);

create table research_task_documents (
    reason text not null,
    rt_id int not null,
    rl_id int not null,
    rg_id int not null,
    d_id int not null default 0,

    foreign key (rt_id, rl_id, rg_id) references research_tasks,
    foreign key (d_id) references documents (d_id)
        on delete set default
);

create index on research_task_documents (rt_id, d_id);
create index on research_task_documents (d_id, rt_id);

create table comments_datas (
    cd_id serial primary key,

    d_id int references documents (d_id) not null,
    pos varchar(50) not null,
    
    resc_id int,
    resc_cd_id int,

    question boolean not null,
    constraint question_consistency
        check (resc_id is null or resc_id is not null and question)
);

create index on comments_datas (d_id);

create table comments (
    c_id serial unique,
    cd_id int references comments_datas (cd_id),

    content text not null,

    pc_id int not null, 
    
    r_id int not null references researchers,

    refd_id int references documents (d_id),
    refpos varchar(50),
    constraint ref_consistency check (refd_id is null = refpos is null),

    primary key(c_id, cd_id),
    foreign key (pc_id, cd_id) references comments (c_id, cd_id)
        deferrable
);

create index on comments (pc_id);
create index on comments (r_id);

alter table comments_datas add foreign key (resc_id, resc_cd_id) 
    references comments; 
