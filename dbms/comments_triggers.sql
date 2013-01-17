create or replace function check_resolved() returns trigger as $$
begin
    if (NEW.resc_id is not null and not is_root_of(NEW.c_id, NEW.resc_id)) then
        raise 'resolution should be in tree';
    else
        return NEW;
    end if;
end;
$$ language plpgsql;

drop trigger if exists on_comment_changed on comments;
create trigger on_Comment_changed after update or insert on comments
for each row execute procedure check_resolved();
