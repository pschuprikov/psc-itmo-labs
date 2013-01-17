create or replace view alive_research_groups as 
    select * from research_groups where closed is null;

create or replace view alive_research_lines as
    select * from research_lines where closed is null;

create or replace view alive_research_lines_full as
    select rl.rl_id as rl_id, rl.rg_id as rg_id,
           rl.title as ltitle, rg.title as gtitle,
           rl.closed as lclosed, rg.closed as gclosed from
        alive_research_groups rg inner join alive_research_lines rl
        on rg.rg_id = rl.rg_id;

create or replace view alive_research_tasks as
    select * from research_tasks where resolved is null;

create or replace view alive_research_tasks_full as
    select rlf.*, rt.rt_id, rt.title as ttitle from
        alive_research_lines_full rlf inner join alive_research_tasks rt
        on rt.rl_id = rlf.rl_id and rt.rg_id = rlf.rg_id;

create or replace view active_documents as
    select rtf.*, d.d_id, d.title as dtitle from
        alive_research_tasks_full rtf inner join research_task_documents rtd
        on (rtf.rt_id = rtd.rt_id) inner join documents d 
        on (rtd.d_id = d.d_id);

create or replace view root_comments as
    select * from comments where pc_id = c_id;

create or replace view descendants as 
    with recursive descen(desc_id, anc_id) as
        (select c_id, pc_id from comments
            where pc_id != c_id
         union all 
            select c.c_id, d.anc_id from 
            comments c inner join descen d 
            on (c.pc_id != c.c_id and c.pc_id = d.desc_id))
    select * from descen;

create or replace view documents_root_comments as
    select * from root_comments natural join documents;

create or replace view line_touched_documents as
    select distinct rl.title as ttitle, d.title as dtitle, 
    rl.rl_id, d.d_id, rl.opened, rl.closed from 
    documents d inner join research_task_documents rtd
        on (d.d_id = rtd.d_id) inner join research_tasks rt
        on (rtd.rt_id = rt.rt_id) inner join research_lines rl
        on (rl.rl_id = rt.rl_id)
