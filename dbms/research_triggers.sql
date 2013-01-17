create or replace function check_valid_close() returns trigger as $$
begin
    if OLD.closed is not null then
        raise 'group is closed';
    elsif NEW.closed is not null then
        update research_lines set closed = NEW.closed where
            rg_id = OLD.rg_id and closed is null;
    end if;
    return NEW;
end;
$$ language plpgsql;

create or replace function check_line_valid_close() returns trigger as $$
begin
    if OLD.closed is not null then
        raise 'line is closed';
    else
        return NEW;
    end if;
end;
$$ language plpgsql;

create or replace function check_only_group_title() returns trigger as $$
begin
    if exists (select * from alive_research_groups where
        title = NEW.title) then
        raise 'there alredy exists an alive group %', NEW.title;
    else
        return NEW;
    end if;
end;
$$ language plpgsql;

create or replace function check_only_line_title() returns trigger as $$
begin
    if exists (select * from alive_research_lines where
        title = NEW.title) then
        raise 'there alredy exists an alive line %', NEW.title;
    else
        return NEW;
    end if;
end;
$$ language plpgsql;

drop trigger if exists on_group_update on research_groups;
create trigger on_group_update before update on research_groups
for each row execute procedure check_valid_close();

drop trigger if exists on_line_update on research_lines;
create trigger on_line_update after update on research_lines
for each row execute procedure check_line_valid_close();

drop trigger if exists on_group_insert on research_groups;
create trigger on_group_insert before insert or update on research_groups
for each row execute procedure check_only_group_title();

drop trigger if exists on_line_insert on research_lines;
create trigger on_line_insert before insert or update on research_lines
for each row execute procedure check_only_line_title();

