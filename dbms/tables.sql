create table documents (
    d_id serial primary key,
    title text not null,
    authors text not null,
    year int not null,
    url text not null,
    abstract text
);

create table researchers (
    r_id serial primary key,
    fname varchar(300) not null,
    lname varchar(300) not null,
    degree varchar(300),
    title varchar(300),
    university varchar(1000),
    about text
);

create table research_groups (
    rg_id serial primary key,
    title varchar(500) not null,
    opened date not null,
    closed date,
    mr_id int not null
);

create table research_group_memberships (
    role text not null,
    r_id int not null,
    rg_id int not null,
    primary key (r_id, rg_id),
    foreign key (r_id) references researchers (r_id),
    foreign key (rg_id) references research_groups (rg_id)
);
    
alter table research_groups add foreign key(rg_id, mr_id) 
    references research_group_memberships (rg_id, r_id);

create table research_lines(
    rl_id serial,
    title varchar(200) not null,
    opened date not null,
    closed date,
    rg_id int not null, 
    mr_id int not null,
    
    primary key (rg_id, rl_id),
    unique(rg_id, title),
    foreign key (rg_id) references research_groups (rg_id)
);

create table research_line_memberships (
    role text not null,
    r_id int not null,
    rg_id int not null,
    rl_id int not null,

    primary key (rl_id, r_id),
    foreign key (rl_id, rg_id) references research_lines (rl_id, rg_id),
    foreign key (r_id, rg_id) references research_group_memberships 
        (r_id, rg_id)
);
alter table research_lines add foreign key (rl_id, mr_id) 
    references research_line_memberships (rl_id, r_id);

create type priority as enum ('low', 'high', 'normal');

create table research_tasks (
    rt_id serial,
    rl_id int not null, 
    rg_id int not null,
    task text not null,
    title varchar(200) not null,
    prio priority not null,
    resolved date,

    foreign key (rl_id, rg_id) references research_lines (rl_id, rg_id),
    primary key (rt_id, rl_id)
);

create table research_asignees (
    resolution text not null,
    resolved date,

    rt_id int not null,
    rl_id int not null,
    ar_id int not null,

    primary key (rt_id, rl_id),
    foreign key (rt_id, rl_id) references research_tasks
        (rt_id, rl_id),
    foreign key (ar_id, rl_id) references research_line_memberships
        (r_id, rl_id)
);

create table research_task_documents (
    expected text not null,
    rt_id int not null,
    rl_id int not null,
    d_id int not null,

    foreign key (rt_id, rl_id) references research_tasks (rt_id, rl_id),
    foreign key (d_id) references documents (d_id)
);
