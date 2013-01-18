insert into rooms (id) values (462), (454), (476);
insert into places (room_id, id) values 
    (462, 0), (462, 1), (462, 2),
    (454, 0), (454, 1), (454, 2),
    (476, 0), (476, 1), (476, 2);

insert into units (id, name) values (0, 'piece');

insert into items (id, unit_id, name, amount) values 
    (0, 0, 'chair', 10),
    (1, 0, 'bed'  , 10),
    (2, 0, 'table', 10);
select place_item(0, 462, 3);
select place_item(0, 454, 3);
select place_item(0, 476, 3);
select place_item(1, 462, 3);
select place_item(1, 454, 3);
select place_item(1, 476, 3);
select place_item(2, 462, 2);
select place_item(2, 454, 2);
select place_item(2, 476, 2);

insert into departments (id, name) values
    (0, 'ITP');

insert into groups (id, department_id, name) values
    (0, 0, '4539'),
    (1, 0, '4538');

insert into students (id, group_id, first_name, last_name) values
    (0, 0, 'Alan'  , 'Griffiths'),
    (1, 0, 'Alex'  , 'Miller'   ),
    (2, 0, 'Allan' , 'Kelly'    ),
    (3, 0, 'Andres', 'Noras'    );

insert into time_units (id, lasting, name) values
    (0, interval '1 month', 'month'),
    (1, interval '1 day'  , 'day'  );

insert into contracts (id, room_id, place_id, student_id, unit_id, 
                       signed, lasting, price) values 
    (0, 454, 0, 0, 0, date '2012-12-12', 6, '500.0'),
    (1, 454, 1, 1, 0, date '2012-12-21', 6, '2500.0'),
    (2, 462, 0, 2, 0, date '2012-2-20' , 4, '500.0'),
    (3, 462, 1, 3, 0, date '2012-10-10', 4, '2500.0');

insert into payments (id, contract_id, amount) values
    (0, 0, '300.0' ),
    (1, 0, '200.0' ),
    (2, 1, '500.0' ),
    (3, 1, '500.0' ),
    (4, 0, '2500.0');
