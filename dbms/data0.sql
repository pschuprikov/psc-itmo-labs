insert into researchers (fname, lname) values
    ('Isaac', 'Newton'),
    ('Albert', 'Einstein'),
    ('Niels', 'Bohr'),
    ('Erich', 'Gamma'),
    ('Donald', 'Knuth'),
    ('Galileo', 'Galiley'),
    ('Albreht', 'Durer'),
    ('Dmitri', 'Mendeleev'),
    ('Lev', 'Landau'),
    ('Werner', 'Heizenberg');

select create_research_group('Flowers', 'Isaac', 'Newton');
select attach_group_researcher('Flowers', 'Niels', 'Bohr', 'assistant');
select attach_group_researcher('Flowers', 'Lev', 'Landau', 'assistant');
select attach_group_researcher('Flowers', 'Werner', 'Heizenberg', 'assistant');

select create_research_group('Grass', 'Albert', 'Einstein');
select attach_group_researcher('Grass', 'Albreht', 'Durer', 'assistant');
select attach_group_researcher('Grass', 'Galileo', 'Galiley', 'researcher');
select attach_group_researcher('Grass', 'Werner', 'Heizenberg', 'researcher');

select create_research_line('Rose', 'Flowers', 'Isaac', 'Newton');
select create_research_line('Dandelion', 'Flowers', 'Isaac', 'Newton');
select create_research_line('Lizard', 'Grass', 'Galileo', 'Galiley');
select attach_line_researcher('Rose', 'Flowers', 'Werner', 'Heizenberg',
'researcher');
