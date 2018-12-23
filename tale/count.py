import os

def line_count(f):
    with open(f,encoding='utf-8') as codes:
        for i,l in enumerate(codes):
            pass
        return i+1
    
def statistics(path):
    files = [ x for x in os.listdir(path) if x.endswith(".h") or x.endswith(".cpp")]
    total = 0
    ret = {}
    for f in files:
        count = line_count(f)
        ret[f] = count
        total += count
    ret["total"] = total
    return ret
            
if __name__ =="__main__":
    statis = statistics(os.getcwd())
    statis = sorted(statis.items(),key = lambda x: x[1])
    for k,v in statis:
        print("{:30} : {:5}".format(k,v))
