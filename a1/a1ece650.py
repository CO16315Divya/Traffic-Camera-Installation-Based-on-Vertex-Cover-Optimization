#!/usr/bin/env python3
import sys
import re
import math
import numpy
from collections import defaultdict

# YOUR CODE GOES HERE
#regex
ver_dict=dict()
name_dict = defaultdict(list)
edges_dict=dict()
coord_arr=[]
name_regex='\"[a-zA-Z ]+?\"'
num_regex='-?\d+'
coord_regex='\('+num_regex+','+num_regex+'\)'
ver_name = 0
streets=[]
lines=[]
flag=0
ver_index=1
points=[]
intersection_points=[]
tuples=[]


class Point:
    def __init__(self,x,y,intersection=False,index=0):
        self.x=float(x)
        self.y=float(y)
        self.intersection=intersection
        self.index=index

    def __str__(self):
        return '('+str(self.x)+','+str(self.y)+')'

    def __repr__(self):
        return str(self)

class Line:
    def __init__(self,src,dest,sname):
        self.src=src
        self.dest=dest
        self.sname=sname

    def __str__(self):
        return str(self.src)+'-->'+str(self.dest)

class Street:    
    def __init__(self, line_arr, name):
        self.lines = line_arr
        self.name = name
       

    def __repr__(self):
        return str(self)

class Tuple:
    def __init__(self,p0,p1,p2):
        self.vA=Point(p0.x,p0.y,False)
        self.vB=Point(p1.x,p1.y,False)
        self.vC=Point(p2.x,p2.y,False)





class Graph:


    def findEdges(self,points, intersectiontuples, intersection_points):
        
        t_edges = []
        for tuples in intersectiontuples:
            p0 = (tuples.vA.x, tuples.vA.y)
            p1 = (tuples.vB.x, tuples.vB.y)
            p2 = (tuples.vC.x, tuples.vC.y)
            # point1 = Point(tuples.vA.x, tuples.vA.y)
            # point2 = Point(tuples.vB.x, tuples.vB.y)
            # point3 = Point(tuples.vC.x, tuples.vC.y)
            mpl = []
            newList = []
            
            for iPoint in intersection_points:
                
                iCoord = (iPoint.x, iPoint.y)
                if self.collinear(iCoord, p0, p2) and iCoord != p1:
                    mpl.append(iCoord)
            
            if mpl == []:
                mpl.append(p0)
                mpl.append(p2)
                mpl.append(p1)
                t_edges.append((p0, p1))
                t_edges.append((p1, p2))

            else:
                mpl.append(p0)
                mpl.append(p2)
                mpl.append(p1)
                mpl = list(set(mpl))

                if p2[0] - p0[0] == 0 or p2[1] - p0[1] == 0:
                    newList = sorted(mpl, key=lambda x: (float(x[0])), reverse=False)
                else:
                    newList = sorted(mpl, key=lambda x: (float(x[0])), reverse=False)
            for k in range(0, len(newList) - 1):
                t_edges.append((newList[k], newList[k + 1]))
        t_edges=list(set(t_edges))

        t_edges_new = []
        for edge in t_edges:
            x1,y1=edge[0]
            x2,y2=edge[1]
            if x1 == x2 and y1 == y2:
                continue
            else:
                t_edges_new.append(edge)    

        self.printEdges(t_edges_new,points)
        # getEdgeIndex(t_edges, vertices)



    def printEdges(self,edges,points):
        
        
        print("E = {")
          
        l=0
        for e in edges:
            x1,y1=e[0]
            x2,y2=e[1]
            
            for p in points:
                
                if x1==p.x and y1==p.y:
                    index1=p.index

            for p in points:

                if x2==p.x and y2==p.y:
                    index2=p.index
                
            l+=1
            if(l!=len(edges)):
                print("  <"+str(index1)+","+str(index2)+">,")
            else:
                print("  <"+str(index1)+","+str(index2)+">")

        print("}")


    def xdist(self,x, y, tolerence1=1e-09, tolerence2=0.0):
        return abs(x - y) <= max(tolerence1 * max(abs(x), abs(y)), tolerence2)
     
    
    def find_distance(self,x1, y1, x2, y2):
       
        dist = math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)
        return dist

    def intersect(self,l1,l2):
        
        
        try:
            x1,y1 = l1.src.x, l1.src.y
            x2,y2 = l1.dest.x, l1.dest.y
            x3,y3 = l2.src.x, l2.src.y
            x4,y4 = l2.dest.x, l2.dest.y

            x_num = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))
            x_den = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
            x_intersect = x_num / x_den

            y_num = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
            y_den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
            y_intersect = y_num / y_den

            
            #check distance
            a=self.find_distance(l1.src.x,l1.src.y,x_intersect,y_intersect)
            b=self.find_distance(x_intersect,y_intersect,l1.dest.x,l1.dest.y)
            c=self.find_distance(l1.src.x,l1.src.y,l1.dest.x,l1.dest.y)
            # print("a,b,c ",a,b,c)
            d=self.find_distance(l2.src.x,l2.src.y,x_intersect,y_intersect)
            e=self.find_distance(x_intersect,y_intersect,l2.dest.x,l2.dest.y)
            f=self.find_distance(l2.src.x,l2.src.y,l2.dest.x,l2.dest.y)


            if self.xdist(a+b,c) and self.xdist(d+e,f):
                
                return x_intersect,y_intersect
                
                
            else: 
                return None,None
            

        except Exception as exp:  
            return None, None

    def collinear(self,p0, p1, p2):
        
        x1, y1 = p1[0] - p0[0], p1[1] - p0[1]
        x2, y2 = p2[0] - p0[0], p2[1] - p0[1]
        
        return abs(x1 * y2 - x2 * y1) < 1e-12



    def check_intersections(self):
        

        intersection_points=[]
        points=[]
        tuples=[]
        global ver_index  
       
        
        #checking intersections for each line in the street
        for i1 in range(0, len(streets)):
            for i2 in range(i1 + 1, len(streets)):
                for line1 in streets[i1].lines:
                    for line2 in streets[i2].lines:
                        
                        omit=False
                        yes=False
                        xin, yin =self.intersect(line1, line2)
                        if xin is not None:
                            x1,y1 = line1.src.x, line1.src.y
                            x2,y2 = line1.dest.x, line1.dest.y
                            x3,y3 = line2.src.x, line2.src.y
                            x4,y4 = line2.dest.x, line2.dest.y


                            if(line1.sname!=line2.sname):

                                for p in intersection_points:               #adding intersection points to array
                                    
                                    # print("p.x xin p.y yin",p.x,xin,p.y,yin)
                                    if p.x == xin and p.y == yin:
                                        
                                        omit=True

                                    else:
                                        pass
                                           
                                if(omit== False):
                                    intersection_points.append(Point(xin, yin,True))
                                    
                                
                           
                                # #adding to points array
                                
                                yes=False
                                for p in points:               
                                    if p.x == xin and p.y == yin:
                                        yes=True
                                    else:
                                        pass
                                            
                                if(yes== False):
                                    points.append(Point(xin, yin,True,ver_index))
                                    ver_index+=1
                                    # print("2nd--",intersection_points)
                                # print("final &last",points)




                                xpoints = [x1, x2, x3, x4]
                                ypoints = [y1, y2, y3, y4]

                                for xd,yd in zip(xpoints, ypoints):
                                    flag2 = False
                                    for i in points:
                                        if i.x == xd and i.y == yd:
                                            flag2 = True
                                            break
                                    if flag2 == False:        
                                        points.append(Point(xd, yd,True,ver_index))
                                        ver_index+=1




                                #adding two intersecting lines tuple
                                tuples.append(Tuple(Point(x1, y1,False), Point(xin, yin,True), Point(x2, y2,False)))
                                tuples.append(Tuple(Point(x3, y3,False), Point(xin, yin,True), Point(x4, y4,False)))
            
        #print vertices


        print("V = {") 
        if(len(points)==2):
            print("}")
            print("E = {")
            print("}")
        else:

            for i in points:
                lf="{0:.2f}".format(i.x)
                lfy="{0:.2f}".format(i.y)
                print("  "+str(i.index) + ": " +str(Point(lf,lfy)))
            print("}")
            self.findEdges(points, tuples, intersection_points)






    def print_graph(self):

        if len(streets)==[]:
            displayError("No streets available to show!")
            return
        else:
            self.check_intersections()

    def print_lines(self):
            if lines==[]:
                print("No lines available")
            else:
                
                for xt in lines:
                    print("LINES-",xt.src.x,xt.src.y,xt.dest.x,xt.dest.y)
        

    def print_streets(self):
        if streets==[]:
            print("No streets available")
        else:
            
            for i in streets:
                print(i.name)
                for xt in i.lines:
                    print("Streets-",xt.src.x,xt.src.y,xt.dest.x,xt.dest.y)
                    


    def mod_street(self,input_str):
        
        self.remove_street(input_str)
        
        if flag==1:
            
            self.add_street(input_str)
        else:
            displayError("No such street exists")
          
        

    def remove_street(self,input_str):
        global flag
        flag=0
        input_str=input_str.strip()
        street_name=re.search(name_regex,input_str).group()
        
        if streets==[]:
            displayError("There is no street available to modify")
            return
        else:
            
            for index,i in enumerate(streets):
                # print("ll", i.name)
                if(i.name.lower()==street_name.lower()):
                    # print("matched in remove function")
                    del streets[index]
                    flag=1
                    break
         
            if flag == 1:
                k=len(lines)
                while k>0:
                    if(lines[k-1].sname.lower()==street_name.lower()):
                        x1,y1=lines[k-1].src.x,lines[k-1].src.y
                        x2,y2=lines[k-1].dest.x,lines[k-1].dest.y
                        del lines[k-1]                           
                        
                    k=k-1
                    
            


    def add_street(self,input_str):
        # tokenss=list(input_str)
        # tokenss=re.split(',|"|(|)| |!', input_str)
        # print("TOKENS: ",tokenss)


        temp_points=[]
        global ver_name
        global lines
        
        coord_base=re.findall(coord_regex,input_str)
        street_name=re.search(name_regex,input_str).group()
        
        

        for i in coord_base:
            
            c=re.split('\(|,|\)', i)
            
            x_cord=c[1]
            y_cord=c[2]
            
            # print("x,y are-",x_cord,y_cord)

            point_obj=Point(x_cord,y_cord,False)
            temp_points.append(point_obj)
            
            #adding the values to dictionary
            #check if street name already exists
            for str_name in streets:
                if(str_name.name==street_name):
                    displayError("Street already exists.")
                    return

            
        #adding lines to array
        
        
        l = len(temp_points)
        for i, p in enumerate(temp_points):
            if i < (l - 1):
                lines.append(Line(temp_points[i], temp_points[i + 1],street_name))

        # self.print_lines()

        #adding streets info
        streets.append(Street(lines, street_name))

        
     
        
class validity:

    

    def add_rx(self,input_str):
        ipt_cmd='add\s'+name_regex+'\s('+coord_regex+'\s){1,}'+coord_regex+'$'
        if(re.match(ipt_cmd,input_str)):
            Graph().add_street(input_str)
        else:
            displayError("Not a Match(Incorrect format)")

    def mod_rx(self, input_str):
        ipt_cmd='mod\s'+name_regex+'\s('+coord_regex+'\s){1,}'+coord_regex+'$'
        if(re.match(ipt_cmd,input_str)):
            Graph().mod_street(input_str)
        else:
            displayError("Not a Match(Incorrect format)")

    def rm_rx(self,input_str):
        ipt_cmd='rm\s'+name_regex+'$'
        if(re.match(ipt_cmd,input_str)):
            Graph().remove_street(input_str)
            if(flag==0):
                displayError("No such street")
        else:
            displayError("Not a Match(Incorrect format)")

    def gg_rx(self,input_str):
        
        if input_str.strip()=='gg':
            Graph().print_graph()
        else:
            displayError("Not a Match(Incorrect format)")



def displayError(error):
    print("Error: {}".format(error))


def main():
    # YOUR MAIN CODE GOES HERE
    

    # sample code to read from stdin.
    # make sure to remove all spurious print statements as required
    # by the assignment
    try:
        while True:
            line = sys.stdin.readline()
            line=line.rstrip()
            
            if line == '':
                break

            else:
                tokens=line.split()
                # print("Tokens :",tokens)
                if('add'==tokens[0].lower()):
                    validity().add_rx(line)                    
                elif('mod'==tokens[0].lower()):
                    validity().mod_rx(line)
                elif('rm'==tokens[0].lower()):
                    validity().rm_rx(line)
                elif('gg'==tokens[0].lower()):
                    validity().gg_rx(line)
                    
                else:
                    displayError("Command error !")
                    

            

    except KeyboardInterrupt:
        sys.exit(0)


    except(BrokenPipeError, IOError):
        # sys.stdout.write("THIS IS A BROKEN PIPE")
        # sys.stdout.flush()
        sys.exit(0)
        pass
    

   
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == "__main__":
    main()



