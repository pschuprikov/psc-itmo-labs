create or replace function place_item(in_item_id int, in_room_id int, in_amount int) 
    returns void
as $$
begin
    update items set amount = amount - in_amount where
        id = in_item_id;
    if not exists (select * from placings where 
                       item_id = in_item_id and room_id = in_room_id) then
        insert into placings (room_id, item_id) 
            values (in_room_id, in_item_id);
    end if;
    update placings set amount = amount + in_amount where
        item_id = in_item_id and room_id = in_room_id;
end;
$$ language plpgsql security definer;

create or replace function return_item(in_item_id int, in_room_id int, in_amount int)
    returns void
as $$
begin
    update placings set amount = amount - in_amount where
        item_id = in_item_id and room_id = in_room_id;
    update items set amount = amount + in_amount where
        id = in_item_id;
end;
$$ language plpgsql security definer;
                            
