a := 100;

if a <> 100 then
  if a = 150 then
    print(3333);
  end_if;
  print(222);
else
  print(4444);
  if a = 2 then 
    print(5555);
  else
    print(6666);
  end_if;
end_if;


a:=0;

for b := 1 to 10 by 2 do
  a := a + 1;
end_for;

print(a);

Index := 0;
Sum := 0;
WHILE Index < 10 DO
    Sum := Sum + Index;
    Index := Index + 1;
END_WHILE;

print(Sum);

Index := 0;
Sum := 0;
REPEAT
    Sum := Sum + Index;
    Index := Index + 1;
UNTIL Index >= 10 END_REPEAT;

print(Sum);


Value2 := 0;
WHILE 1 DO

  Value := 0;
  REPEAT
    print(Value);
    IF Value > 10 THEN
      EXIT;
    END_IF;
    Value := Value + 1;
  UNTIL 0 END_REPEAT;

  print(Value2);
  IF Value2 > 10 THEN
    EXIT;
  END_IF;
  Value2 := Value2 + 1;

END_WHILE;