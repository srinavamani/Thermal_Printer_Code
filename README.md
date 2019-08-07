Printer Data Feed Protocol Format

Rotation:

~R00120010^! 

~		Start Byte
R		Rotation
0012		Packet byte size
0010		Rotation Count
^!		End Bytes

****************************************************************************************

Tamil Font (Amma - 85AECDAEBD):

~T001985AECDAEBDS^!

~		Start Byte
T		Tamil Font
0019		Packet byte size
85AECDAEBD	Data (Amma)
S		Font Size (Small / Large)
^!		End Bytes

****************************************************************************************

English Font (Single line printing)

~E0016HelloRSL~^!

~		Start Byte
E		English font
0016		Packet byte size
Hello		Data
R		Font type (R - Regular / B - Bold)
S		Font Size (S - Small / L - Large)
L		Font Allignment (L - Left / R - Right / C - Center)
~^!		End Bytes

****************************************************************************************

English Font (Multi line printing)

~E0026HelloRSL~WorldBLC~^!

~		Start Byte
E		English font
0026		Packet byte size
Hello		First line data
R		Font type (R - Regular / B - Bold)
S		Font Size (S - Small / L - Large)
L		Font Allignment (L - Left / R - Right / C - Center)
~		Next line intimation
World		Second lind data
B		Bold Fonts
L		Large Fonts
C		Center allignment
~^!		End Bytes

****************************************************************************************

Image Printing

~I00120460^!

~		Start Byte
I		Image printing
0012		Packet byte size
0460		Image Height in pixels
^!		End Bytes
