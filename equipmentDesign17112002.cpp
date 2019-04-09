#include <bits/stdc++.h>

using namespace std;

int check(int type, int min , int max){         //to check the entered input
    while(type < min || type>max){
        cout << "Input invalid , Enter it correctly ! ";
        cin >> type;
    }

    return type;
}

double calculatePd(double oPrr, double lHead){      //to calculate design pressure
    if (lHead >= 0.05*oPrr){
        return (oPrr + lHead);
    }else return (oPrr*1.05);
}

double calculateTd(double oT, int oC){              //to calculate design temperature
    double T;
    switch (oC){
        case 1:
        T= (oT);
        break;
        
        case 2:
        T= (oT + 20.0);
        break;
        
        case 3:
        T= (oT + 20.0);
        break;
        
        case 4:
        T= (oT + 50.0);
        break;
        
        default:
        T= (oT + 50.0);
        break;
    }
    
    if(T<250){
        T=250;
    }
    
    return T;
}

double stdCalculator(double t){      //to calculate the standard thickness available in the market
    vector<double> std = {5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 14.0, 16.0, 18.0, 20.0, 22.0,
                            25.0, 28.0, 32.0, 36.0, 40.0, 45.0, 50.0, 56.0, 63.0, 71.0, 80.0};
    
    double tStd;
    vector<double>::iterator low;
    low = lower_bound(std.begin(), std.end(), t);
    int pos= low - std.begin();
    if(pos < std.size()){
        tStd = std[pos];
    }else tStd = ceil(t);
    
    return tStd;
}

double shellThickness(double Do, double Di, double Pd, double F, double J, double Ca){
    double tShell;
    if(Do != 0){
        tShell = Pd*(pow(10,5))*Do/(2*F*(pow(10,8))*J - Pd*(pow(10,5)))*1000;   
    }else if(Di != 0){
        tShell = Pd*(pow(10,5))*Di/(2*F*(pow(10,8))*J + Pd*(pow(10,5)))*1000;   
    }
    cout <<"\n";
    cout << "The Theoretical thickness of the Shell is              : " << tShell << " mm" << endl;
    tShell+=Ca;
    cout << "Theoretical Shell thickness with Corrosion Allowance   : "<< tShell << " mm" << endl;
    
    
    double tShellStd = stdCalculator(tShell);
    

    cout << "The Standard Shell Thickness is                        : " << tShellStd << " mm" << endl;  

    return tShellStd;                      
}

double min(vector<double> a){
    double mini = a[0]; 
    for(int i=1;i<(a.size());i++){
        if(a[i]<mini){
            mini = a[i];
        }
    } 
    
    return mini;
}

double calculateC(double x, double y){  //function to calculate C from the table
    
    vector<vector<double> > cT{     
        {0.00,0.002,0.005,0.01,0.02,0.04},
        {0.15,4.55,2.66,2.15,1.95,1.75},
        {0.20,2.30,1.70,1.45,1.37,1.32},
        {0.25,1.38,1.14,1.00,1.00,1.00},
        {0.30,0.92,0.77,0.77,0.77,0.77},
        {0.40,0.59,0.59,0.59,0.59,0.59},
        {0.50,0.55,0.55,0.55,0.55,0.55}
    };                                  
    
    int i=0;double c;                       //double interpolation function
    while(i < cT[i].size()){
        if(x>cT[0][i] && x<=cT[0][i+1]){
            int j=0;
            while(j<cT.size()){
                if(y>cT[j][0] && y<=cT[j+1][0]){
                    double y1,y2;
                    y1 = cT[j][i+1] + (cT[j][i+1] - cT[j][i])*(x - cT[0][i+1])/(cT[0][i+1] - cT[0][i]);
                    y2 = cT[j+1][i+1] + (cT[j][i+1] - cT[j][i])*(x - cT[0][i+1])/(cT[0][i+1] - cT[0][i]);
                    c = y2 + (y1 - y2)*(y - cT[j+1][0])/(cT[j][0] - cT[j+1][0]);
                    break;

                }j++;
            }
        }i++;
    }
    

    return c;
    
}

double dishedHead(double tShellStd, double Do, double Di, double Pd, double F,double J){
    
    double t=tShellStd;   //assuming t=tShellStd;
    double tN=0;double diff;
    
    do{
        if(tN != 0) t = (t+tN)/2 ;
        
        if(Do==0){
            Do = Di + 2*t/1000;
        }else if(Di==0){
            Di = Do - 2*t/1000;
        }
        double Ri,Ro,ri,ro;
        Ri = 0.70*Di;      //assuming Ri to be 70% of Di 
        Ro = Ri + t/1000;
        ri = 0.06*Di;       //ri is 6-10% of Di
        ro = ri + t/1000;
        double ho;
        double temp = pow((Ro - Do/2)*(Ro + Do/2 -2*ro),0.5);
        //cout<<temp<<endl;
        ho = (Ro - temp);
        //cout<<"ho="<<ho<<endl;
        
        vector<double> he;
        he.push_back(ho);
        //cout << "he[0]="<<he[0]<<endl;
        he.push_back((pow(Do,2))/(4*Ro));
        //cout << "he[1]="<<he[1]<<endl;
        he.push_back(pow(Do*ro/2,0.5));
        //cout << "he[2]="<<he[2]<<endl;
        
        double hE = min(he);
        //cout<<hE;
        
        double y = hE/Do;
        
        double x = t/(1000*Do);
        
        //cout<<"x="<< x <<endl;
        //cout<<"y="<< y <<endl;
        
        double C = calculateC(x,y);
        
        tN = (Pd*100000*Do*C/(2*F*(pow(10,8))*J))*1000;        //in mm
        
        diff = (tN>t ? (tN-t) : (t-tN));
    }while(diff>0.1);


    double tDished = (tN+t)/2;
    return tDished;
}

double flatHead(double Pd, double J, double F, double Di, double Do, int ch){

    
    double C,De;
    switch(ch){
        case 1 :
            De=Di;
            C=0.45;
            break;
        case 2 :
            De=Di;
            C=0.55;
            break;
        case 3 :
            De=Di;
            C=0.7;
            break;
        case 4 :
            De=Di;
            C=0.55;
            break;
    }
                     
    double t=C*De*pow((Pd*100000/(F*100000000)),0.5)*1000;
    
    return t;    
}

//conicalHead(Pd,  J, F,  Di, Do, Ca , tShellStd-Ca);
void conicalHead(double Pd, double J, double F, double Di, double Do, double Ca , double tstdca){
    double tj,Z;
    int a;
    cout<<"Please enter the half Apex angle (20/30/45/60 , in degrees) : ";
    cin >> a;
    switch(a){
        case 20:
        Z=1.00;
        break;
        case 30:
        Z=1.35;
        break;
        case 45:
        Z=2.05;
        break;
        case 60:
        Z=3.2;
        break;
    }
    
    tj = (Pd*100000*Do*Z/(2*F*100000000*J))*1000;
    double taj;
    double Dk;
    double s=0.5*pow(Do*tstdca*1000/cos(a),0.5);
    Dk=Di-2*s*sin(a);
    taj=(Pd*100000*Dk/(2*F*J*100000000-Pd*100000))*(1/cos(a))*1000;
    
    if((tj-taj)<1){
      cout<<"\nThe thickness of fabrication is    : "<<tj<<" mm\n";
      cout<<"The thickness after CA             : "<<tj+Ca<<" mm\n";
      cout<<"The Standard Thickness is          : "<<stdCalculator(tj+Ca)<<" mm\n";
    }
    else{
        if(stdCalculator(tj+Ca)-stdCalculator(taj+Ca) <1){
            cout<<"\nThe thickness of fabrication is  : "<<tj<<" mm\n";
            cout<<"The thickness after CA           : "<<tj+Ca<<" mm\n";
            cout<<"The Standard Thickness is        : "<<stdCalculator(tj+Ca)<<" mm\n";
        }else{
          cout<<"\nAt the junction till "<<s<<"mm from the junction "<<stdCalculator(tj+Ca)<<" mm (standard) of thickness is used"<<endl;
          cout<<"After "<<s<<" mm The thickness of "<<stdCalculator(taj+Ca)<<" mm(standard) is used"<<endl;
        }
    }   
}

void Compensation(double PD, double J , double f , double DIn, double DOut ,double ca ,double tstd){
    
    cout<<"\n-----COMPENSATION---------------\n"<<endl;
        cout<< " Do you want to drill a hole in the shell ?  (y/n)  : ";
    char ch;
    cin >> ch;
    if (ch=='y'){
        double a,A,d,tr,trd,As,AI,AO,AD,H1,H2,tn,Ar,tp;
        cout<< " Enter the nozzle wall thickness (m)                : ";
        cin>>tn;
        cout<< " Enter inside diameter (m)                          : ";
        cin>>d;
        tr=PD*100000*DOut/(2*f*100000000*J+PD*100000);
        cout<< " Shell Thickness for further calculation            : " <<tr<<" m"<<endl;
        A=(d + 2*ca/1000 )*tr;
        cout<< " The area to be compensated is (m^2)                : "<<A<<endl;
        As=(d + 2*ca/1000)*((tstd/1000)-tr-(ca/1000));
        cout<< " The reinforcement area is (m^2)                    : "<<As<<endl;
        cout<< " Enter the nozzel Outside Height (m)                : ";
        cin>>H1;
        if(H1>pow((d+2*ca/1000)*(tn-ca/1000),0.5))
            H1=pow((d+2*ca/1000)*(tn-ca/1000),0.5);
        cout<< " Enter the nozzel Inside Height  (m)                : ";
        cin>>H2;
        if(H2>pow((d+2*ca/1000)*(tn-2*ca/1000),0.5))
            H2=pow((d+2*ca/1000)*(tn-2*ca/1000),0.5);
        trd=PD*1000*d/(2*f*J*100000000+PD*1000);
        AO=2*H1*(tn-trd-ca/1000);
        cout<< " The area of external nozzle (m^2)                  : "<<AO<<endl;
        AI=2*H2*(tn-2*ca/1000);
        cout<< " The area on internal nozzle (m^2)                  : "<<AI<<endl;
        AD=AI+AO+As;
        if(AD>A)
        cout<< " No external Compensation is required "<<endl;
        else{
        Ar=A-AD;
        cout<< " The area to be compensated is (m^2), ring-pad area : "<<Ar<<endl;
        tp=Ar/(2*(d+2*ca/1000)-(d+2*ca/1000+2*trd));
        cout<< " The Ring pad of thickness "<<tp<<" m is used for compensation"<<endl;
        }
    }
       else
       cout<<" No Compensation is required";
}





int main(){
    
    string name;
    cout << "-_-_-_-_-_-_-_-_-_-_-_-   EQUIPMENT DESIGN (Submitted by Abdulahad Khan, 17112002)   -_-_-_-_-_-_-_-_-_-_-_-\n\n\n";
    cout << "What is your name? ";
    getline (cin, name);
    cout << "\nWelcome, " << name << "!\n";
    cout << "Let's design an unfired pressure vessel.\n\n";
  
    double opTemp;
    int opCase;
    cout << "What's the operating temperature (in K) ? ";
    cin  >> opTemp;
    cout << "Choose a case for your operating condition:(mention the case number)\n";
    cout << "Case 1: Unheated parts.\n";
    cout << "Case 2: Heated by means of steam, hot water, or similar media.\n";
    cout << "Case 3: Direct heating by fire, flue gas or electricity but SHIELDED vessel.\n";
    cout << "Case 4: Direct heating by fire, flue gas or electricity but UNSHIELDED vessel.\n";
    cin  >> opCase;
    opCase = check(opCase , 1 ,4);
  
    double opPrr, liqHead=0 ;
    char head;
    cout << "What's the operating pressure (in bar) ? ";
    cin  >> opPrr;
    
    start:
        cout << "Is there any liquid Head ? Answer should be in one character(y/n) ! ";
        cin  >> head;
    
    if(head == 'y' || head=='Y'){
        cout << "What is the liquid head ? (in bar) ";
        cin >> liqHead;
    }else if(head=='n' || head=='N'){
        liqHead=0;
    }else {
        cout << "Above Question not answered correctly! Reanswer correctly.\n";
        goto start;
    }
    
    double Pd, Td;
    Pd = calculatePd(opPrr,liqHead);
    Td = calculateTd(opTemp,opCase);
    cout << "\nDESIGN TEMPERATURE is : " << Td << " K and DESIGN PRESSURE is : " << Pd <<" bar "<< endl << endl;
    
    cout << "For the above Temperature, what is the maximum allowable stress ? (in multiple of 10^8, from pp:261-265, BCB) ";
    double F;
    cin >> F;

    cout << "\nWhich class of vessel you want ? (class 1,2 or 3): ";
    int classType;
    cin >> classType;

    classType = check(classType , 1,3);
    double J;

    switch(classType){
        case 1:
            cout << "1. J=1.0, for fully radiographed joints.\n";
            cout << "2. J=0.9, for single welded butt joints with backing strips.\n";
            int class1;
            cin >> class1;
            class1 = check(class1 , 1,2);
            if(class1 == 1) J = 1.0;
            else J = 0.9;
            break;

        case 2:
            cout << "1. J=0.85, for double welded butt joints with full penetration.\n";
            cout << "2. J=0.80, for single welded butt joints with backing strips.\n";
            int class2;
            cin >> class2;
            class2 = check(class2 , 1,2);
            if(class2 == 1) J = 0.85;
            else J = 0.80;
            break;    

        case 3:
            cout << "1. J=0.70, for double welded butt joints with full penetartion.\n";
            cout << "2. J=0.65, for single welded butt joints with backing strip.\n";
            cout << "3. J=0.60, for single welded butt joints with backing strip which may not remain in place.\n";
            cout << "4. J=0.55, for single welded butt joints without backing strip\n";
            cout << "5. J=0.50, for single full lap joints for circumferential seams only.\n";
            int class3;
            cin >> class3;
            class3 = check(class3 , 1, 5);
            if(class3 == 1) J = 0.70;
            else if(class3 == 2) J = 0.65; 
            else if(class3 == 3) J = 0.60;
            else if(class3 == 4) J = 0.55;
            else J = 0.50;
            break;
    }

    dia:
    cout << "Which one do you know ? (1. Do (Outer diameter) or 2. Di (Internal diameter)) ";
    double Do=0, Di=0;
    int diaCase;
    cin  >> diaCase;

    switch(diaCase){
        case 1:
        cout << "what is the outer diameter (in m) ? ";
        cin  >> Do;
        break;
        
        case 2:
        cout << "what is the inner diameter (in m) ? ";
        cin  >> Di;
        break;
        
        default:
        cout << "Answer above question correctly ! \n";
        goto dia;
    }

    cout << "Enter the corrosion Allowance : (in mm) ";
    double Ca; cin >> Ca;
    
    double tShellStd = shellThickness(Do,Di,Pd,F,J,Ca);    //shell thickness calculator
    if(Do == 0){
        Do = Di + 2*(tShellStd-Ca)/1000;
    }else if(Di == 0){
        Di = Do - 2*(tShellStd-Ca)/1000;
    }
    
    cout <<"\n---------------------Shell Designed Successfully------------------------------------------\n\n";
    
    cout << "Which Head do you wanna design for this shell ? \n";
    cout << "1. Dished Head \n";
    cout << "2. Flat Head \n";
    cout << "3. Conical Head\n";
    cout << "4. All three\n";
    int headType;
    cin >> headType;
    headType = check(headType , 1, 4);

    switch (headType)
    {
        case 1:
            cout << "Assuming Ri = 70% of Di and ri = 6% of Di\n";
            cout << "Theoretical Thickness of dishedHead            : " <<dishedHead(tShellStd, Do, Di, Pd, F, J) <<" mm"<<endl;
            cout << "Dished Head Thickness with corrosion Allowance : "<< dishedHead(tShellStd, Do, Di, Pd, F, J) + Ca<<" mm"<<endl;
            cout << "Standard Thickness of dishedHead               : " <<stdCalculator(dishedHead(tShellStd, Do, Di, Pd, F, J) + Ca) <<" mm"<< endl;
            break;
        case 2:
            cout<<"Please specify method of attachment to shell"<<endl;
            cout << "1. Flanged flat heads butt welded to shell \n"<<
            "2. Plates welded to the inside of the shell \n"<<
            "3. Plates Welded to the end of the shell (no inside welding) \n"<<
            "4. Plates welded to the end of the shell with an additional fillet weld on the inside "<<endl;
            int ch;
            cin >> ch;
            ch = check(ch , 1, 4);
            cout << "Theoretical Thickness of Flat Head             : " <<flatHead( Pd, J,  F,  Di,  Do,ch) <<" mm"<<endl;
            cout << "Flat Head Thickness with corrosion Allowance   : "<< flatHead( Pd, J,  F,  Di,  Do,ch) + Ca<<" mm"<<endl;
            cout << "Standard Thickness of Flat Head                : " <<stdCalculator(flatHead( Pd, J,  F,  Di,  Do,ch) + Ca) <<" mm"<< endl;
            break;
        case 3:
            conicalHead(Pd,  J, F,  Di, Do, Ca , tShellStd-Ca);
            break;
        case 4:
            cout << "\n------------------For Dished Head------------\n\n";
            cout << "Assuming Ri = 70% of Di and ri = 6% of Di\n";
            cout << "Theoretical Thickness of dishedHead            : " <<dishedHead(tShellStd, Do, Di, Pd, F, J) <<" mm"<<endl;
            cout << "Dished Head Thickness with corrosion Allowance : "<< dishedHead(tShellStd, Do, Di, Pd, F, J) + Ca<<" mm"<<endl;
            cout << "Standard Thickness of dishedHead               : " <<stdCalculator(dishedHead(tShellStd, Do, Di, Pd, F, J) + Ca) <<" mm"<< endl;
            cout << "\n------------------For Flat Head--------------\n\n";
            cout<<"Please specify method of attachment to shell"<<endl;
            cout << "1. Flanged flat heads butt welded to shell \n"<<
            "2. Plates welded to the inside of the shell \n"<<
            "3. Plates Welded to the end of the shell (no inside welding) \n"<<
            "4. Plates welded to the end of the shell with an additional fillet weld on the inside "<<endl;
            int ch1;
            cin >> ch1;
            ch1 = check(ch1 , 1, 4);
            cout << "Theoretical Thickness of Flat Head             : " <<flatHead( Pd, J,  F,  Di,  Do,ch1) <<" mm"<<endl;
            cout << "Flat Head Thickness with corrosion Allowance   : "<< flatHead( Pd, J,  F,  Di,  Do,ch1) + Ca<<" mm"<<endl;
            cout << "Standard Thickness of Flat Head                : " <<stdCalculator(flatHead( Pd, J,  F,  Di,  Do,ch1) + Ca) <<" mm"<< endl;
            cout << "\n------------------For Conical Head--------------\n\n";
            conicalHead(Pd,  J, F,  Di, Do, Ca , tShellStd-Ca);
            break;
        default:
            break;
    }

    Compensation(Pd,J,F,Di,Do,Ca,tShellStd);
    
    return 0;
}