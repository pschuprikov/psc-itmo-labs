create or replace function create_document(in_title text, in_authors text,
    in_year int) returns void as $$
begin
    insert into documents (title, authors, year) values
        (in_title, in_authors, in_year);
end;
$$ language plpgsql;

create or replace function find_document(in_title text)
    returns int as $$
declare
    cur_d_id int;
begin
    select d_id into cur_d_id from documents where title = in_title;
    return cur_d_id;
end;
$$ language plpgsql;

create or replace function check_valid_task()
returns trigger as $$
begin
    if not exists (select * from alive_research_tasks 
        where rt_id = OLD.rt_id) then
        raise 'Task is resolved';
    end if;
end;
$$ language plpgsql;

drop trigger if exists on_task_document_change on research_task_documents;
create trigger on_task_document_change before update or delete 
on research_task_documents execute procedure check_valid_task();

create or replace function attach_document(in_title text, in_ttitle text,
    in_ltitle text, in_gtitle text, in_reason text) returns void as $$
begin
    with rt as (select * from research_tasks 
        where rt_id = find_alive_task(in_ttitle, in_ltitle, in_gtitle))
    insert into research_task_documents 
        (reason, rt_id, rl_id, rg_id, d_id) values
        (in_reason, (select rt_id from rt), (select rl_id from rt),
         (select rg_id from rt), find_document(in_title));
end;
$$ language plpgsql;

