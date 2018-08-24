import os

def line_count(f):
    with open(f,encoding='utf-8') as codes:
        for i,l in enumerate(codes):
            pass
        return i+1
    
def statistics(path):
    files = [ x for x in os.listdir(path) if x.endswith(".h") or x.endswith(".cpp")]
    total = 0
    for f in files:
        count = line_count(f) 
        print("{} : {}".format(f,count))
        total += count
    return total
            
if __name__ =="__main__":
    total = statistics(os.getcwd())
    print("total : {}".format(total))
