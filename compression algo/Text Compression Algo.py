'''fp = open("x.txt","r")
dict1 = {}
for i in range(128):
    dict1[chr(0+i)] = 0
#print(dict1)
for i in fp.read():
    dict1[i] += 1
dict2 ={}
for i,j in dict1.items():
    if(j != 0):
        dict2[i] = j
#print(dict2)
l = sorted(dict2.items(),key = lambda x:x[1])
print(l)'''
# saw this on google
'''def buildTree(tuples) : 
    while len(tuples) > 1 :
        leastTwo = tuple(tuples[0:2])                  # get the 2 to combine
        theRest  = tuples[2:]                          # all the others
        combFreq = leastTwo[0][0] + leastTwo[1][0]     # the branch points freq
        tuples   = theRest + [(combFreq,leastTwo)]     # add branch point to the end
                                 # sort it into place
    return tuples[0]'''
class leafnodes(object):
    def __init__(self,tup1,tup2):
        self.right = tup2
        self.left = tup1
        self.data = tup1[1]+tup2[1]
    def return_node(self):
        return self.data
    def return_left(self):
        return self.left
    def return_right(self):
        return self.right

class node():
    def __init__(self, data):
        self.data = data
        self.children = []
        self.parent = None
    def add_children(self, child):
        child.parent = self
        self.children.append(child)

fp = open("x.txt","r")
dict1 = {}
tuples=[]
for i in fp.read():
    dict1[i] = 0
fp.seek(0)
for i in fp.read():
    dict1[i] +=1

# making the priority queue
l = sorted(dict1.items(),key = lambda x:x[1])   #sorting the dictionary using values/frequencies
list_of_frequencies = dict(l).values()
print(l)
print(list_of_frequencies)
'''algo for hoffmann tree:'''
list2 = []  #list to store tree nodes
list_len = len(l) - 1
for i in range(list_len):
    data1 = l.pop(0)
    lf1 = node(data1)
    data2 = l.pop(0)
    lf2 = node(data2)
    sub_root = node(data1[1] + data2[1])
    sub_root.add_children(lf1)
    sub_root.add_children(lf2)
    l.append((sub_root.children,sub_root.data))
    l = sorted(dict1.items(),key = lambda x:x[1])
    list2.append(sub_root)




'''node = leafnodes(l[0],l[1])  #making the very first subtree
list2.append((node,node.return_node()))
l.append((node,node.return_node()))
l = sorted(dict1.items(),key = lambda x:x[1])'''
'''list_len = len(l) - 1
for i in range(list_len):
    if(0<= ord(l[0][0]) and ord(l[1][0])<=128):
        node = leafnodes(l[0],l[1])
        list2.append((node,node.return_node()))
        l.append((node,node.return_node()))
        l.pop(0)
        l.pop(0)
        l = sorted(dict1.items(),key = lambda x:x[1])'''
        
print(list2)

'''def tree_trav(some_list, sorted_dict):
    i = len(some_list) - 1
    while(somelist[i][0].return_right() != sorted_dict[1]):
        if()'''
        
        
print(list2[4].children[0].data)    
    


'''now for making the huffman tree we will take the first 2 elements(lowest frequency) of the priority queue
add them and then add this added frequency back to the queue, do this process recursively untill all
nodes are given an added frequency value...
eg- l = [(a,2),(b,10),(c,15),(d,16),(e,16),(f,17)] take a and b out add their freq and add it to the queue
l = [(12,((a,2),(b,10))),(c,15),(d,16),(e,16),(f,17)]
ultimately,
l = [(76,(44,((17,f),(27,((15,c),(12,((a,2),(b,10)))))),(32,((d,16),(e,16))))]
bits assigned:
a - 0110
b - 0111
c - 010
d - 10
e - 11
f - 00'''
