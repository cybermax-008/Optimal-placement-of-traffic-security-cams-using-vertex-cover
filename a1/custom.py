import sys
import re
from math import sqrt

def extract_coordinates(str): #take coordinates group from matched pattern to convert into list of coordinates
    digits=map(int, re.findall(r'[-]?\d+',str)) 
    coordinates = [[digits[i], digits[i + 1]] for i in range(len(digits) - 1)]
    del coordinates[1::2]
    return coordinates

class Point(object):
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)
    def __repr__(self):
        return '({0:.2f}, {1:.2f})'.format(self.x, self.y)
    def __str__(self):
        return repr(self)

class Line(object):
    def __init__(self,p1,p2):
        self.src=p1
        self.dst=p2
    def __repr__(self):
        return repr(self.src) + '--->'+ repr(self.dst)

def distance(a,b):
    return sqrt((a.x - b.x)**2 + (a.y - b.y)**2)

def is_between(a,c,b):
    return round(distance(a,c) + distance(c,b),2) == round(distance(a,b),2)

def gradient(l):
    m = None
    if l.src.x != l.dst.x:
        m = (1./(l.src.x -l.dst.x))*(l.src.y - l.dst.y)
        return m
    
def parallel(l1,l2):
    return gradient(l1) == gradient(l2)

def intersect(l1, l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y
    
    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    xcoor =  xnum / xden

    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
    ycoor = ynum / yden    
    
    return Point(xcoor, ycoor)

def input_parser(str):
    re_pat=r'([ac])\s+?("[a-zA-Z ]+")\s+?((\(-?\d+,-?\d+\)\s*?)+)$'
    pattern=re.compile(re_pat)
    try:
        match=pattern.match(str)
        match.group()
    except:
        re_pat=r'(r)\s+?("[a-zA-Z ]+")'
        pattern=re.compile(re_pat)
        match=pattern.match(str)
    return match


class vertices(object):
    def __init__(self,point,ind):
        self.ver=point
        self.ind=ind

class street_pool(object):
    street_info={}
    def __init__(self,ops,street_name,coordinates):
        self.ops=ops
        self.street_name=street_name
        self.coordinates=coordinates
    def add(self):
        if self.street_name not in street_pool.street_info:
            street_pool.street_info[self.street_name]=self.coordinates
        else:
            sys.stderr.write("Error:The Street" + self.street_name+" already exists.\n")
    def remove(self):
        try:
            del street_pool.street_info[self.street_name]
        except KeyError:
            sys.stderr.write("Error: 'c' or 'r' specified for a street that does not exist.\n")   
    def change(self):
        if self.street_name in street_pool.street_info:
            self.remove()
            self.add()
        else:
            sys.stderr.write("Error: 'c' or 'r' specified for a street that does not exist.\n")

class street_features(object):
    def __init__(self,st_db):
        self.line_dict={}
        self.st_db=st_db
        self.vertices={}
        self.list_edges=[]
        self.list_vertices=[]
        self.intersec=[]
    def line_form(self):
        for street in self.st_db:
            for i in range(len(self.st_db[street])-1):
                if i< len(self.st_db[street]):
                    l=Line(Point(self.st_db[street][i][0],self.st_db[street][i][1]),Point(self.st_db[street][i+1][0],self.st_db[street][i+1][1]))
                    self.line_dict.setdefault(street,[]).append(l)
                else:
                    break
        return self.line_dict
    def vertices_form(self):
        index=0
        for street in self.line_dict:
            i=0
            street_a=street
            while i<len(self.line_dict[street_a]):
                l1=self.line_dict[street_a][i]
                for street1 in self.line_dict:
                    if street1 == street_a:
                        continue
                    street_b=street1
                    j=0
                    while j<len(self.line_dict[street1]):
                        l2=self.line_dict[street1][j]
                        #print l1
                        #print l2
                        if not parallel(l1,l2):
                            inter=intersect(l1,l2)
                            a=l1.src
                            b=l1.dst
                            c=inter
                            d=l2.src
                            e=l2.dst
                            temp1=[a,b,c,d,e]
                            if is_between(a,c,b) and is_between(d,c,e):
                                a=['{0:.2f}'.format(a.x),'{0:.2f}'.format(a.y)]
                                b=['{0:.2f}'.format(b.x),'{0:.2f}'.format(b.y)]
                                c=['{0:.2f}'.format(c.x),'{0:.2f}'.format(c.y)]
                                d=['{0:.2f}'.format(d.x),'{0:.2f}'.format(d.y)]
                                e=['{0:.2f}'.format(e.x),'{0:.2f}'.format(e.y)]
                                if not c in self.intersec:
                                    self.intersec.append(c)
                                temp=[c,a,b,d,e]
                                for t in range(len(temp)):
                                    flag1=0
                                    if not temp[t] in self.vertices.values():
                                        self.vertices[index]=temp[t]
                                        index=index+1
                        j=j+1
                i=i+1
        return self.vertices
    def edge_form(self):
        for vert in self.vertices:
            if self.vertices[vert] in self.intersec:
                intr=self.vertices[vert]
                inter_ind=vert
                for street in self.line_dict:
                    i=0
                    while i<len(self.line_dict[street]):
                        l1=self.line_dict[street][i]
                        a=l1.src
                        b=l1.dst
                        a1=['{0:.2f}'.format(a.x),'{0:.2f}'.format(a.y)]
                        b1=['{0:.2f}'.format(b.x),'{0:.2f}'.format(b.y)]
                        for vert in self.vertices:
                            if a1 == self.vertices[vert]:
                                src_ind=vert
                        for vert in self.vertices:
                            if b1 == self.vertices[vert]:
                                dst_ind=vert
                        c=Point(intr[0],intr[1])
                        if is_between(a,c,b):
                            self.list_edges.append([src_ind,inter_ind])
                            self.list_edges.append([dst_ind,inter_ind])
                        i=i+1
        return self.list_edges

    def edge_form1(self):

        for street in self.line_dict:
            i=0
            while i<len(self.line_dict[street]):
                points=[]
                l=self.line_dict[street][i]
                a=l.src
                b=l.dst
                a1=['{0:.2f}'.format(a.x),'{0:.2f}'.format(a.y)]
                b1=['{0:.2f}'.format(b.x),'{0:.2f}'.format(b.y)]
                for vert in self.vertices:
                    if b1 == self.vertices[vert]:
                        src_ind=vert
                for vert in self.vertices:
                    if a1 == self.vertices[vert]:
                        dst_ind=vert
                #print(street+"-"+"line: "+str(i)+"==== "+"("+str('{0:.2f}'.format(a.x))+","+str('{0:.2f}'.format(a.y))+") "+"("+str('{0:.2f}'.format(b.x))+","+str('{0:.2f}'.format(b.y))+") ")
                for inter in self.intersec:
                    c=Point(inter[0],inter[1])
                    if is_between(a,c,b):
                        points.append(c)
                    else:
                        continue
                sorted_points=[]
                # print(street+"-"+"line: "+str(i)+"==== "+"("+str('{0:.2f}'.format(a.x))+","+str('{0:.2f}'.format(a.y))+") "+"("+str('{0:.2f}'.format(b.x))+","+str('{0:.2f}'.format(b.y))+") ")
                if len(points)>0:
                    target=b
                    sorted_points=sorted(points, key=lambda e: distance(e, target))
                sort_str=""
                # for k in points:
                #     sort_str+="("+str(k)+") "
                # print(street+"-"+"line: "+str(i)+"==== "+ "("+str('{0:.2f}'.format(a.x))+","+str('{0:.2f}'.format(a.y))+")"+sort_str+"  ("+str('{0:.2f}'.format(b.x))+","+str('{0:.2f}'.format(b.y))+") ")

                sorted_index=[]
                for j in sorted_points:
                    j1=['{0:.2f}'.format(j.x),'{0:.2f}'.format(j.y)] 
                    for vert in self.vertices:
                        if j1 == self.vertices[vert]:
                            j1_ind=vert
                            sorted_index.append(j1_ind)

                if len(sorted_index)>0:
                    sorted_index.insert(0,src_ind)
                    sorted_index.append(dst_ind)
                    for f in range(len(sorted_index)-1):
                        temp=[sorted_index[f],sorted_index[f+1]]
                        #print(temp)
                        if not (sorted_index[f] == sorted_index[f+1]) and temp not in self.list_edges:
                            self.list_edges.append([sorted_index[f],sorted_index[f+1]])

                # print(street+"-"+"line: "+str(i)+"==== "+"("+str('{0:.2f}'.format(a.x))+","+str('{0:.2f}'.format(a.y))+") "+"("+str('{0:.2f}'.format(b.x))+","+str('{0:.2f}'.format(b.y))+") ")
                # sort_str=""
                # for k in sorted_index:
                #     sort_str+="("+str(k)+") "
                # print(street+"-"+"line: "+str(i)+"==== "+"("+str(src_ind)+") "+sort_str+"("+str(dst_ind)+")")
                i=i+1
        return self.list_edges





