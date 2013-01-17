create or replace function create_comment(in_title text, in_pos text,
    in_content text, in_fname text, in_lname text, in_question boolean)
returns int as $$
declare 
    cur_c_id int;
begin
    insert into comments (content, question, r_id, d_id, pos) 
        values (in_content, in_question, 
            find_alive_researcher(in_fname, in_lname), 
            find_document(in_title), in_pos) returning c_id into cur_c_id;
    return cur_c_id;
end;
$$ language plpgsql;

create or replace function answer_comment(in_pc_id int, in_content text,
    in_fname text, in_lname text)
returns int as $$
declare
    cur_c_id int;
begin
    insert into comments (content, pc_id, r_id) values
        (in_content, in_pc_id, find_alive_researcher(in_fname, in_lname))
        returning c_id into cur_c_id;
    return cur_c_id;
end;
$$ language plpgsql;

create or replace function create_comment_reference(in_c_id int, in_title text,
    in_pos text)
returns void as $$
begin
    update comments set refd_id = find_document(in_title),
                        refpos = in_pos 
        where c_id = in_c_id;
end;
$$ language plpgsql;

create or replace function mark_resolved(in_c_id int, in_resc_id int)
returns void as $$
begin
    update comments set resc_id = in_resc_id where
        c_id = in_c_id;
end;
$$ language plpgsql;

create or replace function is_root_of(in_rc_id int, in_c_id int)
returns boolean as $$
begin
    if exists (select * from root_comments where c_id = in_rc_id) and
       (in_rc_id = in_c_id or
        exists (select * from descendants 
                where anc_id = in_rc_id and desc_id = in_c_id))then
            return true;
    else
        return false;
    end if;
end;
$$ language plpgsql;
