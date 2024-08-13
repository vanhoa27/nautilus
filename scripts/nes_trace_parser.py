trace = """
Final trace: B0()
    CONST   4   0   ui64    
    CONST   5   0   ui64    
    CONST   6   0   ui64    
    CONST   7   0   ui64    
    CONST   8   0   ui64    
    CONST   9   0   ui64    
    CONST   10  bool    
    CALL    11  ($1)    ptr 
    CONST   12  0   ui64    
    CALL    13  ($11)   ptr 
    CONST   14  0   ui64    
    ASSIGN  15  14  ui64    
    ASSIGN  16  15  ui64    
    CALL    17  ($2,$16)    ptr 
    CALL    18  ($3)    ptr 
    CONST   19  24  ui64    
    ASSIGN  20  19  ui64    
    CONST   21  1   ui64    
    MUL 22  20  21  ui64    
    ADD 23  18  22  ptr 
    LOAD    24  23  ui64    
    CONST   25  bool    
    ASSIGN  26  25  bool    
    ASSIGN  27  24  ui64    
    CONST   28  32  ui64    
    ASSIGN  29  28  ui64    
    CONST   30  1   ui64    
    MUL 31  29  30  ui64    
    ADD 32  18  31  ptr 
    LOAD    33  32  ui64    
    CONST   34  bool    
    ASSIGN  35  34  bool    
    ASSIGN  36  33  ui64    
    CONST   37  0   ui64    
    ASSIGN  38  37  ui64    
    CONST   39  1   ui64    
    MUL 40  38  39  ui64    
    ADD 41  18  40  ptr 
    LOAD    42  41  ui64    
    CONST   43  bool    
    ASSIGN  44  43  bool    
    ASSIGN  45  42  ui64    
    CONST   46  8   ui64    
    ASSIGN  47  46  ui64    
    CONST   48  1   ui64    
    MUL 49  47  48  ui64    
    ADD 50  18  49  ptr 
    LOAD    51  50  ui64    
    CONST   52  bool    
    ASSIGN  53  52  bool    
    ASSIGN  54  51  ui64    
    CONST   55  16  ui64    
    ASSIGN  56  55  ui64    
    CONST   57  1   ui64    
    MUL 58  56  57  ui64    
    ADD 59  18  58  ptr 
    LOAD    60  59  bool    
    CONST   61  bool    
    ASSIGN  62  61  bool    
    ASSIGN  63  60  bool    
    CALL    64  ($17,$18)   ptr 
    CALL    65  ($64)   ptr 
    CALL    66  ($18)   ui64    
    CONST   67  0   ui64    
    JMP 0   B5()    void    
B1()
    ASSIGN  70  67  ui64    
    ASSIGN  71  70  ui64    
    CALL    72  ($18,$71)   ptr 
    CONST   73  48  ui64    
    ASSIGN  74  73  ui64    
    CONST   75  1   ui64    
    MUL 76  74  75  ui64    
    ADD 77  72  76  ptr 
    LOAD    78  77  ui64    
    CONST   79  0   ui64    
    ASSIGN  80  78  ui64    
    CONST   81  0   ui64    
    CONST   82  0   ui64    
    ASSIGN  83  82  ui64    
    ASSIGN  84  83  ui64    
    CALL    85  ($72,$84)   ptr 
    CONST   86  0   ui64    
    ASSIGN  87  79  ui64    
    CONST   88  72  ui64    
    ASSIGN  89  88  ui64    
    CONST   90  1   ui64    
    MUL 91  89  90  ui64    
    ADD 92  72  91  ptr 
    LOAD    93  92  ui64    
    CONST   94  0   ui64    
    CONST   95  0   ui64    
    CONST   96  0   ui64    
    ASSIGN  97  93  ui64    
    JMP 0   B22()   void    
B2()
    CALL    105 ($18)   void    
    ASSIGN  106 27  ui64    
    ASSIGN  6   106 ui64    
    CONST   107 1   ui64    
    ASSIGN  4   107 ui64    
    ASSIGN  108 45  ui64    
    ASSIGN  8   108 ui64    
    ASSIGN  109 54  ui64    
    ASSIGN  9   109 ui64    
    ASSIGN  110 63  bool    
    ASSIGN  10  110 bool    
    CALL    111 ($64)   ptr 
    CONST   112 48  ui64    
    ASSIGN  113 112 ui64    
    CONST   114 1   ui64    
    MUL 115 113 114 ui64    
    ADD 116 111 115 ptr 
    LOAD    117 116 ui64    
    CONST   118 0   ui64    
    ASSIGN  119 117 ui64    
    CONST   120 0   ui64    
    CONST   121 0   ui64    
    ASSIGN  122 121 ui64    
    ASSIGN  123 122 ui64    
    CALL    124 ($111,$123) ptr 
    CONST   125 0   ui64    
    ASSIGN  126 118 ui64    
    CONST   127 72  ui64    
    ASSIGN  128 127 ui64    
    CONST   129 1   ui64    
    MUL 130 128 129 ui64    
    ADD 131 111 130 ptr 
    LOAD    132 131 ui64    
    CONST   133 0   ui64    
    CONST   134 0   ui64    
    CONST   135 0   ui64    
    ASSIGN  136 132 ui64    
    EQ  137 126 136 ui64    
    CMP 138 137 B6()    B7()    void    
B3()
    CONST   100 1   ui64    
    ADD 101 67  100 ui64    
    ASSIGN  67  101 ui64    
    JMP 0   B5()    void    
B4()
    CONST   199 32  ui64    
    MUL 200 199 81  ui64    
    ASSIGN  201 200 ui64    
    CONST   202 1   ui64    
    MUL 203 201 202 ui64    
    ADD 204 85  203 ptr 
    CONST   205 8   ui64    
    ASSIGN  206 205 ui64    
    CONST   207 1   ui64    
    MUL 208 206 207 ui64    
    ADD 209 204 208 ptr 
    LOAD    210 209 ui64    
    ASSIGN  211 210 ui64    
    ASSIGN  212 211 ui64    
    CALL    213 ($65,$212)  ptr 
    CONST   214 ptr 
    JMP 0   B29()   void    
B5() ControlFlowMerge
    LT  68  67  66  ui64    
    CMP 69  68  B1()    B2()    void    
B6()
    CALL    139 ($64)   void    
    ASSIGN  140 4   ui64    
    ASSIGN  141 8   ui64    
    ASSIGN  142 9   ui64    
    ASSIGN  143 10  bool    
    ASSIGN  144 140 ui64    
    ASSIGN  145 141 ui64    
    ASSIGN  146 142 ui64    
    ASSIGN  147 143 bool    
    CALL    148 ($2,$144,$145,$146,$147)    bool    
    ASSIGN  149 12  ui64    
    ASSIGN  150 149 ui64    
    CALL    151 ($11,$150)  void    
    ASSIGN  152 6   ui64    
    ASSIGN  153 152 ui64    
    CALL    154 ($11,$153)  void    
    ASSIGN  155 4   ui64    
    ASSIGN  156 155 ui64    
    CALL    157 ($11,$156)  void    
    ASSIGN  158 5   ui64    
    ASSIGN  159 158 ui64    
    CALL    160 ($11,$159)  void    
    ASSIGN  161 8   ui64    
    ASSIGN  162 161 ui64    
    CALL    163 ($11,$162)  void    
    ASSIGN  164 4   ui64    
    ASSIGN  165 8   ui64    
    ASSIGN  166 164 ui64    
    ASSIGN  167 165 ui64    
    CALL    168 ($2,$166,$167)  ui64    
    ASSIGN  169 168 ui64    
    ASSIGN  170 169 ui64    
    CALL    171 ($11,$170)  void    
    ASSIGN  172 148 bool    
    ASSIGN  173 172 bool    
    CALL    174 ($11,$173)  void    
    ASSIGN  175 7   ui64    
    ASSIGN  176 175 ui64    
    CALL    177 ($11,$176)  void    
    CALL    178 ($11)   ui64    
    CALL    179 ($11)   ui64    
    CALL    180 ($11)   bool    
    CALL    181 ($11)   ui64    
    CALL    182 ($1,$2,$11) void    
    CONST   183 bool    
    CMP 184 148 B8()    B9()    void    
B7()
    CONST   319 32  ui64    
    MUL 320 319 120 ui64    
    ASSIGN  321 320 ui64    
    CONST   322 1   ui64    
    MUL 323 321 322 ui64    
    ADD 324 124 323 ptr 
    CONST   325 16  ui64    
    ASSIGN  326 325 ui64    
    CONST   327 1   ui64    
    MUL 328 326 327 ui64    
    ADD 329 324 328 ptr 
    LOAD    330 329 i64 
    CONST   331 bool    
    ASSIGN  332 331 bool    
    ASSIGN  333 330 i64 
    CONST   334 8   ui64    
    ASSIGN  335 334 ui64    
    CONST   336 1   ui64    
    MUL 337 335 336 ui64    
    ADD 338 329 337 ptr 
    CONST   339 24  ui64    
    ASSIGN  340 339 ui64    
    CONST   341 1   ui64    
    MUL 342 340 341 ui64    
    ADD 343 324 342 ptr 
    LOAD    344 343 i64 
    CONST   345 bool    
    ASSIGN  346 345 bool    
    ASSIGN  347 344 i64 
    CONST   348 8   ui64    
    ASSIGN  349 348 ui64    
    CONST   350 1   ui64    
    MUL 351 349 350 ui64    
    ADD 352 343 351 ptr 
    JMP 0   B25()   void    
B8()
    CONST   185 1   i32 
    JMP 0   B26()   void    
B9()
    CONST   357 0   i32 
    ASSIGN  185 357 i32 
    JMP 0   B26()   void    
B10()
    CONST   187 1   i32 
    JMP 0   B27()   void    
B11()
    CONST   361 0   i32 
    ASSIGN  187 361 i32 
    JMP 0   B27()   void    
B12()
    ASSIGN  190 4   ui64    
    ASSIGN  191 8   ui64    
    ASSIGN  192 190 ui64    
    ASSIGN  193 191 ui64    
    CALL    194 ($2,$192,$193)  void    
    CONST   195 0   ui32    
    RETURN  195 ui32    
B13()
    CONST   365 0   ui32    
    ASSIGN  195 365 ui32    
    RETURN  195 ui32    
B14()
    CONST   217 16  ui64    
    ASSIGN  218 217 ui64    
    CONST   219 1   ui64    
    MUL 220 218 219 ui64    
    ADD 221 213 220 ptr 
    CONST   222 16  ui64    
    ASSIGN  223 222 ui64    
    CONST   224 1   ui64    
    MUL 225 223 224 ui64    
    ADD 226 204 225 ptr 
    CONST   227 8   ui64    
    ASSIGN  228 227 ui64    
    ASSIGN  229 228 ui64    
    ASSIGN  230 229 ui64    
    CALL    231 ($221,$226,$230)    i32 
    CONST   232 0   i32 
    EQ  233 231 232 i32 
    CMP 234 233 B16()   B17()   void    
B15()
    CONST   369 ptr 
    ASSIGN  277 369 ptr 
    JMP 0   B28()   void    
B16()
    CONST   235 16  ui64    
    ASSIGN  236 235 ui64    
    CONST   237 1   ui64    
    MUL 238 236 237 ui64    
    ADD 239 204 238 ptr 
    CONST   240 24  ui64    
    ASSIGN  241 240 ui64    
    CONST   242 1   ui64    
    MUL 243 241 242 ui64    
    ADD 244 204 243 ptr 
    CONST   245 24  ui64    
    ASSIGN  246 245 ui64    
    CONST   247 1   ui64    
    MUL 248 246 247 ui64    
    ADD 249 213 248 ptr 
    LOAD    250 249 i64 
    CONST   251 bool    
    ASSIGN  252 251 bool    
    ASSIGN  253 250 i64 
    LOAD    254 244 i64 
    CONST   255 bool    
    ASSIGN  256 255 bool    
    ASSIGN  257 254 i64 
    ASSIGN  258 257 i64 
    ASSIGN  259 252 bool    
    ASSIGN  260 256 bool    
    OR  261 259 260 bool    
    ADD 262 253 258 i64 
    ASSIGN  263 261 bool    
    ASSIGN  264 263 bool    
    ASSIGN  265 262 i64 
    ASSIGN  266 265 i64 
    STORE   249 266 i64 
    CONST   267 8   ui64    
    ASSIGN  268 267 ui64    
    CONST   269 1   ui64    
    MUL 270 268 269 ui64    
    ADD 271 244 270 ptr 
    CONST   272 8   ui64    
    ASSIGN  273 272 ui64    
    CONST   274 1   ui64    
    MUL 275 273 274 ui64    
    ADD 276 249 275 ptr 
    CONST   277 ptr 
    JMP 0   B28()   void    
B17()
    CALL    373 ($213)  ptr 
    CONST   374 ptr 
    ASSIGN  214 374 ptr 
    JMP 0   B29()   void    
B18()
    CONST   280 8   ui64    
    ASSIGN  281 280 ui64    
    CONST   282 1   ui64    
    MUL 283 281 282 ui64    
    ADD 284 204 283 ptr 
    LOAD    285 284 ui64    
    ASSIGN  286 285 ui64    
    ASSIGN  287 286 ui64    
    CALL    288 ($65,$287)  ptr 
    CONST   289 16  ui64    
    ASSIGN  290 289 ui64    
    CONST   291 1   ui64    
    MUL 292 290 291 ui64    
    ADD 293 288 292 ptr 
    CONST   294 16  ui64    
    ASSIGN  295 294 ui64    
    CONST   296 1   ui64    
    MUL 297 295 296 ui64    
    ADD 298 204 297 ptr 
    CONST   299 16  ui64    
    ASSIGN  300 299 ui64    
    ASSIGN  301 300 ui64    
    ASSIGN  302 301 ui64    
    CALL    303 ($293,$298,$302)    ptr 
    CONST   304 1   ui64    
    JMP 0   B30()   void    
B19()
    CONST   378 1   ui64    
    ASSIGN  304 378 ui64    
    JMP 0   B30()   void    
B20()
    CONST   308 0   ui64    
    ASSIGN  81  308 ui64    
    CONST   309 1   ui64    
    ADD 310 86  309 ui64    
    ASSIGN  86  310 ui64    
    ASSIGN  311 86  ui64    
    ASSIGN  312 311 ui64    
    CALL    313 ($72,$312)  ptr 
    CONST   314 1   ui64    
    JMP 0   B31()   void    
B21()
    CONST   382 1   ui64    
    ASSIGN  314 382 ui64    
    JMP 0   B31()   void    
B22() ControlFlowMerge
    EQ  98  87  97  ui64    
    CMP 99  98  B3()    B4()    void    
B23()
    JMP 0   B25()   void    
B24()
    JMP 0   B32()   void    
B25() ControlFlowMerge
    JMP 0   B32()   void    
B26() ControlFlowMerge
    CMP 186 183 B10()   B11()   void    
B27() ControlFlowMerge
    EQ  188 185 187 i32 
    CMP 189 188 B12()   B13()   void    
B28() ControlFlowMerge
    EQ  278 213 277 ptr 
    CMP 279 278 B18()   B19()   void    
B29() ControlFlowMerge
    NEQ 215 213 214 ptr 
    CMP 216 215 B14()   B15()   void    
B30() ControlFlowMerge
    ADD 305 81  304 ui64    
    ASSIGN  81  305 ui64    
    EQ  306 80  81  ui64    
    CMP 307 306 B20()   B21()   void    
B31() ControlFlowMerge
    ADD 315 87  314 ui64    
    ASSIGN  87  315 ui64    
    JMP 0   B22()   void    
B32() ControlFlowMerge
    CMP 353 346 B23()   B24()   void
"""

def generate_mermaid_diagram(trace):
    lines = trace.strip().split('\n')
    current_block = None
    diagram = ["```mermaid\nflowchart TD"]

    for line in lines:
        stripped_line = line.strip()
        if stripped_line.startswith("B"):
            current_block = stripped_line.split("()")[0]
        elif stripped_line.startswith("CMP") or stripped_line.startswith("JMP"):
            parts = stripped_line.split()
            command = parts[0]
            targets = [part.split("()")[0] for part in parts if "()" in part]
            for target in targets:
                diagram.append(f"{current_block} --> {target}")

    return "\n".join(diagram) + "\n```"

mermaid_diagram = generate_mermaid_diagram(trace)
print(mermaid_diagram)
