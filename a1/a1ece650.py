import sys
import re
from custom import extract_coordinates,Point,Line,distance,is_between,intersect,input_parser,vertices,street_pool,street_features

def output():
    inp_dict=street_pool.street_info
    sorted(inp_dict)
    #print inp_dict
    if bool(inp_dict) is False:
        sys.stderr.write('Error:There is no enough streets to get vertices and edges.\n')
        print "V = {"
        print "}"
        print "E = {"
        print "}"
    else:
        st_ins=street_features(inp_dict)
        st_ins.line_form()
        V=st_ins.vertices_form()
        E=st_ins.edge_form1()
        print "V = {"
        for num in V:
            print str(num) + ": " + "(" + str(V[num][0]) + "," + str(V[num][1]) + ")"
        print "}"
        print "E = {"
        for num in E:
            print "<"+str(num[0])+","+str(num[1])+">"
        print "}"

def main():
    while True:
        try:
            inp=sys.stdin.readline()
        except EOFError:
            break

        if inp =='':
            break
        sys.stderr.write(inp) #DEBUG
        match=input_parser(inp)
        if inp == 'g\n':
            output()
            continue
        elif match:
            ops=match.group(1)
            #print ops
            street_name=match.group(2)
            street_name=street_name.lower()
            #print street_name
            try:
                coordinates=extract_coordinates(match.group(3))
            except:
                coordinates=None
        else:
            sys.stderr.write("Error:Invalid Input Format\n")
            continue
        street_instance=street_pool(ops,street_name,coordinates)
        #print street_instance.ops
        #print street_instance.street_name
        #print street_instance.coordinates
        if ops == 'a':
            street_instance.add()
            #print street_instance.street_info
        elif ops =='r':
            street_instance.remove()
            #print street_instance.street_info
        elif ops == 'c':
            street_instance.change()
            #print 'Debug:',street_instance.street_info
        else:
            sys.stderr.write('Error:invalid\n')
    #print 'Finished reading input'
    return sys.exit(0)

if __name__ == '__main__':
    main()
