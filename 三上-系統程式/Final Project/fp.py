ad = ['START','END','BASE'] # 這裡是不會影響address的assembler directive
tb = ['CLEAR','COMPR','TIXR'] # two-byte

def decToHex(num):
    value = ''

    # 算好每個位元
    while num > 0:
        digit = num % 16
        if digit >= 10:
            value = chr(digit + 55) + value
        else:
            value = chr(digit + 48) + value
        num //= 16
    
    # 如果不到4位數就用0補齊
    while len(value) < 4:
        value = '0' + value
    
    return value

txtLocation = '/Users/erichsieh/Desktop/NTNU/三上/系統程式/fpf/40847016S/fp.txt' # fp.txt檔所在的絕對路徑
with open(txtLocation,'r',encoding='ascii',errors='ignore') as fp:
    txt = fp.readlines() # 獲得所有文字
    symtab = {}
    loc = 0

    for line in txt:
        # 字串處理
        l = line.replace('\n','')
        lt = l.replace('\t',' ')
        lst = lt.split(' ')

        # 將標籤放入symbol table
        if lst[0] != '' and len(lst) == 3:
            symtab[lst[0]] = loc
        
        # 計算address
        if lst[1] in ad:
            loc += 0
        elif lst[1] in tb:
            loc += 2
        elif lst[1][0]=='+':
            loc += 4
        elif lst[1] == 'RESW':
            loc += int(lst[2])*3
        elif lst[1] == 'RESB':
            loc += int(lst[2])
        elif lst[1] == 'BYTE':
            if lst[2][2:]=='EOF\'':
                loc += 3
            else:
                loc += 1
        else:
            loc += 3
        
    print('SYMBOL TABLE')
    for name,num in symtab.items():
        value = decToHex(num)
        print('%-6s  %s'%(name,value))
fp.close()

finish = input('Press enter to end program:')