create or replace function create_comment(in_title text, in_pos text,
    in_content text, in_fname text, in_lname text, in_question boolean)
returns int as $$
declare 
    cur_c_id int;
begin
    select * into cur_c_id from nextval('comments_c_id_seq');
    with cd as (
        insert into comments_datas(d_id, pos, question) values
            (find_document(in_title), in_pos, in_question) returning cd_id)
    insert into comments (cd_id, r_id, c_id, pc_id, content) values
        ((select cd_id from cd), find_alive_researcher(in_fname, in_lname),
         cur_c_id, cur_c_id, in_content);
    return cur_c_id;
end;
$$ language plpgsql;

create or replace function answer_comment(in_pc_id int, in_content text,
    in_fname text, in_lname text)
returns int as $$
declare
    cur_c_id int;
begin
    with p as (select c_id, cd_id from comments where c_id = in_pc_id)
    insert into comments (cd_id, r_id, pc_id, content) values
        ((select cd_id from p), find_alive_researcher(in_fname, in_lname),
         (select c_id from p), in_content) returning c_id into cur_c_id;
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

create or replace function mark_resolved(in_cd_id int, in_resc_id int)
returns void as $$
begin
    update comments_datas set resc_id = in_resc_id where
        cd_id = in_cd_id;
end;
$$ language plpgsql;

create or replace function is_root_of(in_rc_id int, in_c_id int)
returns boolean as $$
begin
    if exists (select * from root_comments where c_id = in_rc_id) and
       (in_rc_id = in_c_id or
        exists (select * from descendants 
                where anc_id = in_rc_id and desc_id = in_c_id)) then
            return true;
    else
        return false;
    end if;
end;
$$ language plpgsql;
