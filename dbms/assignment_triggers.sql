create or replace function check_task_resolved() 
returns trigger as $$
begin
    if OLD.resolved is not null then
        raise 'task is already resolved';
    else
        return NEW;
    end if;
end;
$$ language plpgsql;

drop trigger if exists on_tasks_update on research_tasks;
create trigger on_tasks_update before update on research_tasks
for each row execute procedure check_task_resolved();

create or replace function check_asignee_resolved()
returns trigger as $$
begin
    if OLD.resolved is not null then
        raise 'asignee already resolved';
    else
        return NEW;
    end if;
end;
$$ language plpgsql;

create or replace function check_everyone_resolved()
returns trigger as $$
begin
    if NEW.resolved is not null and 
        not exists (select * from research_asignees where
            rt_id = NEW.rt_id and resolved is null and r_id != OLD.r_id) then
        update research_tasks set resolved = NEW.resolved where
            rt_id = NEW.rt_id;
    end if;
    return NEW;
end;
$$ language plpgsql;

drop trigger if exists on_asignee_update on research_asignees;
create trigger on_asignee_update after update on research_asignees
for each row execute procedure check_everyone_resolved();

drop trigger if exists on_asignee_update_bef on research_asignees;
create trigger on_asignee_update_bef before update on research_asignees
for each row execute procedure check_asignee_resolved();
