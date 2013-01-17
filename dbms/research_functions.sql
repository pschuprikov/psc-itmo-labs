create or replace function find_alive_group(in_title text)
returns int as $$
declare
    cur_rg_id int;
begin
    select rg_id into cur_rg_id from alive_research_groups where
        title = in_title;
    if cur_rg_id is null then
        raise 'No alive groups with title %', in_title;
    else
        return cur_rg_id;
    end if;
end; 
$$ language plpgsql;

create or replace function find_alive_researcher(in_fname text, in_lname text)
returns int as $$
declare
    cur_r_id int;
begin
    select r_id into cur_r_id from researchers R where
        fname = in_fname and lname = in_lname;
    if cur_r_id is null then
        raise 'no researcher called % %', in_fname, in_lname;
    else
        return cur_r_id;
    end if;
end; 
$$ language plpgsql;

create or replace function find_alive_line(in_title text, in_gtitle text)
returns int as $$
declare
    cur_rl_id int;
begin
    select rl_id into cur_rl_id from alive_research_lines_full
        where ltitle = in_title and gtitle = in_gtitle;
    if cur_rl_id is null then
        raise 'No alive line with name % within group %', 
            in_title, in_gtitle;
    else
        return cur_rl_id;
    end if;
end; 
$$ language plpgsql;

create or replace function attach_group_researcher(in_title text, 
    in_fname text, in_lname text, in_role text) returns boolean as $$
begin
    insert into research_group_memberships (role, r_id, rg_id) values
        (in_role, find_alive_researcher(in_fname, in_lname), 
            (select rg_id from alive_research_groups where title = in_title));
    return true;
end;
$$ language plpgsql;

create or replace function attach_line_researcher(in_title text,
    in_gtitle text, in_fname text, in_lname text, 
    in_role text) returns boolean as $$
begin
    with rl as (select rl_id, rg_id from alive_research_lines_full where
        ltitle = in_title and gtitle = in_gtitle)
    insert into research_line_memberships (role, r_id, rg_id, rl_id) values
        (in_role, find_alive_researcher(in_fname, in_lname),
            (select rg_id from rl), (select rl_id from rl));
    return true; 
end;
$$ language plpgsql;

create or replace function detach_group_researcher(in_title text,
    in_fname text, in_lname text) returns void as $$
begin
    delete from research_group_memberships 
        where r_id = find_alive_researcher(in_fname, in_lname) and
             rg_id = find_alive_group(in_title);
end;
$$ language plpgsql;

create or replace function detach_line_researcher(in_title text,
    in_gtitle text, in_fname text, in_lname text) returns void as $$
begin
    delete from research_line_memberships
        where r_id = find_alive_researcher(in_fname, in_lname) and
            rl_id = find_alive_line(in_title, in_gtitle);
end;
$$ language plpgsql;

create or replace function create_research_group(in_title text, in_fname text, 
    in_lname text) returns void as $$
begin
    with RG as 
        (insert into research_groups (title, mr_id, opened) values
            (in_title, find_alive_researcher(in_fname, in_lname), now())
            returning rg_id, mr_id)
    insert into research_group_memberships (role, r_id, rg_id) values
        ('manager', (select mr_id from RG), (select rg_id from RG));
end;
$$ language plpgsql;

create or replace function create_research_line(in_title text,
    in_gtitle text, in_fname text, in_lname text) 
returns void as $$
begin
    with RL as
        (insert into research_lines (title, mr_id, rg_id, opened) values
            (in_title, find_alive_researcher(in_fname, in_lname),
             find_alive_group(in_gtitle), now()) returning rl_id, rg_id, mr_id)
    insert into research_line_memberships (role, r_id, rg_id, rl_id) values
        ('manager', (select mr_id from RL), (select rg_id from RL), 
            (select rl_id from RL));
end;
$$ language plpgsql;

create or replace function cease_research_line(in_title text,
    in_gtitle text)
returns void as $$
declare
    cur_rl_id int;
begin
    cur_rl_id := find_alive_line(in_title, in_gtitle);
    update research_lines set closed = now() where
        rl_id = cur_rl_id;
end;
$$ language plpgsql;

create or replace function cease_research_group(in_title text)
    returns void as $$
declare
    cur_rg_id int;
begin
    cur_rg_id := find_alive_group(in_title);
    update research_groups set closed = now() where
        rg_id = cur_rg_id;
end;
$$ language plpgsql;

