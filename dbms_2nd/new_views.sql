
create or replace view pricing_intervals as
    select p1.pricing_id, p1.price, 
        p1.ts as fr, min(p2.ts) as to from 
        price_changes p1 inner join price_changes p2
        on p1.pricing_id = p2.pricing_id and p1.ts < p2.ts
        group by p1.ts, p1.pricing_id, p1.price
    union
        select pr.id as pricing_id, pr.start_price,
            timestamp '0001-1-12' as fr, min(p.ts) as to
            from pricings pr inner join price_changes p
            on pr.id = p.pricing_id
            group by pr.id;
