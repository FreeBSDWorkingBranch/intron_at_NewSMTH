import sys, io

if len(sys.argv) != 2:
  print("Usage: " + sys.argv[0] + " <original-file>");
  sys.exit();

fin = io.open(sys.argv[1], "rt");

while fin.readable():
  line = fin.readline();
  if not line:
    break;
  kw = line.split();
  if len(kw) > 1:
    if kw[0] == "__sfr":
      print("SFR(" + kw[3].strip(";") + ", " + kw[2].strip("()") + ");");
    elif kw[0] == "__sbit":
      addr = int(kw[2].strip("()"), 16);
      raddr = (addr // 8) * 8;
      bit = addr % 8;
      print("SBIT(%s, 0x%02X, %d);" % (kw[3].strip(";"), raddr, bit));
    else:
      print(line, end='');
  else:
    print(line, end='');

