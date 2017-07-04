DEFINT A-Z

PRINT "Diameter of lens (D>0)"; : INPUT D
R = INT(D / 2)
PRINT "Magnification factor (0>Mó"; LTRIM$(STR$(R)); ")"; : INPUT M
S = SQR(R ^ 2 - M ^ 2)

DIM TFM(D * D)

FOR Y = -R TO -R + D - 1
    FOR X = -R TO -R + D - 1
        IF X ^ 2 + Y ^ 2 >= S ^ 2 THEN
            A = X
            B = Y
        ELSE
            Z = SQR(R ^ 2 - X ^ 2 - Y ^ 2)
            A = INT(X * M / Z + .5)
            B = INT(Y * M / Z + .5)
        END IF
        TFM(1 + (Y + R) * D + (X + R)) = (B + R) * D + (A + R)
    NEXT X
NEXT Y

PRINT "Filename to output computations"; : INPUT Filename$
OPEN Filename$ FOR OUTPUT AS #1
PRINT #1, "//Generated with a magnification factor of"; M
PRINT #1, "int LensDiameter = "; LTRIM$(RTRIM$(STR$(D))); ";"
PRINT #1, "int LensBuf[] ={";
S = 0
FOR Y = 1 TO D
    FOR X = 1 TO D
        IF X > 1 THEN
               IF S <> 0 THEN PRINT #1, ", ";
               S = 1
               PRINT #1, LTRIM$(RTRIM$(STR$(TFM((Y - 1) * D + X))));
        END IF
    NEXT X
    REM PRINT #1,
NEXT Y
PRINT #1, "};"
CLOSE #1

