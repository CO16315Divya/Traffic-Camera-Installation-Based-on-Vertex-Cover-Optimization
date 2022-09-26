#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <sstream>
#include <ctime>
#include <unistd.h>

using namespace std;

class Point {
public:
    int x;
    int y;

    Point() {
        x = 0;
        y = 0;
    }

    Point(int x , int y) {
        this->x = x;
        this->y = y;
    }
};

class Line {
public:
    std::vector<Point> points;
    void addPoint(Point p) {
        points.push_back(p);
    }
};

class Street {
public:
    std::vector<Line> lines;

    void addLine(Line line) {
        lines.push_back(line);
    }
};

class Graph{
public:
    std::vector<Street> streets;

    void addStreet(Street street){
        streets.push_back(street);
    }

};


template<class T>
T get_randomvalue() {
    T random_value; //Declare value to store data into
    size_t size = sizeof(random_value); //Declare size of data
    std::ifstream urandom("/dev/urandom", std::ios::in|std::ios::binary); //Open stream
    if(urandom) //Check if stream is open
    {
        urandom.read(reinterpret_cast<char*>(&random_value), size); //Read from urandom
        if(!urandom) //Read Failed
        {
            std::cerr << "Error: Failed to read from /dev/urandom" << std::endl;
            exit(2);
        }
        urandom.close(); //close stream
    }
    else //Open failed
    {
        std::cerr << "Error: Failed to open /dev/urandom" << std::endl;
        exit(2);
    }
    return random_value;
}

int random_int(int min, int max) {
    int range = max-min;
    int i = get_randomvalue<int>();
    int result = (i % (range+1) + (range+1) ) % (range+1) + min;
    //return min + rand() % range;
    return result;
}
class Random_gen{
public:
    int max_s=10;
    int max_n=5;
    int max_l=5;
    int max_c=20;


    Street street_obj;
    vector<string> list_of_streets;
    Graph graph_obj;
    void gen_streets();
    void remove_streets();

    //constructor
    Random_gen(int max_s,int max_n,int max_l,int max_c){
        this->max_s=max_s;
        this->max_l=max_l;
        this->max_n=max_n;
        this->max_c=max_c;


    }


};

void print_streets(Graph obj, std::string cmd, int no_of_streets, vector<string> street_names){
    string output="";
    /*
    cout << "PrintStreets\n";
    cout << "numofstreets " << no_of_streets << endl;
    cout << "streetnames\n";
    for (string x : street_names) cout << x << " ";
    cout << endl;
*/
    for(int i=0;i<street_names.size();i++){
        output="";
        output=output+ cmd+" \""+ street_names[i]+"\" ";

        if(cmd=="add"){
            for(int j =0;j < obj.streets[i].lines.size();j++){
                for(int k =0;k < obj.streets[i].lines[j].points.size(); k++){
                    output=output+ "("+ to_string(obj.streets[i].lines[j].points[k].x)+","+to_string(obj.streets[i].lines[j].points[k].y)+")"+" ";
                }
            }
            //output+="\n";
            cout<<output<<endl;
        }

        
    }
    if(cmd=="add"){
        cout<<"gg"<<endl;
    }
        for(int i=0;i<street_names.size();i++){
        output="";    

        output=output+ cmd+" \""+ street_names[i]+"\" ";

        if(cmd=="rm"){
            //output+="\n";
            cout<<output<<endl;
        }

        
    }
    //else cout<<output<<endl;
    // std::cout<<output;

    // cout << "finishofprintstreets\n";

}

int num_streets;
void printError(string msg);
bool check_validity(Street obj);
void print_streets(Street obj, std::string cmd, int no_of_streets, vector<string> street_names);
bool check_overlapping(Graph obj);
void Random_gen::gen_streets(){

    
    // generating street name

    string streetFirstName[14] = {"Weber ","King ","WildGinger ","Columbia ","Albert ","Devenport ","Philip ","Erb ","Victoria ","Lester ","Iron ", "Union ", "Yelp ","Bridgeport "};

    string streetLastName[4] = {"Road ","Blvd ","Street ","Ave "};

    string streetDirection[4] = {"E","W","S","N"};

    num_streets = random_int(2,max_s);
    // cout<<"NUM OF STREETS= "<<num_streets<<endl;
    for(int i=0;i<num_streets;i++){

        bool same = false;
        int index1= random_int(0,13);
        int index2= random_int(0,3);
        int index3= random_int(0,3);

        string street = streetFirstName[index1] + streetLastName[index2] + streetDirection[index3];

        for(int j=0;j<list_of_streets.size();j++){
            if(street== list_of_streets[j]){
                same = true;
            }

        }
        //checking if the street name already exists
        if (same==true){
            i--;
        }
        else{
            list_of_streets.push_back(street);
        }


    }

    int flag=0;
    int trial_streets=0;
    Line line_obj;
    Street street_obj;
    int failed_points=0;
    vector<Point> points_store;
    //generating coordinates

    do{
        
        graph_obj.streets.clear();
        street_obj.lines.clear();
        line_obj.points.clear();

        if(trial_streets > 25){
                    printError("failed to generate valid input for 25 simultaneous attempts");
                    exit(0);
        }
        for(int i=0;i<num_streets;i++){
            street_obj.lines.clear();
            line_obj.points.clear();
            long long int num_lines=random_int(1,max_n);
            //bool samepoints=false;
            // cout<<"num of lines in a street"<<num_lines<<endl;

            for(int j=0;j<num_lines+1;j++)
            {
                
                bool samepoints=false;
                // line_obj.points.clear();

                //cout<<samepoints<<" for";
                //generating random coordinates
                int x_point=random_int(-max_c, max_c);
                int y_point=random_int(-max_c, max_c);
                
                for(int k=0;k<line_obj.points.size();k++)
                {
                    
                    //checking for self loops
                    
                    if(line_obj.points[k].x==x_point &&  line_obj.points[k].y==y_point){
                        samepoints=true;
                    }
                }

                if(samepoints==true) {
                    failed_points++;
                    if(failed_points>25){
                        printError("No success generating coordinates. Try changing values.");
                        exit(0);
                    }
                    j--;
                }
                else{
                    
                    Point p(x_point,y_point);
                    line_obj.addPoint(p);
                    points_store.push_back(p);

                    // for(int k=0;k<line_obj.points.size();k++){
                    //     cout<<"X="<<line_obj.points[k].x<<"Y="<<line_obj.points[k].y<<endl;
                    // }
                    

                }
                //street_obj.addLine(line_obj);
            }
                            street_obj.addLine(line_obj);
            graph_obj.addStreet(street_obj);
        }

        //graph_obj.addStreet(street_obj);
        trial_streets += 1;

    } while(check_overlapping(graph_obj));

    // cout << "beforeprint\n";
    print_streets(graph_obj,"add",num_streets,list_of_streets);
    // cout << "outofgen\n";
}


//github
bool isInBetween(Point p1,Point p2,float xcoor,float ycoor) {
    float distance1 = sqrt(pow((p1.x - xcoor),2) + pow((p1.y - ycoor),2));
    float distance2 = sqrt(pow((xcoor - p2.x), 2) + pow((ycoor - p2.y),2));
    float distance3 = sqrt(pow((p1.x - p2.x),2) + pow((p1.y - p2.y),2));
    float a = distance1 + distance2;
    float b = distance3;
    double rel_tol=1e-09;
    double abs_tol=0.0;
    if (abs(a - b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)) {
    //if (abs(a-b) <= rel_tol) {
        // cout << "returning true from isinbetween\n";
        return true;
    }
    return false;
    //return abs(a - b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol);
}
bool collinear(Point p0,Point p1,Point p2){
    float x1 = p1.x - p0.x;
    float y1 = p1.y - p0.y;
    float x2 = p2.x- p0.x;
    float y2 = p2.y - p0.y;

    return abs(x1 * y2 - x2 * y1) < 1e-12;
}
bool intersect(Point p1,Point p2,Point p3,Point p4) {

    int x1 = p1.x , y1 = p1.y;
    int x2 = p2.x , y2 = p2.y;
    int x3 = p3.x , y3 = p3.y;
    int x4 = p4.x , y4 = p4.y;

    float xnum = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4));
    float xden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
    float xcoor = xnum / xden;

    float ynum = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
    float yden = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    float ycoor = ynum / yden;

    if ((xcoor == x1 && ycoor == y1) || (xcoor == x2 && ycoor == y2) || (xcoor == x3 && ycoor == y3) || (xcoor == x4 && ycoor == y4)) {
        return false;
    }

    if (isInBetween(p1,p2,xcoor,ycoor) && isInBetween(p3,p4,xcoor,ycoor)) {
        //cout << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << endl;
        //cout << xcoor << " " << ycoor << endl;
        //cout << p3.x << " " << p3.y << " " << p4.x << " " << p4.y << endl;
        // cout << "returning true from intersect\n";
        return true;
    } else {
        return false;
    }
}

bool check_intersection(Street routeObject) {

    for (int i = 0 ; i<routeObject.lines.size();i++) {
        for(int j = i+1;j<routeObject.lines.size();j++) {
            for(int k = 0;k<routeObject.lines[i].points.size()-1;k++) {
                for(int l = 0;l<routeObject.lines[j].points.size()-1;l++) {
                    if(intersect(routeObject.lines[i].points[k], routeObject.lines[i].points[k+1], routeObject.lines[j].points[l], routeObject.lines[j].points[l+1])) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}



bool check_overlapping(Graph obj){

    for (int i = 0 ; i < obj.streets.size()-1; i++) {
        for(int j = i+1; j < obj.streets.size(); j++) {

            for(int m=0; m < obj.streets[i].lines.size(); m++) {
                for(int n=0; n < obj.streets[j].lines.size(); n++) {

                    for(int k = 0; k < obj.streets[i].lines[m].points.size() - 1; k++) {
                        for(int l = 0; l < obj.streets[j].lines[n].points.size() - 1; l++) {

                            Point p1=obj.streets[i].lines[m].points[k];
                            Point p2=obj.streets[i].lines[m].points[k+1];
                            Point p3=obj.streets[j].lines[n].points[l];
                            Point p4=obj.streets[j].lines[n].points[l+1];

                            if(collinear(p1, p2, p3) && collinear(p1, p2, p4) && (isInBetween(p1, p2, p3.x, p3.y) || isInBetween(p1, p2, p4.x, p4.y) || isInBetween(p3, p4, p1.x, p1.y) || isInBetween(p3, p4, p2.x, p2.y))) {
                                // cout << "collinear\n";
                                return true;
                            }
                        }

                    }

                }
            }

        }
    }


    for (int i = 0 ; i < obj.streets.size(); i++) {
        if (check_intersection(obj.streets[i])) {
            // cout << "overlap within street "<< i <<"\n";
            return true;
        }
    }
    // cout << "returning false since no collinear and no overlap\n";
    return false;
}
void printError(string message){

    std::cerr<< "Error: "<<message<<endl;
}

//remove commands
void Random_gen::remove_streets(){

    print_streets(graph_obj,"rm",num_streets,list_of_streets);


}

int main (int argc, char **argv) {
    std::string s_value;
    int max_s = 10; // [2,k] k >=2
    std::string n_value;
    int max_n = 5; // [1,k] k >= 1
    std::string l_value;
    int max_l = 5; //[5,k] k>=5
    std::string c_value;
    int max_c = 20; //x[-k,k] k>=1
    int c;

    opterr = 0;

    // expected options are '-s value', '-n value', '-l value', and '-c value'
    while ((c = getopt (argc, argv, "s:n:l:c:?")) != -1)
        switch (c)
        {
        case 's':
            s_value = optarg;
            max_s = std::atoi(s_value.c_str());
            if(max_s < 2) {
                printError("Option -s must have value >= 2");
                exit(1);
            }
            break;

        case 'n':
            n_value = optarg;
            max_n = atoi(n_value.c_str());
            if(max_n < 1) {
                printError("Option -n must have value >= 1");
                exit(1);
            }
            break;

        case 'l':
            l_value = optarg;
            max_l = atoi(l_value.c_str());
            if(max_l< 5) {
                printError("Option -l must have value >= 5");
                exit(1);
            }
            break;

        case 'c':
            c_value = optarg;
            max_c = atoi(c_value.c_str());
            if(max_c < 1) {
                printError("Option -c must have value >= 1");
                exit(1);
            }
            break;

        case '?':
            printError("Invalid option: " + to_string(optopt));
            exit(1);
        default:
            return 0;
        }

    while(true) {
        Random_gen graph_obj = Random_gen(max_s, max_n,max_l, max_c);
        graph_obj.gen_streets();
        // cout<<"after gen streets";
        sleep(random_int(5,max_l));
        // sleep(150);
        // cout<<"before remove";
        graph_obj.remove_streets();
        
    }
    return 0;
}