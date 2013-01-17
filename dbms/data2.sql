select create_document('GavGav', 'Dog', 2012);
select create_document('MjauMjau', 'Cat', 2013);
select create_document('MuMuMu', 'Cow', 1009);

select attach_document('GavGav', 'Greed', 'Lizard', 'Grass', 
    'who said gav?');
select attach_document('MuMuMu', 'Fight', 'Rose', 'Flowers', 
    'who is on the field?');
