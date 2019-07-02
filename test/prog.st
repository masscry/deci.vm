a := 100;

if a <> 100 then
  if a = 150 then
    3333;
  end_if;
  222;
else
  4444;
  if a = 2 then 
    5555;
  else
    6666;
  end_if;
end_if;


a:=0;

for b := 1 to 10 by 2 do
  a := a + 1;
end_for;

a;

Index := 0;
Sum := 0;
WHILE Index < 10 DO
    Sum := Sum + Index;
    Index := Index + 1;
END_WHILE;

Sum;

Index := 0;
Sum := 0;
REPEAT
    Sum := Sum + Index;
    Index := Index + 1;
UNTIL Index >= 10 END_REPEAT;

Sum;
