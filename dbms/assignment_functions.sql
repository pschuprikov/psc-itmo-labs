create or replace function create_research_task(in_title text,
    in_ltitle text, in_gtitle text, in_fname text, in_lname text,
    in_task text) 
returns void as $$
begin
    with RT as 
        (insert into research_tasks (rl_id, rg_id, title, task) values
         (find_alive_line(in_ltitle, in_gtitle), find_alive_group(in_gtitle),
          in_title, in_task) returning rt_id, rl_id, rg_id)
    insert into research_asignees (rt_id, rl_id, r_id, rg_id) values
         ((select rt_id from RT), (select rl_id from RT),
          find_alive_researcher(in_fname, in_lname), (select rg_id from RT));
end;
$$ language plpgsql;

create or replace function find_alive_task(in_title text, in_ltitle text, 
    in_gtitle text) returns int as $$
declare
    cur_rt_id int;
begin
    select rt_id into cur_rt_id from alive_research_tasks_full where
        ttitle = in_title and ltitle = in_ltitle and gtitle = in_gtitle;
    if cur_rt_id is null then
       raise 'no living task with title % from research line % within group %', 
           in_title, in_ltitle, in_gtitle;
    else
        return cur_rt_id;
    end if;
end;
$$ language plpgsql;

create or replace function assign_research_task(in_title text,
    in_ltitle text, in_gtitle text, in_fname text, in_lname text) 
returns void as $$
begin
    with rt as (select rl_id, rt_id, rg_id from 
        alive_research_tasks_full where ttitle = in_title
        and ltitle = in_ltitle and gtitle = in_gtitle)
    insert into research_asignees (rt_id, rl_id, r_id, rg_id) values
        ((select rt_id from rt), (select rl_id from rt),
         find_alive_researcher(in_fname, in_lname), (select rg_id from rt));
end;
$$ language plpgsql;

create or replace function post_resolution(in_resolution text, 
    in_ttitle text, in_ltitle text, in_gtitle text, 
    in_fname text, in_lname text) returns void as $$
declare
    cur_rt_id int;
begin
    cur_rt_id := find_alive_task(in_ttitle, in_ltitle, in_gtitle);
    update research_asignees 
        set resolution = in_resolution, resolved = now() where
        rt_id = cur_rt_id and r_id = find_alive_researcher(in_fname, in_lname);
end;
$$ language plpgsql;

