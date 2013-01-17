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
    references research_group_memberships (rg_id, r_id);

create table research_lines(
    rl_id serial primary key,
    title varchar(200) not null,
    opened date not null,
    closed date,
    rg_id int not null, 
    mr_id int not null,
    
    unique (rl_id, rg_id),
    foreign key (rg_id) references research_groups (rg_id)
);

create table research_line_memberships (
    role text not null,
    r_id int not null,
    rg_id int not null,
    rl_id int not null,

    primary key (rl_id, r_id),
    foreign key (rl_id, rg_id) references research_lines (rl_id, rg_id),
    foreign key (r_id, rg_id) references 
        research_group_memberships (r_id, rg_id)
);
alter table research_lines add foreign key (rl_id, mr_id) 
    references research_line_memberships (rl_id, r_id);

create type priority as enum ('low', 'high', 'normal');

create table research_tasks (
    rt_id serial primary key,
    rl_id int not null, 
    rg_id int not null,
    task text not null,
    title varchar(200) not null,
    prio priority not null default 'normal',
    resolved date,

    foreign key (rl_id, rg_id) references research_lines (rl_id, rg_id),
    unique (rt_id, rl_id)
);

create table research_asignees (
    resolution text,
    resolved date,

    rt_id int not null,
    rl_id int not null,
    r_id int not null,

    primary key (rt_id, r_id),
    foreign key (rt_id, rl_id) references 
        research_tasks (rt_id, rl_id),
    foreign key (r_id, rl_id) references 
        research_line_memberships (r_id, rl_id),

    check (resolution is not null and resolved is not null or
           resolution is     null and resolved is     null)
);

create table research_task_documents (
    reason text not null,
    rt_id int not null,
    d_id int not null default 0,

    foreign key (rt_id) references research_tasks (rt_id),
    foreign key (d_id) references documents (d_id)
        on delete set default
);

create table comments (
    c_id serial primary key,
    r_id int not null references researchers,
    content text not null,
    
    pc_id int references comments (c_id), 
    
    d_id int references documents (d_id) on delete set default,
    pos varchar(50),
    constraint pos_consistency check (d_id is null = pos is null),

    refd_id int references documents (d_id),
    refpos varchar(50),
    constraint ref_consistency check (refd_id is null = refpos is null),

    resc_id int references comments,
    question boolean,
    constraint question_consistency
        check (resc_id is null or resc_id is not null and question),
    
    constraint only_parent_question 
        check (pc_id is null != question is null),
    constraint only_parent_pos
        check (pc_id is null != d_id is null)
);
