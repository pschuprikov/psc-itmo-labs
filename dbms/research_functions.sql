create or replace function create_research_group(in_title text, in_fname text, 
    in_lname text) returns void as $$
declare
   cur_mr_id int;
   cur_rg_id int;
begin
    select r_id into cur_mr_id from researchers R where
        R.fname = in_fname and R.lname = in_lname;
    select * into cur_rg_id from nextval('research_groups_rg_id_seq');
    with ins as 
        (insert into research_groups (rg_id, title, mr_id, opened) values
            (cur_rg_id, in_title, cur_mr_id, now()))
    insert into research_group_memberships (role, r_id, rg_id) values
        ('manager', cur_mr_id, cur_rg_id);
end;
$$ language plpgsql;

create or replace function cease_research_group(in_title text)
    returns void as $$
begin
    update research_groups set closed = now() where
        title = in_title;
end;
$$ language plpgsql;

create or replace function research_group_alive(in_rg_id int)
returns boolean as $$
begin
    return exists (select * from research_groups where 
        rg_id = in_rg_id and closed != null);
end;
$$ language plpgsql;

create or replace function on_research_group_update() returns trigger as $$
begin
    if OLD.closed != null then
        return OLD;
    elsif NEW.closed != null then
        update researche_lines set closed = NEW.closed where
            rg_id = OLD.rg_id;
    end if;
    return NEW;
end;
$$ language plpgsql;



create or replace function attach_group_researcher(in_title text, 
    in_fname text, in_lname text, in_role text) returns boolean as $$
declare
    cur_r_id int;
    cur_rg_id int;
begin
    select rg_id into cur_rg_id from research_groups RG where
            RG.title = title;
    if not research_group_alive(cur_rg_id) then
        return false;
    end if;
    select r_id into cur_r_id from researchers R where
            R.fname = in_fname and R.lname = in_lname;
    insert into research_group_memberships (role, r_id, rg_id) values
        (in_role, cur_r_id, cur_rg_id);
    return true;
end;
$$ language plpgsql;

