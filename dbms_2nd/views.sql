create or replace view contracts_expiration as
    select c.id, c.student_id, c.room_id, c.place_id, c.signed,
           c.signed + tu.lasting * c.lasting as expire from
        contracts c inner join pricings p on (c.pricing_id = p.id)
        inner join time_units tu 
        on (tu.id = p.unit_id);

create or replace view contracts_alive as
    select * from contracts_expiration_pricing where
        expire > now();

create or replace view contracts_payment as
    select c.id, c.student_id, c.room_id, c.place_id, 
        coalesce(sum(p.amount), '0') as paid, 
            pr.price * c.lasting as cost from
        contracts c inner join pricings pr on (c.pricing_id = pr.id)
        left join payments p on (c.id = p.contract_id)
        group by c.id, pr.price;

create or replace view contracts_not_paid as
    select * from contracts_payment where
        paid < cost;

create or replace view students_deptors as
    select s.* from contracts_not_paid c inner join students s
        on (c.student_id = s.id);

create or replace view free_places as 
    select p.* from places p left join contracts c on
        (p.id = c.id and p.room_id = c.room_id);


