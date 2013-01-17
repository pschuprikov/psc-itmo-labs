create or replace function fill_commits() returns void as $$
declare
    c1 int;
    c2 int;
    c3 int;
begin
    c1 := create_comment('GavGav', 'section 2.3', 'Is this wrong?', 
        'Isaac', 'Newton', 'true');
    c2 := create_comment('GavGav', 'section 2.4', 'This is whong!', 
        'Albert', 'Einstein', 'false');
    c3 := answer_comment(c1, 'Yes', 'Albert', 'Einstein');
    perform (select create_comment_reference(c3, 'MuMuMu', 'last section'));
    perform (select mark_resolved(c1, c3));
end;
$$ language plpgsql;

select fill_commits();
