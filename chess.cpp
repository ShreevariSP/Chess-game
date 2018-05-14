/*Required Header files*/
#include<iostream>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include<stdio.h>
#include<string.h>
#include<cmath>
using namespace std;//Using standard namespace for all further variables.

int check=0;//Global variable used for storing presence/absence of a check.

/*
Positions of every Pawn on the playing area Board(and off the playing area of the Board for that matter) are depicted using a row-column notation using the following structure.
*/
struct position
{
    int r,c;
};

/*
**All captured Pawns are sent to their respective prisons, which is off the playing area on the opponent's side of the Board.
**The captured Pawns in the Prison are placed in Game commencement order.
**Each Prison consists of an elite row for the more powerful Pawns, pawn row for soldiers.
*/
struct prison
{
    int eliteRow, pawnRow;
}wPrison = {0,1}, bPrison = {11,10};
/*
**Following helper functions are defined later.
*/
int isClearPath(position,position);
void updateCheck(char);
void swapPawnsAt(position, position);
/*
The Pawn class with all characteristics of a Chess-Pawn.
*/
class Pawn
{
public:
    static Pawn* board[12][8];//Chess board
public:
    string name;//Name of the Pawn
    position initPos;//Initial position of Pawn
    char color;//Color of Pawn
    int moves;//Number of moves of the Pawn

    //Constructor overloading
    Pawn()//Constructor 1
    {}
    Pawn(char pName, position inPos, char inColor, int inMoves)
    {
        switch(pName)
        {
        case 'D':
            name = "Dummy";
            break;
        case 'R':
            name = "Rook";
            break;
        case 'B':
            name = "Bishop";
            break;
        case 'H':
            name = "Horse";
            break;
        case 'Q':
            name = "Queen";
            break;
        case 'S':
            name = "Soldier";
            break;
        case 'K':
            name = "King";
            break;
        }
        moves=inMoves;
        color=inColor;
        initPos = inPos;
    }
    Pawn(string pName, position inPos, char inColor)//Constructor 2
    {
        name = pName;
        moves=0;
        color=inColor;
        initPos = inPos;
    }
    position getPos()//Function to get the current position of the Pawn
    {
        position out;
        for(int r=0; r<12; ++r)
            for(int c=0; c<8; ++c)
             if(board[r][c]->initPos.r==initPos.r&&board[r][c]->initPos.c==initPos.c)
                out = {r,c};
        return out;
    }
    //Function overloading below
    Pawn* pawnAt(position inPos)//To get the Pawn located at input position
    {
        return board[inPos.r][inPos.c];
    }
    Pawn* pawnAt(int inR, int inC)//To get the Pawn located at input position
    {
        return board[inR][inC];
    }
    int compat(position tarPos)
    /*
        Parameters : Target position(type-position).
        Return value : Compatibility of move(Boolean).
    */
    {
        if(pawnAt(tarPos)->color == this->color)//Target is same Pawn
            return 0;
        if(tarPos.r<2||tarPos.r>10)//Target is imprisoned
            return 0;
        position pos = this->getPos();
        if(pos.r<2||pos.r>10)//Self is imprisoned
            return 0;
        if(name.compare("Dummy")==0)//Self is empty
            return 0;
        else if(name.compare("Rook")==0)
        {
            if(tarPos.r==pos.r||tarPos.c==pos.c)
                return isClearPath(pos,tarPos);
            else return 0;
        }
        else if(name.compare("Bishop")==0)
        {
            if(abs(tarPos.r-pos.r)==abs(tarPos.c-pos.c))
                return isClearPath(pos,tarPos);
            else return 0;
        }
        else if(name.compare("Horse")==0)
        {
            if(abs(tarPos.r-pos.r)==2&&abs(tarPos.c-pos.c)==1||abs(tarPos.r-pos.r)==1&&abs(tarPos.c-pos.c)==2)
                return 1;
            else return 0;
        }
        else if (name.compare("Queen")==0)
        {
            if((abs(tarPos.r-pos.r)==abs(tarPos.c-pos.c))||(tarPos.r==pos.r||tarPos.c==pos.c))
                return isClearPath(pos,tarPos);
            else return 0;
        }
        else if(name.compare("Soldier")==0)
        {
            Pawn* tarPawn = pawnAt(tarPos);
            if(tarPawn->name.compare("Dummy")==0)//Not capturing any Pawn
            {
                if(pos.r==initPos.r&&pos.c==initPos.c)//Soldier Pawn can move twice at initial position
                {
                    if((abs(tarPos.r-pos.r)==1||abs(tarPos.r-pos.r)==2)&&tarPos.c==pos.c)
                        return isClearPath(pos,tarPos);
                    else return 0;
                }
                else
                {
                    if(abs(tarPos.r-pos.r)==1&&tarPos.c==pos.c)
                        return isClearPath(pos,tarPos);
                    else return 0;
                }
            }
            else//Capturing a Pawn
            {
                if(abs(tarPos.r-pos.r)==1&&abs(tarPos.c-pos.c)==1)
                    return isClearPath(pos,tarPos);
                else return 0;
            }
        }
        else if(name.compare("King")==0)
        {
            Pawn* tarPawn = pawnAt(tarPos);
            if(
			((pawnAt(tarPos.r+1,tarPos.c-1)->name.compare("King")==0)&&(pawnAt(tarPos.r+1,tarPos.c-1)->color!=color))||
			((pawnAt(tarPos.r+1,tarPos.c  )->name.compare("King")==0)&&(pawnAt(tarPos.r+1,tarPos.c  )->color!=color))||
			((pawnAt(tarPos.r+1,tarPos.c+1)->name.compare("King")==0)&&(pawnAt(tarPos.r+1,tarPos.c+1)->color!=color))||
			((pawnAt(tarPos.r  ,tarPos.c-1)->name.compare("King")==0)&&(pawnAt(tarPos.r  ,tarPos.c-1)->color!=color))||
			((pawnAt(tarPos.r  ,tarPos.c+1)->name.compare("King")==0)&&(pawnAt(tarPos.r  ,tarPos.c+1)->color!=color))||
			((pawnAt(tarPos.r-1,tarPos.c-1)->name.compare("King")==0)&&(pawnAt(tarPos.r-1,tarPos.c-1)->color!=color))||
			((pawnAt(tarPos.r-1,tarPos.c  )->name.compare("King")==0)&&(pawnAt(tarPos.r-1,tarPos.c  )->color!=color))||
			((pawnAt(tarPos.r-1,tarPos.c+1)->name.compare("King")==0)&&(pawnAt(tarPos.r-1,tarPos.c+1)->color!=color))
		    )//Check for King-King proximity
		    {
                cout<<"King-King repulsion";
                return 0;
		    }
			else
			{
			    if(tarPawn->name.compare("Dummy")==0&&moves==0)//Castling check
                {
                    if(tarPos.c-pos.c<0&&tarPos.r==pos.r)//Castling to left
                    {
                        if((pawnAt(tarPos.r,tarPos.c-1)->name.compare("Rook")==0)&&(pawnAt(tarPos.r,tarPos.c-1)->moves==0)&&isClearPath(pos, tarPos))
                            return 2;
                    }
                    else if(tarPos.c-pos.c>0&&tarPos.r==pos.r)//Castling to right
                    {
                        if((pawnAt(tarPos.r,tarPos.c+1)->name.compare("Rook")==0)&&(pawnAt(tarPos.r,tarPos.c+1)->moves==0)&&isClearPath(pos,tarPos))
                            return 3;
                    }
                }
                if((abs(tarPos.r-pos.r)==1&&abs(tarPos.c-pos.c)==1)||(abs(tarPos.r-pos.r)==1&&tarPos.c==pos.c)||(tarPos.r==pos.r&&abs(tarPos.c-pos.c==1)))
                //Normal move
                {
                    return isClearPath(pos,tarPos);
                }
                else return 0;
		}
        }
        cout<<"\nThere seems to be a problem with the game.\nPlease contact the Developer Team.";
    }
    void moveTo(position tarPos)
    /*
    Parameters : Target position(type-position).
    Return value : None. It moves the Pawn to target position.
    */
    {
        if(compat(tarPos)==1)
        {
            position cPos = this->getPos();

            swapPawnsAt(tarPos,{0,3});
            swapPawnsAt(tarPos,this->getPos());

            updateCheck(color);

            swapPawnsAt(this->getPos(),cPos);
            swapPawnsAt(tarPos, {0,3});
            if(check==1)
            {
                system("color cf");//Change color to red
                cout<<"\tCHECK!!";
            }
            else//Revert changes
            {
                system("color 2f");//Change color to green
                capture(pawnAt(tarPos),cPos);
                swapPawnsAt(tarPos,this->getPos());
                this->moves++;//Increase number of moves
            }
        }
		else if(compat(tarPos)==2||compat(tarPos)==3)
		{
            position cPos = this->getPos();
			swapPawnsAt(tarPos, {0,3});
			swapPawnsAt(this->getPos(), tarPos);
			position pos1 = {tarPos.r,tarPos.c-1};
			position pos2 = {tarPos.r,tarPos.c+1};
			swapPawnsAt(pos1,pos2);

			updateCheck(color);

			swapPawnsAt(pos1,pos2);
			swapPawnsAt(this->getPos(),cPos);
			swapPawnsAt(tarPos, {0,3});
			if(check==1)
			{
                system("color cf");//Change color to red
				cout<<"\tCHECK!!";
            }
			else//Revert changes
            {
                system("color 2f");//Change color to green
                capture(pawnAt(tarPos),cPos);
                swapPawnsAt(tarPos,this->getPos());
                swapPawnsAt(pos1,pos2);
                this->moves++;//Increase number of moves
            }
        }
        else
        {
            system("color cf");//Change color to red
            cout<<"\tINCOMPATIBLE MOVE!";
        }
    }
    void capture(Pawn* Oppn, position cPos)
    /*
    Parameters : Opposition Pawn(type-Pawn*), current position of self(type-position).
    Return value : None. Captures the Opposition Pawn(i.e. sends it to prison).
    */
    {
        position capturePos;
        switch(Oppn->color)
        {
            case 'W':
            {
                capturePos.r = Oppn->initPos.r-2;
                capturePos.c = Oppn->initPos.c;
                break;
            }
            case 'B':
            {
                capturePos.r = Oppn->initPos.r+2;
                capturePos.c = Oppn->initPos.c;
                break;
            }
            case 'G':
            {
                capturePos = cPos;
            }
        }
        swapPawnsAt(Oppn->getPos(),capturePos);
    }
    void display()//Displays information of self
    {
        cout<<this->color<<this->name[0]<<" at "<<this->getPos().r<<","<<this->getPos().c;
    }
    ~Pawn()//Destructor
    {
        cout<<"DEATH!";
    }
};

Pawn* Pawn::board[12][8];//Definition of static Chess board
Pawn* pawnAt(position inPos)//To get the Pawn located at input position
{
    return Pawn::board[inPos.r][inPos.c];
}
Pawn* pawnAt(int inR, int inC)//To get the Pawn located at input position
{
    return Pawn::board[inR][inC];
}

/*
Function checks if the path between two positions is clear.
*/
int isClearPath(position pos1, position pos2)
    /*
    Parameters : Positions(type - position) of the starting and end points of the path.
    Return value : Boolean value(type - int) pertaining to the 'clear'ness of the path.
    (Note - Non-linear path returns False)
    */
{
    if     (pos2.r>pos1.r&&pos2.c==pos1.c)//Pawn 2 to South of Pawn 1
    {
        for(int i=pos1.r+1,j=pos1.c; i<pos2.r&&j==pos2.c; ++i)//Traversing S
        {
            if(pawnAt(i,j)->color!='G')
                return 0;
        }
        return 1;
    }
    else if(pos2.r>pos1.r&&pos2.c>pos1.c)//Pawn 2 to South-East of Pawn 1
    {
        for(int i=pos1.r+1,j=pos1.c+1; i<pos2.r&&j<pos2.c; ++i,++j)//Traversing SE
        {
            if(pawnAt(i,j)->color!='G')
                return 0;
        }
        return 1;
    }
    else if(pos2.r==pos1.r&&pos2.c>pos1.c)//Pawn 2 to East of Pawn 1
    {
        for(int i=pos1.r,j=pos1.c+1; i==pos2.r&&j<pos2.c; ++j)//Traversing E
        {
            if(pawnAt(i,j)->color!='G')
                return 0;
        }
        return 1;
    }
    else if(pos2.r<pos1.r&&pos2.c>pos1.c)//Pawn 2 to North-East of Pawn 1
    {
        for(int i=pos1.r-1,j=pos1.c+1; i>pos2.r&&j<pos2.c; --i,++j)//Traversing NE
        {
            if(pawnAt(i,j)->color!='G')
                return 0;
        }
        return 1;
    }
    else if(pos2.r<pos1.r&&pos2.c==pos1.c)//Pawn 2 to North of Pawn 1
    {
        for(int i=pos1.r-1,j=pos1.c; i>pos2.r&&j==pos2.c; --i)//Traversing N
        {
            if(pawnAt(i,j)->color!='G')
                return 0;
        }
        return 1;
    }
    else if(pos2.r<pos1.r&&pos2.c<pos1.c)//Pawn 2 to North-West of Pawn 1
    {
        for(int i=pos1.r-1,j=pos1.c-1; i>pos2.r&&j>pos2.c; --i,--j)//Traversing NW
        {
            if(pawnAt(i,j)->color!='G')
                return 0;
        }
        return 1;
    }
    else if(pos2.r==pos1.r&&pos2.c<pos1.c)//Pawn 2 to West of Pawn 1
    {
        for(int i=pos1.r-1,j=pos1.c; i==pos2.r&&j>pos2.c; --i)//Traversing W
        {
            if(pawnAt(i,j)->color!='G')
                return 0;
        }
        return 1;
    }
    else if(pos2.r>pos1.r&&pos2.c<pos1.c)//Pawn 2 to South-West of Pawn 1
    {
        for(int i=pos1.r+1,j=pos1.c-1; i<pos2.r&&j>pos2.c; ++i,--j)//Traversing SW
        {
            if(pawnAt(i,j)->color!='G')
                return 0;
        }
        return 1;
    }
    else//Non-linear path(Incompatible path)
        return 0;
}

/*
Function updates the Global variable 'check' based on the presence/absence of a check on the opponent.
*/
void updateCheck(char color)
    /*
    Parameters : Color(type - char) of the Player whose check status is to be updated.
    Return value : None
    */
{
    position kingPos;
    if(color=='W')
	   kingPos = {2,4};
    else if(color=='B')
	   kingPos = {9,4};
    for(int i=2; i<10; ++i)     //Traversal through all positions on the playable area
        for(int j=0; j<8; ++j)
        {
		  Pawn* cPawn = pawnAt(i,j); //Current pawn
            if(cPawn->color!=color)//Filtering for opponent Pawns (and filtering Pawn types)
            {
                if(cPawn->compat(kingPos))
                {
                    check=1;
                    return;
                }
            }
        }
    check=0;        //Default update value : Check absent
}

/*
Function to swap Pawns at two positions.
*/
void swapPawnsAt(position p1, position p2)
    /*
    Parameters : Positions(type - position) of the Pawns to be swapped.
    Return value : None.
    */
{
    Pawn* temp;
    temp = Pawn::board[p1.r][p1.c];
    Pawn::board[p1.r][p1.c] = Pawn::board[p2.r][p2.c];
    Pawn::board[p2.r][p2.c] = temp;
}


/*
Function to populate the Pawn::board with Pawns of CHESS.
*/
void populateBoard()
    /*
    Parameters : None.
    Return value : None.
    */
{
    Pawn::board[2][7] = new Pawn("Rook",{2,7},'W');
    Pawn::board[2][0] = new Pawn("Rook",{2,0},'W');
    Pawn::board[9][7] = new Pawn("Rook",{9,7},'B');
    Pawn::board[9][0] = new Pawn("Rook",{9,0},'B');

    Pawn::board[2][6] = new Pawn("Horse",{2,6},'W');
    Pawn::board[2][1] = new Pawn("Horse",{2,1},'W');
    Pawn::board[9][6] = new Pawn("Horse",{9,6},'B');
    Pawn::board[9][1] = new Pawn("Horse",{9,1},'B');

    Pawn::board[2][5] = new Pawn("Bishop",{2,5},'W');
    Pawn::board[2][2] = new Pawn("Bishop",{2,2},'W');
    Pawn::board[9][5] = new Pawn("Bishop",{9,5},'B');
    Pawn::board[9][2] = new Pawn("Bishop",{9,2},'B');

    Pawn::board[2][3] = new Pawn("Queen",{2,3},'W');
    Pawn::board[9][3] = new Pawn("Queen",{9,3},'B');

    Pawn::board[2][4] = new Pawn("King",{2,4},'W');
    Pawn::board[9][4] = new Pawn("King",{9,4},'B');
    for(int c=0; c<8; ++c)
    {
        Pawn::board[3][c] = new Pawn("Soldier",{3,c},'W');
        Pawn::board[8][c] = new Pawn("Soldier",{8,c},'B');
    }
    for(int c=0; c<8; ++c)
    {
        Pawn::board[0][c] = new Pawn("Dummy",{0,c},'G');
        Pawn::board[1][c] = new Pawn("Dummy",{1,c},'G');
        Pawn::board[4][c] = new Pawn("Dummy",{4,c},'G');
        Pawn::board[5][c] = new Pawn("Dummy",{5,c},'G');
        Pawn::board[6][c] = new Pawn("Dummy",{6,c},'G');
        Pawn::board[7][c] = new Pawn("Dummy",{7,c},'G');
        Pawn::board[10][c] = new Pawn("Dummy",{10,c},'G');
        Pawn::board[11][c] = new Pawn("Dummy",{11,c},'G');
    }
}
/*
A function to update the index of games with a new game.
*/
void updateIndex(string name)
    /*
    Parameters : name(type-string).
    Return value : None.
    */
{
    ifstream fin("My_Games.txt",ios::in);
    string game = "INVALID";    //Initialization
    while(!fin.eof())
    {
        fin>>game;
        if(name.compare(game)==0)
            return;
    }
    fin.close();
    ofstream fout("My_Games.txt", ios::app);//file opened in append mode
    fout<<name<<" ";
    fout.close();
}

/*
A function to display the index of saved games.
*/
void savedGameIndex()
    /*
        Parameters : None.
        Return value : None.
    */
{
    ifstream fin("My_Games.txt",ios::in);
    cout<<"\nThe following games have been saved.\n";
    if(!fin)
        return;
    while(!fin.eof())
    {
        string temp;    //temporary string variable
        cout<<"\n";
        fin>>temp;
        cout<<temp;
    }
    cin.get();
}

/*
A function to save a game
*/
void saveGame(char gameName[64])
    /*
    Parameters : Name of the game to be saved(Incl. file extension)(type-char*).
    Return value : None. Saves current game to a file and updates the index.
    */
{

    ofstream fout;
    fout.open(gameName, ios::out);
    for(int i=0; i<12; ++i)
        for(int j=0; j<8; ++j)
        {
            char output[5];
            output[0] = Pawn::board[i][j]->name[0];
            output[1] = char(48 + Pawn::board[i][j]->initPos.r);
            output[2] = char(48 + Pawn::board[i][j]->initPos.c);
            output[3] = Pawn::board[i][j]->color;
            output[4] = char(48 + Pawn::board[i][j]->moves);
            for(int i=0; i<5; ++i)
                fout.put(output[i]);
        }
    fout.close();
    updateIndex(gameName);
    cout<<"\nGame saved successfully.";
    cin.get();
}

/*
A function to load a saved game from a file.
*/
void loadGame(char gameName[64])
    /*
    Parameters : Name of the game to be loaded(Incl. file extension)(type-char*).
    Return value : None. Loads current game from file.
    */
{
    ifstream fin;
    fin.open(gameName, ios::in);
    fin.seekg(0);
    if(!fin)
    {
        cout<<"\nThe game you entered does not exist. Play a new game instead :)";
        populateBoard();
        cin.get();
        return;
    }
    for(int i=0; i<12; ++i)
        for(int j=0; j<8; ++j)
        {
            char name, input[5];
            position initPos;
            char color;
            int moves;
            for(int i=0; i<5; ++i)
                fin.get(input[i]);
            name = input[0];
            initPos.r = ((int)input[1])-48;
            initPos.c = ((int)input[2])-48;
            color = input[3];
            moves = ((int)input[4])-48;
            Pawn::board[i][j] = new Pawn(name, initPos, color, moves);
        }

   fin.close();
   cout<<"Game has been successfully loaded. Enjoy playing!";
   cin.get();
}
/*
A function to delete a saved game.
*/
void deleteGame(char gameName[64])
    /*
    Parameters : Name of the game to be deleted(Incl. file extension)(type-char*).
    Return value : None. Deletes saved game file and updates the index.
    */
{
    remove(gameName);
    fstream finout("My_Games.txt", ios::in|ios::out);
    ofstream fout;
    fout.open("temp.txt",ios::out);
    string temp;
    while(!finout.eof())
    {
        temp = "INVALID";
        finout>>temp;
        if(temp.compare(gameName)==0)
            continue;
        fout<<temp<<" ";
    }
    finout.close();
    fout.close();
    finout.open("My_Games.txt", ios::out);
    ifstream fin("temp.txt",ios::in);
    while(!fin)
        cout<<"Error.";
    temp="INVALID";
    while(!fin.eof())
    {
        fin>>temp;
        if(temp.compare("INVALID")!=0)
            finout<<temp<<" ";
    }
    remove("temp.txt");
    cout<<"\nGame successfully removed.";
}

/*
Function to display the board.
*/
void display()
    /*
    Parameters : None.
    Return value : None.
    */
{
    cout<<"\n#############################################";
    cout<<"\n#%%%%%%%_____________________________%%%%%%%#";
    cout<<"\n#%%%%%%%|@|07|06|05|04|03|02|01|00|@|%%%%%%%#";
    cout<<"\n#%%%%%%%|=|==|==|==|==|==|==|==|==|=|%%%%%%%#";
    for(int i=11; i>=0; --i)
    {
        cout<<"\n#%%%%%%%"<<"|"<<char(65+i);
        for(int j=7; j>=0; --j)
        {
            if(pawnAt(i,j)->color!='G')
                cout<<"|"<<(char)((int)pawnAt(i,j)->color+32)<<pawnAt(i,j)->name[0];
            else
                cout<<"|  ";
        }
        cout<<"|"<<char(65+i)<<"|%%%%%%%#\n#%%%%%%%";
        if(i==10||i==2)
            cout<<"|=|==|==|==|==|==|==|==|==|=|%%%%%%%#\n#%%%%%%%";
        cout<<"|=|==|==|==|==|==|==|==|==|=|%%%%%%%#";
    }
    cout<<"\n#%%%%%%%|@|07|06|05|04|03|02|01|00|@|%%%%%%%#";
    cout<<"\n#%%%%%%%=============================%%%%%%%#";
    cout<<"\n#############################################";
}

/*
Function to play the game.
*/
void game()
{
    int end=0,save=0;
    char color='W';
    position from, to;
    char fromR, toR;
    system("cls");
    do
    {
        display();
        cout<<"\n\n"<<color<<" chance ";
        cout<<"Enter your move :\nFrom : ";
        cin>>fromR>>from.c;
        cout<<"To : ";
        cin>>toR>>to.c;
        //Conversion of input from char to int
        from.r = ((int)fromR)%97;
        to.r = ((int)toR)%97;
        //<TODO> from !=to
        system("cls");
        cout<<"*********************************************\n\t";
        pawnAt(from)->moveTo(to);
        cout<<"\n*********************************************";
        display();
        color = (color=='W')?'B':'W';//Inverting the color
        cout<<"\n\nDo you wish to continue the game?(1/0)\n";
        cin>>end;
        system("cls");
        system("color 80");//Changing color to Grey
    }while(end!=0);
    cout<<"\nDo you wish to save this game?(1/0)\n";
    cin>>save;
    char temp[10];
    gets(temp);
    if(save)
    {
        char gameName[64];
        cout<<"\nEnter the name of this game(No spaces,No special characters, Underscores allowed) : ";
        gets(gameName);
        saveGame(strcat(gameName,".dat"));
    }
}
int main()
{
    ofstream file;
    file.open("My_Games.txt", ios::app);
    system("title CHESS");//Changing window title to CHESS
    system("color 80");//Changing color to Grey
    int choice=0;
    cout<<"\nWELCOME TO CHESS!\n\nInput format : <row character><column number><enter>\n\n\nPlease ensure you have maximized this window.";
    do
    {
        cout<<"\nPlease select your choice\n0 - Exit\n1 - New Game\n2 - Load Game\n3 - Delete Game";
        cin>>choice;
        switch(choice)
        {
        case 1:
            {
                populateBoard();
                game();
                break;
            }
        case 2:
            {
                savedGameIndex();
                char gameName[64];
                cout<<"\nEnter your Game-name : ";
                gets(gameName);
                loadGame(strcat(gameName,".dat"));
                game();
                break;
            }
        case 3:
            {
                char gameName[64];
                savedGameIndex();
                cout<<"\nEnter the name : ";
                gets(gameName);
                deleteGame(strcat(gameName,".dat"));
                savedGameIndex();
                break;
            }
        default:
            choice = 0;
        }
        system("cls");
    }while(choice);
    system("cls");
    system("color 3f");
    char temp;
    cin.get(temp);
    cout<<"\n\n\t\t^^^^^^^^^^^^^^^\n\t\t|| THANK YOU ||\n\t\t~~~~~~~~~~~~~~~";
    cin.get();
    return 0;
}